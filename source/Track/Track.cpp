#include <Track.h>

Track::Track(const TrackConfig& config, std::shared_ptr<lo::ServerThread> osc_server): m_config(config) {
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
        case Modules::AMBI_ENCODER:
            {
                auto ambi_config = std::dynamic_pointer_cast<AmbiEncoderConfig>(module_config);
                std::shared_ptr<AmbiEncoder> ambi_encoder = std::make_shared<AmbiEncoder>(ambi_config, osc_server);
                m_modules.push_back(std::static_pointer_cast<Module> (ambi_encoder));
            }
            break;
        case Modules::SUM:
            {
                auto sum_config = std::dynamic_pointer_cast<SumConfig>(module_config);
                std::shared_ptr<Sum> sum = std::make_shared<Sum>(sum_config, osc_server);
                m_modules.push_back(std::static_pointer_cast<Module> (sum));

            }
            break;
        case Modules::FILTER:
            {
                auto filter_config = std::dynamic_pointer_cast<FilterConfig>(module_config);
                std::shared_ptr<Filter> filter = std::make_shared<Filter>(filter_config, osc_server);
                m_modules.push_back(std::static_pointer_cast<Module> (filter));

            }
            break;
        default:
            break;
        }
    }
}

Track::~Track() {
}

size_t Track::initialize(size_t n_input_channels, size_t output_channel_offset) {
    std::cout << "[debug] starting init for track " << m_config.name << std::endl;
    
    m_max_n_channels = n_input_channels;
    m_output_channel_offset = output_channel_offset;
    m_n_input_channels = n_input_channels;
    size_t n_output_channels = n_input_channels;
    for (auto module : m_modules) {
        // every module returns the number of output channels it has with a given number of input channels
        n_output_channels = module->initialize(n_output_channels);
        if (n_output_channels > m_max_n_channels) {
            m_max_n_channels = n_output_channels;
        }
        std::cout << "[debug] initialized module with " << n_output_channels << " output channels" << std::endl;
    }
    m_n_output_channels = n_output_channels;
    return n_output_channels;
}

void Track::prepare(HostAudioConfig host_audio_config) {
    for (auto module : m_modules) {
        module->prepare(host_audio_config);
    }
    m_buffer.initialize(m_max_n_channels, host_audio_config.m_host_buffer_size);
}

void Track::process(float** in, float** out, size_t nframes) {
    for (size_t i = 0; i < m_n_input_channels; i++) {
        for (size_t j = 0; j < nframes; j++) {
            m_buffer.setSample(i, j, in[i][j]);
        }
    }
    for (auto& module : m_modules) {
        module->process(m_buffer, nframes);
    }
    for (size_t i = 0; i < m_n_output_channels; i++) {
        for (size_t j = 0; j < nframes; j++) {
            out[i + m_output_channel_offset][j] = m_buffer.getSample(i, j);
        }
    }
}