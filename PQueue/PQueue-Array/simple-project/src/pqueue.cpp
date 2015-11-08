/*
 * File: pqueue.cpp (array version)
 * --------------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file implements the pqueue.h interface using a dynamic array as
 * the underlying representation.  Most of the code is already in place.
 * The only thing that is missing is the implementation of the actual
 * commands.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include "error.h"
#include "pqueue.h"
using namespace std;

PriorityQueue::PriorityQueue() {
    array = new ValuePriorityPair[INITIAL_CAPACITY];
    capacity = INITIAL_CAPACITY;
    count = 0;
}

PriorityQueue::~PriorityQueue() {
    delete[] array;
}

int PriorityQueue::size() {
   return count;
}

bool PriorityQueue::isEmpty() {
   return count == 0;
}

void PriorityQueue::clear() {
   //create new empty array
    ValuePriorityPair* newArr = new ValuePriorityPair[INITIAL_CAPACITY];
    delete[] array; //delete old array
    array = newArr; //assign array to new array
    count = 0;
}

void PriorityQueue::enqueue(string value, double priority) {
    if(priority < 0){
        error("Out of bounds");
    }
    int index = 0;
    if(count == capacity){
        expandCapacity();
    }
    if(!isEmpty()){
        for(int i = size()-1; i >= 0; i--){
            if(array[i].priority < priority){
                index = i+1;
                break;
            }
            array[i+1] = array[i];
            index = i;
        }
    }
    ValuePriorityPair pair;
    pair.priority = priority;
    pair.value = value;
    array[index] = pair;
    count++;

}

string PriorityQueue::dequeue() {
   ValuePriorityPair pair = array[0];
   ValuePriorityPair* newArr = new ValuePriorityPair[capacity];
   //copy the array without the first pair
   for(int i = 1; i < count; i++){
       newArr[i-1] = array[i];
   }
   delete[] array;
   array = newArr;
   count--;
   return pair.value;
}

string PriorityQueue::peek() {
   return array[0].value;
}

double PriorityQueue::peekPriority() {
   return array[0].priority;
}

/*
 * Implementation notes: copy constructor and assignment operator
 * --------------------------------------------------------------
 * The constructor and assignment operators follow the standard
 * paradigm described in the textbook.
 */

PriorityQueue::PriorityQueue(const PriorityQueue & src) {
   deepCopy(src);
}

PriorityQueue & PriorityQueue::operator=(const PriorityQueue & src) {
   if (this != &src) {
      if (array != NULL) delete[] array;
      deepCopy(src);
   }
   return *this;
}
