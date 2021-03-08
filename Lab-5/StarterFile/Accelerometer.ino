#include "Accelerometer.h"
#include "calibration.h"


float getMeasurement(byte pin, float CALIBRATION_VALUE) {
    return analogRead(pin)/675.0 * 3.0 -1.5 +CALIBRATION_VALUE;
}

float getDegrees(float accelV, float magnitude) {
    return 0;
}

float calculateMagnitude(float aX, float aY, float aZ) {
    return sqrt(aX*aX+aY*aY+aZ*aZ);
}

void accelerometerTask(void* taskData) {
    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel = getMeasurement(aData->x.pin,X_CALIBRATION);
    float yAccel = getMeasurement(aData->y.pin,Y_CALIBRATION);
    float zAccel = getMeasurement(aData->z.pin,Z_CALIBRATION);
    Serial.print("X ");
    Serial.println(xAccel);
    Serial.print("Y ");
    Serial.println(yAccel);
    Serial.print("Z ");
    Serial.println(zAccel);
    float accelMag = calculateMagnitude(xAccel,yAccel,zAccel);
    *aData->x.angle = getDegrees(xAccel, accelMag);
    *aData->y.angle = getDegrees(yAccel, accelMag);
    *aData->z.angle = getDegrees(zAccel, accelMag);
    float s = (millis()-aData->timeInMS)/1000;
    totalDistance += accelMag*s*s;
    *aData->x.distance += xAccel*s*s;
    *aData->y.distance += yAccel*s*s;
    *aData->z.distance += zAccel*s*s;
    aData->timeInMS = millis();
}
