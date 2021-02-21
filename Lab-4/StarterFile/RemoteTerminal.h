

typedef struct RemoteTerminalDataStruct{
    MeasurementStatus* temperature;
    MeasurementStatus* current;
    MeasurementStatus* voltage;
    bool* resetEEPROM;
}RemoteTerminalData;


void printOptions(RemoteTerminalData* data);
void getUserInput(RemoteTerminalData* data);
void printMeasurementStatus(MeasurementStatus* state);
void remoteTerminalTask(void*);
