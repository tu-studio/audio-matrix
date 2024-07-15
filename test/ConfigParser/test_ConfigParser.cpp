#include <gtest/gtest.h>
#include <time.h>
#include <benchmark/benchmark.h>
#include <ConfigParser.h>
#include <GainConfig.h>
#include <string>
#include <yaml-cpp/yaml.h>

TEST(ConfigParser, basic_parse){
    
    std::string file_name = TEST_CONFIG_0;
    ConfigParser parser = ConfigParser(file_name);
    std::shared_ptr<AudioMatrixConfig> conf = parser.get_config();
    
    auto track_test = conf->tracks[0];
    ASSERT_EQ( track_test.name, "test_track" );
    
    auto module_gain_0 = track_test.modules[0];
    ASSERT_EQ( module_gain_0->module_type(), Modules::GAIN );
    auto module_gain_cast_0 = std::dynamic_pointer_cast<GainConfig>(module_gain_0);
    ASSERT_FLOAT_EQ( module_gain_cast_0->factor, 3.0f );
    ASSERT_EQ( module_gain_cast_0->osc_controllable, true );
    ASSERT_EQ( module_gain_cast_0->osc_path, std::string("/gain") );


    // for(auto track = conf->tracks.begin(); track != conf->tracks.end(); ++track){
    //     std::cout << track->name << std::endl;

    //     for (const std::shared_ptr<ModuleConfig> mc : track->modules){
    //         switch (mc->module_type())
    //         {
    //         case Modules::GAIN:
    //             {
    //                 std::cout << "\tgain" << std::endl;
    //                 auto module_gain = std::dynamic_pointer_cast<GainConfig>(mc);
    //                 std::cout << "\t\tfactor: " << module_gain->factor << std::endl;
    //             }
    //             break;
    //         case Modules::FILTER:
    //             {
    //                 std::cout << "\tfilter" << std::endl;
    //                 auto module_filter = std::dynamic_pointer_cast<FilterConfig>(mc);
    //                 std::cout << "\t\ttype: " << module_filter->type << ", freq: " << module_filter->freq << std::endl;
    //             }
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    // }
}

TEST(ConfigParser, more_complex_parse){
    
    std::string file_name = TEST_CONFIG_0;
    ConfigParser parser = ConfigParser(file_name);
    std::shared_ptr<AudioMatrixConfig> conf = parser.get_config();
    

    auto track_test = conf->tracks[1];
    ASSERT_EQ( track_test.name, "test_track_2" );

    auto module_gain_1 = track_test.modules[0];
    ASSERT_EQ( module_gain_1->module_type(), Modules::GAIN );
    ASSERT_EQ( module_gain_1->osc_controllable, false );
    auto module_gain_cast_1 = std::dynamic_pointer_cast<GainConfig>(module_gain_1);
    ASSERT_FLOAT_EQ( module_gain_cast_1->factor, 3.0f );

    auto module_gain_2 = track_test.modules[1];
    ASSERT_EQ( module_gain_2->module_type(), Modules::GAIN );
    ASSERT_EQ( module_gain_2->osc_controllable, false );

    auto module_gain_cast_2 = std::dynamic_pointer_cast<GainConfig>(module_gain_2);
    ASSERT_FLOAT_EQ( module_gain_cast_2->factor, 1.0f );

    auto module_gain_3 = track_test.modules[2];
    ASSERT_EQ( module_gain_3->module_type(), Modules::GAIN );
    auto module_gain_cast_3 = std::dynamic_pointer_cast<GainConfig>(module_gain_3);
    ASSERT_FLOAT_EQ( module_gain_cast_3->factor, 1.0f );

    auto module_sum_1 = track_test.modules[3];
    ASSERT_EQ( module_sum_1->module_type(), Modules::SUM );
    auto module_sum_cast_1 = std::dynamic_pointer_cast<SumConfig>(module_sum_1);

    auto module_sum_2 = track_test.modules[4];
    ASSERT_EQ( module_sum_2->module_type(), Modules::SUM );
    auto module_sum_cast_2 = std::dynamic_pointer_cast<SumConfig>(module_sum_2);

    auto module_filter_1 = track_test.modules[5];
    ASSERT_EQ( module_filter_1->module_type(), Modules::FILTER );
    auto module_filter_cast_1 = std::dynamic_pointer_cast<FilterConfig>(module_filter_1);
    ASSERT_EQ( module_filter_cast_1->order, 4 );
    ASSERT_FLOAT_EQ( module_filter_cast_1->freq, 150 );
    ASSERT_EQ( module_filter_cast_1->type, FilterType::HP );

    auto module_filter_2 = track_test.modules[6];
    ASSERT_EQ( module_filter_2->module_type(), Modules::FILTER );
    ASSERT_EQ( module_filter_1->osc_controllable, false );

    auto module_filter_cast_2 = std::dynamic_pointer_cast<FilterConfig>(module_filter_2);
    ASSERT_EQ( module_filter_cast_2->order, 1 );
    ASSERT_FLOAT_EQ( module_filter_cast_2->freq, 1000 );
    ASSERT_EQ( module_filter_cast_2->type, FilterType::LP );

    // for(auto track = conf->tracks.begin(); track != conf->tracks.end(); ++track){
    //     std::cout << track->name << std::endl;

    //     for (const std::shared_ptr<ModuleConfig> mc : track->modules){
    //         switch (mc->module_type())
    //         {
    //         case Modules::GAIN:
    //             {
    //                 std::cout << "\tgain" << std::endl;
    //                 auto module_gain = std::dynamic_pointer_cast<GainConfig>(mc);
    //                 std::cout << "\t\tfactor: " << module_gain->factor << std::endl;
    //             }
    //             break;
    //         case Modules::FILTER:
    //             {
    //                 std::cout << "\tfilter" << std::endl;
    //                 auto module_filter = std::dynamic_pointer_cast<FilterConfig>(mc);
    //                 std::cout << "\t\ttype: " << module_filter->type << ", freq: " << module_filter->freq << std::endl;
    //             }
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    // }
}

