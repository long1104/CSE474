#ifdef __cplusplus
extern "C" {
#endif

#ifndef CONTACTOR_H_
#define CONTACTOR_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

//Used for updating the contactor output through a TCB
typedef struct ContactorTaskData {
    byte contactorPin;
    float* contactorStatus;
} ContactorData;


void contactorTask (void*);
void updateContactor(float* contactorStatus, const byte* contactorPin);

#endif

#ifdef __cplusplus
}
#endif
