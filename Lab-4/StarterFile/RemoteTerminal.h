#ifndef REMOTE_TERMINAL_H_
#define REMOTE_TERMINAL_H_

typedef struct RemoteTerminalDataStruct{
    MeasurementStatus* temperature;
    MeasurementStatus* current;
    MeasurementStatus* voltage;
    bool* resetEEPROM;
}RemoteTerminalData;


void printOptions();
void getUserInput(RemoteTerminalData* data);
void printMeasurementStatus(MeasurementStatus* state);
void remoteTerminalTask(void*);

#endif
