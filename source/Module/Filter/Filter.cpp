#include <Filter.h>

Filter::Filter(FilterConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config){

}

size_t Filter::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    for (size_t i = 0; i < input_channels; i++)
    {
        int n_taps = m_config->order;
        m_memory_x.push_back(std::vector<double>(n_taps, 0.0) );
        m_memory_y.push_back(std::vector<double>(n_taps, 0.0) );
    }
    return m_n_output_channels;
    
}

void Filter::prepare(HostAudioConfig host_audio_config){
    calculate_filter_coefficients(host_audio_config.m_host_sample_rate);
}

void Filter::process(AudioBufferF &buffer, size_t n_frames){
    int order = m_config->order / 2;
    for (size_t channel = 0; channel < m_n_input_channels; channel++) {
        for (size_t sample = 0; sample < n_frames; sample++) {
            float current_val = buffer.getSample(channel, sample);
            float filtered_val = current_val * m_b[0];

            for (size_t i = 0; i < order; i++)
            {
                filtered_val += m_memory_x[channel][i] * m_b[i+1] + m_memory_y[channel][i] * m_a[i];
            }

            for (size_t i = 1; i < order - 1; i++)
            {
                size_t index = order - i;
                m_memory_x[channel][index] = m_memory_x[channel][index-1];
                m_memory_y[channel][index] = m_memory_x[channel][index-1];
            }
            m_memory_x[channel][0] = current_val;
            m_memory_y[channel][0] = filtered_val;
            

            buffer.setSample(channel, sample, filtered_val);
        }
    }
}

void Filter::calculate_filter_coefficients(double samplerate){
    // TODO calculate filter coeffs here
    double * a_coeffs; // length: order
    double * b_coeffs; // length: order+1
    int order = m_config->order / 2;
    switch (m_config->type)
    {
    case FilterType::LP :
        {
            double cutoff_freq = 2*samplerate / m_config->freq;

            a_coeffs = dcof_bwlp(order, cutoff_freq );

            int * b_coeffs_int = ccof_bwlp(order);
            double scaling_factor = sf_bwlp(order, cutoff_freq);

            b_coeffs = (double *) malloc((order +1 )* sizeof(double));
            for (size_t i = 0; i < order + 1; i++)
            {
                b_coeffs[i] = scaling_factor * b_coeffs_int[i];
            }

            free(b_coeffs_int);
        }
        break;
    case FilterType::HP :
        {
            double cutoff_freq = 2*samplerate / m_config->freq;

            a_coeffs = dcof_bwhp(order, cutoff_freq );

            int * b_coeffs_int = ccof_bwhp(order);
            double scaling_factor = sf_bwhp(order, cutoff_freq);

            b_coeffs = (double *) malloc((order +1 )* sizeof(double));
            for (size_t i = 0; i < order + 1; i++)
            {
                b_coeffs[i] = scaling_factor * b_coeffs_int[i];
            }

            free(b_coeffs_int);
        }
        break;
    
    default:
        std::cout << "[ERROR] invalid Filter Type" << std::endl;
        throw "Invalid Filter Type";
    }
    
    m_a = std::vector<double>(a_coeffs, a_coeffs + m_config->order);
    m_b = std::vector<double>(b_coeffs, b_coeffs + m_config->order + 1);

    free(a_coeffs);
    free(b_coeffs);
}