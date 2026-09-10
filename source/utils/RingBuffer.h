#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <cstddef>
#include <vector>
#include <AudioBuffer.h>

class RingBuffer {
public:
    RingBuffer() = default;
    void initialize(size_t numChannels, size_t maxNumSamples);

    void clear();
    [[nodiscard]] float delay(size_t channel, size_t offset, float x);
    size_t getMaxNumSamples([[maybe_unused]] size_t channel) const;
private:
    AudioBuffer<float> buffer;
    std::vector<size_t> readPos;
    std::vector<size_t> writePos;
};

#endif //RINGBUFFER_H
