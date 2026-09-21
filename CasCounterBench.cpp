#include <benchmark/benchmark.h>

#include <CasCounter.hpp>

namespace {

void BM_CasCounterBench(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = casCounterMain(1000);
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_CasCounterBench);
