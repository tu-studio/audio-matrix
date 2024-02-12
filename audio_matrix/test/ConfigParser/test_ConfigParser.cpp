#include <gtest/gtest.h>
#include <time.h>
#include <benchmark/benchmark.h>
#include <ConfigParser.h>
#include <GainConfig.h>
#include <string>
#include <yaml-cpp/yaml.h>

TEST(ConfigParser, basic_parse){
    std::string file_name = SIMPLE_WFS_MIXER_CONFIG_PATH;
    ConfigParser parser = ConfigParser(file_name);
    std::shared_ptr<AudioMatrixConfig> conf = parser.get_config();
    auto track_wfs = conf->tracks[0];
    ASSERT_EQ(track_wfs.name, "wfs");
    auto module_gain = track_wfs.modules[0];
    ASSERT_EQ(module_gain->module_type(), Modules::GAIN);
    auto module_gain_cast = std::dynamic_pointer_cast<GainConfig>(module_gain);
    for(auto track = conf->tracks.begin(); track != conf->tracks.end(); ++track){
        std::cout << track->name << std::endl;

        for (const std::shared_ptr<ModuleConfig> mc : track->modules){
            switch (mc->module_type())
            {
            case Modules::GAIN:
                {
                    std::cout << "\tgain" << std::endl;
                    auto module_gain = std::dynamic_pointer_cast<GainConfig>(mc);
                    std::cout << "\t\tfactor: " << module_gain->factor << std::endl;
                }
                break;
            case Modules::FILTER:
                {
                    std::cout << "\tfilter" << std::endl;
                    auto module_filter = std::dynamic_pointer_cast<FilterConfig>(mc);
                    std::cout << "\t\ttype: " << module_filter->type << ", freq: " << module_filter->freq << std::endl;
                }
                break;
            default:
                break;
            }
        }
    }
    // YAML::Node config = parser.get_config();

    // assert(config.IsMap());
    
}


