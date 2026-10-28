#pragma once

#include <atomic>
#include <optional>

#include <Node.hpp>
#include <TaggedPointer.hpp>

template <typename T>
class AtomicStack {
public:
    using Node = ::Node<T>;
    using value_type = Node::value_type;

    using TaggedPointer = ::TaggedPointer<Node>;
    using AtomicTaggedPointer = std::atomic<TaggedPointer>;
    static_assert(AtomicTaggedPointer::is_always_lock_free);

    ~AtomicStack() {
        freeLinkedList(head_.load(std::memory_order_relaxed).pointer());
    }

    void push(value_type payload) {
        auto newNode = new Node{std::move(payload)};

        TaggedPointer oldHead = head_.load(std::memory_order_acquire);
        TaggedPointer newHead;
        do {
            newNode->next = oldHead.pointer();
            newHead = oldHead.next(newNode);
        } while (!head_.compare_exchange_weak(oldHead, newHead));
    }

    std::optional<value_type> pop() {
        TaggedPointer oldHead = head_.load(std::memory_order_acquire);

        TaggedPointer newHead;
        do {
            if (oldHead.pointer() == nullptr) {
                return std::nullopt;
            }
            newHead = oldHead.next(oldHead.pointer()->next);
        } while (!head_.compare_exchange_weak(oldHead, newHead));

        Node* node = oldHead.pointer();
        value_type result = std::move(node->payload);
        delete node;
        return result;
    }

    Node* head(std::memory_order order = std::memory_order_relaxed) const {
        return head_.load(order).pointer();
    }

private:
    static void freeLinkedList(Node* head) {
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }

    AtomicTaggedPointer head_;
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
