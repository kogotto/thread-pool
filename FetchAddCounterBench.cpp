#include <benchmark/benchmark.h>

#include <FetchAddCounter.hpp>

namespace {

void BM_FetchAddCounterBench(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = fetchAddCounterMain(1000, 10'000);
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_FetchAddCounterBench);
