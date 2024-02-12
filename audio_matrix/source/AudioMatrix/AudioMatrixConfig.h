#ifndef AUDIOMATRIXCONFIG_H
#define AUDIOMATRIXCONFIG_H

#include <TrackConfig.h>


struct AudioMatrixConfig{
    // analog zu Anira InferenceConfig
    int port = DEFAULT_INPUT_PORTS;
    int n_input_channels = DEFAULT_INPUT_PORTS;
    std::string osc_base_path = DEFAULT_OSC_BASE_PATH;
    std::string osc_position_path = DEFAULT_OSC_POS_PATH;
    std::vector<TrackConfig> tracks;
};


#endif //AUDIOMATRIXCONFIG_H