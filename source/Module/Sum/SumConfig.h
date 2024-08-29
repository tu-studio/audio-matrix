#ifndef SUMCONFIG_H
#define SUMCONFIG_H
#include <ModuleConfig.h>

struct SumConfig: ModuleConfig
{
    const Modules module_type() const override {
        return Modules::SUM;
    }
};

typedef std::shared_ptr<SumConfig> SumConfigPtr;

#endif //SUMCONFIG_H