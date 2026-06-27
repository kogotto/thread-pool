#include <benchmark/benchmark.h>

#include <string>

namespace {

void BM_longString(benchmark::State& state) {
    for (auto&& _ : state) {
        std::string s = "Some long string to avoid small string optimization";
        benchmark::DoNotOptimize(s);
    }
}

void BM_smallString(benchmark::State& state) {
    for (auto&& _ : state) {
        std::string s = "123";
        benchmark::DoNotOptimize(s);
    }
}

} // namespace

BENCHMARK(BM_longString);
BENCHMARK(BM_smallString);
