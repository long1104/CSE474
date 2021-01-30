#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef CONTACTOR_H_
#define CONTACTOR_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


void contactorTask (void*);


typedef struct ContactorTaskData{
    const byte* contactorPin;
    float* contactorStatus;
} ContactorData;


#endif

#ifdef __cplusplus 
} 
#endif 
