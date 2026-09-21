#include <gtest/gtest.h>

#include <CasCounter.hpp>

TEST(CasCounter, Test) {
    constexpr int count = 1000;
    constexpr int iterations = 10000;
    ASSERT_EQ(count * iterations, casCounterMain(count, iterations));
}
