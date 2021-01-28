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
  char* units;
  char* label;
}PrintedData;

typedef struct ScreenStruct {
  XYButton *button;
  int dataLen;
  PrintedData **data;
}Screen;

typedef struct PointStruct{
  int x,y;
}Point;

typedef struct TouchScreenTaskData {
  int* current_screen;
  Screen screens[];
} TouchScreenData;

void drawButton(XYButton button);

#endif

#ifdef __cplusplus 
} 
#endif 
