#ifndef AUDIOMATRIXCONFIG_H
#define AUDIOMATRIXCONFIG_H

#include <TrackConfig.h>
#include <vector>

#define DEFAULT_INPUT_PORTS 64
#define DEFAULT_PORT 12345

struct AudioMatrixConfig{
    int port = DEFAULT_INPUT_PORTS;
    int n_input_channels = DEFAULT_INPUT_PORTS;
    std::vector<TrackConfig> tracks;
};

typedef std::shared_ptr<AudioMatrixConfig> AudioMatrixConfig_ptr

#endif //AUDIOMATRIXCONFIG_H