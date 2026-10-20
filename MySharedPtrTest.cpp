#include <gtest/gtest.h>

#include <iostream>
#include <thread>
#include <vector>

#include <MySharedPtr.hpp>

class SayingGoodbye {
public:
    SayingGoodbye(int n)
        : n_{n}
    {}

    ~SayingGoodbye() {
        std::cout << "Goodbye" << std::endl;
    }
private:
    int n_;
};

using SharedPtr = MySharedPtr<SayingGoodbye>;

class Testing {
public:
    Testing(SharedPtr shared)
        : object_{std::move(shared)}
    {}
    int64_t getCounter() const {
        return object_.block_->counter_.load(std::memory_order_relaxed);
    }
private:
    SharedPtr object_;
};

namespace {

constexpr int THREADS_COUNT = 300;

void threadMain(SharedPtr shared) {
}

TEST(MySharedPtr, Test) {
    SharedPtr shared{new SayingGoodbye(666)};
    Testing testing{shared};
    std::vector<std::jthread> threads;
    threads.reserve(THREADS_COUNT);
    for (int i = 0; i < THREADS_COUNT - 1; ++i) {
        threads.emplace_back(
            threadMain,
            shared
        );
    }
    threads.emplace_back(threadMain, std::move(shared));

    for (auto&& thread : threads) {
        thread.join();
    }

    ASSERT_EQ(1, testing.getCounter());
}

}
