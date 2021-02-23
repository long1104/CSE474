//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef DATA_LOGGING_H_
#define DATA_LOGGING_H_

#include "Measurement.h"
#include "EEPROM.h"

#define EEPROM_POS_TEMP_MIN 0
#define EEPROM_POS_TEMP_MAX 4
#define EEPROM_POS_CURRENT_MIN 8
#define EEPROM_POS_CURRENT_MAX 12
#define EEPROM_POS_VOLTAGE_MIN 16
#define EEPROM_POS_VOLTAGE_MAX 20


typedef struct DataLoggingTaskStruct {
    MeasurementStatus* current;
    MeasurementStatus* temperature;
    MeasurementStatus* voltage;
    bool* resetFlag;
}DataLoggingTaskData;


void writeFloatToEEPROM(float value, int pos);
float readFloatFromEEPROM(int pos);
void updateEEPROM(MeasurementStatus* measurements, int posMin, int posMax);
void resetMeasurements(MeasurementStatus* measurements, float resetValue, int posMin, int posMax);
void dataLoggingTask (void*);




#endif

//#ifdef __cplusplus
//}
//#endif
