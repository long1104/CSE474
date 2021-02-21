#include <stdlib.h>
#include <stdbool.h>
#include "TouchScreenTask.h"

#define BUTTON_TEXT_SIZE 2
#define TOUCH_TIMEOUT 5
#define  PIXELS_PER_CHAR_X 6
#define PIXELS_PER_CHAR_Y 8
#define PADDING_X 10
#define PADDING_Y 10

void setCursor(int x, int y) {
    /****************
        Function name: setCursor
        Function inputs: x: x position of tft cursor, y: y position of tft cursor
        Function outputs: void return
        Function description: updates the TFT cursor position WITH x and y padding
        Authors:    Long Nguyen / Chase Arline
    ****************/
    tft.setCursor(x + PADDING_X, y + PADDING_Y);
    return;
}

void deleteButton(XYButton button) {
    /****************
         Function name: drawButton
         Function inputs: button: the XYButton to be drawn on the screen
         Function outputs: void return
         Function description: draws the XYButton on the TFT screen
         Authors:    Long Nguyen / Chase Arline
     ****************/
    tft.fillRect(button.x, button.y, button.xLength, button.yLength, BACKGROUND_COLOR);
    tft.setTextColor(BACKGROUND_COLOR);
    tft.setTextSize(BUTTON_TEXT_SIZE);
    String label = String(*(button.buttonLabelPtr));
    int halfLabelX = PIXELS_PER_CHAR_X * label.length() * BUTTON_TEXT_SIZE / 2;
    int halfLabelY = PIXELS_PER_CHAR_Y * BUTTON_TEXT_SIZE / 2;
    tft.setCursor(button.x + button.xLength / 2 - halfLabelX,
                  button.y + button.yLength / 2 - halfLabelY);                        //aligns the label to be middle of the button
    tft.print(*(button.buttonLabelPtr));
    return;
}

void drawButton(XYButton button) {
    /****************
        Function name: drawButton
        Function inputs: button: the XYButton to be drawn on the screen
        Function outputs: void return
        Function description: draws the XYButton on the TFT screen
        Authors:    Long Nguyen / Chase Arline
    ****************/
    tft.fillRect(button.x, button.y, button.xLength, button.yLength, button.color);
    tft.setTextColor(WHITE);
    tft.setTextSize(BUTTON_TEXT_SIZE);
    String label = String(*(button.buttonLabelPtr));
    int halfLabelX = PIXELS_PER_CHAR_X * label.length() * BUTTON_TEXT_SIZE / 2;
    int halfLabelY = PIXELS_PER_CHAR_Y * BUTTON_TEXT_SIZE / 2;
    tft.setCursor(button.x + button.xLength / 2 - halfLabelX,
                  button.y + button.yLength / 2 - halfLabelY);                        //aligns the label to be middle of the button
    tft.print(*(button.buttonLabelPtr));
    return;
}

Point getTouchInput() {
    /****************
        Function name: getTouchInput
        Function inputs: no parameters
        Function outputs: Point: touch input data
        Function description: gathers input data from the touch screen
        Authors:    Long Nguyen / Chase Arline
    ****************/
    TSPoint point = ts.getPoint();
    unsigned long timeout = millis();
    while (point.z <= MIN_PRESSURE && millis() - timeout < TOUCH_TIMEOUT) {             //check for valid user input for a time period
        point = ts.getPoint();
    }
    pinMode(XM, OUTPUT);                                                                //set pins back to output (touch uses them for input)
    pinMode(YP, OUTPUT);
    point.x = tft.width() - map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);             //from tft example, scales the x coordinate to useful 240x320 coords
    point.y = map(point.y, TS_MINY, TS_MAXY, tft.height(), 0);                          //from tft example, scales y coordiante
    return Point{point.x, point.y};
}

bool isButton(Point point, XYButton button) {
    /****************
        Function name: isButton
        Function inputs: point: touch screen data, button: XYButton to be tested
        Function outputs: bool, button was touched -> true, button was not touched -> false
        Function description: checks the touch input to see if it is on a button
        Authors:    Long Nguyen / Chase Arline
    ****************/
    return (((point.x - button.x) < button.xLength && (point.x - button.x) > 0)         //checking if touch input is within the boundary of button
            && (point.y - button.y) < button.yLength && (point.y - button.y) > 0);
}

void drawLabel(PrintedData *printablePtr) {
    /****************
        Function name: drawlabel
        Function inputs: label: pointer to label start, x: x coordinate, y: y coordinate, color: text color
        Function outputs: void return
        Function description: draws the label on the display
        Authors:    Long Nguyen / Chase Arline
    ****************/
    String labelS = String(printablePtr->labelPtr);
    setCursor(printablePtr->x, printablePtr->y);
    tft.setTextColor(printablePtr->color);
    tft.setTextSize(printablePtr->textSize);
    tft.print(labelS);
    return;
}

