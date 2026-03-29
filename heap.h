
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

// For heapify we use Floyd's algorithm for O(n) time complexity. 
// Floyd's algorithm is calling a recursive sift_down function on each node starting from the last node.
// The alternative is calling sift_up starting from the first node however that would lead to O(nlogn) complexity
// The reason is that for a binary heap of height h, time complexity sift_down on a node at depth d is h - d, whereas that of sift_up is d,
// In a binary heap, there are more nodes at deeper levels so repeatedly calling sift_up will be more expensive as there are more nodes in deeper levels.
template <Ordered T>
void MinHeap<T>::heapify() {
    
    auto siftDown = [this](std::size_t index) {
        int children = this->hasChildren(index);
        if (children == 0) {
            return;
        }
        
        std::size_t smallest_at = index;
        std::size_t left_child = 2 * index + 1;

        if (this->array[left_child] < this->array[smallest_at]) {
            smallest_at = left_child;
        }

        if (children == 2) {
            std::size_t right_child = 2 * index + 2;
            if (this->array[right_child] < this->array[smallest_at]) {
                smallest_at = right_child;
            }
        }

        if (smallest_at != index) {
            this->array.swap(index, smallest_at);
            siftDown(smallest_at);
        }

    };

    for (int i = (array.size() / 2) - 1; i >= 0; i--) {
        // In fact, we only need to start at the last non-leaf node since leaves are already valid heaps.
        // Also, we need to use a signed int here because std::size_t is always positive so i >= 0 will always be true.
        // decrementing i when i is 0 just wraps around to a large number if i is std::size_t.
        siftDown(i);
    }
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
