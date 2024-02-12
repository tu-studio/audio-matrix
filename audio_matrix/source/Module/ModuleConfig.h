#ifndef MODULECONFIG_H
#define MODULECONFIG_H

#include <string>

enum Modules{
    DUMMY,
    GAIN,
    FILTER,
    DISTANCE_COMPENSATOR,
    SUM
};

struct ModuleConfig{
    bool osc_controllable;
    std::string osc_path;

    virtual const Modules module_type() const {
        return Modules::DUMMY;
    }
};


#endif //MODULECONFIG_H