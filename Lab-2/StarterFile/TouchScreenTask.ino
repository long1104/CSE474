#include <stdlib.h>
#include <stdbool.h>
#include "TouchScreenTask.h"

void drawButton(XYButton button){
  tft.fillRect(button.x, button.y, button.xLength, button.yLength, button.color);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(button.x + (button.xLength / 3), button.y + (button.yLength / 3));
  tft.print(*(button.buttonLabel));
}

Point getTouchInput(){
    TSPoint point = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    // scale from 0->1023 to tft.width
    //point.x = map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
    if (point.z > MINPRESSURE && point.z < MAXPRESSURE) {
      point.x = tft.width()-map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
      // point.y = (tft.height()-map(point.y, TS_MINY, TS_MAXY, tft.height(), 0));
      point.y = map(point.y, TS_MINY, TS_MAXY, tft.height(), 0);
      return Point{point.x,point.y};
    }
    return Point{-1, -1};;
}

bool isButton(Point point, XYButton button){
  return (((point.x-button.x)<button.xLength && (point.x-button.x)>0) 
  && (point.y - button.y)<button.yLength && (point.y - button.y)>0);
}

void drawData(PrintedData* printable){
    tft.setTextSize(2);
    tft.setCursor(printable->x,printable->y);
    tft.setTextColor(printable->color);
    tft.print(printable->label);
    tft.setTextColor(BACKGROUND_COLOR);
    String dataString = String(printable->oldData);
    dataString.concat(printable->units);
    tft.print(dataString);
    tft.setCursor(printable->x,printable->y);
    tft.setTextColor(printable->color);
   // printable->oldData = *(printable->dataIn);
    String newData = String(printable->label);
    newData.concat(printable->oldData);
    newData.concat(printable->units);
    tft.println(newData);
}

void displayTask(int* currScreen, Screen screenList[], bool isScroll) {
    
    //Serial.print(*currScreen);
    drawScreen((screenList[*currScreen]), isScroll);
    for(int i=0;i<screenList[*currScreen].dataLen;i+=1){
        Serial.println("HELLO");
        Serial.println(i);
        Serial.println(*(screenList[*currScreen].data[i]->dataIn));
      //  drawData(screenList[*currScreen].data[i]);
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
        if (*(screenList[2].data[0]->dataIn) == 1) {
          *(screenList[2]).button->buttonLabel = "ON";
        } else {
          *(screenList[2]).button->buttonLabel = "OFF";
        }
        Serial.println(*(screenList[2]).button->buttonLabel);
        newScreen = true;
    }
    return newScreen;
}

void drawScreen(Screen screen, bool newScreen){
  if (newScreen) {
    tft.fillScreen(BACKGROUND_COLOR);
    drawButton(previous);
    drawButton(next);
    if (screen.button != NULL) {
      drawButton(*(screen.button));
    }
  }
}

void touchScreenTask(void* tscreenData) {
   TouchScreenData* datas = (TouchScreenData*) tscreenData;
   bool isScroll = inputTask(datas->current_screen, datas->screens);
   displayTask(datas->current_screen, datas->screens, isScroll);
}
