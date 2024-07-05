#ifndef MODULE_FILTERCONFIG_H
#define MODULE_FILTERCONFIG_H

#include <ModuleConfig.h>

enum FilterType {
    HP,
    LP,
    BP
};

struct FilterConfig : ModuleConfig{
    float freq;
    FilterType type;
    int order;
    
    const Modules module_type() const override {
        return Modules::FILTER;
    }
};

typedef std::shared_ptr<FilterConfig> FilterConfigPtr;

#endif //MODULE_FILTERCONFIG_H