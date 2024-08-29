#include <Filter.h>
#include <FilterConfig.h>
#include <gtest/gtest.h>
#include <AudioBuffer.h>
#include <HostAudioConfig.h>
#include <math.h>
#include <fstream>
#include <iostream>


TEST(Filter, simple_lp_test){
    float sample_rate = 48000;
    float buf_size = 1024;
    HostAudioConfig host_config = HostAudioConfig(buf_size, sample_rate);


    int n_channels = 1;

    FilterConfig conf = FilterConfig();
    conf.freq = 200;
    conf.type = FilterType::LP;
    conf.order = 4;
    FilterConfigPtr conf_ptr = std::make_shared<FilterConfig>(conf);


    float freq_1 = 100;
    float freq_2 = 400;

    AudioBufferF buffer = AudioBufferF(n_channels,buf_size);
    for (size_t i = 0; i < buf_size; i++)
    {
        double val = sin(freq_1 * (i / sample_rate) * 2.0 * M_PI) + sin(freq_2 * (i / sample_rate) * 2.0 * M_PI);
        val = val / 2;
        buffer.setSample(0, i, val);
    }

    std::ofstream pre_file;
    pre_file.open("filter_test_pre.txt");
    for (size_t i = 0; i < buf_size; i++)
    {
        pre_file << buffer.getSample(0, i) << ";";
    }
    pre_file.close();
    
    Filter filter = Filter(conf_ptr, nullptr);
    filter.initialize(1);
    filter.prepare(host_config);

    filter.process(buffer, buf_size);

    std::ofstream post_file;
    post_file.open("filter_test_post.txt");
    for (size_t i = 0; i < buf_size; i++)
    {
        post_file << buffer.getSample(0, i) << ";";
    }
    post_file.close();
}