#include "Alarm.h"


char *alarm_arr[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};

void updateCurrentAlarm(Alarm alarm) {
    /****************
    Function name: updateCurrentAlarm
    Function inputs: currentPtr: pointer to overCurrent alarm 
    Function outputs: None
    Function description: updates the overCurrent alarm status every 2 cycles
    Authors:    Long Nguyen / Chase Arline
    ****************/
    *(alarm.alarmVal) = (float)(*(alarm.measuredVal)>=20 || *(alarm.measuredVal) <= -5);
    *(alarm.alarmVal) += (float)*(alarm.ack);
    return;
}

void updateHVORAlarm(Alarm alarm) {
    /****************
    Function name: updateHVORAlarm
    Function inputs: hvorPtr: pointer to hvor alarm 
    Function outputs: None
    Function description: updates the hvor alarm status every three cycles
    Authors:    Long Nguyen / Chase Arline
    ****************/
    *(alarm.alarmVal) = (float)(*(alarm.measuredVal)>=405 || *(alarm.measuredVal) <= 280);
    *(alarm.alarmVal) += (float)*(alarm.ack);
    return;
}

void updateHVIAAlarm(Alarm alarm) {
    /****************
    Function name: updateHVIAAlarm
    Function inputs: hviaPtr: pointer to hvia alarm, clockCountPtr: pointer to clockCount
    Function outputs: None
    Function description: updates the hvia alarm status every cycle
    Authors:    Long Nguyen / Chase Arline
    ****************/
    *(alarm.alarmVal) = !*(alarm.measuredVal);
    *(alarm.alarmVal) += (float)*(alarm.ack);
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
    updateCurrentAlarm(*(data->overCurrentAlarm));
    updateHVORAlarm(*(data->hvorAlarm));
    updateHVIAAlarm(*(data->hviaAlarm));
    return;
}
