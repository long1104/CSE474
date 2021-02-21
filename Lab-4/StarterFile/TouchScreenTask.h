#ifdef __cplusplus
extern "C" {
#endif

#ifndef TouchScreen_H_
#define TouchScreen_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

#define MAX_BUTTONS 2       //max buttons on any screen (excluding prev/next). defines length of button array
#define MAX_DATA 6          //max printed data on any screen. defines length of printed data array

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
    volatile float oldData;               //volatile for some printed data, not all
    PRINT_TYPE type;
    volatile float* dataInPtr;           // ^^^
    char* labelPtr;
    char* unitsPtr;
} PrintedData;

//Screen represents a screen to be drawn on the display
typedef struct ScreenStruct {
    int dataLen, buttonLen;             //real amount of printed data/buttons, only occupies a fraction of the array below
    PrintedData *dataPtr[MAX_DATA];
    XYButton *buttonsPtr[MAX_BUTTONS];
} Screen;

//Point represents XY coordinates
typedef struct PointStruct {
    int x, y;
} Point;

//TouchScreenData is used for the TCB and holds data relevant to each cycle in the TCB
typedef struct TouchScreenTaskData {
    bool* acknowledgeDrawn;
    int* clockCount;
    int* currentScreenPtr;
    int* lastScreenPtr;
    bool *changeScreenPtr;
    Alarm alarms[3];
    Screen screens[3];
} TouchScreenData;


void drawButton(XYButton button);
bool isButton(Point point, XYButton button);
void drawLabel(char* label, int x, int y, int color);
void drawData(PrintedData* printable, bool newScreen);
String printDataToString(float val, PRINT_TYPE type);
void displayTask(int* currScreen, Screen screenList[], bool newScreen, Alarm alarms[], int* lastScreenPtr, bool* acknowledgeDrawn);
bool inputTask(int* currScreen, Screen screenList[], Alarm alarms[], int* lastScreenPtr);
void drawScreen(Screen screen, bool newScreen, int* currScreenPtr, int* lastScreenPtr, bool* acknowledgeDrawn);
void touchScreenTask(void* tscreenData);
void setCursor(int x, int y);

#endif

#ifdef __cplusplus
}
#endif
