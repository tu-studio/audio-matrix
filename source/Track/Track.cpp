#include <ModuleConfig.h>
#include <Track.h>

Track::Track(const TrackConfig& config, std::shared_ptr<lo::ServerThread> osc_server): m_config(config) {
    for (const std::shared_ptr<ModuleConfig> module_config : m_config.modules) {
        switch (module_config->module_type()) {
        // MODULE SWITCH CASES GO HERE
        case Modules::GAIN:
            createAndAddModule<GainConfig, Gain>(module_config, osc_server);
            break;
        case Modules::AMBI_ENCODER:
            createAndAddModule<AmbiEncoderConfig, AmbiEncoder>(module_config, osc_server);
            break;
        case Modules::SUM:
            createAndAddModule<SumConfig, Sum>(module_config, osc_server);
            break;
        case Modules::FILTER:
            createAndAddModule<FilterConfig, Filter>(module_config, osc_server);
            break;
        case Modules::DISTANCE_GAIN:
            createAndAddModule<DistanceGainConfig, DistanceGain>(module_config, osc_server);
            break;
        case Modules::DELAY:
            createAndAddModule<DelayConfig, Delay>(module_config, osc_server);
            break;
        // END MODULE SWITCH CASES
        default:
            break;
        }
    }
}

Track::~Track() {
}

size_t Track::initialize(size_t n_input_channels, size_t output_channel_offset) {
    // std::cout << "[debug] starting init for track " << m_config.name << std::endl;
    
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
        // std::cout << "[debug] initialized module with " << n_output_channels << " output channels" << std::endl;
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
