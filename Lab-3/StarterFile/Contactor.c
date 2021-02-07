#include <stdlib.h>
#include <stdbool.h>
#include "Contactor.h"



void updateContactor(float* contactorStatusPtr, byte contactorPin) {
    /****************
        Function name: updateContactor
        Function inputs: contactorStatusPtr: pointer to boolean status of contactor (held in float as 1.0 or 0.0)
        Function outputs: void return
        Function description: updates the output pin of the contactor
        Authors:    Long Nguyen / Chase Arline
    *****************/
    digitalWrite(contactorPin, (int)(*contactorStatusPtr));           //set contactor output high/low
    return;
}

void contactorTask(void* cData) {
    /****************
        Function name: contactorTask
        Function inputs: cData: pointer to contactor task data
        Function outputs: void return
        Function description: updates the state of the contactor output
        Authors:    Long Nguyen / Chase Arline
    *****************/
    ContactorData* data = (ContactorData*) cData;

    // Update all sensors
    updateContactor(data->contactorStatus, data->contactorPin);

    return;
}
