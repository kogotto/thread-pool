#include <gtest/gtest.h>

#include <CasCounter.hpp>

TEST(CasCounter, Test) {
    constexpr int count = 1000;
    ASSERT_EQ(count, casCounterMain(count));
}
