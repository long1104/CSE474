#include "Soc.h"

float tempArr[4] = { -10, 0, 25, 45};
float voltArr[5] = {200, 250, 300, 350, 400};
float dataChart[4][5] = {{0, 10, 35, 100, 100},
    {0, 0, 20, 80, 100},
    {0, 0, 10, 60, 100},
    {0, 0, 0, 50, 100}
};

void updateStateOfCharge(float* stateOfChargeReadingPtr, float* temperature, float* hvCurrent, float* hvVoltage) {
    /****************
        Function name: updateStateOfCharge
        Function inputs: stateOfChargeReadingPtr: pointer to the status of the state of charge value
        Function outputs: void return
        Function description: updates the value of the state of charge data through the measured value
        Authors:    Long Nguyen / Chase Arline
    *****************/
    *stateOfChargeReadingPtr = 0;
    float voltageOC = computeOpenCircuitVoltage(*hvCurrent, *hvVoltage);
    return;
}

float computeOpenCircuitVoltage(float hvCurrent, float hvVoltage) {
    return hvVoltage + (0.5) * hvCurrent;
}

float getStateOfCharge(float temperature, float voltageOC) {
    float returnSOC = 0;
    int temperatureLower = -1;
    int temperatureUpper = -1;
    int voltageLower     = -1;
    int voltageUpper     = -1;
    if (voltageOC <= 200)  {
        returnSOC = 0;
//        Serial.println("Out of battery");
    } else if (voltageOC >= 400)  {
        returnSOC = 100;
//        Serial.println("Full battery");
    } else {
        getTemperatureBound(temperature, &temperatureLower, &temperatureUpper);
        getVoltageBound(voltageOC, &voltageLower, &voltageUpper);
//        Serial.print("temperatureLowerBound: "); Serial.println(temperatureLower);
//        Serial.print("temperatureUpperBound: "); Serial.println(temperatureUpper);
//        Serial.print("voltageLowerBound: "); Serial.println(voltageLower);
//        Serial.print("voltageUpperBound: "); Serial.println(voltageUpper);
    }
}

void getTemperatureBound(float temperatureVal, int* lowerBound, int* upperBound) {
    int tempLowerBound = -1;
    int tempUpperBound = -1;
    for (int i = 0; i < 3; i++) {
        float a = tempArr[i];
        float b = tempArr[i + 1];
        if (a == temperatureVal) {                             // Case where match exactly one of the listed value
            tempLowerBound = i;
            tempUpperBound = -1;
            break;
        } else if (b == temperatureVal) {
            tempLowerBound = -1;
            tempUpperBound = i + 1;
            break;
        } else if (a < temperatureVal && b > temperatureVal) { // Case where it is falling in between values.
            tempLowerBound = i;
            tempUpperBound = i + 1;
            break;
        }
    }
    *lowerBound = tempLowerBound;
    *upperBound = tempUpperBound;
    return;
}

void getVoltageBound(float voltageOCVal, int* lowerBound, int* upperBound) {
    int voltLowerBound = -1;
    int voltUpperBound = -1;
    for (int i = 0; i < 4; i++) {
        float a = voltArr[i];
        float b = voltArr[i + 1];
        if (a == voltageOCVal) {                           // Case where match exactly one of the listed value
            voltLowerBound = i;
            voltUpperBound = -1;
            break;
        } else if (b == voltageOCVal) {
            voltLowerBound = -1;
            voltUpperBound = i + 1;
            break;
        } else if (a < voltageOCVal && b > voltageOCVal) { // Case where it is falling in between values.
            voltLowerBound = i;
            voltUpperBound = i + 1;
            break;
        }
    }
    *lowerBound = voltLowerBound;
    *upperBound = voltUpperBound;
    return;
}

void socTask(void* sDataPtr) {
    /****************
        Function name: socTask
        Function inputs: sDataPtr: pointer to state of charge task data
        Function outputs: void return
        Function description: updates the state of charge value through the task data
        Authors:    Long Nguyen / Chase Arline
    *****************/
    SocData* data = (SocData*) sDataPtr;
    updateStateOfCharge(data->stateOfCharge, data->measuredTemperature, data->measuredHvCurrent, data->measuredHvVoltage);
    return;
}
