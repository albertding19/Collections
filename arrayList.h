
#pragma once
#include "collections.h"

static constexpr std::size_t DEFAULT_CAPACITY{20};
static constexpr int DEFAULT_FACTOR{2};

template <Comparable T>
class ResizingArrayList : public Collection<T> {
public:
    ResizingArrayList() : capacity(DEFAULT_CAPACITY), count(0), factor(DEFAULT_FACTOR), array(std::make_unique<T[]>(DEFAULT_CAPACITY)) {}

    // copy constructor
    ResizingArrayList(const ResizingArrayList<T> &other) : capacity(other.capacity), count(0), factor(other.factor), array(std::make_unique<T[]>(other.capacity)) {
        for (std::size_t i = 0; i < other.size(); i++) {
            add(other.get(i));
        }
    }

    // move constructor
    ResizingArrayList(ResizingArrayList <T> &&other) noexcept : capacity(other.capacity), count(other.count), factor(other.factor), array(std::move(other.array)) {
        other.capacity = 0;
        other.count = 0;
    }

    ~ResizingArrayList() = default;

    void add(const T &item) override;

    bool contains(const T &item) const override;

    std::size_t size() const override;

    T &operator[](std::size_t index);

    T get(std::size_t index) const;

    friend std::ostream &operator<<(std::ostream &os, const ResizingArrayList<T> &l) requires Printable<T> {
        os << '[';
        if (l.count >= 1) {
            os << l.array[0];
        }

        for (std::size_t i = 1; i < l.count; i++) {
            os << ", " << l.array[i];
        }

        os << ']';
        return os;
    }

private:
    std::size_t capacity;
    std::size_t count;
    int factor;
    std::unique_ptr<T[]> array;

    void resize() {
        std::size_t new_cap = capacity * factor;
        std::unique_ptr<T[]> new_array = std::make_unique<T[]>(new_cap);
        for (std::size_t i = 0; i < size(); i++) {
            new_array[i] = array[i];
        }
        array = std::move(new_array);
        capacity = new_cap;
    }
};

template <Comparable T>
void ResizingArrayList<T>::add(const T &item) {
    if (count + 1 > capacity) {
        resize();
    }
    array[count] = item;
    count++;
}

template <Comparable T>
bool ResizingArrayList<T>::contains(const T &item) const {
    for (std::size_t i = 0; i < size(); i++) {
        if (array[i] == item) {
            return true;
        }
    }
    return false;
}

template <Comparable T>
std::size_t ResizingArrayList<T>::size() const {
    return count;
}

template <Comparable T>
T &ResizingArrayList<T>::operator[](std::size_t index) {
    if (index < 0 || index >= size()) {
        throw std::out_of_range("Index out of range");
    }
    return array[index];
}

template <Comparable T>
T ResizingArrayList<T>::get(std::size_t index) const {
    if (index < 0 || index >= size()) {
        throw std::out_of_range("Index out of range");
    }
    return array[index];
}
