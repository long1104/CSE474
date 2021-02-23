#include "DataLogging.h"




void writeFloatToEEPROM(float value, int pos){
    EEPROM.put(value, pos);
    return;
}

float readFloatFromEEPROM(int pos){
    float value;
    EEPROM.get(pos, value);
    return value;
}

void updateEEPROM(MeasurementStatus* measurements, int posMin, int posMax){
    if(measurements->minFlag){
        writeFloatToEEPROM(measurements->minimum, posMin);
    }
    if(measurements->maxFlag){
        writeFloatToEEPROM(measurements->maximum, posMax);
    }
    return;
}

void resetMeasurements(MeasurementStatus* measurements, float resetValue, int posMin, int posMax){
    measurements->minimum=resetValue;
    measurements->maximum=resetValue;
    writeFloatToEEPROM(resetValue, posMin);
    writeFloatToEEPROM(resetValue, posMax);
}

void dataLoggingTask (void* dlDataPtr){
     DataLoggingTaskData* data = (DataLoggingTaskData*) dlDataPtr;
     updateEEPROM(data->temperature, EEPROM_POS_TEMP_MIN, EEPROM_POS_TEMP_MAX);
     updateEEPROM(data->current, EEPROM_POS_CURRENT_MIN, EEPROM_POS_CURRENT_MAX);
     updateEEPROM(data->voltage, EEPROM_POS_VOLTAGE_MIN, EEPROM_POS_VOLTAGE_MAX);
     if(*(data->resetFlag)){
        resetMeasurements(data->current, 0, EEPROM_POS_CURRENT_MIN, EEPROM_POS_CURRENT_MAX);
        resetMeasurements(data->voltage, -1, EEPROM_POS_VOLTAGE_MIN, EEPROM_POS_VOLTAGE_MAX);
        resetMeasurements(data->temperature,0, EEPROM_POS_TEMP_MIN, EEPROM_POS_TEMP_MAX);
     }
}
