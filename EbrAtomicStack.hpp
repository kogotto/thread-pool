#pragma once

#include <atomic>
#include <optional>

template <typename T>
class EbrAtomicStack {
public:
    friend class Testing;
    using value_type = T;
    struct Node {
        value_type payload;
        Node* next;
    };

    Node* head(std::memory_order order = std::memory_order_relaxed) const {
        return head_.load(order);
    }

    void push(value_type payload) {
        Node* newNode = new Node{
            std::move(payload),
            head_.load(std::memory_order_relaxed)
        };
        while (!head_.compare_exchange_weak(newNode->next, newNode));
    }

    std::optional<T> pop() {
        auto head = head_.load(std::memory_order_relaxed);
        while (head != nullptr &&
               !head_.compare_exchange_weak(head, head->next));
        if (head == nullptr) {
            return std::nullopt;
        }
        value_type result = std::move(head->payload);
        delete head;
        return result;
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
