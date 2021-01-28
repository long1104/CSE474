#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <string.h>
#include "StarterFile.h"
#include "Measurement.h"
#include "TaskControlBlock.h"
#include "Alarm.h"
#include "Contactor.h"
#include "Soc.h"
#include "TouchScreenTask.h"

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

#define MINPRESSURE 10
#define MAXPRESSURE 1000

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

char* prevLabel = "Prev";
char* nextLabel = "Next";
char* onOffLabel = "OFF";
XYButton previous = {0,280,80, 40, BLUE, &prevLabel};
XYButton next = {160, 280, 80, 40, GREEN, &nextLabel};
XYButton batteryButton = {0,0,240,160, RED, &onOffLabel};
int BACKGROUND_COLOR = BLACK;


//XYButton buttons[] = {nextButton, previousButton, batteryToggle};

//Task Control Blocks
TCB measurementTCB;         // Declare measurement TCB
TCB alarmTCB;               // Declare alarm TCB
TCB contactorTCB;           // Declare contactor TCB
TCB socTCB;                 // Declare soc TCB
TCB touchScreenTCB;

int clockCount = 0;

Screen batteryMonitor;
Screen alarmMonitor;
Screen measurementMonitor;

// Data to print
PrintedData socDataPrint;
PrintedData temperatureData;
PrintedData hvCurrentData;
PrintedData hvVoltageData;
PrintedData hvilData;
PrintedData hivaData;           // High Voltage Alarm
PrintedData overCurrentData;
PrintedData hvorData;           // High Voltage Out of Range
PrintedData batteryData;  


// Measurement Data
measurementData measure;    // Declare measurement data structure - defined in Measurement.h
float hvCurrent;
float hvVoltage;
float temperature;
bool HVIL;
const byte hvilPin = 22;

// Alarm Data
char* alarm_states[] = {"NOT ACTIVE", "ACTIVE, NOT ACKNOWLEDGED", "ACTIVE, ACKNOWLEDGED"};
// .... other shared global data ....
alarmData alarm;
char* hviaVal     = alarm_states[0];
char* overCurrent  = alarm_states[0];
char* hvorVal     = alarm_states[0];

int batteryOnOff = 0;

// SOC data
socData soc;
float socVal = 0;

// Touch Screen Task data
TouchScreenData tscreenData;
int currentScreen = 0;

int getClockCount(){
  return clockCount;
}


int i=0;
void loop() {
  /****************
    * Function name:    loop
    * Function inputs:  Sensor data, touch input
    * Function outputs: Display data and lights indicating alarm status, contactor status, sensor data, & state of charge
    * Function description: This is a round robin scheduler to run a series of tasks
    *                       that create a user interface to a battery management system
    * Author(s): 
    *****************/
    
    PrintedData printedTemp = {0,0,GREEN,0,&temperature,"Temperature: ", "C"};
    PrintedData printedTemp2 = {0,20,GREEN,0,&temperature,"Current: ", "C"};

    
    
    //Screen screens[] = {MeasurementMonitor, BatteryMonitor, AlarmMonitor};
    int currentScreen=0;
    drawButton(previous);
    drawButton(next);
    while(1){
         int startTimer = millis();
//       Point point = getTouchInput();
//       bool newScreen = false;
//       if(isButton(point, previous)){
//          currentScreen=(currentScreen+2)%3;
//          newScreen = true;
//          //Serial.println(screens[currentScreen]);
//       }else if(isButton(point, next)){
//          currentScreen=(currentScreen+1)%3;
//          //Serial.println(screens[currentScreen]);
//          newScreen = true;
//       }

        //touchScreenTCB.task(touchScreenTCB.taskDataPtr);
        measurementTCB.task(measurementTCB.taskDataPtr);
        Serial.print("temperature: ");Serial.println(temperature);
        Serial.print("current: "); Serial.println(hvCurrent);
        Serial.print("voltage: "); Serial.println(hvVoltage);
        Serial.print("HVIL: "); Serial.println(HVIL);
        Serial.println();
//       drawData(&printedTemp);
//       if ((int)temperature % 2 == 0) {
//          drawData(&printedTemp2);
//       }
//       temperature++;
//        testFunction(&printedTemp);
//        Serial.println(printedTemp.oldData);
        clockCount++;
        delay(1000-(millis()-startTimer));
    }
}

void testFunction(PrintedData* printData) {
    printData->oldData += 1;
}

void setup() {  
  /****************
    * Function name: setup
    * Function inputs: None
    * Function outputs: None
    * Function description: sets up the scheduler.
    * Author(s): 
    *****************/
    hvCurrent = 0;
    hvVoltage = 0;
    temperature = 0;
    HVIL = false;


    socDataPrint = {0,0,GREEN,0,&socVal,"SOC value: ", "C"};
    temperatureData = {0,20,GREEN,0,&temperature,"Temperature: ", "C"};
    hvCurrentData = {0,40,GREEN,0,&hvCurrent,"HV Current: ", "C"};
    hvVoltageData = {0,60,GREEN,0,&hvVoltage,"HV Voltage: ", "C"};
    hvilData = {0,80,GREEN,0,&temperature,"hvil: ", "C"};
    hivaData = {0,0,GREEN,0,&temperature,"hivl: ", "C"};        // High Voltage Alarm
    overCurrentData = {0,20,GREEN,0,&temperature,"Over Current: ", "C"};
    hvorData = {0,40,GREEN,0,&temperature,"HV out of range: ", "C"};
    batteryData = {160, 80, WHITE, (float)0, (float*)&batteryOnOff, "OFF", ""};
    batteryMonitor = {&batteryButton,0,{&batteryData}};
    alarmMonitor = {NULL,3,{&hivaData, &overCurrentData, &hvorData}};
    measurementMonitor = {NULL, 5,{&socDataPrint, &temperatureData, &hvCurrentData, &hvVoltageData, &hvilData}};
    tscreenData = {&currentScreen, {measurementMonitor, alarmMonitor, batteryMonitor}};
    touchScreenTCB.task = &touchScreenTask;
    touchScreenTCB.taskDataPtr = (void*) &tscreenData;
    touchScreenTCB.next = NULL;
    touchScreenTCB.prev = NULL;

    
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
    tft.fillScreen(BLACK);


    return;
}
