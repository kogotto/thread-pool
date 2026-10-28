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

TEST(EbrAtomicStack, Pop) {
    EbrAtomicStack<int> stack;
    Testing observer{stack};
    stack.push(1);
    stack.push(2);
    auto first = stack.pop();
    ASSERT_TRUE(first);
    ASSERT_EQ(*first, 2);
    auto second = stack.pop();
    ASSERT_TRUE(second);
    ASSERT_EQ(*second, 1);
    auto third = stack.pop();
    ASSERT_FALSE(third);
}
