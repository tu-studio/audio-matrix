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

    void prepare(HostAudioConfig host_audio_config);
    void process(jack_nframes_t nframes);

    AudioBufferF& get_buffer();
    
private:
    AudioBufferF m_buffer;
    std::vector<std::shared_ptr<Module>> m_modules;
    size_t m_max_number_of_channels;
    const TrackConfig& m_config; 

};

#endif // TRACK_H