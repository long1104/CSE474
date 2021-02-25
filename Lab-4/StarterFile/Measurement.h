#ifdef __cplusplus
extern "C" {
#endif

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

typedef struct MeasurementStatusStruct {
    float maximum;
    bool maxFlag;
    float* data;
    bool minFlag;
    float minimum;
    bool resetFlag;
} MeasurementStatus;

typedef struct MeasurementTaskData {
    float* hvilStatus;
    int* hvilPin;
    MeasurementStatus* temperature;
    int* temperaturePin;
    MeasurementStatus* current;
    int* currentPin;
    MeasurementStatus* voltage;
    int* voltagePin;
    bool* resetFlag;
} MeasurementData;

void resetMinMax(MeasurementStatus* history);
void updateMeasurementStatus(MeasurementStatus* history, float newVal);
void measurementTask (void*);
void updateHVIL(float* hvilReadingPtr, int *pin);
void updateTemperature(MeasurementStatus* temperature, int *pin) ;
void updateHvCurrent(MeasurementStatus* current, int *pin) ;
void updateHvVoltage(MeasurementStatus* voltage, int*pin);

#endif

#ifdef __cplusplus
}
#endif
