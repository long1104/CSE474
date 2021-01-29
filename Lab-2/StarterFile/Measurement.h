#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_


#define HVIL_OPEN   false
#define HVIL_CLOSED true

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct MeasurementTaskData {
    float* hvilStatus;
    const byte* hvilPin;
    float* temperature;
  	float* hvCurrent;
	  float* hvVoltage;
} MeasurementData;


void measurementTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
