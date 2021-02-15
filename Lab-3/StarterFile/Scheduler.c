#include "Scheduler.h"

TCB *tasksPtr = NULL;

void Scheduler() {
    /****************
         Function name:    Scheduler
         Function inputs:  No parameters
         Function outputs: No return
         Function description: This is the round robin scheduler, it executes all tasks in a sequential order,
                               the tasks create a user intreface for a battery management system
         Authors:    Long Nguyen / Chase Arline
       *****************/
    TCB* current = tasksPtr;
    while (current != NULL) {
        current->task(current->taskDataPtr);
        current = current->next;
    }
    return;
}
