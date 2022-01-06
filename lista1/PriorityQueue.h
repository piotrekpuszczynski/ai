//
// Created by Piotr Puszczyński on 09/11/2021.
//

#ifndef LISTA1_PRIORITYQUEUE_H
#define LISTA1_PRIORITYQUEUE_H


#include "Node.h"

class Element {
public:
    Node* value;
    Element* next = nullptr;

    explicit Element(Node* value) {
        this->value = value;
    }
};

class PriorityQueue {
public:
    Element* root;

    PriorityQueue() {
        root = nullptr;
    }

    void insert(Node* value) {

        if (root == nullptr) {
            root = new Element(value);
        } else if (root->value->h + root->value->g > value->h + value->g) {
            auto* newElement = new Element(value);
            newElement->next = root;
            root = newElement;
        } else {
            Element* temp = root;
            while (temp->next != nullptr && temp->next->value->h + temp->next->value->g < value->h + value->g) {
                temp = temp->next;
            }

            auto* newElement = new Element(value);
            newElement->next = temp->next;
            temp->next = newElement;
        }
    }

    Node* pop() {
        Element* toReturn = root;
        root = root->next;
        return toReturn->value;
    }
};


#endif //LISTA1_PRIORITYQUEUE_H
