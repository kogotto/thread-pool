#include <benchmark/benchmark.h>

#include <FetchAddCounter.hpp>

namespace {

void BM_RelaxedCounter(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = fetchAddCounterMain(
            1000,
            10'000,
            std::memory_order_relaxed
        );
        benchmark::DoNotOptimize(result);
    }
}

void BM_SeqCstCounter(benchmark::State& state) {
    for (auto&& _ : state) {
        auto result = fetchAddCounterMain(
            1000,
            10'000,
            std::memory_order_seq_cst
        );
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_RelaxedCounter);
BENCHMARK(BM_SeqCstCounter);
