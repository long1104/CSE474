#ifdef __cplusplus  
extern "C" { 
#endif 


typedef struct XYButtonStruct {
  int x,y, xLength, yLength, color;
} XYButton;

typedef struct PrintedDataStruct{
  int x,y,color;
  float oldData;
  float* dataIn;
  char* label;
  char* units;
}PrintedData;

typedef struct ScreenStruct {
  XYButton* buttons;
  PrintedData* data;
}Screen;

typedef struct PointStruct{
  int x,y;
}Point;

#ifdef __cplusplus 
} 
#endif 
