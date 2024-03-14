#include <gtest/gtest.h>
#include <time.h>
#include <benchmark/benchmark.h>
#include <Gain.h>
#include <Track.h>
#include <AudioBuffer.h>
#include <ConfigParser.h>
#include <OSCServer.h>
#include <HostAudioConfig.h>

#ifndef RANDOMDOUBLE
#define RANDOMDOUBLE

static double randomDouble(int LO, int HI){
  return (double) LO + (double) (std::rand()) / ((double) (RAND_MAX/(HI-LO)));
}

#endif // RANDOMDOUBLE

static void BM_Track_process_buffer(benchmark::State& state) {

    int channel_count = 2;
    HostAudioConfig host_audio_config(512, 48000);

    AudioBufferF test_input_buffer(channel_count, host_audio_config.m_host_buffer_size);
    AudioBufferF test_output_buffer(channel_count, host_audio_config.m_host_buffer_size);
    
    std::string file_name = TEST_CONFIG_0;
    
    ConfigParser parser = ConfigParser(file_name);
    OSCServer osc_server(parser.get_config()->port);
    TrackConfig track_config = parser.get_config()->tracks[0];

    Track test_track(track_config, osc_server.get_server_thread());
    test_track.initialize(channel_count, 0);
    test_track.prepare(host_audio_config);

    std::srand(std::time(0));
    for(int i = 0; i < channel_count; i++){
        for(int j = 0; j < host_audio_config.m_host_buffer_size; j++){
            test_input_buffer.setSample(i, j, randomDouble(-1, 1));
        }
    }

    std::shared_ptr<Gain> test_gain = std::dynamic_pointer_cast<Gain>(test_track.get_modules()[0]);
    for (int i = 0; i < channel_count; i++) {
        test_gain->set_gain(i, 0.5f);
    }

    float** in = test_input_buffer.getArrayOfWritePointers();
    float** out = test_output_buffer.getArrayOfWritePointers();
    size_t nframes = host_audio_config.m_host_buffer_size;

    for (auto _ : state) {
    // This code gets timed
        test_track.process(in, out, nframes);
    }
}
// Register the function as a benchmark
BENCHMARK(BM_Track_process_buffer);

TEST(Track, process_buffer){

    int channel_count = 2;
    HostAudioConfig host_audio_config(512, 48000);

    AudioBufferF test_input_buffer(channel_count, host_audio_config.m_host_buffer_size);
    AudioBufferF test_output_buffer(channel_count, host_audio_config.m_host_buffer_size);
    
    std::string file_name = TEST_CONFIG_0;
    
    ConfigParser parser = ConfigParser(file_name);
    OSCServer osc_server(parser.get_config()->port);
    TrackConfig track_config = parser.get_config()->tracks[0];

    Track test_track(track_config, osc_server.get_server_thread());
    test_track.initialize(channel_count, 0);
    test_track.prepare(host_audio_config);

    std::srand(std::time(0));
    for(int i = 0; i < channel_count; i++){
        for(int j = 0; j < host_audio_config.m_host_buffer_size; j++){
            test_input_buffer.setSample(i, j, randomDouble(-1, 1));
        }
    }

    std::shared_ptr<Gain> test_gain = std::dynamic_pointer_cast<Gain>(test_track.get_modules()[0]);
    for (int i = 0; i < channel_count; i++) {
        test_gain->set_gain(i, 0.5f);
    }

    float** in = test_input_buffer.getArrayOfWritePointers();
    float** out = test_output_buffer.getArrayOfWritePointers();
    size_t nframes = host_audio_config.m_host_buffer_size;

    test_track.process(in, out, nframes);

    for (int i = 0; i < channel_count; i++) {
        for (int j = 0; j < host_audio_config.m_host_buffer_size; j++) {
            EXPECT_FLOAT_EQ(
                test_input_buffer.getSample(i, j) * 0.5f,
                test_output_buffer.getSample(i, j)
            );
        }
    }
}

TEST(Track, benchmark){
    benchmark::SetBenchmarkFilter("BM_Track_process_buffer");
    benchmark::RunSpecifiedBenchmarks();
    std::cout << "----------------------------------------------------------------------------" << std::endl;
}
