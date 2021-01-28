#include <stdlib.h>
#include <stdbool.h>
#include "TouchScreenTask.h"

const int TEXT_SIZE = 2;

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
      point.x = tft.width()-map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
      // point.y = (tft.height()-map(point.y, TS_MINY, TS_MAXY, tft.height(), 0));
      point.y = map(point.y, TS_MINY, TS_MAXY, tft.height(), 0);
      Serial.print("X: ");Serial.println(point.x);
      Serial.print("Y: ");Serial.println(point.y);
      Serial.print("Z: ");Serial.println(point.z);
      return Point{point.x,point.y};
}

bool isButton(Point point, XYButton button){
  return (((point.x-button.x)<button.xLength && (point.x-button.x)>0) 
  && (point.y - button.y)<button.yLength && (point.y - button.y)>0);
}

void drawLabel(PrintedData* printable){
  String label = String(printable->label);
  tft.setCursor(printable->x, printable->y);
  tft.setTextColor(printable->color);
  tft.setTextSize(TEXT_SIZE);
  tft.print(label);
}

void drawData(PrintedData* printable){
    String newData = String(printable->label);
    int pixelShift = TEXT_SIZE * newData.length() * 6; // 6pixels for each character (x value)
    float temp = *(printable->dataIn);
    if(temp!=printable->oldData){
          tft.setTextSize(TEXT_SIZE);
          tft.setCursor(printable->x+pixelShift, printable->y);
          String dataString = String(printable->oldData);
          dataString.concat(printable->units);
          tft.setTextColor(BACKGROUND_COLOR);
          tft.print(dataString);

          printable->oldData=temp;          
          tft.setCursor(printable->x+pixelShift, printable->y);
          dataString = String(printable->oldData);
          dataString.concat(printable->units);
          tft.setTextColor(printable->color);
          tft.println(dataString);
    }
}

void displayTask(int* currScreen, Screen screenList[], bool isScroll) {
    drawScreen((screenList[*currScreen]), isScroll);
    Serial.print("change screen: ");Serial.println(isScroll);
    for(int i=0;i<screenList[*currScreen].dataLen;i+=1){
        drawData((PrintedData *)screenList[*currScreen].data[i]);
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
        Serial.println("Hello");
        newScreen = true;
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
