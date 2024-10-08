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
#include <AmbiEncoderConfig.h>
#include <SumConfig.h>
#include <DistanceGainConfig.h>
#include <DelayConfig.h>


class ConfigParser
{

public:
    ConfigParser(std::string config_file);
    ~ConfigParser();

    std::shared_ptr<AudioMatrixConfig> get_config();
private:

    void parse_module_osc_params(YAML::Node module, std::shared_ptr<ModuleConfig> config);
    ModuleConfigPtr parse_module(YAML::Node module);

    // Module Specific Config Parsers
    ModuleConfigPtr parse_module_gain(YAML::Node module);
    ModuleConfigPtr parse_module_filter(YAML::Node module);
    ModuleConfigPtr parse_module_ambi_encoder(YAML::Node module);
    ModuleConfigPtr parse_module_sum(YAML::Node module);
    ModuleConfigPtr parse_module_distance_gain(YAML::Node module);
    ModuleConfigPtr parse_module_delay(YAML::Node module);

    template <typename T>
    T get_config_option(YAML::Node module, const std::string &option_name, T default_value, bool is_main_parameter=false, bool is_required=false, bool warn_when_default=true);


    YAML::Node config;
    std::shared_ptr<AudioMatrixConfig> parsed_config;
    int port;
    int n_input_channels;
};


#endif // CONFIGPARSER_H