#include "Alarm.h"


char *alarm_arr[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};

void updateCurrentAlarm(float *current){
    *current = (getClockCount()%6)/2;
}

void updateHVORAlarm(float *hvor){
    *hvor = (getClockCount()%9)/3;
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
    
    // Update all sensors
    updateCurrentAlarm(data->overCurrent);
    updateHVORAlarm(data->hvor_val);
    updateHVIAAlarm(data->hvia_val);
  return;
}
