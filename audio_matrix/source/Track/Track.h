#ifndef TRACK_H
#define TRACK_H

#include <AudioBuffer.h>
#include <Module.h>
#include <vector>
#include <TrackConfig.h>
#include <Gain.h>

class Track {

public:
    Track(const TrackConfig& config);
    ~Track();

    void initialize(int channels, int frames);
    void process(jack_nframes_t nframes);

    AudioBuffer& get_buffer();
    
private:
    AudioBuffer m_buffer;
    std::vector<std::shared_ptr<Module>> m_modules;
    const TrackConfig& m_config; 

};

#endif // TRACK_H