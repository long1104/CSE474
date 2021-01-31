#include <stdlib.h>
#include <stdbool.h>
#include "Measurement.h"

float temperatureValues[] = { -10, 5, 25};
float currentValues[] = { -20, 0, 20};
float voltageValues[] = {10, 150, 45};

void updateHVIL(float* hvilReading, const byte* pin) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    *hvilReading = !(float)(int)digitalRead(*pin);
    return;
}

void updateTemperature(float* temperatureReading, int* clockCountPtr) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    *temperatureReading = temperatureValues[(*clockCountPtr) % 3];
    return;
}

void updateHvCurrent(float* currentReading, int* clockCountPtr) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    *currentReading = currentValues[((*clockCountPtr) % 6) / 2];
    return;
}

void updateHvVoltage(float* voltageReading, int* clockCountPtr) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    *voltageReading = voltageValues[((*clockCountPtr) % 9) / 3];
    return;
}

void measurementTask(void* mData) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    MeasurementData* data = (MeasurementData*) mData;

    // Update all sensors
    updateHVIL(data->hvilStatus, data->hvilPin);
    updateTemperature(data->temperature, data->clockCountPtr);
    updateHvCurrent(data->hvCurrent, data->clockCountPtr);
    updateHvVoltage(data->hvVoltage, data->clockCountPtr);

    return;
}
