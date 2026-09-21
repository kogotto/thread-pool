#include <benchmark/benchmark.h>

#include <FetchAddCounter.hpp>

namespace {

inline auto toInt(std::memory_order order) {
    return static_cast<int64_t>(order);
}

inline auto fromInt(int64_t value) {
    return static_cast<std::memory_order>(value);
}

void BM_CounterWithCustomOrder(benchmark::State& state) {
    auto order = fromInt(state.range(0));
    for (auto&& _ : state) {
        auto result = fetchAddCounterMain(
            1000,
            1000'000,
            order
        );
        benchmark::DoNotOptimize(result);
    }
}

} // namespace

BENCHMARK(BM_CounterWithCustomOrder)
    ->Arg(toInt(std::memory_order_relaxed))
    ->Arg(toInt(std::memory_order_seq_cst))
    ->ArgName("order");
