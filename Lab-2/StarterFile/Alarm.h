#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef ALARM_H_
#define ALARM_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct alarmTaskData {
    char** hvia_val;
    char** overCurrent;
    char** hvor_val;
} alarmData;


void alarmTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
