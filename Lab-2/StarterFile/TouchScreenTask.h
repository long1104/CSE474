#ifdef __cplusplus
extern "C" {
#endif

#ifndef TouchScreen_H_
#define TouchScreen_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef enum {
    ALARM, NUMBER, BOOL
} PRINT_TYPE;

typedef struct XYButtonStruct {
    int x, y, xLength, yLength, color;
    char** buttonLabelPtr;
} XYButton;


typedef struct PrintedDataStruct {
    int x, y, color;
    float oldData;
    PRINT_TYPE type;
    float* dataInPtr;
    char* labelPtr;
    char* unitsPtr;
} PrintedData;

typedef struct ScreenStruct {
    XYButton *buttonPtr;
    int dataLen;
    PrintedData **dataPtr;
} Screen;

typedef struct PointStruct {
    int x, y;
} Point;

typedef struct TouchScreenTaskData {
    int* clockCount;
    int* currentScreenPtr;
    bool *changeScreenPtr;
    Screen screens[];
} TouchScreenData;


void drawButton(XYButton button);
bool isButton(Point point, XYButton button);
void drawLabel(PrintedData* printable);
void drawData(PrintedData* printable, bool newScreen);
String printDataToString(float val, PRINT_TYPE type);
void displayTask(int* currScreen, Screen screenList[], bool newScreen);
bool inputTask(int* currScreen, Screen screenList[]);
void drawScreen(Screen screen, bool newScreen);
void touchScreenTask(void* tscreenData);
void setCursor(int x, int y);

#endif

#ifdef __cplusplus
}
#endif
