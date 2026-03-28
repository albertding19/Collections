
#pragma once
#include "collections.h"
#include "arrayList.h"

template <Ordered T>
class MinHeap : public Heap<T> {
public:
    void push(const T &item) override;
    T pop() override;
    const T &peek() const override;
    void heapify() override;

    MinHeap() : array(ResizingArrayList<T>()) {}

    // Copy constructor
    MinHeap(const MinHeap<T> &other) : array(ResizingArrayList<T>()) {
        for (std::size_t i = 0; i < other.array.size(); i++) {
            push(other.array[i]);
        }
    }

    // Move constructor
    MinHeap(MinHeap<T> &&other) noexcept : array(std::move(other.array)) {}

    // Copy assignment
    MinHeap &operator=(const MinHeap<T> &other) {
        if (this == &other) return *this;
        array = other.array; // using ResizingArrayLists's copy assignment
        return *this;
    }

    // Move assignment
    MinHeap &operator=(MinHeap<T> &&other) noexcept {
        if (this == &other) return *this;
        array = std::move(other.array); // std::move casts to && type so this uses ResizingArrayLists's move assignment
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const MinHeap<T> &heap) requires Printable<T> {
        os << array;
        return os;
    }

private:
    ResizingArrayList<T> array;

    void shiftDown();
    void shiftUp();
    int hasChildren(std::size_t index) const;
};

template <Ordered T>
void MinHeap<T>::push(const T &item) {
    array.add(item);
    shiftUp();
}

template <Ordered T>
T MinHeap<T>::pop() {
    if (array.size() == 0) { throw std::out_of_range("Pop on empty heap"); }
    array.swap(0, array.size() - 1);
    T result{array.pop()};
    shiftDown();
    return result;
}

template <Ordered T>
const T &MinHeap<T>::peek() const {
    if (array.size() == 0) { throw std::out_of_range("Peek on empty heap"); }
    return array[0];
}

template <Ordered T>
void MinHeap<T>::heapify() {
    // TODO
    return;
}

template <Ordered T>
int MinHeap<T>::hasChildren(std::size_t index) const {
    std::size_t children_at{2 * index + 1};
    std::size_t count{array.size()};
    if (children_at >= count) {
        return 0;
    }
    if (children_at + 1 >= count) {
        return 1;
    }
    return 2;
}

template <Ordered T>
void MinHeap<T>::shiftDown() {
    // Called after a pop(), only the root may be out of position
    
    std::size_t curr_index{0};

    while (hasChildren(curr_index) == 2) {
        std::size_t left_index{2 * curr_index + 1};
        std::size_t right_index{2 * curr_index + 2};

        T curr_value = array.get(curr_index);
        T left_value = array.get(left_index);
        T right_value = array.get(right_index);

        if (curr_value > left_value || curr_value > right_value) {
            if (left_value <= right_value) {
                array.swap(curr_index, left_index);
                curr_index = left_index;
            } else {
                array.swap(curr_index, right_index);
                curr_index = right_index;
            }
        } else {
            return;
        }
    }

    if (hasChildren(curr_index) == 1) {
        std::size_t child_index{2 * curr_index + 1};
        if (array.get(curr_index) > array.get(child_index)) {
            array.swap(curr_index, child_index);
        }
    }
}

template <Ordered T>
void MinHeap<T>::shiftUp() {
    
    std::size_t curr_index{array.size() - 1};

    while (curr_index > 0) {
        std::size_t parent_index{(curr_index - 1) / 2};
        if (array.get(curr_index) < array.get(parent_index)) {
            array.swap(curr_index, parent_index);
            curr_index = parent_index;
        } else {
            return;
        }
    }
}
