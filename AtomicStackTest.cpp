#include <gtest/gtest.h>

#include <ranges>
#include <thread>
#include <vector>

#include <AtomicStack.hpp>

namespace {

AtomicStack stack;

void threadMain(int nodesCount) {
    for (auto i : std::views::iota(0, nodesCount)) {
        stack.push(i);
    }
}

} // namespace

TEST(AtomicStack, Test) {
    constexpr int threadsCount = 8;
    constexpr int nodesPerThread = 1'000'000;

    std::vector<std::jthread> threads;
    threads.reserve(threadsCount);
    for (auto i : std::views::iota(0, threadsCount)) {
        threads.emplace_back(threadMain, nodesPerThread);
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    ASSERT_EQ(threadsCount * nodesPerThread, linkedListSize(stack.head()));
}
