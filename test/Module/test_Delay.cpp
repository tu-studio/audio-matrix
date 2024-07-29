#include <Delay.h>
#include <DelayConfig.h>
#include <gtest/gtest.h>
#include <AudioBuffer.h>

TEST(Delay, simple_test){
    DelayConfigPtr conf = std::make_shared<DelayConfig>();
    conf->delay_time = 500;
    size_t channels = 1;
    size_t frames = 50;
    HostAudioConfig host_audio_config(frames,100);

    Delay d(conf, nullptr);
    d.initialize(channels);
    d.prepare(host_audio_config);

    AudioBufferF initial_buffer(channels, frames);
    AudioBufferF process_buffer(channels, frames);

    for (size_t i = 0; i < frames; i++)
    {
        initial_buffer.setSample(0, i, i);
        process_buffer.setSample(0, i, i);
    }

    d.process(process_buffer, frames);
    
    for (size_t i = 0; i < frames; i++)
    {
        EXPECT_FLOAT_EQ(process_buffer.getSample(0, i), 0.0f);
    }
    process_buffer.clear();
    d.process(process_buffer, frames);

    for (size_t i = 0; i < frames; i++)
    {
        EXPECT_FLOAT_EQ(process_buffer.getSample(0, i), initial_buffer.getSample(0, i));
    }
    
    
}