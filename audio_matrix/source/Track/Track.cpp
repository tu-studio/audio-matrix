#include <Track.h>

Track::Track(const TrackConfig& config, std::shared_ptr<lo::ServerThread> osc_server): m_config(config) {
    std::cout << "Track constructor" << std::endl;
    for (const std::shared_ptr<ModuleConfig> module_config : m_config.modules){
        switch (module_config->module_type())
        {
        case Modules::GAIN:
            {
                auto gain_config = std::dynamic_pointer_cast<GainConfig>(module_config);
                std::shared_ptr<Gain> gain = std::make_shared<Gain>(gain_config, osc_server);
                m_modules.push_back(std::static_pointer_cast<Module>(gain));
            }
            break;
        default:
            break;
        }
    }
}

Track::~Track() {
    std::cout << "Track destructor" << std::endl;
}

size_t Track::initialize(size_t number_input_channels, size_t output_channel_offset) {
    m_output_channel_offset = output_channel_offset;
    m_number_input_channels = number_input_channels;
    size_t number_output_channels = number_input_channels;
    for (auto module : m_modules) {
        number_output_channels = module->initialize(number_output_channels);
        if (number_output_channels > m_max_number_of_channels) {
            m_max_number_of_channels = number_output_channels;
        }
    }
    m_number_output_channels = number_output_channels;
    return number_output_channels;
}

void Track::prepare(HostAudioConfig host_audio_config) {
    for (auto module : m_modules) {
        module->prepare(host_audio_config);
    }
    m_buffer.initialize(m_max_number_of_channels, host_audio_config.m_host_buffer_size);
}

void Track::process(float** in, float** out, size_t nframes) {
    for (size_t i = 0; i < m_number_input_channels; i++) {
        for (size_t j = 0; j < nframes; j++) {
            m_buffer.setSample(i, j, in[i][j]);
        }
    }
    for (auto& module : m_modules) {
        module->process(m_buffer, nframes);
    }
    for (size_t i = 0; i < m_number_output_channels; i++) {
        for (size_t j = 0; j < nframes; j++) {
            out[i + m_output_channel_offset][j] = m_buffer.getSample(i, j);
        }
    }
}