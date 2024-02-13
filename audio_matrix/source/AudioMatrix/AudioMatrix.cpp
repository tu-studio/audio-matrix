#include <AudioMatrix.h>

AudioMatrix::AudioMatrix(std::string config_path): config_parser(config_path)
{
    m_audio_matrix_config = *config_parser.get_config().get();
    m_osc_server = OSCServer(m_audio_matrix_config.port);
    for (int i = 0; i < m_audio_matrix_config.tracks.size(); i++) {
        m_tracks.emplace_back(std::make_unique<Track>(m_audio_matrix_config.tracks[i], m_osc_server.get_server_thread()));
    }
    m_osc_server.start();
    initialize();
}

AudioMatrix::~AudioMatrix() {
}

void AudioMatrix::initialize() {
    size_t output_channel_offset = 0;
    for (int i = 0; i < m_tracks.size(); i++) {
        output_channel_offset += m_tracks[i]->initialize(m_audio_matrix_config.n_input_channels, output_channel_offset);
    }
    m_n_output_channels = output_channel_offset;
}

void AudioMatrix::prepare(HostAudioConfig host_audio_config) {
    m_host_audio_config = host_audio_config;

    for (int i = 0; i < m_tracks.size(); i++) {
        m_tracks[i]->prepare(m_host_audio_config);
    }
}

void AudioMatrix::process(float** in, float** out, size_t nframes) {
    for (int i = 0; i < m_tracks.size(); i++) {
        m_tracks[i]->process(in, out, nframes);
    }
}

size_t AudioMatrix::get_n_input_channels() {
    return m_audio_matrix_config.n_input_channels;
}

size_t AudioMatrix::get_n_output_channels() {
    return m_n_output_channels;
}

std::string AudioMatrix::get_ouput_port_name(size_t channel) {
    std::string name;
    size_t channel_number = 0;
    for (int i = 0; i < m_tracks.size(); i++) {
        if (channel < m_tracks[i]->get_output_channel_offset() + m_tracks[i]->get_n_output_channels()) {
            name = m_tracks[i]->get_name();
            channel_number = channel - m_tracks[i]->get_output_channel_offset();
            break;
        }
    }
    return name + "_" + std::to_string(channel_number);
}