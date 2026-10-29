#include <gtest/gtest.h>

#include <ranges>
#include <thread>
#include <vector>

#include <AtomicStack.hpp>

namespace {

AtomicStack<int> stack;

void pushThreadMain(int nodesCount) {
    for (auto i : std::views::iota(0, nodesCount)) {
        stack.push(i);
    }
}

} // namespace

TEST(AtomicStack, Push) {
    constexpr int threadsCount = 8;
    constexpr int nodesPerThread = 1'000'000;

    std::vector<std::jthread> threads;
    threads.reserve(threadsCount);
    for (auto i : std::views::iota(0, threadsCount)) {
        threads.emplace_back(pushThreadMain, nodesPerThread);
    }

    for (auto&& thread : threads) {
        thread.join();
    }

    ASSERT_EQ(threadsCount * nodesPerThread, linkedListSize(stack.head()));
}
