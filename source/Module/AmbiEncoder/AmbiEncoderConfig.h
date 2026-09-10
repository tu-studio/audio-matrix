#ifndef AMBIENCODERCONFIG_H
#define AMBIENCODERCONFIG_H

#include <ModuleConfig.h>

struct AmbiEncoderConfig : ModuleConfig{
    int order;
    float nearfield_size;

    const Modules module_type() const override{
        return Modules::AMBI_ENCODER;
    }
};

using AmbiEncoderConfigPtr = std::shared_ptr<AmbiEncoderConfig>;

#endif //AMBIENCODERCONFIG_H
