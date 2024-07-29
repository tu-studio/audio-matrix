#ifndef MODULE_DELAY_CONFIG_H
#define MODULE_DELAY_CONFIG_H

#include <ModuleConfig.h>

struct DelayConfig : ModuleConfig{
    float delay_time;
    
    const Modules module_type() const override {
        return Modules::DELAY;
    }
};

typedef std::shared_ptr<DelayConfig> DelayConfigPtr;

#endif //MODULE_DELAY_CONFIG_H