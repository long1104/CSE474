#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef ALARM_H_
#define ALARM_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

extern char *alarm_arr[];


typedef struct AlarmTaskData {
    float* hvia_val;
    float* overCurrent;
    float* hvor_val;
} AlarmData;


void alarmTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
