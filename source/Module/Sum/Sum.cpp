#include <Sum.h>

Sum::Sum(std::shared_ptr<SumConfig> config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config) {}

size_t Sum::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = 1;
    return m_n_output_channels;
}

void Sum::process(AudioBufferF &buffer, size_t nframes) {
    // skip first channel, since we use that one as a base to add all other channels
    for (size_t channel = 1; channel < m_n_input_channels; channel++) {
        for (size_t sample = 0; sample < nframes; sample++) {
            buffer.setSample(0, sample, buffer.getSample(0,sample) + buffer.getSample(channel, sample));
        }
    }
}