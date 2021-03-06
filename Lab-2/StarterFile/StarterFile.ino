#include "StarterFile.h"
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


Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);                       // Display initialization
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 374);                                  // TFT initialization


char* prevLabelPtr = "Prev";
char* nextLabelPtr = "Next";
char* onOffLabelPtr = "BATTERY TOGGLE";

//Initialization of buttons on the screens
XYButton previous = {0, 280, 80, 40, PURPLE, &prevLabelPtr};
XYButton next = {160, 280, 80, 40, PURPLE, &nextLabelPtr};
XYButton batteryButton = {0, 0, 240, 160, PURPLE, &onOffLabelPtr};


//Task Control Blocks
TCB measurementTCB = {};                                                            // Declare measurement TCB
TCB alarmTCB = {};                                                                  // Declare alarm TCB
TCB contactorTCB = {};                                                              // Declare contactor TCB
TCB socTCB = {};                                                                    // Declare soc TCB
TCB touchScreenTCB = {};
TCB *tasksPtr = NULL;

int clockCount = 0;
bool changeScreen = true;

Screen batteryMonitor = {};
Screen alarmMonitor = {};
Screen measurementMonitor = {};

// Data to print
PrintedData socDataPrint = {};                                                      // State of Charge Data
PrintedData temperatureData = {};                                                   // Temperature Data
PrintedData hvCurrentData = {};                                                     // High Voltage Current Data
PrintedData hvVoltageData = {};                                                     // High Voltage Voltage Data
PrintedData hvilData = {};                                                          // High Voltage Interlock input  data
PrintedData hivaData = {};                                                          // High Voltage Alarm
PrintedData overCurrentData = {};                                                   // Over-Current Alarm 
PrintedData hvorData = {};                                                          // High Voltage Out of Range Alarm
PrintedData batteryData = {};                                                       // Battery On/Off data

//Labels for screens (non changing data)
PrintedData measurementLabel = {};                                                  // Label for measurement screen
PrintedData alarmLabel = {};                                                        // Label for the alarm screen

float zero = 0;                                                                     // Used for any non-changing printed data (labels)

// Measurement Data
MeasurementData measure;                                                            // Measurement Data structure, used in TCB
float hvCurrent = 0;                                                                // high voltage current value
float hvVoltage = 0;                                                                // high voltage voltage value
float temperature = 0;                                                              // temperature value
float HVIL = 0;                                                                     // high voltage interlock status
const byte hvilPin = 22;                                                            // IO pin for hvil input

//ContactorData
ContactorData contactor = {};                                                       // contactor data structure, used in TCB
const byte contactorPin = 24;                                                       // IO pin for contactor output


AlarmData alarm;                                                                    // Alarm data structure, used in TCB
float hviaVal = 0;                                                                  // high voltage interlock alarm status
float overCurrent = 0;                                                              // over-current alarm status
float hvorVal = 0;                                                                  // high voltage out of range alarm status

float batteryOnOff = 0;                                                             // battery status (open/closed)

// SOC data
SocData soc = {};                                                                   // state of charge data struct, used in TCB
float socVal = 0;                                                                   // state of charge value

// Touch Screen Task data
TouchScreenData tscreenData = {};                                                   // Touch screen data struct, used in TCB
int currentScreen = 0;                                                              // indicates which screen is currently displayed


void Scheduler() {
   /****************
        Function name:    Scheduler
        Function inputs:  No parameters
        Function outputs: No return
        Function description: This is the round robin scheduler, it executes all tasks in a sequential order, 
                              the tasks create a user intreface for a battery management system
        Authors:    Long Nguyen / Chase Arline
      *****************/
    for (int i = 0; i < NUM_OF_TASKS; i++) {
        tasksPtr[i].task(tasksPtr[i].taskDataPtr);
    }
    return;
}

void loop() {
    /****************
        Function name:    loop
        Function inputs:  No parameters
        Function outputs: No return
        Function description: 
        Authors:    Long Nguyen / Chase Arline
      *****************/
    unsigned long startTimer = 0;
    while (1) {
        startTimer = millis();
        Scheduler();
        clockCount++;                                                                               // times the scheduler has run
        if (millis() - startTimer > 0 && millis() - startTimer < 1000) {                            // delay for rest of second 
            delay(1000 - (millis() - startTimer));
        }
    }
    return;
}