TEST(ConfigParser, ambisonics_parse){
    
    std::string file_name = TEST_CONFIG_0;
    ConfigParser parser = ConfigParser(file_name);
    std::shared_ptr<AudioMatrixConfig> conf = parser.get_config();
    

    auto track_test = conf->tracks[2];
    ASSERT_EQ( track_test.name, "test_track_ambisonics" );

    auto module_hoa_1 = track_test.modules[0];
    ASSERT_EQ( module_hoa_1->module_type(), Modules::AMBI_ENCODER );
    ASSERT_EQ( module_hoa_1->osc_controllable, false );
    auto module_hoa_cast_1 = std::dynamic_pointer_cast<AmbiEncoderConfig>(module_hoa_1);
    ASSERT_EQ( module_hoa_cast_1->order, 3 );

    auto module_hoa_2 = track_test.modules[1];
    ASSERT_EQ( module_hoa_2->module_type(), Modules::AMBI_ENCODER );
    ASSERT_EQ( module_hoa_2->osc_controllable, false );
    auto module_hoa_cast_2 = std::dynamic_pointer_cast<AmbiEncoderConfig>(module_hoa_2);
    ASSERT_EQ( module_hoa_cast_2->order, 2 );

    auto module_hoa_3 = track_test.modules[2];
    ASSERT_EQ( module_hoa_3->module_type(), Modules::AMBI_ENCODER );
    ASSERT_EQ( module_hoa_3->osc_controllable, true );
    auto module_hoa_cast_3 = std::dynamic_pointer_cast<AmbiEncoderConfig>(module_hoa_3);
    ASSERT_EQ( module_hoa_cast_3->order, 3 );
  

    // for(auto track = conf->tracks.begin(); track != conf->tracks.end(); ++track){
    //     std::cout << track->name << std::endl;

    //     for (const std::shared_ptr<ModuleConfig> mc : track->modules){
    //         switch (mc->module_type())
    //         {
    //         case Modules::GAIN:
    //             {
    //                 std::cout << "\tgain" << std::endl;
    //                 auto module_gain = std::dynamic_pointer_cast<GainConfig>(mc);
    //                 std::cout << "\t\tfactor: " << module_gain->factor << std::endl;
    //             }
    //             break;
    //         case Modules::FILTER:
    //             {
    //                 std::cout << "\tfilter" << std::endl;
    //                 auto module_filter = std::dynamic_pointer_cast<FilterConfig>(mc);
    //                 std::cout << "\t\ttype: " << module_filter->type << ", freq: " << module_filter->freq << std::endl;
    //             }
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    // }
}

