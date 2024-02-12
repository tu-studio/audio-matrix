#ifndef FILTERCONFIG_H
#define FILTERCONFIG_H

#include <ModuleConfig.h>

enum FilterType {
    HP,
    LP,
    BP
};

struct FilterConfig : ModuleConfig{
    float freq;
    FilterType type;
    
    const Modules module_type() const override {
        return Modules::FILTER;
    }
};

#endif //FILTERCONFIG_H