void setup() {
    /****************
        Function name: setup
        Function inputs: None
        Function outputs: None
        Function description: initializes global variables, sets up and queues scheduler tasks, initialize display, set pinmodes for IO
        Authors:    Long Nguyen / Chase Arline
      *****************/
    pinMode(hvilPin, INPUT);                                                                        //hvil -> input pin
    pinMode(contactorPin, OUTPUT);                                                                  //contactor -> output pin

    // initialize all printed data values for the touch screen

    //State of charged printed data
    socDataPrint = {ORIGIN_X, ORIGIN_Y+40, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &socVal, "SOC value: ", ""};

    //Measurement printed data
    temperatureData = {ORIGIN_X, ORIGIN_Y+60, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &temperature, "Temperature: ", "C"};
    hvCurrentData = {ORIGIN_X, ORIGIN_Y+80, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &hvCurrent, "HV Current: ", "A"};
    hvVoltageData = {ORIGIN_X, ORIGIN_Y+100, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &hvVoltage, "HV Voltage: ", "V"};
    hvilData = {ORIGIN_X, ORIGIN_Y+120, PURPLE, SMALL_SCRIPT, DEFAULT_BOOL, BOOL, &HVIL, "hvil: ", ""};
    measurementLabel = {ORIGIN_X,ORIGIN_Y, PURPLE, MED_SCRIPT, DEFAULT_BOOL, LABEL, &zero, "Measurements", ""};
    
    //Alarm printed data
    hivaData = {ORIGIN_X, ORIGIN_Y+40, PURPLE, SMALL_SCRIPT, DEFAULT_ALARM, ALARM, &hviaVal, "hivl: ", ""};
    overCurrentData = {ORIGIN_X, ORIGIN_Y+60, PURPLE, SMALL_SCRIPT, DEFAULT_ALARM, ALARM, &overCurrent, "Over Current: ", ""};
    hvorData = {ORIGIN_X, ORIGIN_Y+80, PURPLE, SMALL_SCRIPT, DEFAULT_ALARM, ALARM, &hvorVal, "HV Alarm: ", ""};
    alarmLabel = {ORIGIN_X, ORIGIN_Y, PURPLE, MED_SCRIPT, DEFAULT_BOOL, LABEL, &zero, "Alarms", ""};

    //Battery/Contactor printed data
    batteryData = {ORIGIN_X, ORIGIN_Y+160, PURPLE, SMALL_SCRIPT, DEFAULT_BOOL, BOOL, &batteryOnOff, "Battery Connection: ", ""};

   
    PrintedData *batteryPrints[] = {&batteryData};                                                                                         // holds battery data
    PrintedData *alarmPrints[] = {&alarmLabel, &hivaData, &overCurrentData, &hvorData};                                                    // holds alarm data
    PrintedData *measurementPrints[] = {&measurementLabel, &socDataPrint, &temperatureData, &hvCurrentData, &hvVoltageData, &hvilData};    // holds measurement data
    
    //Initialize Screen structs for interface
    batteryMonitor = Screen{&batteryButton, BATTERY_NUM_PRINTS, batteryPrints};          
    alarmMonitor = Screen{NULL, ALARM_NUM_PRINTS, alarmPrints};
    measurementMonitor = Screen{NULL, MEASURE_NUM_PRINTS, measurementPrints};

    //Initialize touchscreen/display TCB
    tscreenData = {&clockCount, &currentScreen, &changeScreen , {measurementMonitor, alarmMonitor, batteryMonitor}};
    touchScreenTCB.task = &touchScreenTask;
    touchScreenTCB.taskDataPtr = (void*) &tscreenData;
    touchScreenTCB.next = NULL;
    touchScreenTCB.prev = NULL;


    // Initialize Measurement TCB
    measure                     = {&HVIL, hvilPin, &temperature, &hvCurrent, &hvVoltage, &clockCount};
    measurementTCB.task         = &measurementTask;
    measurementTCB.taskDataPtr  = (void*) &measure;
    measurementTCB.next         = NULL;
    measurementTCB.prev         = NULL;


    // Initialize Contactor TCB
    contactor = {contactorPin,  &batteryOnOff};
    contactorTCB.task          = &contactorTask;
    contactorTCB.taskDataPtr   = (void*) &contactor;
    contactorTCB.next          = NULL;
    contactorTCB.prev          = NULL;


    // Initialize Alarm TCB
    alarm                      = {&hviaVal, &overCurrent, &hvorVal, &clockCount};
    alarmTCB.task              = &alarmTask;
    alarmTCB.taskDataPtr       = (void*) &alarm;
    alarmTCB.next              = NULL;
    alarmTCB.prev              = NULL;


    // Initialize SOC TCB
    soc                        = {&socVal, &clockCount};
    socTCB.task                = &socTask;
    socTCB.taskDataPtr         = (void*) &soc;
    socTCB.next                = NULL;
    socTCB.prev                = NULL;

    //Array of tasks used in scheduler 
    TCB temp[] = {measurementTCB, alarmTCB, socTCB, touchScreenTCB, contactorTCB};
    tasksPtr = temp;                                                                 

    // Initialize serial communication
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial1.setTimeout(1000);
    tft.reset();
    uint16_t identifier = tft.readID();
    if (identifier == 0x9325) {
        Serial.println(F("Found ILI9325 LCD driver"));
    } else if (identifier == 0x9328) {
        Serial.println(F("Found ILI9328 LCD driver"));
    } else if (identifier == 0x4535) {
        Serial.println(F("Found LGDP4535 LCD driver"));
    } else if (identifier == 0x7575) {
        Serial.println(F("Found HX8347G LCD driver"));
    } else if (identifier == 0x9341) {
        Serial.println(F("Found ILI9341 LCD driver"));
    } else if (identifier == 0x8357) {
        Serial.println(F("Found HX8357D LCD driver"));
    } else if (identifier == 0x0101) {
        identifier = 0x9341;
        Serial.println(F("Found 0x9341 LCD driver"));
    } else if (identifier == 0x1111) {
        identifier = 0x9328;
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
        identifier = 0x9328;
    }
    tft.begin(identifier);

    return;
}
