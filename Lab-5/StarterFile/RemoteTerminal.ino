#include "RemoteTerminal.h"


void printOptions() {
    Serial.println("[1] Reset EEPROM");
    Serial.println("[2] HV Current Range [Hi, Lo]");
    Serial.println("[3] HV Voltage Range [Hi, Lo]");
    Serial.println("[4] Temperature Range [Hi, Lo]");
    Serial.print("Enter your menu choice [1-4]: ");
}
void getUserInput(RemoteTerminalData* dataPtr) {
    /****************
    Function name: getUserInputw
    Function inputs: data: the remote terminal task data
    Function outputs: void return
    Function description: gets user input from serial monitor and responds appropriately
    Authors:    Long Nguyen / Chase Arline
    *****************/
    char datas = Serial.read();
    dataPtr->printOps = true;
    switch(datas) {
    case '1':
        *(dataPtr->resetEEPROM)=1;
        break;
    case '2':
        printMeasurementStatus(dataPtr->current);
        break;
    case '3':
        printMeasurementStatus(dataPtr->voltage);
        break;
    case '4':
        printMeasurementStatus(dataPtr->temperature);
        break;
    default:
        dataPtr->printOps=false;
    }

}
void printMeasurementStatus(MeasurementStatus* statePtr) {
    /****************
    Function name: printMeasurementStatus
    Function inputs: statePtr: the measurement history state (min/max data)
    Function outputs: void return
    Function description: prints the measurement histry [MAX, MIN] to the serial monitor
    Authors:    Long Nguyen / Chase Arline
    *****************/
    String p = "\n[";
    p.concat(String(statePtr->maximum));
    p.concat(",");
    p.concat(String(statePtr->minimum));
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
