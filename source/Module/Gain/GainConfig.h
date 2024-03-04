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

typedef std::shared_ptr<GainConfig> GainConfigPtr;


#endif //GAINCONFIG_H