void deletePrintedData(PrintedData *printablePtr) {
    String boxDim = String(printablePtr->labelPtr);
    boxDim.concat(printDataToString(printablePtr->oldData, printablePtr->type));
    boxDim.concat(printablePtr->unitsPtr);
    tft.fillRect(printablePtr->x + PADDING_X, printablePtr->y + PADDING_Y, boxDim.length()*PIXELS_PER_CHAR_X * printablePtr->textSize, PIXELS_PER_CHAR_Y * printablePtr->textSize, BACKGROUND_COLOR);
    return;
}

void drawData(PrintedData* printablePtr, bool newScreen) {
    /****************
        Function name: drawData
        Function inputs: printablePtr: holds the data values, labels, units, and pointers, newScreen: has the screen just been changed?
        Function outputs: void return
        Function description: prints data onto the screen (only if old data != new data)
        Authors:    Long Nguyen / Chase Arline
    ****************/
    String labelL = printablePtr->labelPtr;
    int pixelShift = printablePtr->textSize * labelL.length() * PIXELS_PER_CHAR_X;                    //shifting over in order to avoid redrawing label (only draw the changed data)
    float temp = *(printablePtr->dataInPtr);
    if (temp != printablePtr->oldData || newScreen) {
        String clearBox = printDataToString(printablePtr->oldData, printablePtr->type);
        clearBox.concat(printablePtr->unitsPtr);
        tft.fillRect(printablePtr->x + pixelShift + PADDING_X, printablePtr->y + PADDING_Y, clearBox.length()*PIXELS_PER_CHAR_X * printablePtr->textSize, PIXELS_PER_CHAR_Y * printablePtr->textSize, BACKGROUND_COLOR);
        printablePtr->oldData = temp;
        String dataString = printDataToString(printablePtr->oldData, printablePtr->type);
        dataString.concat(printablePtr->unitsPtr);
        setCursor(printablePtr->x + pixelShift, printablePtr->y);
        tft.setTextColor(printablePtr->color);
        tft.setTextSize(printablePtr->textSize);
        tft.print(dataString);                                                          //prints new data where old data used to be (normal text color)
    }
    return;
}


String printDataToString(float val, PRINT_TYPE type) {
    /****************
        Function name: printDataToString
        Function inputs: val: value to be converted to string, type: enum that tells type of variable
        Function outputs: String, the value of the parameter converted to it's string format
        Function description: converts a value and its type into its string representation
        Authors:    Long Nguyen / Chase Arline
    ****************/
    String ret;
    switch (type) {
        case ALARM: ret = alarm_arr[(int)val];
            break;
        case NUMBER: ret = String(val);
            break;
        case BOOL: ret = (int)val ? "CLOSED" : "OPEN";
            break;
        case LABEL: ret = "";                         // label does not have any changing data
            break;
        default: ret = String(val);
    }
    return ret;
}

void displayTask(int* currScreenPtr, Screen screenList[], bool newScreen, Alarm alarms[], int* lastScreenPtr, bool* acknowledgeDrawn) {
    /****************
        Function name: displayTask
        Function inputs: currScreenPtr: points to value of current screen, screenList: list of all screens, newScreen: are we drawing a new screen?
        Function outputs: void return
        Function description: draws all data structures that are inside of the screen, draws new screen if newScreen is true
        Authors:    Long Nguyen / Chase Arline
    ****************/
    drawScreen(screenList, newScreen, alarms, currScreenPtr, lastScreenPtr, acknowledgeDrawn);
    for (int i = 0; i < screenList[*currScreenPtr].dataLen; i++) {
        drawData(screenList[*currScreenPtr].dataPtr[i], newScreen);
    }
    return;
}

bool emergencyCheck(Alarm alarms[]) {
    bool emergency = false;
    for (int i = 0; i < 3; i++) {
        if (*(alarms[i].alarmVal) == 1 && *(alarms[i].ack) == 0) {
            emergency = true;
            break;
        }
    }
    return emergency;
}

