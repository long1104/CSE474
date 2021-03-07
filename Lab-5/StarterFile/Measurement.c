#include <stdlib.h>
#include <stdbool.h>
#include "Measurement.h"


void updateHVIL(float* hvilReadingPtr, int* pin) {
    /****************
        Function name: updateHVIL
        Function inputs: hvilReadingPtr: pointer to status of HVIL, pin: hardware input pin of HVIL measurement
        Function outputs: void return
        Function description: updates the value of the HVIL through the measurement pin
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *hvilReadingPtr = !(float)(int)digitalRead(*pin);                //read HVIL input
    return;
}

void updateTemperature(MeasurementStatus* temperaturePtr, int*pin) {
    /****************
        Function name:updateTemperature
        Function inputs:temperaturePtr: measurement history for the temperature, pin: the pin to perform analog read from potentiometer
        Function outputs: void return
        Function description: updates the value of the temperature through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    float readT = ((float)(analogRead(*pin)) * 55.0 / 1023.0) - 10;
    updateMeasurementStatus(temperaturePtr, readT);
    return;
}

void updateHvCurrent(MeasurementStatus* currentPtr, int *pin) {
    /****************
        Function name: updateHvCurrent
        Function inputs: currentPtr: measurement history for the current, pin: the pin to perform analog read from potentiometer
        Function outputs: void return
        Function description: updates the value of the current through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    float readC = ((float)analogRead(*pin)) * 50.0 / 1023.0 - 25;
    updateMeasurementStatus(currentPtr, readC);
    return;
}

void updateHvVoltage(MeasurementStatus* voltagePtr, int*pin) {
    /****************
        Function name: updateHvVoltage
        Function inputs: voltagePtr: measurement history for voltage, pin: the pin to perform analog read from potentiometer
        Function outputs: void return
        Function description: updates the value of the voltage through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    float readV = ((float)analogRead(*pin)) * 450.0 / 1023.0;
    updateMeasurementStatus(voltagePtr, readV);
    return;
}

void updateMeasurementStatus(MeasurementStatus* historyPtr, float newVal) {
    /****************
    Function name: updateMeasurementStatus
    Function inputs: historyPtr: measurement status history, newVal: the newly read value from the measurement pins
    Function outputs: void return
    Function description: updates the measurement history and sets flags for the EEPROM if needed
    Authors:    Long Nguyen / Chase Arline
    *****************/
    *(*historyPtr).data = newVal;
    if(historyPtr->resetFlag) {
        historyPtr->maximum=newVal;
        historyPtr->minimum=newVal;
        historyPtr->maxFlag=true;
        historyPtr->minFlag=true;
        historyPtr->resetFlag=false;
    } else if(newVal>(*historyPtr).maximum) {
        historyPtr->maxFlag=true;
        historyPtr->maximum = newVal;
    } else if (newVal<(*historyPtr).minimum) {
        historyPtr->minFlag=true;
        historyPtr->minimum=newVal;
    }
}


void measurementTask(void* mDataPtr) {
    /****************
        Function name: measurementTask
        Function inputs: mDataPtr: pointer to measurement task data
        Function outputs: void return
        Function description: updates the HVIL input, temperature input, HV current input, HV voltage input with the measured values
        Authors:    Long Nguyen / Chase Arline
    *****************/
    MeasurementData* data = (MeasurementData*) mDataPtr;
    // Update all sensors
    updateHVIL(data->hvilStatus, data->hvilPin);
    updateTemperature(data->temperature, data->temperaturePin);
    updateHvCurrent(data->current, data->currentPin);
    updateHvVoltage(data->voltage, data->voltagePin);

    return;
}
