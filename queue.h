
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