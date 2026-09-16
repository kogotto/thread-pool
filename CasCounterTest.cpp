#include <gtest/gtest.h>

int casCounterMain(int threadsCount);

TEST(CasCounter, Test) {
    constexpr int count = 1000;
    ASSERT_EQ(count, casCounterMain(count));
}
