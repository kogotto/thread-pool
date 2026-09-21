#include <gtest/gtest.h>

#include <FetchAddCounter.hpp>

TEST(FetchAddCounter, Test) {
    constexpr int count = 1000;
    constexpr int iterations = 10'000;
    ASSERT_EQ(count * iterations, fetchAddCounterMain(count, iterations));
}
