/* *************************************************************************************************
 * Copyright Jonathan Carlson 2022
 * ************************************************************************************************/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream> 
using namespace std;

template<typename T>
class LinkedList {
public:

    LinkedList() {}

    T* head = NULL;

    // ---------------------------------------------------------------------------------------------
    // Modify the commented out 'cout' lines below to display information about your datatype.
    void printList(const T* node) {
        // cout << node->value << endl;
        
        if(node->after == head) {
            // cout << node->after->value << " (Circular List...)" << endl;
            return;
        }
            
        if(node->after)
            printList(node->after);
    }

    // ---------------------------------------------------------------------------------------------

    void printList() {
        if(head) printList(head);
    }

    // ---------------------------------------------------------------------------------------------

    void verifyIndex(int index, string functionName, int maxSize) {
        if(index < 0 || index > maxSize) {
            cout << "ERROR: Out of range index - 'LinkedList::" << functionName 
                 << "(" << to_string(index) << ")'" << endl; 
            exit(139);
        } 
    }

    // ---------------------------------------------------------------------------------------------

    int size(const T* node) {
        int i = 1;
        if(node->after && node->after != head)
            i += size(node->after);
        return i;
    }

    // ---------------------------------------------------------------------------------------------

    int size() {
        if(head) return size(head);
        return 0;
    }

    // ---------------------------------------------------------------------------------------------

    T* getElement(int index, T* node) {
        verifyIndex(index, "getElement()", size());
        
        if(index) {
            if(node->after)
                node = getElement(--index, node->after);
        }

        return node;
    }

    // ---------------------------------------------------------------------------------------------

    T* getElement(int index) {
        return getElement(index, head);
    }

    // ---------------------------------------------------------------------------------------------

    void insert(T* newNode, int index) {
        verifyIndex(index, "insert()", size());        
        
        // Insert into empty list
        if(!this->head) {
            this->head = newNode;
            return;
        }

        // Append to end of list (getElement(index) will break because it doesnt exist.)
        if(index == size()) {
            T* node = getElement(index - 1);
            newNode->before = node;
            newNode->after = node->after;
            if(node->after)
                node->after->before = newNode;
            node->after = newNode;
            return;
        }

        // Default (list already contains elements)
        T* node = getElement(index);
        if(node == head)
            this->head = newNode;
        
        newNode->before = node->before;
        newNode->after = node;
        
        if(node->before)
            node->before->after = newNode;
            
        node->before = newNode;
    }

    // ---------------------------------------------------------------------------------------------

    void push_back(T* node) {
        insert(node, size());
    }

    // ---------------------------------------------------------------------------------------------

    void erase(int index) {
        verifyIndex(index, "erase", size() - 1);
        
        T* node = getElement(index);
        
        if(node->before)
            node->before->after = node->after;
            
        if(node->after)
            node->after->before = node->before;

        // If erasing the head, make the next element the head.
        if(node == head && node->after)    
            head = node->after;
        
        node->after = NULL;
        node->before = NULL;
        delete node;

        if(node == head) 
            this->head = NULL;
    }

    // ---------------------------------------------------------------------------------------------

    void makeListCircular() {
        if(!head) {
            cout << "ERROR: LinkedList::makeListCircular(): Cannot make empty list circular."
                 << endl;
            exit(139);
        }
        T* tail = getElement(size() - 1);
        head->before = tail;
        tail->after = head;
    }

    // ---------------------------------------------------------------------------------------------

    T* operator[] (int x) {
        return getElement(x);
    }
};

#endif // LINKEDLIST_H