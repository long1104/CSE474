#include <TimerOne.h>

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
#include "Scheduler.h"
#include "RemoteTerminal.h"
#include "EEPROM.h"
#include "DataLogging.h";

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);                       // Display initialization
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 374);                                  // TFT initialization

char* prevLabelPtr = "Prev";
char* nextLabelPtr = "Next";
char* onLabelPtr = "ON";
char* offLabelPtr = "OFF";
char* ackLabel = "ACKNOWLEDGE";

//Initialization of buttons on the screens
XYButton previous = {0, 280, 80, 40, PURPLE, &prevLabelPtr};
XYButton next = {160, 280, 80, 40, PURPLE, &nextLabelPtr};
XYButton batteryOn = {0, 0, 110, 80, PURPLE, &onLabelPtr};
XYButton batteryOff = {130, 0, 110, 80, PURPLE, &offLabelPtr};
XYButton alarmButton = {40, 140, 160, 40, PURPLE, &ackLabel};

//Task Control Blocks
TCB measurementTCB = {};                                                            // Declare measurement TCB
TCB alarmTCB = {};                                                                  // Declare alarm TCB
TCB contactorTCB = {};                                                              // Declare contactor TCB
TCB socTCB = {};                                                                    // Declare soc TCB
TCB touchScreenTCB = {};
TCB remoteTerminalTCB = {};                                                         // Declare remote terminal TCB
TCB dataLoggingTCB = {};
DataLoggingTaskData dataLoggingTaskData = {};
RemoteTerminalData remoteTerminalTaskData = {};
bool resetEEPROM = false;
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
PrintedData hviaData = {};                                                          // High Voltage Alarm
PrintedData overCurrentData = {};                                                   // Over-Current Alarm
PrintedData hvorData = {};                                                          // High Voltage Out of Range Alarm
PrintedData batteryData = {};                                                       // Battery On/Off data

MeasurementStatus temperatureState = {};
MeasurementStatus currentState = {};
MeasurementStatus voltageState = {};

//Labels for screens (non changing data)
PrintedData measurementLabel = {};                                                  // Label for measurement screen
PrintedData alarmLabel = {};                                                        // Label for the alarm screen

Alarm overCurrentAlarm = {};
Alarm hvorAlarm = {};
Alarm hviaAlarm = {};

float zero = 0;                                                                     // Used for any non-changing printed data (labels)
int clockCount = 0;
// Measurement Data
MeasurementData measure;                                                            // Measurement Data structure, used in TCB
float hvCurrent = 0;                                                                // high voltage current value
float hvVoltage = 0;                                                                // high voltage voltage value
float temperature = 0;                                                              // temperature value
float HVIL = 0;                                                                     // high voltage interlock status
int hvilPin = 21;                                                                   // IO pin for hvil input
int temperaturePin = A13;
int hvVoltagePin = A14;
int hvCurrentPin = A15;


//ContactorData
ContactorData contactor = {};                                                       // contactor data structure, used in TCB
int contactorPin = 24;                                                              // IO pin for contactor output

AlarmData alarm;                                                                    // Alarm data structure, used in TCB
volatile float hviaVal = 0;                                                                  // high voltage interlock alarm status
float overCurrent = 0;                                                              // over-current alarm status
float hvorVal = 0;                                                                  // high voltage out of range alarm status

bool hviaAck = false;                                                               //Acknowledgement values for each alarm
bool hvorAck = false;
bool overCurrentAck = false;


volatile float batteryOnOff = 0;                                                             // battery status (open/closed)

// SOC data
SocData soc = {};                                                                   // state of charge data struct, used in TCB
float socVal = 0;                                                                   // state of charge value

// Touch Screen Task data
TouchScreenData tscreenData = {};                                                   // Touch screen data struct, used in TCB
int currentScreen = 0;
int lastScreen = 0;                                                                 // indicates which screen is currently displayed
bool acknowledgeDrawn = false;

volatile int timerFlag = 0;                                                         //Establishes time base in main loop

void timerISR() {
    timerFlag = 1;                      //set time base flag
}


void hvilISR() {
    batteryOnOff = 0;                   //volatile write to contactor status
    hviaVal = 1;                        //volatile write to hvil alarm status
    digitalWrite(contactorPin, LOW);    //set contactor pin low (turn contactor off)
}


