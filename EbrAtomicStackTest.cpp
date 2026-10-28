#include <gtest/gtest.h>

#include <EbrAtomicStack.hpp>

class Testing {
public:
    using Observed = EbrAtomicStack<int>;
    using Node = Observed::Node;

    Testing(Observed& stack)
        : stack_{&stack} {
    }

    bool empty() const {
        return getHead() == nullptr;
    }

    Node* getHead() const {
        return stack_->head_.load();
    }
private:
    Observed* stack_;
};

namespace {

}

TEST(EbrAtomicStack, Test) {
    EbrAtomicStack<int> stack;
    Testing testing{stack};
    ASSERT_TRUE(true);
}

TEST(EbrAtomicStack, Push) {
    EbrAtomicStack<int> stack;
    Testing testing{stack};
    stack.push(666);
    ASSERT_FALSE(testing.empty());
    auto head = testing.getHead();
    ASSERT_EQ(head->payload, 666);
    ASSERT_EQ(head->next, nullptr);
}
