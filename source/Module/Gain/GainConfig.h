#ifndef GAINCONFIG_H
#define GAINCONFIG_H
#include <ModuleConfig.h>

struct GainConfig: ModuleConfig
{
    float factor = 1;

    const Modules module_type() const override {
        return Modules::GAIN;
    }
};

using GainConfigPtr = std::shared_ptr<GainConfig>;


#endif //GAINCONFIG_H
