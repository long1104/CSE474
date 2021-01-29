#include "Soc.h"


float socValues[] = {0, 50, 10};



void updateStateOfCharge(float* stateOfChargeReading) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
    *stateOfChargeReading = socValues[getClockCount()%3];
    return;
}


void socTask(void* sData) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
    SocData* data = (SocData*) sData;
    
    // Update all sensors
    updateStateOfCharge(data->stateOfCharge);
  return;
}
