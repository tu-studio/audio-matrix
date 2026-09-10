#ifndef MODULECONFIG_H
#define MODULECONFIG_H

#include <string>
#include <memory>

enum Modules{
    DUMMY,
    GAIN,
    FILTER,
    DISTANCE_GAIN,
    SUM,
    AMBI_ENCODER,
    DELAY,
};

struct ModuleConfig{
    bool osc_controllable = false;
    std::string osc_path;

    virtual const Modules module_type() const {
        return Modules::DUMMY;
    }
};

using ModuleConfigPtr = std::shared_ptr<ModuleConfig>;
#endif //MODULECONFIG_H
