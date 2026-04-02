
#pragma once
#include "collections.h"
#include "linkedList.h"

template <Comparable T>
class FifoQueue : public Collection<T> {
public:
    FifoQueue() {}
    void add(const T &item) override;
    std::optional<T> pop(); // note that .value() on std::optional<T> extracts T, or throws exception if empty
    bool contains(const T &item) const override;
    std::size_t size() const override;

    // Copy constructor
    FifoQueue(const FifoQueue<T> &other) : queue(LinkedList<T>()) {
        for (T item : other.queue) {
            queue.append(item);
        }
    }

    // Move constructor
    // noexcept to provide no exception guarantee such that move operation is safe (previous states cannot be restored in case of exception in move operationes)
    FifoQueue(FifoQueue<T> &&other) noexcept : queue(std::move(other.queue)) {}

    FifoQueue &operator=(const FifoQueue<T> &other) {
        if (this == &other) {
            return *this;
        }

        queue = LinkedList<T>();
        for (T item : other.queue) {
            queue.append(item);
        }

        return *this;
    }

    FifoQueue &operator=(FifoQueue<T> &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        queue = std::move(other.queue);

        return *this;
    }

    typename LinkedList<T>::Iterator begin() { return queue.begin(); }

    typename LinkedList<T>::Iterator end() { return queue.end(); }

private:
    LinkedList<T> queue;
};

template <Comparable T>
void FifoQueue<T>::add(const T &item) {
    queue.append(item);
}

template <Comparable T>
std::optional<T> FifoQueue<T>::pop() {
    return queue.popLeft();
}

template <Comparable T>
bool FifoQueue<T>::contains(const T &item) const {
    return queue.contains(item);
}

template <Comparable T> 
std::size_t FifoQueue<T>::size() const {
    return queue.size();
}