#include "Scheduler.h"

TCB *tasksPtr = NULL;

void Scheduler() {
    /****************
         Function name:    Scheduler
         Function inputs:  No parameters
         Function outputs: No return
         Function description: This is the scheduler implementation. It starts at the head of the task queue and executes each sequential task until there is not another task left to be executed.
         Authors:    Long Nguyen / Chase Arline
       *****************/
    TCB* current = tasksPtr;
    while (current != NULL) {
        current->task(current->taskDataPtr);
        current = current->next;
    }
    return;
}
