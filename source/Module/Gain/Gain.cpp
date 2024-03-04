#include <Gain.h>

Gain::Gain(std::shared_ptr<GainConfig> config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config){
    if (m_config->osc_controllable && osc_server != nullptr){
        std::cout << "registering function on path " << m_config->osc_path << std::endl;
        osc_server->add_method(m_config->osc_path, "if", osc_gain_callback, this);
    } else if(m_config->osc_controllable && osc_server == nullptr){
        std::cout << "osc_server is null" << std::endl;
    }
}

size_t Gain::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    for (size_t i = 0; i < input_channels; i++) {
        m_gain.push_back(m_config->factor);
    }
    return m_n_output_channels;
}

void Gain::process(AudioBufferF &buffer, size_t nframes) {
    for (size_t i = 0; i < m_n_input_channels; i++) {
        for (size_t j = 0; j < nframes; j++) {
            buffer.setSample(i, j, buffer.getSample(i, j) * m_gain[i]);
        }
    }
}

void Gain::set_gain(size_t channel, float gain) {
    m_gain[channel] = gain;
}

float Gain::get_gain(size_t channel) {
    return m_gain[channel];
}

int Gain::osc_gain_callback(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    std::ignore = path;
    std::ignore = types;
    std::ignore = argc;
    std::ignore = data;
    Gain* gain = (Gain*) user_data;
    gain->set_gain(argv[0]->i, argv[1]->f);
    std::cout << "Changed gain of " << argv[0]->i <<  " to: " << gain->get_gain(argv[0]->i) << std::endl;
    return 0;
}
