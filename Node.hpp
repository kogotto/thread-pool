#pragma once

template <typename T>
struct Node {
    using value_type = T;

    value_type payload;
    Node* next;
};
