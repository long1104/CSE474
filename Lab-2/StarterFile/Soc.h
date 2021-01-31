#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOC_H_
#define SOC_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct SocTaskData {
    float* stateOfCharge;
    int* clockCountPtr;
} SocData;

void socTask (void*);
void updateStateOfCharge(float* stateOfChargeReading, int* clockCountPtr);

#endif

#ifdef __cplusplus
}
#endif
