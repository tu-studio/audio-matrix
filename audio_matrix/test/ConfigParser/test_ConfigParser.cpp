#include <gtest/gtest.h>
#include <time.h>
#include <benchmark/benchmark.h>
#include <ConfigParser.h>
#include <string>
#include <yaml-cpp/yaml.h>

TEST(ConfigParser, basic_parse){
    std::string file_name = "/home/leto/ak-cloud/STUDIO/seamless-v2/seamless_mixer_conf.yml";
    ConfigParser parser = ConfigParser(file_name);
    // YAML::Node config = parser.get_config();

    // assert(config.IsMap());
    
}


