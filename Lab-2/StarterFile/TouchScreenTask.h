#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef TouchScreen_H_
#define TouchScreen_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>

typedef struct XYButtonStruct {
  int x,y, xLength, yLength, color;
  char** buttonLabel;
} XYButton;

typedef struct PrintedDataStruct{
  int x,y,color;
  float oldData;
  float* dataIn;
  char* label;
  char* units;
}PrintedData;

typedef struct ScreenStruct {
  XYButton *button;
  PrintedData data[];
}Screen;

typedef struct PointStruct{
  int x,y;
}Point;

void drawButton(XYButton button);

#endif

#ifdef __cplusplus 
} 
#endif 
