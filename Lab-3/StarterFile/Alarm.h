#ifdef __cplusplus
extern "C" {
#endif

#ifndef ALARM_H_
#define ALARM_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

//Used for updating the three alarms using a TCB
typedef struct AlarmTaskData {
    float* hvia_val;
    float* overCurrent;
    float* hvor_val;
    int* clockCountPtr;
} AlarmData;


extern char *alarm_arr[];

void alarmTask (void*);
void updateHVIAAlarm(float *hvia, int* clockCountPtr);
void updateHVORAlarm(float *hvor, int* clockCountPtr);
void updateCurrentAlarm(float *current, int* clockCountPtr);

#endif

#ifdef __cplusplus
}
#endif
