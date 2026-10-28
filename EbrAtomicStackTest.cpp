#include <gtest/gtest.h>

#include <EbrAtomicStack.hpp>

class Testing {
public:
    Testing(EbrAtomicStack<int>& stack)
        : stack_{&stack} {
    }

    bool empty() const {
        return stack_->head_.load() == nullptr;
    }
private:
    EbrAtomicStack<int>* stack_;
};

namespace {

}

TEST(EbrAtomicStack, Test) {
    EbrAtomicStack<int> stack;
    Testing testing{stack};
    ASSERT_TRUE(true);
}
