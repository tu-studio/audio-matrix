#ifndef TRACKCONFIG_H
#define TRACKCONFIG_H

#include <ModuleConfig.h>
#include <string>
#include <vector>
#include <memory>

struct TrackConfig{
    std::string name;
    std::vector<std::shared_ptr<ModuleConfig>> modules;
};


#endif //TRACKCONFIG_H