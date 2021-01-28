#include <stdlib.h>
#include <stdbool.h>
#include "Measurement.h"

float temperatureValues[] = {-10, 5, 25};
float currentValues[] = {-20, 0, 20};
float voltageValues[] = {10, 150, 45};

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
    *temperatureReading = temperatureValues[getClockCount()%3];
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
    *currentReading = currentValues[(getClockCount()%6)/2];
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
    *voltageReading = voltageValues[(getClockCount()%9)/3];
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
