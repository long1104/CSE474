#include "Accelerometer.h"


int X_CALIBRATION = 0;
int Y_CALIBRATION = 0;
int Z_CALIBRATION = 0;

float getMeasurement(byte pin, float CALIBRATION_VALUE) {
    return ((analogRead(pin)+CALIBRATION_VALUE)/675.0 *3.3 /0.8);
}

float getDegrees(float accelV, float magnitude) {
    return acos(accelV/magnitude) *57.2958;
}

float calculateMagnitude(float aX, float aY, float aZ) {
    return sqrt(aX*aX+aY*aY+aZ*aZ);
}

float rounding(float value) {
    return (((int)(value*100.0))/10)/10.0;
}

void calibrateAccelerometer(int xPin, int yPin, int zPin){
    int xSum = 0;
    int ySum = 0;
    int zSum = 0;
    unsigned long startMs = millis();
    for(int i=0;i<10;i++){
       while((millis()-startMs)<5);
       startMs=millis();
       xSum+=analogRead(xPin);
       ySum+=analogRead(yPin);
       zSum+=analogRead(zPin);
    }
    X_CALIBRATION=-xSum/10;
    Y_CALIBRATION=-ySum/10;
    Z_CALIBRATION=-zSum/10;
}

void accelerometerTask(void* taskData) {
    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel = getMeasurement(aData->x.pin,X_CALIBRATION);
    float yAccel = getMeasurement(aData->y.pin,Y_CALIBRATION);
    float zAccel = getMeasurement(aData->z.pin,Z_CALIBRATION);
    float accelMag = rounding(calculateMagnitude(xAccel,yAccel,zAccel));
    double s = (millis()-aData->timeInMS)/1000.0;
    if(accelMag >0.03 || accelMag<-0.03) {
        aData->totalVelocity += accelMag*9.8*100*s;
        totalDistance += aData->totalVelocity*s;
    }
    if(xAccel>0.03 || xAccel<-0.03) {
        aData->x.velocity += xAccel*9.8*100*s;
        *aData->x.distance += (aData->x.velocity)*s;
    }
    if(yAccel>0.03 || yAccel<-0.03) {
        aData->y.velocity += yAccel*9.8*100.0*s;
        *aData->y.distance += (aData->y.velocity)*s;
    }
    if(zAccel>0.03 || zAccel<-0.03) {
        aData->z.velocity += zAccel*9.8*100*s;
        *aData->z.distance += (aData->z.velocity)*s;
    }
    *aData->x.angle = getDegrees(*aData->x.distance, *aData->totalDistance);
    *aData->y.angle = getDegrees(*aData->y.distance, *aData->totalDistance);
    *aData->z.angle = getDegrees(*aData->z.distance, *aData->totalDistance);
    Serial.print("s: ");
    Serial.print(s);
    Serial.print("   X: ");
    Serial.print(xAccel);
    Serial.print("   Y: ");
    Serial.print(yAccel);
    Serial.print("   Z: ");
    Serial.println(zAccel);
    aData->timeInMS = millis();
}
