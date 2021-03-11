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
        xSum+=analogRead(xPin);
        ySum+=analogRead(yPin);
        zSum+=analogRead(zPin);
        startMs=millis();

    }
    X_CALIBRATION=-xSum/10;
    Y_CALIBRATION=-ySum/10;
    Z_CALIBRATION=-zSum/10 + 163.84;
}

float trapezoidIntegrate(float lastPeak, float currentPeak, float dt) {
    float area = lastPeak*dt;
    area+=(currentPeak-lastPeak)*dt;
    return  area;
}



void updateVelocity(AccelerometerValue *axis, float dt) {
    axis->lastVelocity = axis->velocity;
    if(axis->rollingAccel>0.01 || axis->rollingAccel<-0.01) {
        axis->velocity += 9.8*100*axis->rollingAccel*dt;
    }
}

void updateDistance(AccelerometerValue *axis, float dt) {
    *axis->distance += axis->velocity*dt;
}

void accelerometerTask(void* taskData) {
    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel = getMeasurement(&aData->x, X_CALIBRATION);
    float yAccel = getMeasurement(&aData->y, Y_CALIBRATION);
    float zAccel = getMeasurement(&aData->z, Z_CALIBRATION);
    float accelMagDist = calculateMagnitude(aData->x.rollingAccel, aData->y.rollingAccel ,0);
    float accelMagDeg = calculateMagnitude(aData->x.rollingAccel, aData->y.rollingAccel ,aData->z.rollingAccel);
    double s = (millis()-aData->timeInMS)/1000.0;
    updateVelocity(&aData->x, s);
    updateDistance(&aData->x, s);
    updateVelocity(&aData->y, s);
    updateDistance(&aData->y, s);
    updateVelocity(&aData->z, s);
    updateDistance(&aData->z, s);
    *aData->x.angle = getDegrees(aData->x.rollingAccel, accelMagDeg);
    *aData->y.angle = getDegrees(aData->y.rollingAccel, accelMagDeg);
    *aData->z.angle = getDegrees(aData->z.rollingAccel, accelMagDeg);
    Serial.print("s: ");
    Serial.print(s);
    Serial.print("   X: ");
    Serial.print(*aData->x.angle);
    Serial.print("   Y: ");
    Serial.print(*aData->y.angle);
    Serial.print("   Z: ");
    Serial.println(*aData->z.angle);
    aData->timeInMS = millis();
}
