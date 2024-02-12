// #include <gtest/gtest.h>
// #include <time.h>
// #include <benchmark/benchmark.h>
// #include <Gain.h>
// #include <Track.h>
// #include <AudioBuffer.h>

// #ifndef RANDOMDOUBLE
// #define RANDOMDOUBLE

// static double randomDouble(int LO, int HI){
//   return (double) LO + (double) (std::rand()) / ((double) (RAND_MAX/(HI-LO)));
// }

// #endif // RANDOMDOUBLE

// static void BM_ModuleGain_process_buffer(benchmark::State& state) {
//     // // Perform setup here
//     // std::srand(std::time(0));

//     // int channel_count = 2;
//     // int frame_count = 512;
//     // int sample_rate = 44100;
//     // TrackConfig track_config;
//     // Track test_track;
//     // test_track.initialize(channel_count, frame_count);
//     // for(int i = 0; i < channel_count; i++){
//     //     for(int j = 0; j < frame_count; j++){
//     //         test_track.get_buffer().set_sample(i, j, randomDouble(-1, 1));
//     //     }
//     // }
//     // Gain test_gain;
//     // test_gain.initialize(channel_count, frame_count, sample_rate);
//     // test_gain.set_gain(0.5f);

//     // for (auto _ : state) {
//     // // This code gets timed
//     //     test_gain.process(test_track.get_buffer(), frame_count);
//     // }
// }
// // Register the function as a benchmark
// BENCHMARK(BM_ModuleGain_process_buffer);

// TEST(ModuleGain, process_buffer){
//     // Perform setup here
//     // "std::srand(std::time(0));

//     // int channel_count = 2;
//     // int frame_count = 512;
//     // int sample_rate = 44100;

//     // Track test_track;
//     // test_track.initialize(channel_count, frame_count);
//     // for(int i = 0; i < channel_count; i++){
//     //     for(int j = 0; j < frame_count; j++){
//     //         test_track.get_buffer().set_sample(i, j, randomDouble(-1, 1));
//     //     }
//     // }
//     // Gain test_gain;
//     // test_gain.initialize(channel_count, frame_count, sample_rate);
//     // test_gain.set_gain(0.5f);

//     // AudioBuffer test_buffer;
//     // test_buffer.initialize(channel_count, frame_count);
//     // for (int i = 0; i < channel_count; i++) {
//     //     float* channel = test_buffer.get_channel_pointer(i);
//     //     for (int j = 0; j < frame_count; j++) {
//     //         channel[j] = test_track.get_buffer().get_sample(i, j);
//     //     }
//     // }

//     // test_gain.process(test_track.get_buffer(), frame_count);

//     // for (int i = 0; i < channel_count; i++) {
//     //     for (int j = 0; j < frame_count; j++) {
//     //         EXPECT_FLOAT_EQ(
//     //             test_buffer.get_sample(i, j) * 0.5f,
//     //             test_track.get_buffer().get_sample(i, j)
//     //         );
//     //     }
//     // }"
// }

// TEST(ModuleGain, benchmark){
//     benchmark::SetBenchmarkFilter("BM_ModuleGain_process_buffer");
//     benchmark::RunSpecifiedBenchmarks();
//     std::cout << "----------------------------------------------------------------------------" << std::endl;
// }
