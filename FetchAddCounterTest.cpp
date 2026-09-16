#include <gtest/gtest.h>

int fetchAddCounterMain(int threadsCount);

TEST(FetchAddCounter, Test) {
    constexpr int count = 1000;
    ASSERT_EQ(count, fetchAddCounterMain(count));
}
