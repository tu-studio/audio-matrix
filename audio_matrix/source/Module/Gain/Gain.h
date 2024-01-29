#ifndef GAIN_H
#define GAIN_H

#include <Module.h>

class Gain : public Module {

public:
    Gain();
    Gain(int input_channels);
    ~Gain();

    int initialize(int input_channels, jack_nframes_t nframes, jack_nframes_t sample_rate, std::shared_ptr<lo::ServerThread> = nullptr) override;
    void process(AudioBuffer &buffer, jack_nframes_t nframes) override;

    void set_gain(float gain);
    float get_gain();

private:
    float gain = 0.f;

    static int osc_gain_callback(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);

};

#endif // GAIN_H