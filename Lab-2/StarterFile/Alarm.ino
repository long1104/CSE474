#include "Alarm.h"


char *alarm_arr[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};

void updateCurrentAlarm(float *current, int* clockCountPtr) {
    *current = ((*clockCountPtr) % 6) / 2;
}

void updateHVORAlarm(float *hvor, int* clockCountPtr) {
    *hvor = ((*clockCountPtr) % 9) / 3;
}

void updateHVIAAlarm(float *hvia, int* clockCountPtr) {
    *hvia = (*clockCountPtr) % 3;
}

void alarmTask(void* aData) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    AlarmData* data = (AlarmData*) aData;
    // Update all sensors
    updateCurrentAlarm(data->overCurrent, data->clockCountPtr);
    updateHVORAlarm(data->hvor_val, data->clockCountPtr);
    updateHVIAAlarm(data->hvia_val, data->clockCountPtr);
    return;
}
