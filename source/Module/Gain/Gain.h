#ifndef GAIN_H
#define GAIN_H

#include <Module.h>
#include <GainConfig.h>
class Gain : public Module {

public:
    Gain() = delete;
    Gain(std::shared_ptr<GainConfig> config, std::shared_ptr<lo::ServerThread> osc_server);
    ~Gain() = default;

    size_t initialize(size_t input_channels) override;
    void process(AudioBufferF &buffer, size_t nframes) override;

    void set_gain(size_t channel, float gain);
    float get_gain(size_t channel);

private:
    std::vector<float> m_gain;
    std::shared_ptr<GainConfig> m_config;
    static int osc_gain_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data);

};

#endif // GAIN_H