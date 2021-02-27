#include "RemoteTerminal.h"


void printOptions() {
    Serial.println("[1] Reset EEPROM");
    Serial.println("[2] HV Current Range [Hi, Lo]");
    Serial.println("[3] HV Voltage Range [Hi, Lo]");
    Serial.println("[4] Temperature Range [Hi, Lo]");
    Serial.print("Enter your menu choice [1-4]: ");
}
void getUserInput(RemoteTerminalData* data) {
    char datas = Serial.read();
    data->printOps = true;
    switch(datas) {
    case '1':
        *(data->resetEEPROM)=1;
        Serial.println();
        break;
    case '2':
        printMeasurementStatus(data->current);
        break;
    case '3':
        printMeasurementStatus(data->voltage);
        break;
    case '4':
        printMeasurementStatus(data->temperature);
        break;
    default:
        data->printOps=false;
    }

}
void printMeasurementStatus(MeasurementStatus* state) {
    String p = "\n[";
    p.concat(String(state->maximum));
    p.concat(",");
    p.concat(String(state->minimum));
    p.concat("]\n");
    Serial.println(p);
}


void remoteTerminalTask(void* rDataPtr) {
    /****************
        Function name: remoteTerminalTask
        Function inputs: rDataPtr: pointer to measurement task data
        Function outputs: void return
        Function description: Gives the user options in the serial monitor and responds to the user input
        Authors:    Long Nguyen / Chase Arline
    *****************/
    RemoteTerminalData* data = (RemoteTerminalData*) rDataPtr;
    if(data->printOps) {
        printOptions();
    }
    // Update all sensors
    getUserInput(data);

    return;
}
