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
    volatile float* contactorStatus;      //volatile for contactor status (batteryOnOff var)
} ContactorData;


void contactorTask (void*);
void updateContactor(volatile float* contactorStatus, int *contactorPin);

#endif

#ifdef __cplusplus
}
#endif
