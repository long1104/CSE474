#ifdef __cplusplus
extern "C" {
#endif

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct MeasurementTaskData {
    float* hvilStatus;
    byte hvilPin;
    float* temperature;
    float* hvCurrent;
    float* hvVoltage;
    int* clockCountPtr;
} MeasurementData;


void measurementTask (void*);
void updateHVIL(float* hvilReadingPtr, byte pin);
void updateTemperature(float* temperatureReadingPtr, int* clockCountPtr) ;
void updateHvCurrent(float* currentReadingPtr, int* clockCountPtr) ;
void updateHvVoltage(float* voltageReadingPtr, int* clockCountPtr);

#endif

#ifdef __cplusplus
}
#endif
