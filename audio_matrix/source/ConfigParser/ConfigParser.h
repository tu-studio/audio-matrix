#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>

class ConfigParser
{

public:
    ConfigParser(std::string config_file);
    ~ConfigParser();

    YAML::Node get_config();
    YAML::Node get_track_config();
private:
    /* data */
    YAML::Node config;
    int port;
    int n_input_channels;

};


#endif // CONFIGPARSER_H