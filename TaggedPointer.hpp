#pragma once

#include <cstdint>

template <typename T>
struct TaggedPointer {
    uint64_t counter_:16{0};
    uint64_t pointer_:48{0};

    TaggedPointer() = default;
    TaggedPointer(uint16_t counter, T* pointer)
        : counter_{counter}
        , pointer_{reinterpret_cast<uint64_t>(pointer)} {
    }

    TaggedPointer next(T* newPointer) {
        return {
            static_cast<uint16_t>(counter() + 1),
            newPointer
        };
    }

    void setPointer(T* p) {
        pointer_ = reinterpret_cast<uint64_t>(p);
    }

    T* pointer() const {
        return reinterpret_cast<T*>(static_cast<uint64_t>(pointer_));
    }

    void setCounter(uint16_t c) {
        counter_ = c;
    }

    uint16_t counter() const {
        return static_cast<uint16_t>(counter_);
    }
};
