#ifdef __cplusplus
extern "C" {
#endif

#ifndef REMOTE_TERMINAL_H_
#define REMOTE_TERMINAL_H_

typedef struct RemoteTerminalDataStruct{
    MeasurementStatus* temperature;
    MeasurementStatus* current;
    MeasurementStatus* voltage;
    bool* resetEEPROM;
    bool printOps;
}RemoteTerminalData;


void printOptions();
void getUserInput(RemoteTerminalData* data);
void printMeasurementStatus(MeasurementStatus* state);
void remoteTerminalTask(void*);

#endif

#ifdef __cplusplus
}
#endif
