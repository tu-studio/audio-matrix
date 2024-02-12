#include <AudioMatrix.h>

AudioMatrix::AudioMatrix(std::string config_path): config_parser(config_path)
{
    m_audio_matrix_config = *config_parser.get_config().get();
    m_osc_server = OSCServer(m_audio_matrix_config.port);
    for (TrackConfig track : m_audio_matrix_config.tracks){
        TrackPtr new_track = std::make_shared<Track>(track, m_osc_server.get_server_thread());
        tracks.push_back(new_track);
        std::cout << "Adding track " << std::endl;
    }
    m_osc_server.start();
    // initialize();
}

AudioMatrix::~AudioMatrix()
{
}

void AudioMatrix::initialize() {
    size_t output_channel_offset = 0;
    for (auto track : tracks) {
        std::cout << "Initializing track count" << std::endl;
        output_channel_offset += track->initialize(m_audio_matrix_config.n_input_channels, output_channel_offset);
        std::cout << "Initializing track " << track->get_name() << std::endl;
        std::cout << "Output channel offset: " << track->get_output_channel_offset() << std::endl;
        std::cout << "Number of output channels: " << track->get_number_output_channels() << std::endl;
    }
    m_n_output_channels = output_channel_offset;
}

void AudioMatrix::prepare(HostAudioConfig host_audio_config) {
    m_host_audio_config = host_audio_config;

    for (auto track : tracks) {
        track->prepare(m_host_audio_config);
    }
}

void AudioMatrix::process(float** in, float** out, size_t nframes) {
    for (auto track : tracks) {
        track->process(in, out, nframes);
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
    for (auto track : tracks) {
        std::cout << "Initializing track " << track->get_name() << std::endl;
        std::cout << "Output channel offset: " << track->get_output_channel_offset() << std::endl;
        std::cout << "Number of output channels: " << track->get_number_output_channels() << std::endl;
        if (channel < track->get_output_channel_offset() + track->get_number_output_channels()) {
            name = track->get_name();
            channel_number = channel - track->get_output_channel_offset();
            break;
        }
    }
    return name + "_" + std::to_string(channel_number);
}