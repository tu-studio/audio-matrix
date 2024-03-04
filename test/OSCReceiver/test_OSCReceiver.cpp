#include <gtest/gtest.h>
#include <time.h>
#include <benchmark/benchmark.h>
#include <OSCReceiver.h>

#ifndef RANDOMDOUBLE
#define RANDOMDOUBLE

static double randomDouble(int LO, int HI){
  return (double) LO + (double) (std::rand()) / ((double) (RAND_MAX/(HI-LO)));
}

#endif // RANDOMDOUBLE

static void BM_OSCReceiver_construct_destruct(benchmark::State& state) {
    // Perform setup here
    std::srand(std::time(0));

    std::unique_ptr<OSCReceiver> oscReceiver = nullptr;

    for (auto _ : state) {
    // This code gets timed
        oscReceiver = std::make_unique<OSCReceiver>();
        oscReceiver.reset();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_OSCReceiver_construct_destruct);

TEST(OSCReceiver, construct_destruct){

    std::unique_ptr<OSCReceiver> oscReceiver = nullptr;

    oscReceiver = std::make_unique<OSCReceiver>();
    oscReceiver.reset();

    EXPECT_EQ(
        oscReceiver,
        nullptr
    );
}

TEST(OSCReceiver, benchmark){
    benchmark::SetBenchmarkFilter("BM_OSCReceiver_construct_destruct");
    benchmark::RunSpecifiedBenchmarks();
    std::cout << "----------------------------------------------------------------------------" << std::endl;
}
