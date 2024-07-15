#include <Filter.h>

Filter::Filter(FilterConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config){

}

size_t Filter::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    m_n_taps = m_config->order / 2 + 1;
    
    for (size_t i = 0; i < input_channels; i++)
    {
        m_memory_1.push_back(std::vector<double>(m_n_taps, 0.0) );
        m_memory_2.push_back(std::vector<double>(m_n_taps, 0.0) );
    }
    return m_n_output_channels;
    
}

void Filter::prepare(HostAudioConfig host_audio_config){
    calculate_filter_coefficients(host_audio_config.m_host_sample_rate);
}

void Filter::process(AudioBufferF &buffer, size_t n_frames){
    
    for (size_t channel = 0; channel < m_n_input_channels; channel++) {
        for (size_t sample = 0; sample < n_frames; sample++) {

            float current_value = buffer.getSample(channel, sample);  

            // filter twice with different memory
            double y_n = filter_sample(current_value, m_memory_1[channel] );
            y_n = filter_sample(y_n, m_memory_2[channel] );

            buffer.setSample(channel, sample, y_n);
        }
    }
}

double Filter::filter_sample(double current_x, std::vector<double> &memory){
    // Filter using this difference equasion (direct form II IIR filter):
    // w[n] = sum_{k=1}^N a_k * w[n-k] + x[n]
    // y[n] = sum_{k=0}^M b_k * w[n-k]

    // TODO if necessary this function could be optimized, by using a ringbuffer for memory and by merging the loops. this wouldn't help readability
    double w_n = current_x;
    double y_n = 0.0;

    // calculate w[n]
    for (size_t i = 1; i < m_n_taps; i++){
        w_n -= memory[i-1] * m_a[i];
    }

    // move elements in memory back
    for (size_t i = 1; i < m_n_taps; i++){
        size_t index = m_n_taps - i;
        memory[index] = memory[index-1];
    }
    memory[0] = w_n;

    // calculate y[n]
    for (size_t i = 0; i < m_n_taps; i++){
        y_n += m_b[i] * memory[i];
    }
    return y_n;
}

void Filter::calculate_filter_coefficients(double samplerate){
    double * a_coeffs; // length: order
    double * b_coeffs; // length: order+1
    int order = m_config->order / 2;

    //calculate cutoff freq as circular frequency
    double cutoff_freq = 2*m_config->freq / samplerate ;

    switch (m_config->type)
    {
    case FilterType::LP :
        {
            a_coeffs = dcof_bwlp(order, cutoff_freq );

            int * b_coeffs_int = ccof_bwlp(order);
            double scaling_factor = sf_bwlp(order, cutoff_freq);

            b_coeffs = (double *) malloc((m_n_taps)* sizeof(double));
            for (size_t i = 0; i < m_n_taps; i++){
                b_coeffs[i] = scaling_factor * b_coeffs_int[i];
            }

            free(b_coeffs_int);
        }
        break;
    case FilterType::HP :
        {

            a_coeffs = dcof_bwhp(order, cutoff_freq );

            int * b_coeffs_int = ccof_bwhp(order);
            double scaling_factor = sf_bwhp(order, cutoff_freq);

            b_coeffs = (double *) malloc((m_n_taps )* sizeof(double));
            for (size_t i = 0; i < m_n_taps; i++){
                b_coeffs[i] = scaling_factor * b_coeffs_int[i];
            }

            free(b_coeffs_int);
        }
        break;
    
    default:
        std::cout << "[ERROR] invalid Filter Type" << std::endl;
        throw "Invalid Filter Type";
    }
    
    m_a = std::vector<double>(a_coeffs, a_coeffs + m_n_taps);
    m_b = std::vector<double>(b_coeffs, b_coeffs + m_n_taps);

    std:: cout << "[debug] a coeffs: ";
    for (size_t i = 0; i < m_n_taps; i++)
    {
        std::cout << m_a[i] << ", ";
        /* code */
    }
    std::cout << std::endl;

    std:: cout << "[debug] b coeffs: ";
    for (size_t i = 0; i < m_n_taps; i++)
    {
        std::cout << m_b[i] << ", ";
        /* code */
    }
    std::cout << std::endl;

    free(a_coeffs);
    free(b_coeffs);
}