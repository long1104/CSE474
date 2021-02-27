#include "DataLogging.h"




void writeFloatToEEPROM(float value, int pos) {
    Serial.print("AT EEPROM ");Serial.print(pos);Serial.print(" is getting value: ");Serial.println(value);
    noInterrupts();
    //EEPROM.put(pos, value);
    byte *perByte = (byte*)&value;
     for(int i=0; i<sizeof(value);i++){
        EEPROM.write(pos+i, perByte[i]);
        Serial.print(" Byte: ");Serial.print(perByte[i]);
     }
     Serial.println();
     interrupts();
     return;
}

float readFloatFromEEPROM(int pos) {
    float value;
    noInterrupts();
    //EEPROM.get(pos, value);
 
    byte *perByte = (byte*)&value;                           //used to set each byte in the float representation
    for(int i=0; i<sizeof(value);i++){
       perByte[i]=EEPROM.read(pos+i);          // set each individual byte in the float to the correct value
    }
    Serial.println(value);
    interrupts();
    return value;
}

void updateEEPROM(MeasurementStatus* measurements, int posMin, int posMax) {
    if(measurements->minFlag) {
        writeFloatToEEPROM(measurements->minimum, posMin);
        measurements->minFlag=false;
    }
    if(measurements->maxFlag) {
        writeFloatToEEPROM(measurements->maximum, posMax);
        measurements->maxFlag=false;
    }
    return;
}

void resetMeasurements(MeasurementStatus* measurements, float resetValue, int posMin, int posMax) {
    measurements->minimum=resetValue;
    measurements->maximum=resetValue;
    measurements->resetFlag=true;
    writeFloatToEEPROM(resetValue, posMin);
    writeFloatToEEPROM(resetValue, posMax);
}

void dataLoggingTask (void* dlDataPtr) {
    DataLoggingTaskData* data = (DataLoggingTaskData*) dlDataPtr;
    if(*(data->resetFlag)) {
        Serial.println("CLEARING EEPROM...");
        resetMeasurements(data->current, 0, EEPROM_POS_CURRENT_MIN, EEPROM_POS_CURRENT_MAX);
        resetMeasurements(data->voltage, -1, EEPROM_POS_VOLTAGE_MIN, EEPROM_POS_VOLTAGE_MAX);
        resetMeasurements(data->temperature,0, EEPROM_POS_TEMP_MIN, EEPROM_POS_TEMP_MAX);
        *(data->resetFlag)=false;
    }
    updateEEPROM(data->temperature, EEPROM_POS_TEMP_MIN, EEPROM_POS_TEMP_MAX);
    updateEEPROM(data->current, EEPROM_POS_CURRENT_MIN, EEPROM_POS_CURRENT_MAX);
    updateEEPROM(data->voltage, EEPROM_POS_VOLTAGE_MIN, EEPROM_POS_VOLTAGE_MAX);

}
