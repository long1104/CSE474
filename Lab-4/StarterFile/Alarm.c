#include "Alarm.h"


char *alarm_arr[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};

void updateCurrentAlarm(Alarm alarm) {
    /****************
        Function name: updateCurrentAlarm
        Function inputs: alarm: Alarm struct holding overCurrent alarm data
        Function outputs: None
        Function description:  updates the overCurrent alarm using shared variables from measurement task and display task
        Authors:    Long Nguyen / Chase Arline
    ****************/
    *(alarm.alarmVal) = (float)(*(alarm.measuredVal) >= 20.0 || (int) * (alarm.measuredVal) <= -5.0);
    if (*(alarm.alarmVal) != 0) {
        *(alarm.alarmVal) += (float) * (alarm.ack);
    } else {
        *(alarm.ack) = 0;
    }
    return;
}

void updateHVORAlarm(Alarm alarm) {
    /****************
        Function name: updateHVORAlarm
        Function inputs: alarm: Alarm struct holding hvor alarm data
        Function outputs: None
        Function description:  updates the hvor alarm using shared variables from measurement task and display task
        Authors:    Long Nguyen / Chase Arline
    ****************/
    *(alarm.alarmVal) = (float)((int) * (alarm.measuredVal) >= 405.0 || (int) * (alarm.measuredVal) <= 280.0);
    if (*(alarm.alarmVal) != 0) {
        *(alarm.alarmVal) += (float) * (alarm.ack);
    } else {
        *(alarm.ack) = 0;
    }
    return;
}

void updateHVIAAlarm(Alarm alarm) {
    /****************
        Function name: updateHVIAAlarm
        Function inputs: alarm: Alarm struct holding hvil alarm data
        Function outputs: None
        Function description: updates the hvil alarm using shared variables from measurement task and display task
        Authors:    Long Nguyen / Chase Arline
    ****************/
    *(alarm.alarmVal) = !*(alarm.measuredVal);
    if (*(alarm.alarmVal) != 0) {
        *(alarm.alarmVal) += (float) * (alarm.ack);
    } else {
        *(alarm.ack) = 0;
    }
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
