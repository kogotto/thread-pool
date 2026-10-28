#pragma once

#include <atomic>

template <typename T>
class EbrAtomicStack {
public:
    friend class Testing;
    using value_type = T;
    struct Node {
        value_type payload;
        Node* next;
    };

    void push(value_type payload) {
        Node* newNode = new Node{
            std::move(payload),
            head_.load(std::memory_order_relaxed)
        };
        while (!head_.compare_exchange_weak(newNode->next, newNode));
    }

    ~EbrAtomicStack() {
        freeLinkedList(head_.load(std::memory_order_relaxed));
    }

private:
    static void freeLinkedList(Node* head) {
        while (head != nullptr) {
            Node* nextHead = head->next;
            delete head;
            head = nextHead;
        }
    }

    std::atomic<Node*> head_{nullptr};
};
