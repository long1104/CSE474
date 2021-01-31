#include "Soc.h"


float socValues[] = {0, 50, 10};



void updateStateOfCharge(float* stateOfChargeReadingPtr, int* clockCountPtr) {
    /****************
        Function name: updateStateOfCharge
        Function inputs: stateOfChargeReadingPtr: pointer to the status of the state of charge value, clockCountPtr: pointer to the value of cycle count
        Function outputs: void return
        Function description: updates the value of the state of charge data through the measured value
        Author(s):
    *****************/
    *stateOfChargeReadingPtr = socValues[(*clockCountPtr) % 3];
    return;
}


void socTask(void* sDataPtr) {
    /****************
        Function name: socTask
        Function inputs: sDataPtr: pointer to state of charge task data
        Function outputs: void return
        Function description: updates the state of charge value through the task data
        Author(s):
    *****************/
    SocData* data = (SocData*) sDataPtr;
    updateStateOfCharge(data->stateOfCharge, data->clockCountPtr);
    return;
}
