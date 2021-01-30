#include <stdlib.h>
#include <stdbool.h>
#include "TouchScreenTask.h"

const int TEXT_SIZE = 1;
const int BUTTON_TEXT_SIZE=2;
const int TOUCH_TIMEOUT = 10;
const int pixelsPerChar_X = 6;
const int pixelsPerChar_Y = 8;
const int xPadding = 10;
const int yPadding = 10;


void setCursor(int x, int y){
  tft.setCursor(x+xPadding, y+yPadding);
}

void drawButton(XYButton button){
  tft.fillRect(button.x, button.y, button.xLength, button.yLength, button.color);
  tft.setTextColor(WHITE);
  tft.setTextSize(BUTTON_TEXT_SIZE);
  String label = String(*(button.buttonLabel));
  int halfLabelX = pixelsPerChar_X*label.length()*BUTTON_TEXT_SIZE/2;
  int halfLabelY = pixelsPerChar_Y*BUTTON_TEXT_SIZE/2;
  tft.setCursor(button.x +button.xLength/2-halfLabelX, button.y+button.yLength/2-halfLabelY);
  tft.print(*(button.buttonLabel));
}

Point getTouchInput(){
    TSPoint point = ts.getPoint();
    unsigned long timeout = millis();
    while(point.z<=MIN_PRESSURE && millis()-timeout <TOUCH_TIMEOUT){
           point = ts.getPoint();
    }
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    // scale from 0->1023 to tft.width
    //point.x = map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
      point.x = tft.width()-map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
      // point.y = (tft.height()-map(point.y, TS_MINY, TS_MAXY, tft.height(), 0));
      point.y = map(point.y, TS_MINY, TS_MAXY, tft.height(), 0);
    return Point{point.x,point.y};
}

bool isButton(Point point, XYButton button){
  return (((point.x-button.x)<button.xLength && (point.x-button.x)>0) 
  && (point.y - button.y)<button.yLength && (point.y - button.y)>0);
}

void drawLabel(PrintedData* printable){
  String label = String(printable->label);
  setCursor(printable->x, printable->y);
  tft.setTextColor(printable->color);
  tft.setTextSize(TEXT_SIZE);
  tft.print(label);
}

void drawData(PrintedData* printable, bool newScreen){
    String newData = String(printable->label);
    int pixelShift = TEXT_SIZE * newData.length() * 6; // 6pixels for each character (x value)
    float temp = *(printable->dataIn);
    if(temp!=printable->oldData || newScreen){
          tft.setTextSize(TEXT_SIZE);
          setCursor(printable->x+pixelShift, printable->y);
          String dataString = printDataToString(printable->oldData, printable->type);
          dataString.concat(printable->units);
          tft.setTextColor(BACKGROUND_COLOR);
          tft.print(dataString);

          printable->oldData=temp;          
          setCursor(printable->x+pixelShift, printable->y);
          dataString = printDataToString(printable->oldData, printable->type);
          dataString.concat(printable->units);
          tft.setTextColor(printable->color);
          tft.println(dataString);
    }
}


String printDataToString(float val, PRINT_TYPE type){
  String ret;
  switch(type){
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
    for(int i=0;i<screenList[*currScreen].dataLen;i++){
        drawData((PrintedData *)screenList[*currScreen].data[i], newScreen);
    }
    
}

bool inputTask(int* currScreen, Screen screenList[]) {
    Point point = getTouchInput();
    bool newScreen = false;
    if(isButton(point, previous)){
       *currScreen=(*currScreen+2)%3;
       newScreen = true;
    }else if(isButton(point, next)){
        *currScreen=(*currScreen+1)%3;
        newScreen = true;
    }
    if (isButton(point, *(screenList[2].button)) && *currScreen == 2) {
        *(screenList[2].data[0]->dataIn) = ((int)*(screenList[2].data[0]->dataIn) + 1) % 2;
    }
    return newScreen;
}

void drawScreen(Screen screen, bool newScreen){
  if (newScreen) {
    tft.fillScreen(BACKGROUND_COLOR);
    drawButton(previous);
    drawButton(next);

    for(int i=0;i<screen.dataLen;i++){
        drawLabel(screen.data[i]);
    }

    if (screen.button != NULL) {
      drawButton(*(screen.button));
    }
  }
}

void touchScreenTask(void* tscreenData) {
   TouchScreenData* datas = (TouchScreenData*) tscreenData;
   displayTask(datas->current_screen, datas->screens, *(datas->changeScreen));
   *(datas->changeScreen) = inputTask(datas->current_screen, datas->screens);
}
