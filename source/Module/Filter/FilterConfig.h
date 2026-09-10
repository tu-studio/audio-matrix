#ifndef MODULE_FILTERCONFIG_H
#define MODULE_FILTERCONFIG_H

#include <ModuleConfig.h>

enum FilterType {
    HP,
    LP,
};

struct FilterConfig : ModuleConfig{
    float freq;
    FilterType type;
    int order;
    
    const Modules module_type() const override {
        return Modules::FILTER;
    }
};

using FilterConfigPtr = std::shared_ptr<FilterConfig>;

#endif //MODULE_FILTERCONFIG_H
