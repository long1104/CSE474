

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
