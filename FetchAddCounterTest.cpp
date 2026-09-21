#include <gtest/gtest.h>

#include <FetchAddCounter.hpp>

TEST(FetchAddCounter, Test) {
    constexpr int count = 1000;
    ASSERT_EQ(count, fetchAddCounterMain(count));
}
