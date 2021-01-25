#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include "StarterFile.h"
#include "Measurement.h"
#include "TaskControlBlock.h"
#include "Alarm.h"
#include "Contactor.h"
#include "Soc.h"

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

XYButton nextButton = {0,0,10,10};
XYButton previousButton = {40,40, 20,20};
XYButton batteryToggle = {120,120, 40,40};

XYButton buttons[] = {nextButton, previousButton, batteryToggle};

//Task Control Blocks
TCB measurementTCB;         // Declare measurement TCB
TCB alarmTCB;               // Declare alarm TCB
TCB contactorTCB;           // Declare contactor TCB
TCB socTCB;                 // Declare soc TCB



// Measurement Data
measurementData measure;    // Declare measurement data structure - defined in Measurement.h
float hvCurrent     = 0;
float hvVoltage     = 0;
float temperature   = 0;
bool HVIL           = 0;
const byte hvilPin  = 22;

// Alarm Data
char* alarm_states[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};
// .... other shared global data ....
alarmData alarm;
char* hviaVal     = alarm_states[0];
char* overCurrent  = alarm_states[0];
char* hvorVal     = alarm_states[0];

bool batteryOnOff = false;

// SOC data
socData soc;
float socVal = 0;

void loop() {
  /****************
    * Function name:    loop
    * Function inputs:  Sensor data, touch input
    * Function outputs: Display data and lights indicating alarm status, contactor status, sensor data, & state of charge
    * Function description: This is a round robin scheduler to run a series of tasks
    *                       that create a user interface to a battery management system
    * Author(s): 
    *****************/
    getTouchInput();
   // while(1){
  //  }
//    Serial.println(alarm_states[0]);
}


void drawButton(XYButton button, int color){
  tft.fillRect(button.x, button.y, button.xLength, button.yLength, color);
}

void getTouchInput(){
  TSPoint point = ts.getPoint();
  // scale from 0->1023 to tft.width
  point.x = map(point.x, TS_MINX, TS_MAXX, tft.width(), 0);
  //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
  point.y = (tft.height()-map(point.y, TS_MINY, TS_MAXY, tft.height(), 0));
  if(((point.x-batteryToggle.x)<batteryToggle.xLength && (point.x-batteryToggle.x)>0) && (point.y - batteryToggle.y)<batteryToggle.yLength && (point.y - batteryToggle.y)>0){
    Serial.println("hello");
      Serial.println(point.x);
    Serial.println(point.y);
  }else{
  }
}

void setup() {  
  /****************
    * Function name: setup
    * Function inputs: None
    * Function outputs: None
    * Function description: sets up the scheduler.
    * Author(s): 
    *****************/

    
    // Initialize Measurement & Sensors
    measure                     = {&HVIL, &hvilPin, &temperature, &hvCurrent, &hvVoltage};
    measurementTCB.task         = &measurementTask;
    measurementTCB.taskDataPtr  = (void*) &measure;
    measurementTCB.next         = NULL;
    measurementTCB.prev         = NULL;
    
    
    // ..... Initialize other tasks and task data ..... 
    // Initialize Display
    // Initialize Touch Input
    // Initialize Contactor
    contactorTCB.task          = &contactorTask;
    contactorTCB.taskDataPtr   = (void*) &batteryOnOff;
    contactorTCB.next          = NULL;
    contactorTCB.prev          = NULL;

    
    // Initialize Alarm
    alarm                      = {&hviaVal, &overCurrent, &hvorVal};
    alarmTCB.task              = &alarmTask;
    alarmTCB.taskDataPtr       = (void*) &alarm;
    alarmTCB.next              = NULL;
    alarmTCB.prev              = NULL;

    
    // Initialize SOC
    soc                        = {&socVal};
    socTCB.task                = &socTask;
    socTCB.taskDataPtr         = (void*) &soc;
    socTCB.next                = NULL;
    socTCB.prev                = NULL;

    // Initialize serial communication
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial1.setTimeout(1000);
    tft.reset();
    uint16_t identifier = tft.readID();
    if(identifier == 0x9325) {
      Serial.println(F("Found ILI9325 LCD driver"));
    } else if(identifier == 0x9328) {
      Serial.println(F("Found ILI9328 LCD driver"));
    } else if(identifier == 0x4535) {
      Serial.println(F("Found LGDP4535 LCD driver"));
    } else if(identifier == 0x7575) {
      Serial.println(F("Found HX8347G LCD driver"));
    } else if(identifier == 0x9341) {
      Serial.println(F("Found ILI9341 LCD driver"));
    } else if(identifier == 0x8357) {
      Serial.println(F("Found HX8357D LCD driver"));
    } else if(identifier==0x0101) {     
      identifier=0x9341;
      Serial.println(F("Found 0x9341 LCD driver"));
    } else if(identifier==0x1111) {     
      identifier=0x9328;
      Serial.println(F("Found 0x9328 LCD driver"));
    } else {
      Serial.print(F("Unknown LCD driver chip: "));
      Serial.println(identifier, HEX);
      Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
      Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
      Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
      Serial.println(F("If using the breakout board, it should NOT be #defined!"));
      Serial.println(F("Also if using the breakout, double-check that all wiring"));
      Serial.println(F("matches the tutorial."));
      identifier=0x9328;
    }
    tft.begin(identifier);

    drawButton(nextButton, BLACK);
    drawButton(previousButton, RED);
    drawButton(batteryToggle, GREEN);

    return;
}
