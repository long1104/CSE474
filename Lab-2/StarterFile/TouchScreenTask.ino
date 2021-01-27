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
  point.x = tft.width()-map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
 // point.y = (tft.height()-map(point.y, TS_MINY, TS_MAXY, tft.height(), 0));
  point.y = map(point.y, TS_MINY, TS_MAXY, tft.height(), 0);
  return Point{point.x,point.y};
}

bool isButton(Point point, XYButton button){
  return (((point.x-button.x)<button.xLength && (point.x-button.x)>0) 
  && (point.y - button.y)<button.yLength && (point.y - button.y)>0);
}

float drawData(PrintedData printable){
    tft.setTextSize(2);
    tft.setCursor(printable.x,printable.y);
    tft.setTextColor(printable.color);
    tft.print(printable.label);
    tft.setTextColor(BACKGROUND_COLOR);
    tft.print(printable.oldData);
    tft.print(printable.units);
    tft.setCursor(printable.x,printable.y);
    tft.setTextColor(printable.color);
    printable.oldData = *printable.dataIn;
    tft.print(printable.label);
    tft.print(printable.oldData);
    tft.print(printable.units);
    return printable.oldData;
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
