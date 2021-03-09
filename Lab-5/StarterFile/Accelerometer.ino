#include "Accelerometer.h"
#include "calibration.h"


float getMeasurement(byte pin, float CALIBRATION_VALUE) {
    return analogRead(pin)/675.0 * 3.0 -1.5 +CALIBRATION_VALUE;
}

float getDegrees(float accelV, float magnitude) {
    return acos(accelV/magnitude) *57.2958;
}

float calculateMagnitude(float aX, float aY, float aZ) {
    return sqrt(aX*aX+aY*aY+aZ*aZ);
}

void accelerometerTask(void* taskData) {
    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel = getMeasurement(aData->x.pin,X_CALIBRATION);
    float yAccel = getMeasurement(aData->y.pin,Y_CALIBRATION);
    float zAccel = getMeasurement(aData->z.pin,Z_CALIBRATION);
    float accelMag = calculateMagnitude(xAccel,yAccel,zAccel);
    float s = 0.1;
    aData->totalVelocity += accelMag*9.8/100.0*s;
    totalDistance += aData->totalVelocity*s;
    aData->x.velocity += xAccel*9.8/100.0*s;
    *aData->x.distance += (aData->x.velocity)*s;
    aData->y.velocity += yAccel*9.8/100.0*s;
    *aData->y.distance += (aData->y.velocity)*s;
    aData->z.velocity += zAccel*9.8/100.0*s;
    *aData->z.distance += (aData->z.velocity)*s;
    *aData->x.angle = getDegrees(*aData->x.distance, *aData->totalDistance);
    *aData->y.angle = getDegrees(*aData->y.distance, *aData->totalDistance);
    *aData->z.angle = getDegrees(*aData->z.distance, *aData->totalDistance);
    Serial.print("X: ");
    Serial.print(aData->x.velocity);
    Serial.print("   Y: ");
    Serial.print(aData->y.velocity);
    Serial.print("   Z: ");
    Serial.println(aData->z.velocity);
    aData->timeInMS = millis();
}
