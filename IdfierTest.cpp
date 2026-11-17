#include <gtest/gtest.h>

#include <algorithm>
#include <mutex>
#include <ranges>
#include <thread>
#include <vector>

#include <Idfier.hpp>

constexpr int THREADS_COUNT = 100;

TEST(Idfier, threadId) {
    std::mutex mutex;
    std::vector<int> result;
    result.reserve(THREADS_COUNT);
    Idfier idfier;

    std::vector<std::jthread> threads;
    threads.reserve(THREADS_COUNT);
    for (auto i : std::views::iota(0, THREADS_COUNT)) {
        threads.emplace_back(
            [&] {
                const auto threadId = idfier.threadId();
                std::lock_guard lock{mutex};
                result.push_back(threadId);
            }
        );
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    std::ranges::sort(result);
    for (auto [i, v] : std::views::enumerate(result)) {
        ASSERT_EQ(i, v);
    }
}
