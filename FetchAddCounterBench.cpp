#include <benchmark/benchmark.h>

#include <FetchAddCounter.hpp>

namespace {

void BM_FetchAddCounterBench(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = fetchAddCounterMain(1000,
                                          10'000,
                                          std::memory_order_relaxed);
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_FetchAddCounterBench);
