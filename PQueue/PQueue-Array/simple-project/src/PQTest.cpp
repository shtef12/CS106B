/*
 * File: PQTest.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This is the starter file for the test program for Assignment #5.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include <string>
#include <sstream>
#include "console.h"
#include "pqueue.h"
#include "simpio.h"
#include "strlib.h"
#include "tokenscanner.h"
using namespace std;

/* Function prototypes */

void helpCommand();
void enqueueCommand(string cmd, PriorityQueue& pq);
void listCommand(PriorityQueue& pq);

/* Main program */

int main() {
   PriorityQueue pq;
   while (true) {
      string cmd = getLine("> ");
      if (cmd == "help") {
         helpCommand();
      }else if(stringContains(cmd,"enqueue")){
          enqueueCommand(cmd, pq);
      }else if(cmd == "dequeue"){
          pq.dequeue();
      }else if(cmd == "peek"){
          cout << pq.peek() << endl;
      }else if(cmd == "peekPriority"){
          cout << pq.peekPriority() << endl;
      }else if(cmd == "clear"){
          pq.clear();
      }else if(cmd == "size"){
          cout << "pq size is: " << pq.size() << endl;
      }else if(cmd == "isEmpty"){
          if(pq.isEmpty())
              cout << "true" << endl;
          else
              cout << "false" << endl;
      }else if(cmd == "list"){
          listCommand(pq);
      }else {
         cout << "Undefined command: " << cmd << endl;
      }
   }
   return 0;
}

/*
 * Function: listCommand
 * ---------------------
 * displays the contents of the priority queue in order of priority
 */
void listCommand(PriorityQueue& pq){
    PriorityQueue newQueue;
    while(pq.isEmpty() != true){
        double prior = pq.peekPriority();
        cout << "value = " << pq.peek() << " priority = " << pq.peekPriority() << endl;
        newQueue.enqueue(pq.dequeue(),prior);

    }
    pq = newQueue;
}

/*
 * Function: enqueueCommand
 * -------------------------
 * adds the value and priority to the priority queue
 */

void enqueueCommand(string cmd, PriorityQueue& pq){
    vector<string> words = stringSplit(cmd," ");
    cout << "value = " << words[1] << " priority = " << words[2] << endl;
    pq.enqueue(words[1],stringToInteger(words[2]));
}

/*
 * Function: helpCommand
 * ---------------------
 * Displays a list of the required commands.  If you extend this program,
 * you should add new entries to this function.
 */

void helpCommand() {
   cout << "enqueue value priority -- Enqueues value at priority" << endl;
   cout << "dequeue -- Dequeues the most urgent item" << endl;
   cout << "peek -- Peeks at the first item without removing it" << endl;
   cout << "peekPriority -- Reports the priority of the first item" << endl;
   cout << "clear -- Clears the queue" << endl;
   cout << "size -- Reports the size of the queue" << endl;
   cout << "isEmpty -- Reports whether the queue is empty" << endl;
   cout << "list -- Lists the elements of the queue" << endl;
   cout << "help -- Prints this message" << endl;
}
