
#pragma once
#include <cstddef>
#include <iostream>

template <typename T>
concept Printable = requires(std::ostream &os, const T &t) {
    { os << t } -> std::same_as<std::ostream &>;

    // this reads "given an ostream and a const T, os << t must be defined and its return type must be same as std::ostream &"
};

template <typename T>
concept Comparable = requires(const T &op1, const T &op2) {
    { op1 == op2 } -> std::convertible_to<bool>;
};

template <Comparable T>
class Collection {
public:
    // note that a const method does not modify the object
    // a const method is declared T foo() const {} wich const after the parameter list
    virtual ~Collection() = default;
    virtual void add(const T &item) = 0;
    virtual bool contains(const T &item) const = 0;
    virtual std::size_t size() const = 0;
    virtual bool empty() const { return size() == 0; }
};