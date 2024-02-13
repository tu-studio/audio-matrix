#ifndef MODULE_H
#define MODULE_H

#include <jack/jack.h>
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <AudioBuffer.h>
#include <string>
#include <iostream>
#include <HostAudioConfig.h>

class Module {

public:
    Module();
    ~Module();

    virtual size_t initialize(size_t input_channels) = 0;
    virtual void prepare(HostAudioConfig host_audio_config);
    virtual void process(AudioBufferF& buffer, size_t nframes) = 0;

protected:
    size_t m_n_input_channels;
    size_t m_n_output_channels;
};

#endif // MODULE_H