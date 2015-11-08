/*
 * File: pqueue.cpp (linked-list version)
 * --------------------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file implements the pqueue.h interface using a linked list as the
 * underlying representation.  Only the copy constructor and assignment
 * operator are included.  Your job is to include definitions for the
 * other exported methods.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include "error.h"
#include "pqueue.h"
using namespace std;

PriorityQueue::PriorityQueue(){
    head = NULL;
    count = 0;
}

PriorityQueue::~PriorityQueue(){
    clear();
    delete head;
}

int PriorityQueue::size(){
    return count;
}

bool PriorityQueue::isEmpty(){
    return head == NULL;
}

void PriorityQueue::clear(){
    //node* curr = head;
    while(head->next != NULL){
        dequeue();
    }
    delete head;
    count = 0;
    head = NULL;
}

//have to order it by priority
void PriorityQueue::enqueue(string value, double priority){
    node* newNode = new node;
    newNode->value = value;
    newNode->priority = priority;
    newNode->next = NULL;

    if(isEmpty()){
        head = newNode; //set head to the new node
        count++;
    }else{
        node* curr = head;
        while(true){
            if(curr->next == NULL){ //if current priority is highest put at end
                curr->next = newNode;
                count++;
                return;
            }
            if(curr->next->priority > priority){ //if next priority is higher make curr next node the new node
                newNode->next = curr->next;
                curr->next = newNode;
                count++;
                return;
            }
            curr = curr->next; //move to the next node
        }
    }
}

string PriorityQueue::dequeue(){
    if(isEmpty()) error("Empty queue");

    string value = head->next->value;
    node* old = head;  //get the first element
    head = head->next; //make head next in list
    delete old;        //delete the first element
    count--;
    return value;
}

string PriorityQueue::peek(){
    if(head == NULL)
        error("Empty queue");
    return head->value;
}

double PriorityQueue::peekPriority(){
    if(head == NULL)
        error("Empty queue");
    return head->priority;
}

// TODO: Add your method definitions here

/*
 * Implementation notes: copy constructor and assignment operator
 * --------------------------------------------------------------
 * The constructor and assignment operators follow a standard paradigm,
 * as described in the textbook.
 */

PriorityQueue::PriorityQueue(const PriorityQueue & src) {
   deepCopy(src);
}

PriorityQueue & PriorityQueue::operator=(const PriorityQueue & src) {
   if (this != &src) {
      clear();
      deepCopy(src);
   }
   return *this;
}
