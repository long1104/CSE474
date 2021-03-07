#ifndef _TASKCONTROLBLOCK_H
#define _TASKCONTROLBLOCK_H

#include <stdlib.h>

// This struct represents a task control block (TCB)
// TCB encapsulates task function and data
typedef struct taskControlBlock {
    void (*task)(void*);
    void *taskDataPtr;
    char *taskName;
    struct taskControlBlock* next;
    struct taskControlBlock* prev;
} TCB;

#endif  // _TASKCONTROLBLOCK_H
