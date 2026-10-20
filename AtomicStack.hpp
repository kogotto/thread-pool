#pragma once

#include <atomic>
#include <optional>

template <typename T>
class AtomicStack {
public:
    using value_type = T;

    struct Node {
        value_type payload;
        Node* next;
    };

    ~AtomicStack() {
        freeLinkedList(head_.load(std::memory_order_relaxed));
    }

    void push(int payload) {
        auto newNode = new Node{payload, head_.load(std::memory_order_relaxed)};
        while (!head_.compare_exchange_weak(newNode->next, newNode)) {}
    }

    std::optional<value_type> pop() {
        Node* oldHead = head_.load(std::memory_order_relaxed);
        while (oldHead != nullptr &&
                !head_.compare_exchange_weak(oldHead, oldHead->next));
        if (oldHead == nullptr) {
            return std::nullopt;
        }
        value_type result = std::move(oldHead->payload);
        delete oldHead;
        return result;
    }

    Node* head(std::memory_order order = std::memory_order_relaxed) const {
        return head_.load(order);
    }
private:
    static void freeLinkedList(Node* head) {
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }

    std::atomic<Node*> head_{nullptr};
};

template <typename N>
int linkedListSize(N* head) {
    int result = 0;
    while (head != nullptr) {
        ++result;
        head = head->next;
    }
    return result;
}
