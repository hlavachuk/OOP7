#include <iostream>
#include <memory>
#include <stdexcept>
#include "Node.h"

using namespace std;

template<typename T>
class DoublyLinkedList {
private:
    shared_ptr<Node<T>> head;
    shared_ptr<Node<T>> tail;
    size_t sz;
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), sz(0) {}
    ~DoublyLinkedList() = default;

    void push_front(const T& val) {
        // (a) Додавання на початок списку
        auto node = make_shared<Node<T>>(val);
        node->next = head;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = head;
        sz++;
    }

    void push_back(const T& val) {
        // (a) Додавання на кінець списку
        auto node = make_shared<Node<T>>(val);
        if (!tail) {
            head = tail = node;
        }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        sz++;
    }

    void pop_front() {
        // (b) Видалення першого елементу
        if (!head) throw runtime_error("List is empty");
        head = head->next;
        if (head) head->prev.reset();
        else tail = nullptr;
        sz--;
    }

    void pop_back() {
        // (b) Видалення останнього елементу
        if (!tail) throw runtime_error("List is empty");
        auto p = tail->prev.lock();
        if (p) {
            p->next = nullptr;
            tail = p;
        }
        else {
            head = tail = nullptr;
        }
        sz--;
    }

    // (e) Перевірка на розмір
    size_t size() const { return sz; }

    // (f) Перевірка чи список не порожній
    bool empty() const { return sz == 0; }

    T& operator[](size_t index) {
        // (c) Доступ до елементу за індексом
        if (index >= sz) throw out_of_range("Index out of range");
        auto cur = head;
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    const T& operator[](size_t index) const {
        if (index >= sz) throw out_of_range("Index out of range");
        auto cur = head;
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }

    void insert_at(size_t index, const T& val) {
        // (d) Додавання за індексом
        if (index > sz) throw out_of_range("Index out of range");
        if (index == 0) { push_front(val); return; }
        if (index == sz) { push_back(val); return; }
        auto cur = head;
        for (size_t i = 0; i < index - 1; ++i) cur = cur->next;
        auto node = make_shared<Node<T>>(val);
        node->next = cur->next;
        node->prev = cur;
        cur->next->prev = node;
        cur->next = node;
        sz++;
    }

    void remove_at(size_t index) {
        // (d) Видалення за індексом
        if (index >= sz) throw out_of_range("Index out of range");
        if (index == 0) { pop_front(); return; }
        if (index == sz - 1) { pop_back(); return; }
        auto cur = head;
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        auto p = cur->prev.lock();
        p->next = cur->next;
        cur->next->prev = p;
        sz--;
    }

    // (g) Пошук елементу
    int find(const T& val) const {
        auto cur = head;
        size_t i = 0;
        while (cur) {
            if (cur->data == val) return (int)i;
            cur = cur->next;
            ++i;
        }
        return -1;
    }

    // (h) Виведення списку через стандартний потік виведення
    friend ostream& operator<<(ostream& os, const DoublyLinkedList<T>& list) {
        auto cur = list.head;
        os << "[";
        bool first = true;
        while (cur) {
            if (!first) os << ", ";
            os << cur->data;
            first = false;
            cur = cur->next;
        }
        os << "]";
        return os;
    }
};