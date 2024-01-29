#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <vector>

class AudioBuffer {
public:
    AudioBuffer();
    AudioBuffer(int channels, int frames);
    ~AudioBuffer();

    void initialize(int channels, int frames);

    float get_sample(int channel, int frame);
    void set_sample(int channel, int frame, float value);

    void clear();

    float* get_sample_pointer(int channel, int sample);
    float* get_channel_pointer(int channel);
    
private:
    std::vector<std::vector<float>> buffer;
    
};

#endif // AUDIOBUFFER_H