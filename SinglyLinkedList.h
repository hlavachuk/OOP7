#include <iostream>
#include <memory>   // підключаємо smart pointers: shared_ptr, make_shared
#include <stdexcept> // підключаємо стандартні виключення: runtime_error, out_of_range
#include "Node.h"

using namespace std;

template<typename T>
class SinglyLinkedList {
private:
    shared_ptr<Node<T>> head; // вказівник на перший вузол списку; shared_ptr для автоматичного звільнення пам'яті
    size_t sz;
public:
    SinglyLinkedList() : head(nullptr), sz(0) {}
    ~SinglyLinkedList() = default; // дефолтний деструктор (smart pointers)

    // Вставка значення на початок списку
    void push_front(const T& val) {
        shared_ptr<Node<T>> node = make_shared<Node<T>>(val); // створюємо новий вузол
        node->next = head; // вказуємо на поточну голову
        head = node; // оновлюємо голову на новий вузол
        sz++;
    }

    // Вставка значення в кінець списку
    void push_back(const T& val) {
        shared_ptr<Node<T>> node = make_shared<Node<T>>(val); // створюємо новий вузол
        if (!head) {
            head = node; // якщо список порожній — голова просто стає цим вузлом
        }
        else {
            shared_ptr<Node<T>> cur = head; // починаємо з голови
            while (cur->next) cur = cur->next; // рухаємося по next до останнього вузла
            cur->next = node; // останній тепер вказує на новий вузол
        }
        sz++;
    }

    // Видалення першого елементу.
    void pop_front() {
        if (!head) throw runtime_error("List is empty"); // якщо пустий, викидаємо помилку
        head = head->next; // зсуваємо голову на наступний вузол, старий вузол звільниться автоматично
        sz--;
    }

    // Видалення останнього елементу. .
    void pop_back() {
        if (!head) throw runtime_error("List is empty"); // якщо пусто — помилка
        if (!head->next) { // якщо лише один елемент - список стає порожнім
            head = nullptr;
            sz = 0;
            return;
        }
        shared_ptr<Node<T>> cur = head; // починаємо з голови
        // шукаємо передостанній вузол: умова гарантує, що cur->next->next існує
        while (cur->next && cur->next->next) cur = cur->next;
        cur->next = nullptr; // спустошуєм останній вузол
        sz--;
    }

    //(e)
    size_t size() const { return sz; }

    //(f)
    bool empty() const { return sz == 0; }

    // Оператор доступу за індексом (с)
    // operator[] повертає посилання на значення за індексом
    T& operator[](size_t index) {
        if (index >= sz) throw out_of_range("Index out of range"); // перевірка меж
        shared_ptr<Node<T>> cur = head; // починаємо з голови
        for (size_t i = 0; i < index; ++i) cur = cur->next;
        return cur->data;
    }
    //так як це не масив і ми не можемо моментально перейти до елемента з потрібним індексом, ми поступово до нього крокуєм

    // Вставка значення за індексом. Якщо index == 0 — викликаємо push_front (d)
    // insert_at вставляє новий вузол на позицію index
    void insert_at(size_t index, const T& val) {
        if (index > sz) throw out_of_range("Index out of range"); // не можна вставити за межами (після останнього допустимо index==sz)
        if (index == 0) { push_front(val); return; } // вставка на початок
        shared_ptr<Node<T>> cur = head; // починаємо з голови
        // доходимо до вузла перед місцем вставки (index-1)
        for (size_t i = 0; i < index - 1; ++i) cur = cur->next;
        shared_ptr<Node<T>> node = make_shared<Node<T>>(val); // створюємо новий вузол
        node->next = cur->next; // новий вузол вказує на наступний після cur
        cur->next = node; // cur тепер зв'язаний з новим вузлом
        sz++; // збільшуємо розмір
    }

    // Видалення елементу за індексом.
    // (d) Видалення елементу за індексом
    // remove_at видаляє вузол на позиції index
    void remove_at(size_t index) {
        if (index >= sz) throw out_of_range("Index out of range"); // перевірка коректності індексу
        if (index == 0) { pop_front(); return; } // видалення першого елементу
        shared_ptr<Node<T>> cur = head; // починаємо з голови
        for (size_t i = 0; i < index - 1; ++i) cur = cur->next; // шукаємо вузол перед тим, що видаляємо
        cur->next = cur->next->next; // переприсвоюємо посилання, тим самим видаляючи вузол з ланцюга
        sz--; // зменшуємо розмір
    }

    // Пошук значення у списку. Повертає індекс першої знахідки або -1, якщо не знайдено.
    // (g) Пошук елементу у списку
    // find повертає індекс або -1 якщо не знайдено
    int find(const T& val) const {
        shared_ptr<Node<T>> cur = head; // починаємо пошук з голови
        size_t i = 0; // лічильник індексу
        while (cur) { // поки вузол існує
            if (cur->data == val) return (int)i; // при збігу повертаємо індекс
            cur = cur->next; // переходимо до наступного вузла
            ++i; // інкремент індексу
        }
        return -1; // не знайдено
    }

    // (h) Виведення списку через стандартний потік виведення
    // operator<< друкує список у форматі [a, b, c]
    friend ostream& operator<<(ostream& os, const SinglyLinkedList<T>& list) {
        shared_ptr<Node<T>> cur = list.head; // починаємо з голови списку
        os << "["; // відкриваємо квадратну дужку
        bool first = true; // прапорець для роздільників
        while (cur) { // проходимо усі вузли
            if (!first) os << ", "; // додаємо кому перед наступними елементами
            os << cur->data; // виводимо значення вузла
            first = false; // після першого елемента роздільники потрібні
            cur = cur->next; // переходимо до наступного вузла
        }
        os << "]"; // закриваємо квадратну дужку
        return os; // повертаємо потік
    }
};