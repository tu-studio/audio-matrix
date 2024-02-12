#ifndef AUDIOMATRIX_H
#define AUDIOMATRIX_H

#include <AudioMatrixConfig.h>
#include <Track.h>
#include <vector>
#include <string>
#include <memory>
#include <ConfigParser.h>
#include <HostAudioConfig.h>
#include <OSCServer.h>

class AudioMatrix
{
public:
    AudioMatrix(std::string config_path);
    ~AudioMatrix();

    void prepare(HostAudioConfig host_audio_config);

private:
    std::vector<Track> tracks;
    std::shared_ptr<AudioMatrixConfig> config;
    ConfigParser config_parser;
    HostAudioConfig m_host_audio_config;
    OSCServer m_osc_server;

};




#endif //AUDIOMATRIX_H