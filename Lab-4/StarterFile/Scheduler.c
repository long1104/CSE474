#include "Scheduler.h"

TCB *tasksPtr = NULL;
TCB *head = NULL;
TCB *tail = NULL;

void Scheduler() {
    /****************
         Function name:    Scheduler
         Function inputs:  No parameters
         Function outputs: No return
         Function description: This is the scheduler implementation. It starts at the head of the task queue and executes each sequential task until there is not another task left to be executed.
         Authors:    Long Nguyen / Chase Arline
       *****************/
    TCB* current = head;
    while (current != NULL) {
        current->task(current->taskDataPtr);
        current = current->next;
    }
    return;
}

void insert(TCB* node) {
    /****************
    Function name: insert
    Function inputs: node: pointer to the TCB node to be inserted in the list
    Function outputs: void return
    Function description: inserts the TCB node into the linked list
    Authors:    Long Nguyen / Chase Arline
    *****************/
    if (head == NULL) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    return;
}

void deleteNode(TCB* node) {
    /****************
    Function name: deleteNode
    Function inputs: node: pointer to TCB node to be inserted into the linked list
    Function outputs: void return
    Function description: inserts the TCB node into the linked list
    Authors:    Long Nguyen / Chase Arline
    *****************/
    if (head != NULL) {
        if (head == tail && head == node) {
            head = NULL;
            tail = NULL;
        } else if (head == node) {
            head = head->next;
            head->prev = NULL;
        } else if (tail == node) {
            tail = tail->prev;
            tail->next = NULL;
        } else if (node->prev != NULL && node->next != NULL) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        node->prev = NULL;
        node->next = NULL;
    }
    return;
}
