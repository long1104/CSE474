#ifdef __cplusplus
extern "C" {
#endif

#ifndef TouchScreen_H_
#define TouchScreen_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

//PRINT_TYPE represents the variable type for printing
typedef enum {
    ALARM, NUMBER, BOOL, LABEL
} PRINT_TYPE;

//XYButton represents a button on the TFT scren
typedef struct XYButtonStruct {
    int x, y, xLength, yLength, color;
    char** buttonLabelPtr;
} XYButton;

//PrintedData represents data that can be printed/updated on the TFT screen
typedef struct PrintedDataStruct {
    int x, y, color, textSize;
    float oldData;
    PRINT_TYPE type;
    float* dataInPtr;
    char* labelPtr;
    char* unitsPtr;
} PrintedData;

//Screen represents a screen to be drawn on the display
typedef struct ScreenStruct {
    XYButton *buttonPtr;
    int dataLen;
    PrintedData **dataPtr;
} Screen;

//Point represents XY coordinates
typedef struct PointStruct {
    int x, y;
} Point;

//TouchScreenData is used for the TCB and holds data relevant to each cycle in the TCB
typedef struct TouchScreenTaskData {
    int* clockCount;
    int* currentScreenPtr;
    bool *changeScreenPtr;   
    Screen screens[3];
    Alarm alarms[3];
} TouchScreenData;


void drawButton(XYButton button);
bool isButton(Point point, XYButton button);
void drawLabel(char* label, int x, int y, int color);
void drawData(PrintedData* printable, bool newScreen);
String printDataToString(float val, PRINT_TYPE type);
void displayTask(int* currScreen, Screen screenList[], bool newScreen, Alarm alarms[]);
bool inputTask(int* currScreen, Screen screenList[], Alarm alarms[]);
void drawScreen(Screen screen, bool newScreen, int* currScreenPtr);
void touchScreenTask(void* tscreenData);
void setCursor(int x, int y);

#endif

#ifdef __cplusplus
}
#endif
