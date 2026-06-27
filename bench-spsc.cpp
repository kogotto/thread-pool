#include <benchmark/benchmark.h>

#include <spsc_test.hpp>

namespace {

void BM_SPSCQueueTest(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = spscTest();
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_SPSCQueueTest);
