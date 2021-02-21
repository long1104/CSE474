#ifdef __cplusplus
extern "C" {
#endif

#ifndef ALARM_H_
#define ALARM_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

typedef struct AlarmStruct {
    volatile float* alarmVal;   //volatile for hvil alarm reading
    float* measuredVal;
    bool* ack;
} Alarm;

//Used for updating the three alarms using a TCB
typedef struct AlarmTaskData {
    Alarm *overCurrentAlarm;
    Alarm *hvorAlarm;
    Alarm *hviaAlarm;
} AlarmData;


extern char *alarm_arr[];

void alarmTask (void*);
void updateHVIAAlarm(Alarm alarm);
void updateHVORAlarm(Alarm alarm);
void updateCurrentAlarm(Alarm alarm);

#endif

#ifdef __cplusplus
}
#endif
