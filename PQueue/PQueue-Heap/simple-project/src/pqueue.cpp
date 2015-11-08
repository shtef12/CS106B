/*
 * File: pqueue.cpp (heap version)
 * -------------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file implements the pqueue.h interface using a heap as the
 * underlying representation.  Only the copy constructor and assignment
 * operator are included.  Your job is to include definitions for the
 * other exported methods and to make a change to the definition of the
 * assignment operator so that it deletes any old storage.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include "error.h"
#include "pqueue.h"
using namespace std;

PriorityQueue::PriorityQueue(){
    array = new node[INITIAL_CAPACITY];
    capacity = INITIAL_CAPACITY;
    count = 0;
}

PriorityQueue::~PriorityQueue(){
    delete[] array;
}

int PriorityQueue::size(){
    return count;
}

bool PriorityQueue::isEmpty(){
    return count == 0;
}

void PriorityQueue::clear(){
    node *arr = new node[INITIAL_CAPACITY];
    delete[] array;
    array = arr;
    count = 0;
}

void PriorityQueue::enqueue(string value, double priority){
    if(count == capacity){
        enlarge();
    }
    node newNode;
    newNode.priority = priority;
    newNode.value = value;

    array[count++] = newNode;
    cout << "enqueue " << value << " at " << count-1 << endl;
    rearrangeUp(count-1);
}

string PriorityQueue::dequeue(){
    string value = array[0].value;
    swapPosition(0,count-1);             //swap first and last elements
    node *newArray = new node[capacity];
    for(int i = 0; i < count-1; i++){    //copy array to new array
        newArray[i] = array[i];
    }
    delete[] array;                     //delete old array
    array = newArray;                   //make array equal to new array
    count--;
    rearrangeDown(0);
    return value;
}

string PriorityQueue::peek(){
    return array[0].value;
}

double PriorityQueue::peekPriority(){
    return array[0].priority;
}

void PriorityQueue::rearrangeUp(int index){
    cout << "index of new element: " << index << endl;
    if(index > 0){
        int parent = (index-1)/2;
        cout << "parent element is at: " << parent << endl;
        if(array[index].priority < array[parent].priority){ //if the parent priority is greater than child, swap positions
            cout << "swapping with parent" << endl;
            swapPosition(index,parent);
            rearrangeUp(parent);                            //check the priority of the new parent
        }
    }
    cout << "current index is root" << endl;
}

void PriorityQueue::rearrangeDown(int index){
    if(index >= count-1) return;     //if index is last in array return
    int leftChild = 2 * index + 1;
    if(leftChild > count-1) return;  //has no children (maybe dont need)
    int rightChild = 2 * index + 2;
    int smallestChild;
    cout << "left child index is: " << leftChild << " right child index is: " << rightChild << endl;
    if(rightChild > count-1){
        smallestChild = leftChild;
    }else{
        smallestChild = (leftChild < rightChild) ? leftChild : rightChild;   //set smallest child to the child with higher priority
        cout << "smallest child index is: " << smallestChild << endl;
    }
    if(array[index].priority > array[smallestChild].priority){
        cout << "child has higher priority, swapping" << endl;
        swapPosition(index,smallestChild);                                  //swap with smallest child
        rearrangeDown(smallestChild);                                       //check new children priorities
    }

}

void PriorityQueue::swapPosition(int index, int indexToSwapWith){
    cout << "swapping index: " << index << " with index: " << indexToSwapWith << endl;
    node temp = array[indexToSwapWith];
    array[indexToSwapWith] = array[index];
    array[index] = temp;
}

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
      if(array != NULL) delete[] array;
      deepCopy(src);
   }
   return *this;
}
