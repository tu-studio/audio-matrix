#ifndef AUDIOMATRIXCONFIG_H
#define AUDIOMATRIXCONFIG_H

#include <TrackConfig.h>


struct AudioMatrixConfig{
    // analog zu Anira InferenceConfig
    const int port;
    const int n_input_channels;
    const std::string osc_base_path;
    const std::string osc_position_path;
    const std::vector<TrackConfig> tracks;
    AudioMatrixConfig(int port, int n_input_channels, const std::string osc_base_path, std::string osc_position_path, std::vector<TrackConfig> tracks)
        : port(port), n_input_channels(n_input_channels), osc_base_path(osc_base_path), osc_position_path(osc_position_path), tracks(tracks) {}
};


#endif //AUDIOMATRIXCONFIG_H