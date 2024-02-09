#ifndef TRACKCONFIG_H
#define TRACKCONFIG_H

#include <ModuleConfig.h>
#include <string>

struct TrackConfig{
    const std::string name;
    const std::vector<ModuleConfig> modules;
    
    TrackConfig(std::string name, std::vector<ModuleConfig> modules)
        : name(name), modules(modules) {}

};


#endif //TRACKCONFIG_H