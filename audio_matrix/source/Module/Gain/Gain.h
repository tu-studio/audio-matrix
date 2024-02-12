#ifndef GAIN_H
#define GAIN_H

#include <Module.h>
#include <GainConfig.h>
class Gain : public Module {

public:
    Gain() = delete;
    Gain(std::shared_ptr<GainConfig> config);
    ~Gain() = default;

    int initialize(int input_channels, jack_nframes_t nframes, jack_nframes_t sample_rate, std::shared_ptr<lo::ServerThread> = nullptr) override;
    void process(AudioBuffer &buffer, jack_nframes_t nframes) override;

    void set_gain(float gain);
    float get_gain();

private:
    float m_gain = 0.f;
    std::shared_ptr<GainConfig> m_config;
    static int osc_gain_callback(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);

};

#endif // GAIN_H