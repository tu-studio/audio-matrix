#ifndef AMBIENCODERCONFIG_H
#define AMBIENCODERCONFIG_H

#include <ModuleConfig.h>

struct AmbiEncoderConfig : ModuleConfig{
    int order;

    const Modules module_type() const override{
        return Modules::AMBI_ENCODER;
    }
};

typedef std::shared_ptr<AmbiEncoderConfig> AmbiEncoderConfigPtr;

#endif //AMBIENCODERCONFIG_H