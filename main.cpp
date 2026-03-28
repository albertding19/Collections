
#include "collections.h"
#include "linkedList.h"
#include "arrayList.h"
#include <memory>
#include "hashmap.h"

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
    // std::move is equivalent to a cast to LinkedList<int> && type
    // std::move is used to indicate that the resources in an object may be moved.
    LinkedList<int> yetAnotherList{std::move(myList)};
    std::cout << "myList: " << myList << '\n';
    std::cout << "yetAnotherList: " << yetAnotherList << '\n';

    ResizingArrayList<int> arrayList{ResizingArrayList<int>()};
    arrayList.add(1);
    arrayList.add(20);
    arrayList.add(3);
    arrayList.add(5);
    std::cout << arrayList.size() << '\n';
    arrayList.remove(0);
    arrayList.remove(2);
    std::cout << arrayList << std::endl;

    HashMap<std::string, int> hashmap = HashMap<std::string, int>();
    hashmap.put("one", 1);
    hashmap.put("two", 2);
    int removed = hashmap.remove("one").value();
    std::cout << hashmap << std::endl;
    std::cout << removed << std::endl;
    return 0;
}