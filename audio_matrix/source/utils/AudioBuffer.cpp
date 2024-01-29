#include <AudioBuffer.h>

AudioBuffer::AudioBuffer() {
}

AudioBuffer::AudioBuffer(int channels, int frames) {
    initialize(channels, frames);
}

AudioBuffer::~AudioBuffer() {
}

void AudioBuffer::initialize(int channels, int frames) {
    buffer.resize(channels);
    for (int i = 0; i < channels; i++) {
        buffer[i].resize(frames);
    }
}

float AudioBuffer::get_sample(int channel, int frame) {
    return buffer[channel][frame];
}

void AudioBuffer::set_sample(int channel, int frame, float value) {
    buffer[channel][frame] = value;
}

void AudioBuffer::clear() {
    for (int i = 0; i < buffer.size(); i++) {
        for (int j = 0; j < buffer[i].size(); j++) {
            buffer[i][j] = 0.0f;
        }
    }
}

float* AudioBuffer::get_sample_pointer(int channel, int sample) {
    return &buffer[channel][sample];
}

float* AudioBuffer::get_channel_pointer(int channel) {
    return buffer[channel].data();
}
