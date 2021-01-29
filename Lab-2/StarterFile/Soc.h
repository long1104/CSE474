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
} SocData;

void socTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
