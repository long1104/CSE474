#include "DataLogging.h"




void writeFloatToEEPROM(float value, int pos) {
    /****************
    Function name: writeFloatToEEPROM
    Function inputs: value: the value to be written in the EEPROM, pos: address in the EEPROM
    Function outputs: void return
    Function description: writes a float to the EEPROM at the specified address
    Authors:    Long Nguyen / Chase Arline
    *****************/
    noInterrupts();
    byte *perByte = (byte*)&value;          //raw 8 bit pointed at start of float
    for(int i=0; i<sizeof(value); i++) {
        EEPROM.write(pos+i, perByte[i]);   //write each byte of the float
    }
    interrupts();
    return;
}

float readFloatFromEEPROM(int pos) {
    /****************
    Function name: readFloatFromEEPROM
    Function inputs: pos: address in the EEPROM to read from
    Function outputs: float: the value read from the EEPROM at the address
    Function description: reads a float from the EEPROM at the address specified
    Authors:    Long Nguyen / Chase Arline
    *****************/
    float value;
    noInterrupts();
    byte *perByte = (byte*)&value;              //raw 8 bit pointed at start of float
    for(int i=0; i<sizeof(value); i++) {
        perByte[i]=EEPROM.read(pos+i);          // set each individual byte in the float to the 8 bits
    }
    interrupts();
    return value;
}

void updateEEPROM(MeasurementStatus* measurementsPtr, int posMin, int posMax) {
    /****************
    Function name: updateEEPROM
    Function inputs: measurementsPtr: pointer to measurement history to check for updates, posMin: eeprom address min, posMax: eeprom address max
    Function outputs: void return
    Function description: updates the eeprom of any changes in measurement history
    Authors:    Long Nguyen / Chase Arline
    *****************/
    if(measurementsPtr->minFlag) {
        writeFloatToEEPROM(measurementsPtr->minimum, posMin);
        measurementsPtr->minFlag=false;
    }
    if(measurementsPtr->maxFlag) {
        writeFloatToEEPROM(measurementsPtr->maximum, posMax);
        measurementsPtr->maxFlag=false;
    }
    return;
}

void resetMeasurements(MeasurementStatus* measurementsPtr, float resetValue, int posMin, int posMax) {
    +
    /****************
    Function name: resetMeasurements
    Function inputs: measurements: the measurement history to reset, resetValue: default resetValue: posMin: eeprom address min, posMax: eeprom address max
    Function outputs: void return
    Function description: updates the measurement history to default values and sets flags for measurement task to reset
    Authors:    Long Nguyen / Chase Arline
    *****************/
    measurementsPtr->minimum=resetValue;
    measurementsPtr->maximum=resetValue;
    measurementsPtr->resetFlag=true;
    writeFloatToEEPROM(resetValue, posMin);
    writeFloatToEEPROM(resetValue, posMax);
}

void dataLoggingTask (void* dlDataPtr) {
    /****************
    Function name: dataLoggingTask
    Function inputs: dlDataPtr: pointer to data logging task data
    Function outputs: void return
    Function description: logs the data in the EEPROM for the measurement history
    Authors:    Long Nguyen / Chase Arline
    *****************/
    DataLoggingTaskData* data = (DataLoggingTaskData*) dlDataPtr;
    if(*(data->resetFlag)) {
        resetMeasurements(data->current, 0, EEPROM_POS_CURRENT_MIN, EEPROM_POS_CURRENT_MAX);
        resetMeasurements(data->voltage, -1, EEPROM_POS_VOLTAGE_MIN, EEPROM_POS_VOLTAGE_MAX);
        resetMeasurements(data->temperature,0, EEPROM_POS_TEMP_MIN, EEPROM_POS_TEMP_MAX);
        *(data->resetFlag)=false;
        Serial.println("EEPROM CLEARED...");
    }
    updateEEPROM(data->temperature, EEPROM_POS_TEMP_MIN, EEPROM_POS_TEMP_MAX);
    updateEEPROM(data->current, EEPROM_POS_CURRENT_MIN, EEPROM_POS_CURRENT_MAX);
    updateEEPROM(data->voltage, EEPROM_POS_VOLTAGE_MIN, EEPROM_POS_VOLTAGE_MAX);

}
