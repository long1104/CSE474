#include "Alarm.h"


char *alarm_arr[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};

void updateCurrentAlarm(float *currentPtr, int* clockCountPtr) {
    /****************
    Function name: updateCurrentAlarm
    Function inputs: currentPtr: pointer to overCurrent alarm 
    Function outputs: None
    Function description: updates the overCurrent alarm status every 2 cycles
    Authors:    Long Nguyen / Chase Arline
    ****************/
    *currentPtr = ((*clockCountPtr) % 6) / 2;\
    return;
}

void updateHVORAlarm(float *hvorPtr, int* clockCountPtr) {
    /****************
    Function name: updateHVORAlarm
    Function inputs: hvorPtr: pointer to hvor alarm 
    Function outputs: None
    Function description: updates the hvor alarm status every three cycles
    Authors:    Long Nguyen / Chase Arline
    ****************/
    *hvorPtr = ((*clockCountPtr) % 9) / 3;
    return;
}

void updateHVIAAlarm(float *hviaPtr, int* clockCountPtr) {
    /****************
    Function name: updateHVIAAlarm
    Function inputs: hviaPtr: pointer to hvia alarm, clockCountPtr: pointer to clockCount
    Function outputs: None
    Function description: updates the hvia alarm status every cycle
    Authors:    Long Nguyen / Chase Arline
    ****************/
    *hviaPtr = (*clockCountPtr) % 3; 
    return;
}

void alarmTask(void* aDataPtr) {
    /****************
        Function name: alarmTask
        Function inputs: aDataPtr: pointer to alarm task data
        Function outputs: void return 
        Function description: updates the hvia (hvil alarm), hvor (high voltage out of range), and overcurrent alarm status
        Authors:    Long Nguyen / Chase Arline
    *****************/
    AlarmData* data = (AlarmData*) aDataPtr;
    // Update all sensors
    updateCurrentAlarm(data->overCurrent, data->clockCountPtr);
    updateHVORAlarm(data->hvor_val, data->clockCountPtr);
    updateHVIAAlarm(data->hvia_val, data->clockCountPtr);
    return;
}
