#ifndef MODULE_DISTANCE_GAIN_H
#define MODULE_DISTANCE_GAIN_H

#include <Module.h>
#include <DistanceGainConfig.h>
#include <vector>
#include <math.h>

class DistanceGain : public Module {

public:
    DistanceGain() = delete;
    DistanceGain(std::shared_ptr<DistanceGainConfig> config, std::shared_ptr<lo::ServerThread> osc_server);
    ~DistanceGain() = default;

    size_t initialize(size_t input_channels) override;
    void process(AudioBufferF &buffer, size_t nframes) override;

    void set_distance(size_t channel, float distance);
    float get_distance(size_t channel);

private:
    std::vector<float> m_prev_gain;
    std::vector<float> m_gain;
    std::shared_ptr<DistanceGainConfig> m_config;
    static int distance_osc_callback(const char *path, const char *types, lo_arg **argv, int argc, lo_message data, void *user_data);

};
#endif // MODULE_DISTANCE_GAIN_H