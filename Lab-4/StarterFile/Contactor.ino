#include <stdlib.h>
#include <stdbool.h>
#include "Contactor.h"

void updateContactor(volatile float* contactorStatusPtr, int *contactorPin, Alarm alarms[]) {
    /****************
        Function name: updateContactor
        Function inputs: contactorStatusPtr: pointer to boolean status of contactor (held in float as 1.0 or 0.0), alarms: alarms used in the system
        Function outputs: void return
        Function description: updates the output pin of the contactor using shared flag from display and the status of the alarms
        Authors:    Long Nguyen / Chase Arline
    *****************/
    if(activeAlarmCheck(alarms)) {
        *contactorStatusPtr=0;
    }
    digitalWrite(*contactorPin, (int)(*contactorStatusPtr && !activeAlarmCheck(alarms)));           //set contactor output high/low
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
    updateContactor(data->contactorStatus, data->contactorPin, data->alarms);

    return;
}
