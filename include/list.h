#pragma once
#include <iostream>


template <class T>
struct Node {
    T data;
    Node* next;

    Node() : next(nullptr) {}
    Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
};


template <class T>
class List {
    Node<T>* first;

    void copyToEmptyList(const List& list) {
        Node<T>* current = nullptr;
        for (Node<T>* ptr = list.first; ptr; ptr = ptr->next)
            current = insertAfter(ptr->data, current);
    }

public:

    List() : first(nullptr) {}

    List(const List& list) {
        copyToEmptyList(list);
    }

    ~List() {
        clear();
    }

    Node<T>* getFirst() const {
        return first;
    }

    List<T>& operator=(const List<T>& list) {
        if (&list != this) {
            clear();
            copyToEmptyList(list);
        }
        return *this;
    }

    friend bool operator==(const List<T>& list1, const List<T>& list2) {
        Node<T> *ptr1 = list1.first, *ptr2 = list2.first;
        while (ptr1 && ptr2) {
            if (ptr1->data != ptr2->data) return false;
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }
        return true;
    }

    friend bool operator!=(const List<T>& list1, const List<T>& list2) {
        return !(list1 == list2);
    }

    Node<T>* pushFront(const T& data) {  // returns new node
        first = new Node<T>(data, first);
        return first;
    }

    void popFront() {
        if (empty()) return;
        Node<T>* newFirst = first->next;
        delete first;
        first = newFirst;
    }

    Node<T>* insertAfter(const T& data, Node<T>* prevNode = nullptr) {  // returns new node
        if (!prevNode)
            return pushFront(data);
        prevNode->next = new Node<T>(data, prevNode->next);
        return prevNode->next;
    }

    void eraseAfter(Node<T>* prevNode = nullptr) {
        if (!prevNode) {
            popFront();
            return;
        }
        if (!prevNode->next) return;
        Node<T>* tmp = prevNode->next->next;
        delete prevNode->next;
        prevNode->next = tmp;
    }

    bool empty() {
        return first == nullptr;
    }

    void clear() {
        while (first) popFront();
    }

    friend std::ostream& operator<<(std::ostream& ostr, const List<T>& list) {
        for (Node<T>* ptr = list.getFirst(); ptr; ptr = ptr->next)
            ostr << ptr->data << "; ";
        ostr << std::endl;
        return ostr;
    }
};