void loop() {
    /****************
        Function name:    loop
        Function inputs:  No parameters
        Function outputs: No return
        Function description:
        Authors:    Long Nguyen / Chase Arline
      *****************/
    while (1) {
        if (timerFlag) {

            if (clockCount %3 == 0 && clockCount != 0) {
                insert(&touchScreenTCB);
            }
            if(clockCount %50 == 0 && clockCount != 0 ) {
                insert(&dataLoggingTCB);
            }
            if (clockCount % 10 == 0 && clockCount != 0) {
                insert(&remoteTerminalTCB);
            }
            Scheduler();
            timerFlag = 0; 
            if (clockCount % 10 == 0) {
                deleteNode(&remoteTerminalTCB);
            }
            if (clockCount % 3 == 0) {
                deleteNode(&touchScreenTCB);
            }
            if(clockCount %50 == 0  ) {
                deleteNode(&dataLoggingTCB);
            }
            clockCount++;
        }
    }
    return;
}

void initializeMeasurementHistory(){
    noInterrupts();
    EEPROM.get(EEPROM_POS_TEMP_MIN, (temperatureState.minimum)); Serial.println(temperatureState.minimum);
    EEPROM.get(EEPROM_POS_TEMP_MAX, (temperatureState.maximum)); Serial.println(temperatureState.maximum);
    EEPROM.get(EEPROM_POS_CURRENT_MIN, (currentState.minimum)); Serial.println(currentState.minimum);
    EEPROM.get(EEPROM_POS_CURRENT_MAX, (currentState.maximum)); Serial.println(currentState.maximum);
    EEPROM.get(EEPROM_POS_VOLTAGE_MIN, (voltageState.minimum));
    EEPROM.get(EEPROM_POS_VOLTAGE_MAX, (voltageState.maximum));
    interrupts();
}

