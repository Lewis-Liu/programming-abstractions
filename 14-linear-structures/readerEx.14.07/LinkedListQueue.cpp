//
// LinkedListQueue.cpp
//
// This file implements a Queue class based upon a linked list data structure.
// It includes support for a reverse() method that does not require additional
// memory allocation.
//
// --------------------------------------------------------------------------
// Attribution: "Programming Abstractions in C++" by Eric Roberts
// Chapter 14, Exercise 7
// Stanford University, Autumn Quarter 2012
// http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1136/materials/CS106BX-Reader.pdf
//
// See chapter 5, exercise 13 for the non-templatized method.
// This code draws heavily upon code in Figures 14-9 and 14-10.
// --------------------------------------------------------------------------
//
// Created by Glenn Streiff on 10/6/16. (derivative work)
// Copyright © 2016 Glenn Streiff. All rights reserved.
//

// Implementation notes: LinkedListQueue constructor
// -------------------------------------------------
// The constructor creates an empty linked list and sets the count to 0.
//

template <typename ValueType>
LinkedListQueue<ValueType>::LinkedListQueue() {
    head = tail = NULL;
    count = 0;
}

// Implementation notes: LinkedListQueue destructor
// ------------------------------------------------
// The destructor frees any heap memory allocated by the queue.
//

template <typename ValueType>
LinkedListQueue<ValueType>::~LinkedListQueue() {
    clear();
}

//
// Implementation notes: size, isEmpty, clear
// ------------------------------------------
// These methods use the count variable and therefore run in constant time.
//

template <typename ValueType>
int LinkedListQueue<ValueType>::size() const {
    return count;
}

template <typename ValueType>
bool LinkedListQueue<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
void LinkedListQueue<ValueType>::clear() {
    while (count > 0) {
        dequeue();
    }
}

//
// Implementation notes: enqueue
// -----------------------------
// This method allocates a new list cell and chains it in at the tail of
// the queue.  If the queue is currently empty, the new cell must also
// become the head pointer in the queue.
//

template <typename ValueType>
void LinkedListQueue<ValueType>::enqueue(ValueType value) {
    Cell *cp = new Cell;
    cp->data = value;
    cp->link = NULL;
    if (head == NULL) {
        head = cp;
    } else {
        tail->link = cp;
    }
    tail = cp;
    count++;
}

//
// Implementation notes: dequeue, peek
// -----------------------------------
// These methods must check for an empty queue and report an error if
// there is no first element.  The dequeue method must also check for
// the case in which the queue becomes empty and set both the head
// and tail pointers to NULL.
//

template <typename ValueType>
ValueType LinkedListQueue<ValueType>::dequeue() {
    if (isEmpty()) error("dequeue: Attempting to dequeue an empty queue");
    Cell *cp = head;
    ValueType result = cp->data;
    head = cp->link;
    if (head == NULL) tail = NULL;
    delete cp;
    count--;
    return result;
}

template <typename ValueType>
ValueType LinkedListQueue<ValueType>::peek() const {
    if (isEmpty()) error("dequeue: Attempting to peek at an empty queue");
    return head->data;
}

template <typename ValueType>
void LinkedListQueue<ValueType>::swap(ValueType & x, ValueType & y) {
    ValueType tmp;
    
    tmp = x;
    x = y;
    y = tmp;
}

// Implementation notes: reverse
// ------------------------------
// Reverses the order of elements in the queue using an in-place algorithm
// by simply starting at the tail and doing a reverse iteration through the
// list, reversing the links along the way.  Finally the head and tail
// pointers are swapped.

template <typename ValueType>
void LinkedListQueue<ValueType>::reverse() {
    if (tail == NULL) return;  // No need to reverse empty list.
    
    for (Cell *cp = tail; cp != head; cp = prev(cp)) {
        cp->link = prev(cp);
    }
    head->link = NULL;
    
    Cell *tmp = head;
    head = tail;
    tail = tmp;
}

// Implementation notes: toString
// ------------------------------
// Return a string representation of the queue contents.

template <typename ValueType>
std::string LinkedListQueue<ValueType>::toString() const {
    std::ostringstream oss;
    for (Cell *cp = head; cp != NULL; cp = cp->link) {
        oss << cp->data << " ";
    }
    return oss.str();
}

// Implementation notes: prev
// --------------------------
// Given a pointer to a cell in a singly linked list, return
// the pointer to the previous cell.
//
// If the input pointer is already at the head, then head is returned.
//
// Going 'backwards' in a linked list is useful when reversing
// a queue based upon a linked list.  However, for singly linked lists,
// go backward is hard.  This primitive does the hard part in O(n)
// time by walking from the beginning of the list until the
// link field of the iterating cell pointer matches the passed-in
// cell pointer.

template <typename ValueType>
typename LinkedListQueue<ValueType>::Cell * LinkedListQueue<ValueType>::prev(Cell *cp) const {
    if (cp == NULL) return NULL;
    if (cp == head) return head;
    
    Cell * prevcp = head;
    while (prevcp != NULL) {
        if (prevcp->link == cp) {
            break;
        }
        prevcp = prevcp->link;
    }
    return prevcp;
}

//
// Implementation notes: copy constructor and assignment operator
// --------------------------------------------------------------
// These methods follow the standard template, leaving the work to deepCopy.
//

template <typename ValueType>
LinkedListQueue<ValueType>::LinkedListQueue(const LinkedListQueue<ValueType> & src) {
    deepCopy(src);
}

template <typename ValueType>
LinkedListQueue<ValueType> & LinkedListQueue<ValueType>::operator=(const LinkedListQueue<ValueType> & src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

//
// Implementation notes: deepCopy
// ------------------------------
// Copies the data from the src parameter into the current object.
// This implementation simply walks down the linked list in the
// source object and enques each value in the destination.
//

template <typename ValueType>
void LinkedListQueue<ValueType>::deepCopy(const LinkedListQueue<ValueType> & src) {
    head = NULL;
    tail = NULL;
    count = 0;
    for (Cell *cp = src.head; cp != NULL; cp = cp->link) {
        enqueue(cp->data);
    }
}
