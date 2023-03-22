#pragma once
#include "Generic_DS/Node.h"
template <class T>

class Stack {
private:
    Node<T>* top;
public:
    Stack() {
        top = nullptr;
    }
    bool isEmpty() {
        if (top == nullptr)
            return true;
        else
            return false;
    }
    bool peek(T& TopEntry) {
        if (isEmpty())
            return false;

        TopEntry = top->getItem();
        return true;
    }
    void push(const T& val) {
        if (top == nullptr) {
            top = new Node<T>;
            top->setNext(nullptr);
            top->setItem(val);
        }
        else {
            Node<T>* temp = new Node<T>;
            temp->setItem(val);
            temp->setNext(top);
            top = temp;
        }
    }
    void pop(T& TopEntry) {
        if (isEmpty()) {

            return;
        }
        else {
            TopEntry = top->getItem();
            Node<T>* temp = top;
            top = top->getNext();
            delete temp;
        }
    }
    T* toArray(int& count)
    {

        count = 0;

        if (!top)
           return nullptr;
        ////counting the no. of items in the Queue
        Node<T>* p = top;
        while (p)
        {
            count++;
            p = p->getNext();
        }


        T* Arr = new T[count];
        p = top;
        for (int i = 0; i < count; i++)
        {
            Arr[i] = p->getItem();
            p = p->getNext();
        }
        return Arr;
    }
    void print() {
        Node<T>* temp = top;
        while (temp != nullptr)
        {
            cout << temp->getItem() << " ";
            temp = temp->getNext();
        }
    }
};

