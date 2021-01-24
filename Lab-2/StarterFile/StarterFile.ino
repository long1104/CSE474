#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Measurement.h"
#include "TaskControlBlock.h"


//Task Control Blocks
TCB measurementTCB;         // Declare measurement TCB



// Measurement Data
measurementData measure;    // Declare measurement data structure - defined in Measurement.h
float hvCurrent     = 0;
float hvVoltage     = 0;
float temperature   = 0;
bool HVIL           = 0;
const byte hvilPin  = 22;


// .... other shared global data ....



void loop() {
  /****************
    * Function name:    loop
    * Function inputs:  Sensor data, touch input
    * Function outputs: Display data and lights indicating alarm status, contactor status, sensor data, & state of charge
    * Function description: This is a round robin scheduler to run a series of tasks
    *                       that create a user interface to a battery management system
    * Author(s): 
    *****************/
    while(1){
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
    measurementTCB.taskDataPtr  = &measure;
    measurementTCB.next         = NULL;
    measurementTCB.prev         = NULL;
    
    
    // ..... Initialize other tasks and task data ..... 
    // Initialize Display
    // Initialize Touch Input
    // Initialize Contactor
    // Initialize Alarm
    // Initialize SOC


    // Initialize serial communication
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial1.setTimeout(1000);
}
