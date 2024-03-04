#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>
#include <Track.h>
#include <AudioMatrixConfig.h>
#include <TrackConfig.h>

#include <ModuleConfig.h>
#include <GainConfig.h>
#include <FilterConfig.h>

class ConfigParser
{

public:
    ConfigParser(std::string config_file);
    ~ConfigParser();

    std::shared_ptr<AudioMatrixConfig> get_config();
private:

    void parse_module_osc_params(YAML::Node module, std::shared_ptr<ModuleConfig> config);
    std::shared_ptr<ModuleConfig> parse_module(YAML::Node module);
    std::shared_ptr<GainConfig> parse_module_gain(YAML::Node module);
    std::shared_ptr<FilterConfig> parse_module_filter(YAML::Node module);

    YAML::Node config;
    std::shared_ptr<AudioMatrixConfig> parsed_config;
    int port;
    int n_input_channels;
    std::string osc_base_path;
    std::string osc_position_path;


};


#endif // CONFIGPARSER_H