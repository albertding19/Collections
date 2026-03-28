
#pragma once
#include "collections.h"
#include "arrayList.h"

template <Comparable T>
class Stack : public Collection<T> {
public:
    void add(const T &item) override;
    bool contains(const T &item) const override;
    T pop();
    T peek() const;
    std::size_t size() const override;

private:
    ResizingArrayList<T> array;
};

template <Comparable T>
void Stack<T>::add(const T &item) {

}