void setup() {
    /****************
        Function name: setup
        Function inputs: None
        Function outputs: None
        Function description: initializes global variables, sets up and queues scheduler tasks, initialize display, set pinmodes for IO, sets up interrupts
        Authors:    Long Nguyen / Chase Arline
      *****************/
    Serial.begin(9600);
    pinMode(hvilPin, INPUT);                                                                        //hvil -> input pin
    pinMode(contactorPin, OUTPUT);                                                                  //contactor -> output pin
    pinMode(temperaturePin, INPUT_PULLUP);
    pinMode(hvVoltagePin, INPUT_PULLUP);
    pinMode(hvCurrentPin, INPUT_PULLUP);
    //setup time base interrupt to be 100ms
    Timer1.initialize(100000);
    Timer1.attachInterrupt(timerISR);
    //setup hvil interrupt routine on the rising edge of hvil pin
    attachInterrupt(digitalPinToInterrupt(hvilPin), hvilISR, RISING);

    // initialize all printed data values for the touch screen

    //State of charged printed data
    socDataPrint = {ORIGIN_X, ORIGIN_Y + 40, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &socVal, "SOC value: ", ""};

    //Measurement printed data
    temperatureData = {ORIGIN_X, ORIGIN_Y + 60, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &temperature, "Temperature: ", "C"};
    hvCurrentData = {ORIGIN_X, ORIGIN_Y + 80, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &hvCurrent, "HV Current: ", "A"};
    hvVoltageData = {ORIGIN_X, ORIGIN_Y + 100, PURPLE, SMALL_SCRIPT, DEFAULT_FLOAT, NUMBER, &hvVoltage, "HV Voltage: ", "V"};
    hvilData = {ORIGIN_X, ORIGIN_Y + 120, PURPLE, SMALL_SCRIPT, DEFAULT_BOOL, BOOL, &HVIL, "hvil: ", ""};
    measurementLabel = {ORIGIN_X, ORIGIN_Y, PURPLE, MED_SCRIPT, DEFAULT_BOOL, LABEL, &zero, "Measurements", ""};

    //Alarm printed data
    hviaData = {ORIGIN_X, ORIGIN_Y + 40, PURPLE, SMALL_SCRIPT, DEFAULT_ALARM, ALARM, &hviaVal, "hivl: ", ""};
    overCurrentData = {ORIGIN_X, ORIGIN_Y + 60, PURPLE, SMALL_SCRIPT, DEFAULT_ALARM, ALARM, &overCurrent, "Over Current: ", ""};
    hvorData = {ORIGIN_X, ORIGIN_Y + 80, PURPLE, SMALL_SCRIPT, DEFAULT_ALARM, ALARM, &hvorVal, "HV Alarm: ", ""};
    alarmLabel = {ORIGIN_X, ORIGIN_Y, PURPLE, MED_SCRIPT, DEFAULT_BOOL, LABEL, &zero, "Alarms", ""};

    //Battery/Contactor printed data
    batteryData = {ORIGIN_X, ORIGIN_Y + 80, PURPLE, SMALL_SCRIPT, DEFAULT_BOOL, BOOL, &batteryOnOff, "Battery Connection: ", ""};

    //Alarm state structs
    overCurrentAlarm = {&overCurrent, &hvCurrent, &overCurrentAck};
    hviaAlarm = {&hviaVal, &HVIL, &hviaAck};
    hvorAlarm = {&hvorVal, &hvVoltage, &hvorAck};

    //Initialize Screen structs for interface
    batteryMonitor = Screen{BATTERY_NUM_PRINTS , 2, {&batteryData}, {&batteryOn, &batteryOff}};
    alarmMonitor = Screen{ALARM_NUM_PRINTS, 1, {&alarmLabel, &hviaData, &overCurrentData, &hvorData}, {&alarmButton}};
    measurementMonitor = Screen{MEASURE_NUM_PRINTS, 0, {&measurementLabel, &socDataPrint, &temperatureData, &hvCurrentData, &hvVoltageData, &hvilData}, {}};

    temperatureState.data = &temperature;
    currentState.data = &hvCurrent;
    voltageState.data = &hvVoltage;

    // Initialize Measurement TCB
    measure                     = {&HVIL, &hvilPin, &temperatureState, &temperaturePin, &currentState, &hvCurrentPin, &voltageState, &hvVoltagePin};
    measurementTCB.task         = &measurementTask;
    measurementTCB.taskDataPtr  = (void*) &measure;
    measurementTCB.next         = &alarmTCB;
    measurementTCB.prev         = NULL;
    measurementTCB.taskName     = "measurement";

    initializeMeasurementHistory();

    //Initialize touchscreen/display TCB
    tscreenData = {&acknowledgeDrawn, &clockCount, &currentScreen, &lastScreen, &changeScreen ,  {overCurrentAlarm, hviaAlarm, hvorAlarm}, {measurementMonitor, alarmMonitor, batteryMonitor}};
    touchScreenTCB.task         = &touchScreenTask;
    touchScreenTCB.taskDataPtr  = (void*) &tscreenData;
    touchScreenTCB.next         = &contactorTCB;
    touchScreenTCB.prev         = &socTCB;
    touchScreenTCB.taskName     = "touchScreen";

    // Initialize Contactor TCB
    contactor = {&contactorPin,  &batteryOnOff};
    contactorTCB.task          = &contactorTask;
    contactorTCB.taskDataPtr   = (void*) &contactor;
    contactorTCB.next          = &dataLoggingTCB;
    contactorTCB.prev          = &touchScreenTCB;
    contactorTCB.taskName      = "contactor";

    remoteTerminalTaskData = {&temperatureState, &currentState, &voltageState, &resetEEPROM, true};
    remoteTerminalTCB.task = &remoteTerminalTask;
    remoteTerminalTCB.taskDataPtr = (void*) &remoteTerminalTaskData;
    remoteTerminalTCB.next = NULL;
    remoteTerminalTCB.prev = &dataLoggingTCB;
    remoteTerminalTCB.taskName = "remote terminal";

    dataLoggingTaskData = {&temperatureState, &currentState, &voltageState, &resetEEPROM};
    dataLoggingTCB.task = &dataLoggingTask;
    dataLoggingTCB.taskDataPtr = (void*) &dataLoggingTaskData;
    dataLoggingTCB.next = &remoteTerminalTCB;
    dataLoggingTCB.prev = &contactorTCB;
    dataLoggingTCB.taskName = "data logging";

    // Initialize Alarm TCB
    alarm                      = {&overCurrentAlarm, &hvorAlarm, &hviaAlarm};
    alarmTCB.task              = &alarmTask;
    alarmTCB.taskDataPtr       = (void*) &alarm;
    alarmTCB.next              = &socTCB;
    alarmTCB.prev              = &measurementTCB;
    alarmTCB.taskName          = "alarm";


    // Initialize SOC TCB
    soc                        = {&socVal, &temperature, &hvCurrent, &hvVoltage};
    socTCB.task                = &socTask;
    socTCB.taskDataPtr         = (void*) &soc;
    socTCB.next                = &touchScreenTCB;
    socTCB.prev                = &alarmTCB;
    socTCB.taskName            = "soc";

    head = &measurementTCB;
    tail = &remoteTerminalTCB;

    // Initialize serial communication

    Serial1.begin(9600);
    Serial1.setTimeout(20);
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
    Timer1.start();
    
    return;
}
