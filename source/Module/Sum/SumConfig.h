#ifndef SUMCONFIG_H
#define SUMCONFIG_H
#include <ModuleConfig.h>

struct SumConfig: ModuleConfig
{
    const Modules module_type() const override {
        return Modules::SUM;
    }
};

using SumConfigPtr = std::shared_ptr<SumConfig>;

#endif //SUMCONFIG_H
