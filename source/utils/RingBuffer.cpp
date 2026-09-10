#include <RingBuffer.h>
#include <cstdint>

void RingBuffer::initialize(size_t numChannels, size_t maxNumSamples) {
    buffer.initialize(numChannels, maxNumSamples);
    readPos.resize(numChannels);
    writePos.resize(numChannels);
    clear();
    for(size_t channel = 0; channel < numChannels; ++channel) {
        readPos[channel] = 0;
        writePos[channel] = 0;
    }
}

void RingBuffer::clear() {
    buffer.clear();
}

[[nodiscard]] float RingBuffer::delay(size_t channel, size_t offset, float x) {
    // doesn't need caching but makes it more legible
    const auto N = getMaxNumSamples(channel);
    size_t wp = writePos[channel];

    // write first, so that we never read stale samples
    // else, offset = 0 would equal to offset = N
    buffer.setSample(channel, wp, x);

    // read position is always just lagging behind the write position by the
    // offset amount (aka. the delay time)
    // using signed value because sub-zero result would underflow `size_t`!
    std::int64_t rp = wp - offset;
    while(rp < 0) rp += N;  // lower bound
    if(rp >= N)   rp -= N;  // upper bound
    float delayed = buffer.getSample(channel, rp);
    readPos[channel] = rp; // store again

    // update write position
    ++wp;
    if(wp >= N)   wp -= N;  // upper bound
    writePos[channel] = wp; // store again

    return delayed;
}

size_t RingBuffer::getMaxNumSamples([[maybe_unused]] size_t channel) const {
    return buffer.getNumSamples();
}
