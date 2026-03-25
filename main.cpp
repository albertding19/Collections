
#include "collections.h"
#include "linkedList.h"
#include <memory>

int main() {
    LinkedList<int> myList{LinkedList<int>()};
    myList.add(1);
    myList.add(2);
    myList.add(3);
    myList.add(4);
    std::cout << myList << std::endl;
    myList.pop();
    myList.add(5);
    LinkedList<int> anotherList{myList};
    std::cout << "myList: " << myList << '\n';
    std::cout << "anotherList: " << anotherList << '\n';
    LinkedList<int> yetAnotherList{std::move(myList)};
    std::cout << "myList: " << myList << '\n';
    std::cout << "yetAnotherList: " << yetAnotherList << '\n';
    return 0;
}