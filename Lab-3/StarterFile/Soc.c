#include "Soc.h"

void updateStateOfCharge(float* stateOfChargeReadingPtr) {
    /****************
        Function name: updateStateOfCharge
        Function inputs: stateOfChargeReadingPtr: pointer to the status of the state of charge value, clockCountPtr: pointer to the value of cycle count
        Function outputs: void return
        Function description: updates the value of the state of charge data through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *stateOfChargeReadingPtr = 0;
    return;
}


void socTask(void* sDataPtr) {
    /****************
        Function name: socTask
        Function inputs: sDataPtr: pointer to state of charge task data
        Function outputs: void return
        Function description: updates the state of charge value through the task data
        Authors:    Long Nguyen / Chase Arline
    *****************/
    SocData* data = (SocData*) sDataPtr;
    updateStateOfCharge(data->stateOfCharge);
    return;
}
