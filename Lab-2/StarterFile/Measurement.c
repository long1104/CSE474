#include <stdlib.h>
#include <stdbool.h>
#include "Measurement.h"


void updateHVIL(bool* hvilReading, const byte* pin) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
    *hvilReading = digitalRead(*pin);
    return;
}

void updateTemperature(float* temperatureReading) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
  	return;
}

void updateHvCurrent(float* currentReading) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
  	return;
}

void updateHvVoltage(float* voltageReading) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
  	return;
}

void measurementTask(void* mData) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
  	measurementData* data = (measurementData*) mData;
    
  	// Update all sensors
  	updateHVIL(data->hvilStatus, data->hvilPin);
  	updateTemperature(data->temperature);
  	updateHvCurrent(data->hvCurrent);
  	updateHvVoltage(data->hvVoltage);
  	
	return;
}
