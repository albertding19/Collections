
#pragma once
#include "collections.h"
#include <memory>
#include <cstddef>
#include <optional>

/*
Move vs Copy (rvalue vs lvalue)

T && corresponds to an rvalue reference
T & corresponds to an lvalue reference

Move constructors/assignments overload T &&, so they are called when an rvalue argument is given
Copy constructors/assignments overload T &, so they are called when an lvalue argument is given

Suppose a is of type T,

T b = a or b = a use copy constructor and assignment because a is an lvalue.

T b = std::move(a) or b = std::move(a) use move constructor and assignment because std::move(a) returns an rvalue (more accurately, an expiring xvalue subtype of rvalue)

The rationale for copy and move is that move is often more efficient, but we can only move if we are certain that we won't need the object we're stealing from again.
Thus move semantics are usually only used on rvalues which are expiring.

Note that std::move is like a label indicating that the resources in an object may be moved. It is equivalent to a static cast to an rvalue
reference type T &&.

a const method means the this becomes a pointer to a const object. So you can't modify member values within a const method

noexcept is required to optimise move constructors and assignments. This is because certain STL containers need strong exception guarantees. This is because, in a move operation,
the previous state cannot be recovered if the operator fails. Unlike a copy operator where the original copy is untouched can can be restores in case of failure.
This is why certain STL containers like vector internally fall back to copy operations unless a no exception guarantee is given by noexcept.
*/

template <Comparable T>
class LinkedList : public Collection<T> {
private:
    struct Node {
        T val;
        Node *prev;
        std::unique_ptr<Node> next;

        Node(const T &input) : val(input), prev(nullptr), next(nullptr) {}
    };

    std::unique_ptr<Node> head;
    Node *tail;

    std::size_t count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Copy Constructor
    LinkedList(const LinkedList<T> &other) : head(nullptr), tail(nullptr), count(0) {
        Node *curr = other.head.get();

        while (curr != nullptr) {
            add(curr->val);
            curr = curr->next.get();
        }
    }

    // Move constructor, steals resources from another class
    // &&other denotes a rvalue reference (just a syntactic peculiarity as &other denotes an lvalue reference which cannot be used for assignments)
    // We use noexcept to avoid copy semantics and increase efficiency
    LinkedList(LinkedList<T> &&other) noexcept : head(std::move(other.head)), tail(other.tail), count(other.count) {
        other.tail = nullptr;
        other.count = 0;
    }

    // Copy Assignment
    LinkedList &operator=(const LinkedList<T> &other) {
        head = nullptr;
        tail = nullptr;
        count = 0;

        Node *curr = other.head.get();

        while (curr != nullptr) {
            add(curr->val);
            curr = curr->next.get();
        }

        return *this;
    }

    // Move assignment
    LinkedList &operator=(LinkedList<T> &&other) noexcept {
        if (this == &other) return *this;

        head = std::move(other.head);
        tail = other.tail;
        count = other.count;

        other.tail = nullptr;
        other.count = 0;

        return *this;
    }

    class Iterator {
    public:
        Iterator(Node *curr_node) : curr_node(curr_node) {}

        T &operator*() {
            return curr_node->val;
        }

        bool operator!=(const Iterator &other) const {
            return curr_node != other.curr_node;
        }

        Iterator &operator++() {
            curr_node = curr_node->next.get();
            return *this;
        }

    private:
        Node *curr_node;
    };

    class ConstIterator {
    public:
        ConstIterator(const Node *curr_node) : curr_node(curr_node) {}

        const T &operator*() const {
            return curr_node->val;
        }

        bool operator!=(const ConstIterator &other) const {
            return curr_node != other.curr_node;
        }

        ConstIterator &operator++() {
            curr_node = curr_node->next.get();
            return *this;
        }

    private:
        const Node *curr_node;
    };

    Iterator begin() { return Iterator(head.get()); }

    Iterator end() { return nullptr; }

    ConstIterator begin() const { return ConstIterator(head.get()); }

    ConstIterator end() const { return nullptr; }

    T &operator[](std::size_t index);

    void add(const T &item) override;

    void append(const T &item);

    void prepend(const T &item);

    std::optional<T> popLeft();

    std::optional<T> pop();

    std::optional<T> peekLeft() const;

    std::optional<T> peekRight() const;

    bool contains(const T &item) const override;

    std::size_t size() const override;

    bool empty() const override;

    ~LinkedList() {
        Node *curr{head.release()};
        while (curr != nullptr) {
            Node *temp{curr->next.release()};
            delete curr;
            curr = temp;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const LinkedList &l) requires Printable<T> {
        os << "[";
        Node *curr{l.head.get()};
        
        if (curr != nullptr) {
            os << curr->val;
            curr = curr->next.get();
        }

        while (curr != nullptr) {
            os << ", " << curr->val;
            curr = curr->next.get();
        }

        os << "]";
        return os;
    }
};

template <Comparable T>
T &LinkedList<T>::operator[](std::size_t index) { 
    if (index < 0 || index >= size()) {
        throw std::out_of_range("Index out of bounds");
    }

    Node *curr = head.get();
    std::size_t remaining = index;

    while (remaining > 0) {
        remaining--;
        curr = curr->next.get();
    }

    return curr->val;
}

template <Comparable T>
void LinkedList<T>::add(const T &item) {
    if (head == nullptr) {
        // make head
        head = std::make_unique<Node>(item);
        tail = head.get();
    }
    else {
        tail->next = std::make_unique<Node>(item);
        tail->next->prev = tail;
        tail = tail->next.get();
    }
    count++;
}

// Note that & return type means the function is returning the exact object, not a copy of it
template <Comparable T>
std::optional<T> LinkedList<T>::pop() {
    if (size() == 0) {
        return std::nullopt;
    }
    T res{tail->val};

    if (size() == 1) {
        head = nullptr;
        tail = nullptr;
    } else {
        Node *temp = tail->prev;
        tail->prev->next = nullptr;
        tail = temp;
    }
    count--;
    return res;
}

template <Comparable T>
bool LinkedList<T>::contains(const T &item) const {
    Node *curr = head.get();
    while (curr != nullptr) {
        if (curr->val == item) {
            return true;
        }
        curr = curr->next.get();
    }
    return false;
}

template <Comparable T>
std::size_t LinkedList<T>::size() const {
    return count;
}

template <Comparable T>
bool LinkedList<T>::empty() const {
    return size() == 0;
}

template <Comparable T>
void LinkedList<T>::append(const T &item) {
    add(item);
}

template <Comparable T>
void LinkedList<T>::prepend(const T& item) {
    std::unique_ptr<Node> new_node = std::make_unique<Node>(item);

    new_node->next = std::move(head);
    if (new_node->next != nullptr) {
        new_node->next->prev = new_node.get();
    }
    if (count == 0) {
        tail = new_node.get();
    }
    head = std::move(new_node);

    count++;
}

template <Comparable T>
std::optional<T> LinkedList<T>::popLeft() {
    if (head == nullptr) {
        return std::nullopt;
    }
    T result = head->val;
    head = std::move(head->next);
    if (head != nullptr) {
        head->prev = nullptr;
    }
    count--;
    if (count == 0) {
        tail = nullptr;
    }
    return result;
}

template <Comparable T>
std::optional<T> LinkedList<T>::peekLeft() const {
    if (head == nullptr) {
        return std::nullopt;
    }
    return head->val;
}

template <Comparable T>
std::optional<T> LinkedList<T>::peekRight() const {
    if (tail == nullptr) {
        return std::nullopt;
    }
    return tail->val;
}