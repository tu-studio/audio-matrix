#ifndef MODULE_DISTANCEGAINCONFIG
#define MODULE_DISTANCEGAINCONFIG

#include <ModuleConfig.h>

struct DistanceGainConfig : ModuleConfig {
    float initial_distance;

    const Modules module_type() const override {
        return Modules::DISTANCE_GAIN;
    }
};

typedef std::shared_ptr<DistanceGainConfig> DistanceGainConfigPtr;

#endif //MODULE_DISTANCEGAINCONFIG