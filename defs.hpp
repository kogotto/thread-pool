#pragma once

#include <stdint.h>

using Int = uint64_t;

inline constexpr Int TASKS_COUNT = 100'000;

inline Int work(Int n) {
    Int product = 1;
    for (Int i = 1; i < n; ++i) {
        product *= i;
        if (product == 0) product = 1;
    }
    return product;
}
