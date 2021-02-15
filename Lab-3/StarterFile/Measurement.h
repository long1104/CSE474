#ifdef __cplusplus
extern "C" {
#endif

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct MeasurementTaskData {
    volatile float* hvilStatus;
    int* hvilPin;
    volatile float* temperature;
    int* temperaturePin;
    volatile float* hvCurrent;
    int* hvCurrentPin;
    volatile float* hvVoltage;
    int* hvVoltagePin;
} MeasurementData;


void measurementTask (void*);
void updateHVIL(volatile float* hvilReadingPtr, int *pin);
void updateTemperature(volatile float* temperatureReadingPtr, int *pin) ;
void updateHvCurrent(volatile float* currentReadingPtr, int *pin) ;
void updateHvVoltage(volatile float* voltageReadingPtr, int*pin);

#endif

#ifdef __cplusplus
}
#endif
