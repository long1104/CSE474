#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOC_H_
#define SOC_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

//
typedef struct SocTaskData {
    float* stateOfCharge;
    float* measuredTemperature;
    float* measuredHvCurrent;
    float* measuredHvVoltage;
} SocData;

extern float tempArr[4];
extern float voltArr[5];
extern float dataChart[4][5];

void socTask (void*);
void updateStateOfCharge(float* stateOfChargeReadingPtr, float* temperature, float* hvCurrent, float* hvVoltage);
float computeOpenCircuitVoltage(float hvCurrent, float hvVoltage);
void getTemperatureBound(float temperatureVal, int* lowerBound, int* upperBound);
void getVoltageBound(float voltageOCVal, int* lowerBound, int* upperBound);
float oneDInterpolation(float x1, float y1, float x2, float y2, float value);

#endif

#ifdef __cplusplus
}
#endif
