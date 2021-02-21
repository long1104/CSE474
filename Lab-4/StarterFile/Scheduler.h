
#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include "TaskControlBlock.h"



extern TCB *tasksPtr;
extern TCB *head;
extern TCB *tail;
void Scheduler();
void insert(TCB* node);
void deleteNode(TCB* node);

#endif
