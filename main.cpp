#include <iostream>
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"

using namespace std;

int main() {
    try {
        SinglyLinkedList<int> s;
        s.push_back(1);
        s.push_back(2);
        s.push_front(0);
        s.insert_at(2, 5); // [0,1,5,2]
        cout << "Singly: " << s << "\n";
        cout << "s[2] = " << s[2] << "\n";
        cout << "find 5 at " << s.find(5) << "\n";
        s.remove_at(2);
        cout << "after remove: " << s << "\n";
        s.pop_front();
        s.pop_back();
        cout << "after pops: " << s << " size=" << s.size() << "\n";

        DoublyLinkedList<string> d;
        d.push_back("one");
        d.push_front("zero");
        d.push_back("two");
        d.insert_at(2, "middle");
        cout << "Doubly: " << d << "\n";
        cout << "d[2] = " << d[2] << "\n";
        cout << "find 'two' at " << d.find(string("two")) << "\n";
        d.remove_at(1);
        cout << "after remove: " << d << "\n";

        // trigger exception
        SinglyLinkedList<int> e;
        // e.pop_back(); // uncomment to test exception
    }
    catch (const exception& ex) {
        cout << "Exception: " << ex.what() << "\n";
    }
    return 0;
}