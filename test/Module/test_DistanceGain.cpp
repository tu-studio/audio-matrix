#include <DistanceGain.h>
#include <DistanceGainConfig.h>
#include <gtest/gtest.h>
#include <AudioBuffer.h>

TEST(DistanceGain, simple_test){
    DistanceGain d = DistanceGain(nullptr, nullptr);
    std::cout << "starting first process" << std::endl;

    size_t channels = 1;
    size_t frames = 10;
    d.initialize(channels);
    AudioBufferF buffer = AudioBufferF(channels, frames);

    for (size_t i = 0; i < channels; i++)
    {
        for (size_t j = 0; j < frames; j++)
        {
            buffer.setSample(i, j, 1.0f); 
        }
        
    }
    d.set_distance(0, 1.5);
    std::cout << "starting first process" << std::endl;
    d.process(buffer,frames);

    for (size_t i = 0; i < channels; i++)
    {
        for (size_t j = 0; j < frames; j++)
        {
            buffer.setSample(i, j, 1.0f); 
        }
        
    }

    d.process(buffer, frames);

    for (size_t i = 0; i < frames; i++)
    {
        EXPECT_FLOAT_EQ( 0.8/1.5, buffer.getSample(0, i) );
    }

}