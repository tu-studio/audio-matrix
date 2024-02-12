#include <Gain.h>

Gain::Gain(std::shared_ptr<GainConfig> config, std::shared_ptr<lo::ServerThread> osc_server) : m_config(config){
    if (m_config->osc_controllable && osc_server != nullptr){
        std::cout << "registering function on path " << m_config->osc_path << std::endl;
        osc_server->add_method(m_config->osc_path, "if", osc_gain_callback, this);
    } else if(m_config->osc_controllable && osc_server == nullptr){
        std::cout << "osc_server is null" << std::endl;
    }
}



int Gain::initialize(int input_channels, jack_nframes_t nframes, jack_nframes_t sample_rate) {
    this->input_channels = input_channels;
    this->output_channels = input_channels;
    name = "Gain";
    std::ignore = sample_rate;

    return 0;
}

void Gain::process(AudioBufferF &buffer, jack_nframes_t nframes) {
    for (int i = 0; i < input_channels; i++) {
        // float* channel = buffer.get_channel_pointer(i);
        // for (int j = 0; j < nframes; j++) {
        //     channel[j] *= m_gain;
        // }
    }
}

void Gain::set_gain(float gain) {
    m_gain = gain;
}

float Gain::get_gain() {
    return m_gain;
}

int Gain::osc_gain_callback(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
    std::ignore = path;
    std::ignore = types;
    std::ignore = argc;
    std::ignore = data;
    Gain* gain = (Gain*) user_data;
    gain->set_gain(argv[1]->f);
    std::cout << "Changed gain of " << argv[0]->i <<  " to: " << gain->get_gain() << std::endl;
    return 0;
}
