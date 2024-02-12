#include <AudioMatrix.h>

AudioMatrix::AudioMatrix(std::string config_path): config_parser(config_path)
{
    config = config_parser.get_config();
    m_osc_server = OSCServer(config->port);
    for (TrackConfig track : config->tracks){
        Track new_track(track, m_osc_server.get_server_thread());
        tracks.push_back(new_track);
    }
    m_osc_server.start();
}

AudioMatrix::~AudioMatrix()
{
}

void AudioMatrix::prepare(HostAudioConfig host_audio_config) {
    m_host_audio_config = host_audio_config;

    for (auto track : tracks) {
        track.prepare(m_host_audio_config);
    }
}