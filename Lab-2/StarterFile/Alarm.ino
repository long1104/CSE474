#include "Alarm.h"


char *alarm_arr[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};

void updateCurrentAlarm(float *current){
    *current = (getClockCount()%6)/2;
}

void updateHVORAlarm(float *hvor){
    Serial.print("before hvor: ");Serial.println(*(contactor.contactorStatus));
    *hvor = (getClockCount()%9)/3;
    Serial.print("before exitHvor: ");Serial.println(*(contactor.contactorStatus));
}

void updateHVIAAlarm(float *hvia){
    *hvia = getClockCount()%3;
}

void alarmTask(void* aData) {
    /****************
    * Function name: 
    * Function inputs: 
    * Function outputs: 
    * Function description: 
    * Author(s): 
    *****************/
    AlarmData* data = (AlarmData*) aData;
    Serial.print("before currentAlarm: ");Serial.println(*(contactor.contactorStatus));
    // Update all sensors
    updateCurrentAlarm(data->overCurrent);
    Serial.print("before hvorAlarm: ");Serial.println(*(contactor.contactorStatus));
    updateHVORAlarm(data->hvor_val);
    Serial.print("before hviaAlarm: ");Serial.println(*(contactor.contactorStatus));
    updateHVIAAlarm(data->hvia_val);
  return;
}
