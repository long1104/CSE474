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

void calibrateAccelerometer(int xPin, int yPin, int zPin) {
    int xSum = 0;
    int ySum = 0;
    int zSum = 0;
    unsigned long startMs = millis();
    for(int i=0; i<10; i++) {
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

void initializeAccelerometer(AccelerometerValue *x, AccelerometerValue *y, AccelerometerValue *z) {
    x->lastAccel = getMeasurement(x->pin, X_CALIBRATION);
    y->lastAccel = getMeasurement(y->pin, Y_CALIBRATION);
    z->lastAccel = getMeasurement(z->pin, Z_CALIBRATION);
}

float trapezoidIntegrate(float lastPeak, float currentPeak, float dt) {
    float area = lastPeak*dt;
    area+=(currentPeak-lastPeak)*dt;
    return  area;
}

void accelerometerTask(void* taskData) {
    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel = getMeasurement(aData->x.pin,X_CALIBRATION);
    float yAccel = getMeasurement(aData->y.pin,Y_CALIBRATION);
    float zAccel = getMeasurement(aData->z.pin,Z_CALIBRATION);
    float accelMag = calculateMagnitude(xAccel,yAccel,zAccel);
    double s = (millis()-aData->timeInMS)/1000.0;
    aData->totalVelocity += accelMag*9.8*100*s;
    totalDistance += aData->totalVelocity*s;
    aData->x.velocity += 9.8*100* trapezoidIntegrate(aData->x.lastAccel, (xAccel+aData->x.lastAccel)/2, s);
    *aData->x.distance += trapezoidIntegrate(aData->z.lastVelocity, aData->z.velocity, s);
    aData->y.velocity += 9.8*100* trapezoidIntegrate(aData->y.lastAccel, (yAccel+aData->y.lastAccel)/2, s);
    *aData->y.distance += trapezoidIntegrate(aData->z.lastVelocity, aData->z.velocity, s);
    aData->z.velocity += 9.8*100* trapezoidIntegrate(aData->z.lastAccel, (zAccel+aData->z.lastAccel)/2, s);
    *aData->z.distance += trapezoidIntegrate(aData->z.lastVelocity, aData->z.velocity, s);
    *aData->x.angle = getDegrees(xAccel, accelMag) -90;
    *aData->y.angle = getDegrees(yAccel, accelMag) -90;
    *aData->z.angle = getDegrees(zAccel-1, accelMag) -180;
    Serial.print("s: ");
    Serial.print(s);
    Serial.print("   X: ");
    Serial.print(xAccel);
    Serial.print("   Y: ");
    Serial.print(yAccel);
    Serial.print("   Z: ");
    Serial.println(zAccel);
    aData->x.lastAccel = xAccel;
    aData->y.lastAccel = yAccel;
    aData->z.lastAccel = zAccel;
    aData->x.lastVelocity =  aData->x.velocity;
    aData->y.lastVelocity =  aData->y.velocity;
    aData->z.lastVelocity =  aData->z.velocity;
    aData->timeInMS = millis();
}
