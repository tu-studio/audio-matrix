#include <gtest/gtest.h>
#include <time.h>
#include <benchmark/benchmark.h>
#include <OSCServer.h>

#ifndef RANDOMDOUBLE
#define RANDOMDOUBLE

static double randomDouble(int LO, int HI){
  return (double) LO + (double) (std::rand()) / ((double) (RAND_MAX/(HI-LO)));
}

#endif // RANDOMDOUBLE

static void BM_OSCServer_construct_destruct(benchmark::State& state) {
    // Perform setup here
    std::srand(std::time(0));

    std::unique_ptr<OSCServer> osc_server = nullptr;

    for (auto _ : state) {
    // This code gets timed
        osc_server = std::make_unique<OSCServer>();
        osc_server.reset();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_OSCServer_construct_destruct);

TEST(OSCServer, construct_destruct){

    std::unique_ptr<OSCServer> osc_server = nullptr;

    osc_server = std::make_unique<OSCServer>();
    osc_server.reset();

    EXPECT_EQ(
        osc_server,
        nullptr
    );
}

TEST(OSCServer, benchmark){
    benchmark::SetBenchmarkFilter("BM_OSCServer_construct_destruct");
    benchmark::RunSpecifiedBenchmarks();
    std::cout << "----------------------------------------------------------------------------" << std::endl;
}
