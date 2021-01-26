#ifdef __cplusplus  
extern "C" { 
#endif 


typedef struct XYButtonStruct {
  int x,y, xLength, yLength, color;
} XYButton;

typedef struct PrintedDataStruct{
  int x,y;
  float data;
  char* label;
  char* units; 
}PrintedData;

typedef struct ScreenStruct {
  XYButton* buttons;
  PrintedData* data;
}Screen;

#ifdef __cplusplus 
} 
#endif 
