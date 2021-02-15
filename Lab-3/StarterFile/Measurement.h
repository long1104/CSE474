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
    int* hvilPin;
    float* temperature;
    int* temperaturePin;
    float* hvCurrent;
    int* hvCurrentPin;
    float* hvVoltage;
    int* hvVoltagePin;
} MeasurementData;


void measurementTask (void*);
void updateHVIL(float* hvilReadingPtr, int *pin);
void updateTemperature(float* temperatureReadingPtr, int *pin) ;
void updateHvCurrent(float* currentReadingPtr, int *pin) ;
void updateHvVoltage(float* voltageReadingPtr, int*pin);

#endif

#ifdef __cplusplus
}
#endif