bool inputTask(int* currScreenPtr, Screen screenList[], Alarm alarms[], int* lastScreenPtr) {
    /****************
        Function name: inputTask
        Function inputs: currScreenPtr: points to value of current screen, screenList: list of Screens, alarms: array of Alarms, needed to set acknowledgement flags, lastScreenPtr: the last screen the display was on
        Function outputs: bool, returns whether or not the display needs to be re-drawn
        Function description: handles touch input from the user and whether any buttons are pressed, determines if a new screen needs to be drawn, sets alarm acknowledgement flags
        Authors:    Long Nguyen / Chase Arline
    ****************/
    bool emergency = emergencyCheck(alarms);
    //    bool emergency = false;
    Point point = getTouchInput();
    bool newScreen = false;
    if (isButton(point, previous) && !emergency) {
        *lastScreenPtr = *currScreenPtr;
        *currScreenPtr = (*currScreenPtr + 2) % 3;
        newScreen = true;
    } else if (isButton(point, next) && !emergency) {
        *lastScreenPtr = *currScreenPtr;
        *currScreenPtr = (*currScreenPtr + 1) % 3;
        newScreen = true;
    } else if (emergency && *currScreenPtr != 1) {
        *lastScreenPtr = *currScreenPtr;
        *currScreenPtr = 1;
        newScreen = true;
    }
    if (isButton(point, *(screenList[2].buttonsPtr[0])) && *currScreenPtr == 2 && *(alarms[1].alarmVal) == 0) { //turn on battery if hvil alarm is not active and button is pressed
        *(screenList[2].dataPtr[0]->dataInPtr) = 1 && (*(alarms[1].alarmVal) == 0);                             //safety check against hvil alarm to make sure code stays atomic (both variable checks are volatile)
    } else if (isButton(point, *(screenList[2].buttonsPtr[1])) && *currScreenPtr == 2) {                        //turn off battery if button is pressed
        *(screenList[2].dataPtr[0]->dataInPtr) = 0;
    }


    if (isButton(point, *(screenList[1].buttonsPtr[0])) && *currScreenPtr == 1) {
        for (int i = 0; i < 3; i++) {
            if (*(alarms[i].alarmVal) == 1) {
                *(alarms[i].ack) = 1;
            }
        }
    }
    return newScreen;
}

void drawScreen(Screen screens[], bool newScreen, Alarm alarms[], int* currScreenPtr, int* lastScreenPtr, bool* acknowledgeDrawn) {
    /****************
        Function name: drawScreen
        Function inputs: screen: the screen to be drawn, newScreen: determines whether we are switching screens, alarms: array of alarms displayed on Alarm screen,
                          currScreenPtr: current screen, lastScreenPtr: last screen the display was on, acknowledgeDrawn: status of the acknowledge button on alarm screen
        Function outputs: void return
        Function description: deletes the last screen and draws the new one if newScreen==true
        Authors:    Long Nguyen / Chase Arline
    ****************/
    if (newScreen) {

        for (int i = 0; i < screens[*lastScreenPtr].dataLen; i++) {
            deletePrintedData(screens[*lastScreenPtr].dataPtr[i]); //for each printed data, print its label
        }
        if (screens[*lastScreenPtr].buttonLen > 0) {
            for (int i = 0; i < screens[*lastScreenPtr].buttonLen; i++) {
                deleteButton(*(screens[*lastScreenPtr].buttonsPtr[i]));
            }
        }
        *acknowledgeDrawn = false;
        for (int i = 0; i < screens[*currScreenPtr].dataLen; i++) {
            drawLabel(screens[*currScreenPtr].dataPtr[i]);                               //for each printed data, print its label
        }

        if (screens[*currScreenPtr].buttonLen > 0) {
            if (*currScreenPtr != 1) {
                for (int i = 0; i < screens[*currScreenPtr].buttonLen; i++) {
                    drawButton(*(screens[*currScreenPtr].buttonsPtr[i]));
                }
            } else if (emergencyCheck(alarms)) {
                *acknowledgeDrawn = true;
                drawButton(*(screens[*currScreenPtr].buttonsPtr[0]));
            }
        }
    } else if (*currScreenPtr == 1 && !emergencyCheck(alarms) && *acknowledgeDrawn) {
        *acknowledgeDrawn = false;
        deleteButton(*(screens[*currScreenPtr].buttonsPtr[0]));
    } else if (*currScreenPtr == 1 && emergencyCheck(alarms) && !*acknowledgeDrawn) {
        *acknowledgeDrawn = true;
        drawButton(*(screens[*currScreenPtr].buttonsPtr[0]));
    }
    return;
}

void touchScreenTask(void* tscreenDataPtr) {
    /****************
        Function name: touchScreenTask
        Function inputs:  tscreenDataPtr: touch scren task data
        Function outputs: None
        Function description: manages both the input task and display task for the touchscreen
        Authors:    Long Nguyen / Chase Arline
    ****************/
    TouchScreenData* datasPtr = (TouchScreenData*) tscreenDataPtr;
    if (*(datasPtr->clockCount) == 0) {
        tft.fillScreen(BACKGROUND_COLOR);
        drawButton(previous);
        drawButton(next);
        displayTask(datasPtr->currentScreenPtr, datasPtr->screens, true, datasPtr->alarms, datasPtr->lastScreenPtr, datasPtr->acknowledgeDrawn);  // if its the first time running the task, draw the whole screen
        *(datasPtr->clockCount)++;
    }
    *(datasPtr->changeScreenPtr) = inputTask(datasPtr->currentScreenPtr, datasPtr->screens, datasPtr->alarms, datasPtr->lastScreenPtr);    // get input from the touchscreen
    displayTask(datasPtr->currentScreenPtr, datasPtr->screens, *(datasPtr->changeScreenPtr), datasPtr->alarms, datasPtr->lastScreenPtr, datasPtr->acknowledgeDrawn);   // display data/labels/buttons
    return;
}
