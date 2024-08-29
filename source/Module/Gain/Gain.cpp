#include <Gain.h>

Gain::Gain(std::shared_ptr<GainConfig> config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config){
    if (m_config->osc_controllable && osc_server != nullptr){
        std::cout << "[info] Gain listening on path " << m_config->osc_path << std::endl;
        osc_server->add_method(m_config->osc_path, "if", osc_gain_callback, this);
    } else if(m_config->osc_controllable && osc_server == nullptr){
        std::cout << "[error] osc_server is null, cannot add gain callback method" << std::endl;
    }
}

size_t Gain::initialize(size_t input_channels) {
    m_n_input_channels = input_channels;
    m_n_output_channels = input_channels;
    for (size_t i = 0; i < input_channels; i++) {
        m_gain.push_back(m_config->factor);
        m_prev_gain.push_back(m_config->factor);
    }
    return m_n_output_channels;
}

void Gain::process(AudioBufferF &buffer, size_t nframes) {
    for (size_t channel = 0; channel < m_n_input_channels; channel++) {
        if (m_gain[channel] != m_prev_gain[channel]){
            float gain_prev = m_prev_gain[channel];
            float gain_new = m_gain[channel];
            float stepsize = (gain_new-gain_prev) / nframes;
            for (size_t frame = 0; frame < nframes; frame++){
                float gain = gain_prev + (stepsize * frame);
                buffer.setSample(channel, frame, buffer.getSample(channel,frame) * gain);
            }
            m_prev_gain[channel] = gain_new;
        } else {
            for (size_t sample = 0; sample < nframes; sample++) {
                buffer.setSample(channel, sample, buffer.getSample(channel, sample) * m_gain[channel]);
            }
        }
    }
}

void Gain::set_gain(size_t channel, float gain) {
    m_gain[channel] = gain;
}

float Gain::get_gain(size_t channel) {
    return m_gain[channel];
}

int Gain::osc_gain_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data) {
    std::ignore = path;
    std::ignore = types;
    std::ignore = argc;
    std::ignore = data;
    Gain* gain = (Gain*) user_data;
    gain->set_gain(argv[0]->i, argv[1]->f);
    return 0;
}
