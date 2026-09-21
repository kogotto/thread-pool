#include <gtest/gtest.h>

#include <ThreeTransitiveThreads.hpp>

TEST(ThreeTransitiveThreads, Test) {
    const int expectedResultA = 666;
    const int expectedResultB = 13000;

    const auto[resultA, resultB] = threeTransitiveThreads(expectedResultA, expectedResultB);
    ASSERT_EQ(expectedResultA, resultA);
    ASSERT_EQ(expectedResultA + expectedResultB, resultB);
}
