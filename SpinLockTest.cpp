#include <gtest/gtest.h>

#include <array>
#include <ranges>
#include <thread>
#include <vector>

#include <SpinLock.hpp>

namespace {

int64_t testBase(int threadsCount, int iterations) {
    int64_t counter{0};
    SpinLock lock;

    std::vector<std::jthread> threads;
    threads.reserve(threadsCount);
    for (auto i : std::views::iota(0, threadsCount)) {
        threads.emplace_back(
            [&counter, &lock, iterations] {
                for (auto i : std::views::iota(0, iterations)) {
                    std::lock_guard guard{lock};
                    ++counter;
                }
            }
        );
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    return counter;
}

std::array<int, 5> generateThreadsCount() {
    return {
        1,
        2,
        4,
        8,
        100,
    };
}

std::array<int, 6> generateIterations() {
    return {
        1,
        10,
        100,
        1000,
        10'000,
        100'000,
    };
}

class SpinLockTest : public testing::TestWithParam<std::tuple<int, int>> {
};

TEST_P(SpinLockTest, Test) {
    auto [threadsCount, iterations] = GetParam();
    ASSERT_EQ(
        testBase(threadsCount, iterations),
        threadsCount * iterations
    );
}

INSTANTIATE_TEST_SUITE_P(SpinLockTest, SpinLockTest,
    testing::Combine(
        testing::ValuesIn(generateThreadsCount()),
        testing::ValuesIn(generateIterations())
    )
);

}
