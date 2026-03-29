
#pragma once
#include "collections.h"
#include "linkedList.h"

template <Comparable T>
class Stack : public Collection<T> {
public:
    void add(const T &item) override;
    bool contains(const T &item) const override;
    std::optional<T> pop();
    std::optional<T> peek() const;
    std::size_t size() const override;

    Stack() : stack(LinkedList<T>()) {}

private:
    LinkedList<T> stack;
};

template <Comparable T>
void Stack<T>::add(const T &item) {
    stack.add(item);
}

template <Comparable T>
bool Stack<T>::contains(const T &item) const {
    return stack.contains(item);
}

template <Comparable T>
std::optional<T> Stack<T>::pop() {
    return stack.pop();
}

template <Comparable T>
std::optional<T> Stack<T>::peek() const {
    return stack.peekRight();
}

template <Comparable T>
std::size_t Stack<T>::size() const {
    return stack.size();
}