#include <stdlib.h>
#include <stdbool.h>
#include "Measurement.h"

float temperatureValues[] = { -10, 5, 25};
float currentValues[] = { -20, 0, 20};
float voltageValues[] = {10, 150, 45};

void updateHVIL(float* hvilReadingPtr, byte pin) {
    /****************
        Function name: updateHVIL
        Function inputs: hvilReadingPtr: pointer to status of HVIL, pin: hardware input pin of HVIL measurement
        Function outputs: void return
        Function description: updates the value of the HVIL through the measurement pin
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *hvilReadingPtr = !(float)(int)digitalRead(pin);                //read HVIL input
    return;
}

void updateTemperature(float* temperatureReadingPtr, int* clockCountPtr) {
    /****************
        Function name:updateTemperature
        Function inputs:temperatureReadingPtr: pointer to status of temperature value, clockCountPtr: pointer to value of the cycle count
        Function outputs: void return
        Function description: updates the value of the temperature through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *temperatureReadingPtr = temperatureValues[(*clockCountPtr) % 3];
    return;
}

void updateHvCurrent(float* currentReadingPtr, int* clockCountPtr) {
    /****************
        Function name: updateHvCurrent
        Function inputs: currentReadingPtr: pointer to status of current value, clockCountPtr: pointer to the value of the cycle count
        Function outputs: void return
        Function description: updates the value of the current through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *currentReadingPtr = currentValues[((*clockCountPtr) % 6) / 2];
    return;
}

void updateHvVoltage(float* voltageReadingPtr, int* clockCountPtr) {
    /****************
        Function name: updateHvVoltage
        Function inputs: voltageReadingPtr: pointer to status of voltage value, clockCountPtr: pointer to the value of the cycle count
        Function outputs: void return
        Function description: updates the value of the voltage through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *voltageReadingPtr = voltageValues[((*clockCountPtr) % 9) / 3];
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
    updateTemperature(data->temperature, data->clockCountPtr);
    updateHvCurrent(data->hvCurrent, data->clockCountPtr);
    updateHvVoltage(data->hvVoltage, data->clockCountPtr);

    return;
}
