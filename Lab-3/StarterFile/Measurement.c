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

void updateTemperature(float* temperatureReadingPtr, int*pin) {
    /****************
        Function name:updateTemperature
        Function inputs:temperatureReadingPtr: pointer to status of temperature value, pin: the pin to perform analog read from potentiometer
        Function outputs: void return
        Function description: updates the value of the temperature through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *temperatureReadingPtr = ((float)(analogRead(*pin)) * 55.0 / 1023.0) - 10;
    return;
}

void updateHvCurrent(float* currentReadingPtr, int *pin) {
    /****************
        Function name: updateHvCurrent
        Function inputs: currentReadingPtr: pointer to status of current value, pin: the pin to perform analog read from potentiometer
        Function outputs: void return
        Function description: updates the value of the current through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *currentReadingPtr = ((float)analogRead(*pin)) * 50.0 / 1023.0 - 25;
    return;
}

void updateHvVoltage(float* voltageReadingPtr, int*pin) {
    /****************
        Function name: updateHvVoltage
        Function inputs: voltageReadingPtr: pointer to status of voltage value, pin: the pin to perform analog read from potentiometer
        Function outputs: void return
        Function description: updates the value of the voltage through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *voltageReadingPtr = ((float)analogRead(*pin)) * 450.0 / 1023.0;
    return;
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
    updateHvCurrent(data->hvCurrent, data->hvCurrentPin);
    updateHvVoltage(data->hvVoltage, data->hvVoltagePin);

    return;
}
