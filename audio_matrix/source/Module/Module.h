#ifndef MODULE_H
#define MODULE_H

#include <jack/jack.h>
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <AudioBuffer.h>
#include <string>
#include <iostream>

class Module {

public:
    Module();
    ~Module();

    virtual int initialize(int input_channels, jack_nframes_t nframes, jack_nframes_t sample_rate) = 0;
    virtual void process(AudioBufferF &buffer, jack_nframes_t nframes) = 0;
    std::string get_name();

protected:
    int input_channels;
    int output_channels;
    std::string name = "Default Module";
};

#endif // MODULE_H