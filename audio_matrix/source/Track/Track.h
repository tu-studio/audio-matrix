#ifndef TRACK_H
#define TRACK_H

#include <AudioBuffer.h>
#include <Module.h>
#include <vector>

class Track {

public:
    Track(/* args */);
    ~Track();

    void initialize(int channels, int frames);
    void process(jack_nframes_t nframes);

    void add_module(Module* module);

    AudioBuffer& get_buffer();
    std::vector<Module*>& get_modules();
    
private:
    AudioBuffer buffer;
    std::vector<Module*> modules;

};

#endif // TRACK_H