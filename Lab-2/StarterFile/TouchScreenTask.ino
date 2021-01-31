#include <stdlib.h>
#include <stdbool.h>
#include "TouchScreenTask.h"

#define TEXT_SIZE 1
#define BUTTON_TEXT_SIZE 2
#define TOUCH_TIMEOUT 20
#define  PIXELS_PER_CHAR_X 6
#define PIXELS_PER_CHAR_Y 8
#define PADDING_X 10
#define PADDING_Y 10



void setCursor(int x, int y) {
    tft.setCursor(x + PADDING_X, y + PADDING_Y);
}

void drawButton(XYButton button) {
    tft.fillRect(button.x, button.y, button.xLength, button.yLength, button.color);
    tft.setTextColor(WHITE);
    tft.setTextSize(BUTTON_TEXT_SIZE);
    String label = String(*(button.buttonLabelPtr));
    int halfLabelX = PIXELS_PER_CHAR_X * label.length() * BUTTON_TEXT_SIZE / 2;
    int halfLabelY = PIXELS_PER_CHAR_Y * BUTTON_TEXT_SIZE / 2;
    tft.setCursor(button.x + button.xLength / 2 - halfLabelX, button.y + button.yLength / 2 - halfLabelY);
    tft.print(*(button.buttonLabelPtr));
}

Point getTouchInput() {
    TSPoint point = ts.getPoint();
    unsigned long timeout = millis();
    while (point.z <= MIN_PRESSURE && millis() - timeout < TOUCH_TIMEOUT) {
        point = ts.getPoint();
    }
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    // scale from 0->1023 to tft.width
    //point.x = map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
    point.x = tft.width() - map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
    // point.y = (tft.height()-map(point.y, TS_MINY, TS_MAXY, tft.height(), 0));
    point.y = map(point.y, TS_MINY, TS_MAXY, tft.height(), 0);
    return Point{point.x, point.y};
}

bool isButton(Point point, XYButton button) {
    return (((point.x - button.x) < button.xLength && (point.x - button.x) > 0)
            && (point.y - button.y) < button.yLength && (point.y - button.y) > 0);
}

void drawLabel(PrintedData* printable) {
    String label = String(printable->labelPtr);
    setCursor(printable->x, printable->y);
    tft.setTextColor(printable->color);
    tft.setTextSize(TEXT_SIZE);
    tft.print(label);
}

void drawData(PrintedData* printable, bool newScreen) {
    String newData = String(printable->labelPtr);
    int pixelShift = TEXT_SIZE * newData.length() * 6; // 6pixels for each character (x value)
    float temp = *(printable->dataInPtr);
    if (temp != printable->oldData || newScreen) {
        tft.setTextSize(TEXT_SIZE);
        setCursor(printable->x + pixelShift, printable->y);
        String dataString = printDataToString(printable->oldData, printable->type);
        dataString.concat(printable->unitsPtr);
        tft.setTextColor(BACKGROUND_COLOR);
        tft.print(dataString);

        printable->oldData = temp;
        setCursor(printable->x + pixelShift, printable->y);
        dataString = printDataToString(printable->oldData, printable->type);
        dataString.concat(printable->unitsPtr);
        tft.setTextColor(printable->color);
        tft.println(dataString);
    }
}


String printDataToString(float val, PRINT_TYPE type) {
    String ret;
    switch (type) {
        case ALARM: ret = alarm_arr[(int)val];
            break;
        case NUMBER: ret = String(val);
            break;
        case BOOL: ret = (int)val ? "CLOSED" : "OPEN";
            break;
        default: ret = String(val);
    }
    return ret;
}

void displayTask(int* currScreen, Screen screenList[], bool newScreen) {
    drawScreen((screenList[*currScreen]), newScreen);
    for (int i = 0; i < screenList[*currScreen].dataLen; i++) {
        drawData((PrintedData *)screenList[*currScreen].dataPtr[i], newScreen);
    }

}

bool inputTask(int* currScreen, Screen screenList[]) {
    Point point = getTouchInput();
    bool newScreen = false;
    if (isButton(point, previous)) {
        *currScreen = (*currScreen + 2) % 3;
        newScreen = true;
    } else if (isButton(point, next)) {
        *currScreen = (*currScreen + 1) % 3;
        newScreen = true;
    }
    if (isButton(point, *(screenList[2].buttonPtr)) && *currScreen == 2) {
        *(screenList[2].dataPtr[0]->dataInPtr) = ((int) * (screenList[2].dataPtr[0]->dataInPtr) + 1) % 2;
    }
    return newScreen;
}

void drawScreen(Screen screen, bool newScreen) {
    if (newScreen) {
        tft.fillScreen(BACKGROUND_COLOR);
        drawButton(previous);
        drawButton(next);

        for (int i = 0; i < screen.dataLen; i++) {
            drawLabel(screen.dataPtr[i]);
        }

        if (screen.buttonPtr != NULL) {
            drawButton(*(screen.buttonPtr));
        }
    }
}

void touchScreenTask(void* tscreenData) {
    TouchScreenData* datas = (TouchScreenData*) tscreenData;
    if(*(datas->clockCount)==0){
      displayTask(datas->currentScreenPtr, datas->screens, true);
    }
    *(datas->changeScreenPtr) = inputTask(datas->currentScreenPtr, datas->screens);
    displayTask(datas->currentScreenPtr, datas->screens, *(datas->changeScreenPtr));
}
