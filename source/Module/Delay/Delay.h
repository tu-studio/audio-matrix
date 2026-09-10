#ifndef MODULE_DELAY_H
#define MODULE_DELAY_H

#include <Module.h>
#include <DelayConfig.h>
#include <RingBuffer.h>
#include <CustomAtomic.h>
#include <vector>

class Delay : public Module {
    static constexpr float max_delay_s = 1.f;
public:
    Delay() = delete;
    Delay(DelayConfigPtr config, std::shared_ptr<lo::ServerThread> osc_server);
    ~Delay() = default;

    size_t initialize(size_t input_channels) override;
    void prepare(HostAudioConfig host_audio_config) override;
    void process(AudioBufferF &buffer, size_t nframes) override;
    void set_time_ms(size_t channel, float time_ms);
    void set_enabled(size_t channel, bool is_enabled);

private:
    DelayConfigPtr m_config;
    RingBuffer m_ringbuffer;
    size_t m_max_delay_samps;
    double m_samplerate;
    std::vector<atomic_int> m_delay_samps;
    std::vector<atomic_bool> m_is_enabled;

    static int osc_delay_frequency_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data);
};
#endif // MODULE_DELAY_H
