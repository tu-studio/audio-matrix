#include <Sum.h>

Sum::Sum(std::shared_ptr<SumConfig> config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config) {}

size_t Sum::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = 1;
    return m_n_output_channels;
}

void Sum::process(AudioBufferF &buffer, size_t nframes) {
    for (size_t i = 0; i < m_n_input_channels; i++) {
        for (size_t j = 0; j < nframes; j++) {
            buffer.setSample(1, j, buffer.getSample(i, j));
        }
    }
}