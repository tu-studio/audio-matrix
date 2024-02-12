#ifndef GAINCONFIG_H
#define GAINCONFIG_H
#include <ModuleConfig.h>

struct GainConfig: ModuleConfig
{
    float gain = 1;

    const Modules module_type() const override {
        return Modules::GAIN;
    }
};


#endif //GAINCONFIG_H