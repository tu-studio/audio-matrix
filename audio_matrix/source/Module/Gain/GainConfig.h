#ifndef GAINCONFIG_H
#define GAINCONFIG_H
#include <ModuleConfig.h>

struct GainConfig: private ModuleConfig
{
    int gain;
};


#endif //GAINCONFIG_H