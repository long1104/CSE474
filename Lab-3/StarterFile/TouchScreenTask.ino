#include <stdlib.h>
#include <stdbool.h>
#include "TouchScreenTask.h"

#define BUTTON_TEXT_SIZE 2
#define TOUCH_TIMEOUT 20
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

void drawData(PrintedData* printablePtr, bool newScreen) {
    /****************
    Function name: drawData
    Function inputs: printablePtr: holds the data values, labels, units, and pointers, newScreen: has the screen just been changed?
    Function outputs: void return
    Function description: prints data onto the screen (only if old data != new data) 
    Authors:    Long Nguyen / Chase Arline
    ****************/
    String newData = String(printablePtr->labelPtr);
    int pixelShift = printablePtr->textSize * newData.length() * PIXELS_PER_CHAR_X;                    //shifting over in order to avoid redrawing label (only draw the changed data)
    float temp = *(printablePtr->dataInPtr);
    if (temp != printablePtr->oldData || newScreen) {
        tft.setTextSize(printablePtr->textSize);
        setCursor(printablePtr->x + pixelShift, printablePtr->y);
        String dataString = printDataToString(printablePtr->oldData, printablePtr->type);
        dataString.concat(printablePtr->unitsPtr);
        tft.setTextColor(BACKGROUND_COLOR);
        tft.print(dataString);                                                            //print over previous data with background color

        printablePtr->oldData = temp;
        setCursor(printablePtr->x + pixelShift, printablePtr->y);
        dataString = printDataToString(printablePtr->oldData, printablePtr->type);
        dataString.concat(printablePtr->unitsPtr);
        tft.setTextColor(printablePtr->color);
        tft.println(dataString);                                                          //prints new data where old data used to be (normal text color)
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

void displayTask(int* currScreenPtr, Screen screenList[], bool newScreen) {
    /****************
    Function name: displayTask
    Function inputs: currScreenPtr: points to value of current screen, screenList: list of all screens, newScreen: are we drawing a new screen? 
    Function outputs: void return
    Function description: draws all data structures that are inside of the screen, draws new screen if newScreen is true 
    Authors:    Long Nguyen / Chase Arline
    ****************/
    drawScreen((screenList[*currScreenPtr]), newScreen);
    for (int i = 0; i < screenList[*currScreenPtr].dataLen; i++) {
        drawData(screenList[*currScreenPtr].dataPtr[i], newScreen);
    }
    return;
}

bool inputTask(int* currScreenPtr, Screen screenList[], Alarm alarms) {
    /****************
    Function name: inputTask
    Function inputs: currScreenPtr: points to value of current screen, screenList: list of Screens
    Function outputs: bool, returns whether or not the display needs to be re-drawn
    Function description: handles touch input from the user and whether any buttons are pressed, determines if a new screen needs to be drawn
    Authors:    Long Nguyen / Chase Arline
    ****************/
    
    Point point = getTouchInput();
    bool newScreen = false;
    if (isButton(point, previous)) {
        *currScreenPtr = (*currScreenPtr + 2) % 3;
        newScreen = true;
    } else if (isButton(point, next)) {
        *currScreenPtr = (*currScreenPtr + 1) % 3;
        newScreen = true;
    }
    if (isButton(point, *(screenList[2].buttonPtr)) && *currScreenPtr == 2) {
        *(screenList[2].dataPtr[0]->dataInPtr) = ((int) * (screenList[2].dataPtr[0]->dataInPtr) + 1) % 2;
    }
    return newScreen;
}

void drawScreen(Screen screen, bool newScreen) {
    /****************
    Function name: drawScreen
    Function inputs: screen: the screen to be drawn, newScreen: are we updating the screen? (function is called every task loop) 
    Function outputs: void return
    Function description: draws the screen if newScreen == true
    Authors:    Long Nguyen / Chase Arline
    ****************/
    if (newScreen) {
        tft.fillScreen(BACKGROUND_COLOR);
        drawButton(previous);
        drawButton(next);

        for (int i = 0; i < screen.dataLen; i++) {
            drawLabel(screen.dataPtr[i]);                               //for each printed data, print its label
        }

        if (screen.buttonPtr != NULL) {
            drawButton(*(screen.buttonPtr));                           // if the screen has a button, draw it
        }
    }
    return;
}

void touchScreenTask(void* tscreenDataPtr) {
    /****************
    Function name: touchScreenTask
    Function inputs: 
    Function outputs: None
    Function description: updates the hvor alarm status every three cycles
    Authors:    Long Nguyen / Chase Arline
    ****************/
    TouchScreenData* datasPtr = (TouchScreenData*) tscreenDataPtr;
    if(*(datasPtr->clockCount)==0){
      displayTask(datasPtr->currentScreenPtr, datasPtr->screens, true);                         // if its the first time running the task, draw the whole screen
    }
    *(datasPtr->changeScreenPtr) = inputTask(datasPtr->currentScreenPtr, datasPtr->screens);    // get input from the touchscreen
    displayTask(datasPtr->currentScreenPtr, datasPtr->screens, *(datasPtr->changeScreenPtr));   // display data/labels/buttons
    return;
}
