#ifndef TRACKCONFIG_H
#define TRACKCONFIG_H

#include <ModuleConfig.h>
#include <string>
#include <vector>

struct TrackConfig{
    std::string name;
    std::vector<ModuleConfigPtr> modules;
};

#endif //TRACKCONFIG_H
