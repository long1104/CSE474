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
    int *contactorPin;
    float* contactorStatus;
} ContactorData;


void contactorTask (void*);
void updateContactor(float* contactorStatus, int *contactorPin);

#endif

#ifdef __cplusplus
}
#endif
