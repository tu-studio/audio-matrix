#include <Sum.h>
#include <SumConfig.h>
#include <gtest/gtest.h>
#include <AudioBuffer.h>


TEST(Sum, simple_test){
    Sum s = Sum(nullptr, nullptr);
    size_t channels = 3;
    size_t frames = 10;
    s.initialize(channels);
    AudioBufferF buffer = AudioBufferF(channels, frames);

    for (size_t i = 0; i < channels; i++)
    {
        for (size_t j = 0; j < frames; j++)
        {
            buffer.setSample(i, j, 1.0f); 
        }
        
    }

    for (size_t i = 0; i < channels; i++)
    {
        for (size_t j = 0; j < frames; j++)
        {
            EXPECT_FLOAT_EQ(1.0, buffer.getSample(i, j));
        }
        
    }

    s.process(buffer, frames);
    for (size_t i = 0; i < frames; i++)
    {
        EXPECT_FLOAT_EQ(1.0 * channels, buffer.getSample(0, i));
    }
    
    

}