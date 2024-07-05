#include <Filter.h>

Filter::Filter(FilterConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config){

}

size_t Filter::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    for (size_t i = 0; i < input_channels; i++)
    {
        int n_taps = m_config->order;
        m_memory_x.push_back(std::vector<float>(n_taps, 0.0f) );
        m_memory_y.push_back(std::vector<float>(n_taps, 0.0f) );
    }
    return m_n_output_channels;
    
}

void Filter::prepare(HostAudioConfig host_audio_config){
    calculate_filter_coefficients(host_audio_config.m_host_sample_rate);
}

void Filter::process(AudioBufferF &buffer, size_t n_frames){
    for (size_t channel = 0; channel < m_n_input_channels; channel++) {
        for (size_t sample = 0; sample < n_frames; sample++) {
            float current_val = buffer.getSample(channel, sample);
            float filtered_val = current_val * m_b[0];

            // TODO order is of LR filter, not of single butterworth
            for (size_t i = 0; i < m_config->order; i++)
            {
                filtered_val += m_memory_x[channel][i] * m_b[i+1] + m_memory_y[channel][i] * m_a[i];
            }

            for (size_t i = 1; i < m_config->order - 1; i++)
            {
                size_t index = m_config->order - i;
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
    m_a.push_back(0);
    m_a.push_back(0);
    m_b.push_back(0.3);
    m_b.push_back(0.3);
    m_b.push_back(0.3);
}