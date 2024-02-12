#ifndef TRACK_H
#define TRACK_H

#include <AudioBuffer.h>
#include <Module.h>
#include <vector>
#include <TrackConfig.h>
#include <Gain.h>
#include <OSCServer.h>
#include <HostAudioConfig.h>

class Track {

public:
    Track(const TrackConfig& config, std::shared_ptr<lo::ServerThread> osc_server);
    ~Track();

    size_t initialize(size_t number_input_channels, size_t output_channel_offset = 0);
    void prepare(HostAudioConfig host_audio_config);
    void process(float** in, float** out, size_t nframes);

    size_t get_output_channel_offset() { return m_output_channel_offset; }
    size_t get_number_output_channels() { return m_number_output_channels; }

    std::string get_name() { return m_config.name; }
    
private:
    AudioBufferF m_buffer;
    std::vector<std::shared_ptr<Module>> m_modules;
    size_t m_output_channel_offset = 0;
    size_t m_number_input_channels = 0;
    size_t m_number_output_channels = 0;
    size_t m_max_number_of_channels = 0;
    const TrackConfig& m_config; 
};

typedef std::shared_ptr<Track> TrackPtr;

#endif // TRACK_H