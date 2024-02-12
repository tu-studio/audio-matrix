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

    void initialize();
    void prepare(HostAudioConfig host_audio_config);
    void process(float** in, float** out, size_t nframes);

    size_t get_n_input_channels();
    size_t get_n_output_channels();

    std::string get_ouput_port_name(size_t channel);

private:
    std::vector<TrackPtr> tracks;
    AudioMatrixConfig m_audio_matrix_config;
    size_t m_n_output_channels;
    ConfigParser config_parser;
    HostAudioConfig m_host_audio_config;
    OSCServer m_osc_server;

};




#endif //AUDIOMATRIX_H