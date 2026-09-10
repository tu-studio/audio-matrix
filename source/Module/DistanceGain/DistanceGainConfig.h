#ifndef MODULE_DISTANCEGAINCONFIG
#define MODULE_DISTANCEGAINCONFIG

#include <ModuleConfig.h>

struct DistanceGainConfig : ModuleConfig {
    float initial_distance;

    const Modules module_type() const override {
        return Modules::DISTANCE_GAIN;
    }
};

using DistanceGainConfigPtr = std::shared_ptr<DistanceGainConfig>;

#endif //MODULE_DISTANCEGAINCONFIG
