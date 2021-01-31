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
    const byte* hvilPin;
    float* temperature;
    float* hvCurrent;
    float* hvVoltage;
    int* clockCountPtr;
} MeasurementData;


void measurementTask (void*);
void updateHVIL(float* hvilReading, const byte* pin);
void updateTemperature(float* temperatureReading, int* clockCountPtr) ;
void updateHvCurrent(float* currentReading, int* clockCountPtr) ;
void updateHvVoltage(float* voltageReading, int* clockCountPtr);

#endif

#ifdef __cplusplus
}
#endif
