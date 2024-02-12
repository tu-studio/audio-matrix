#include <AudioMatrix.h>

AudioMatrix::AudioMatrix(std::string config_path): config_parser(config_path)
{
    config = config_parser.get_config();
    for (TrackConfig track : config->tracks){
        Track new_track(track);        
    }
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