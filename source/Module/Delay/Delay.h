#ifndef MODULE_DELAY_H
#define MODULE_DELAY_H

#include <Module.h>
#include <DelayConfig.h>
#include <RingBuffer.h>

class Delay : public Module {

public:
    Delay() = delete;
    Delay(std::shared_ptr<DelayConfig> config, std::shared_ptr<lo::ServerThread> osc_server);
    ~Delay() = default;

    size_t initialize(size_t input_channels) override;
    void prepare(HostAudioConfig host_audio_config) override;
    void process(AudioBufferF &buffer, size_t nframes) override;

    

private:
    std::shared_ptr<DelayConfig> m_config;
    RingBuffer m_ringbuffer;
};
#endif // MODULE_DELAY_H