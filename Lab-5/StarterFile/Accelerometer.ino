#include "Accelerometer.h"


int X_CALIBRATION = 0;
int Y_CALIBRATION = 0;
int Z_CALIBRATION = 0;

float getMeasurement(AccelerometerValue *axis, float CALIBRATION_VALUE) {
    float reading = ((analogRead(axis->pin)+CALIBRATION_VALUE)/675.0 *3.3 /0.8);
    if(axis->movingAveragePos>=SAMPLE_SIZE) {
        axis->movingAveragePos=0;
    }
    axis->accelerations[axis->movingAveragePos] = reading;
    axis->lastRolling = axis->rollingAccel;
    axis->rollingAccel=calculateAverage(axis->accelerations);
    axis->movingAveragePos++;
    return reading;
}

float calculateAverage(float *arr) {
    float sum = 0;
    for(int i=0; i<SAMPLE_SIZE; i++) {
        sum+=arr[i];
    }
    return sum/SAMPLE_SIZE;
}

float getDegrees(float accelV, float magnitude) {
    return acos(accelV/magnitude) *57.2958  -90;
}

float calculateMagnitude(float aX, float aY, float aZ) {
    return sqrt(aX*aX+aY*aY+aZ*aZ);
}


void calibrateAccelerometer(int xPin, int yPin, int zPin) {
    int xSum = 0;
    int ySum = 0;
    int zSum = 0;
    unsigned long startMs = millis();
    for(int i=0; i<20; i++) {
        while((millis()-startMs)<3);
        xSum+=analogRead(xPin);
        ySum+=analogRead(yPin);
        zSum+=analogRead(zPin);
        startMs=millis();

    }
    X_CALIBRATION=-xSum/20;
    Y_CALIBRATION=-ySum/20;
    Z_CALIBRATION=-zSum/20 + 163.84;
}

float trapezoidIntegrate(float lastPeak, float currentPeak, float dt) {
    float area = lastPeak*dt;
    area+=(currentPeak-lastPeak)*dt/2.0;
    return  area;
}


void updateVelocity(float *lastVelocity, float *velocity, float lastRolling, float rollingAccel, float dt) {
    *lastVelocity = *velocity;
    if(rollingAccel>0.02 || rollingAccel<-0.02) {
        *velocity += 9.8*100*trapezoidIntegrate(lastRolling, rollingAccel, dt);
    }
    if(rollingAccel <0.0125 && rollingAccel > -0.0125){
        *velocity=0;
    }
}

void updateDistance(float *distance, float lastVelocity, float velocity, float dt) {
    *distance += trapezoidIntegrate(lastVelocity, velocity, dt);
}

void accelerometerTask(void* taskData) {

    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel = getMeasurement(&aData->x, X_CALIBRATION);
    float yAccel = getMeasurement(&aData->y, Y_CALIBRATION);
    float zAccel = getMeasurement(&aData->z, Z_CALIBRATION);
    float accelMagDist = calculateMagnitude(aData->x.rollingAccel, aData->y.rollingAccel ,0);
    float accelMagDeg = calculateMagnitude(aData->x.rollingAccel, aData->y.rollingAccel ,aData->z.rollingAccel);
    double s = (millis()-aData->timeInMS)/1000.0;
    updateVelocity(&aData->x.lastVelocity, &aData->x.velocity, aData->x.lastRolling, aData->x.rollingAccel, s);
    updateDistance(aData->x.distance, aData->x.lastVelocity, aData->x.velocity, s);
    updateVelocity(&aData->y.lastVelocity, &aData->y.velocity, aData->y.lastRolling, aData->y.rollingAccel, s);
    updateDistance(aData->y.distance, aData->y.lastVelocity, aData->y.velocity, s);
    updateVelocity(&aData->lastTotalVelocity, &aData->totalVelocity, aData->x.lastRolling, accelMagDist, s);
    updateDistance(aData->totalDistance, aData->lastTotalVelocity, aData->totalVelocity, s);
    aData->lastTotalAccel = accelMagDist;
    *aData->x.angle = getDegrees(aData->x.rollingAccel, accelMagDeg);
    *aData->y.angle = getDegrees(aData->y.rollingAccel, accelMagDeg);
    *aData->z.angle = getDegrees(aData->z.rollingAccel, accelMagDeg);
    
    aData->timeInMS = millis();
}
