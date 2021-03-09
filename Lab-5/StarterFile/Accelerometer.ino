#include "Accelerometer.h"
#include "calibration.h"


float getMeasurement(byte pin, float CALIBRATION_VALUE) {
//    Serial.println(analogRead(pin)/1023.0 *5);
    return analogRead(pin)/675.0 * 3.0 -1.5 +CALIBRATION_VALUE;
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

void accelerometerTask(void* taskData) {
    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel = rounding(getMeasurement(aData->x.pin,X_CALIBRATION));
    float yAccel = rounding(getMeasurement(aData->y.pin,Y_CALIBRATION));
    float zAccel = rounding(getMeasurement(aData->z.pin,Z_CALIBRATION));
    float accelMag = rounding(calculateMagnitude(xAccel,yAccel,zAccel));
    float s = 0.1;
    if(accelMag >0.03 || accelMag<-0.03) {
        aData->totalVelocity += accelMag*9.8*100*s;
        totalDistance += aData->totalVelocity*s;
    }
    if(xAccel>0.05 || xAccel<-0.05) {
        aData->x.velocity += xAccel*9.8*100*s;
        *aData->x.distance += (aData->x.velocity)*s;
    }
    if(yAccel>0.05 || yAccel<-0.05) {
        aData->y.velocity += yAccel*9.8*100.0*s;
        *aData->y.distance += (aData->y.velocity)*s;
    }
    if(zAccel>0.05 || zAccel<-0.05) {
        aData->z.velocity += zAccel*9.8*100*s;
        *aData->z.distance += (aData->z.velocity)*s;
    }
    *aData->x.angle = getDegrees(*aData->x.distance, *aData->totalDistance);
    *aData->y.angle = getDegrees(*aData->y.distance, *aData->totalDistance);
    *aData->z.angle = getDegrees(*aData->z.distance, *aData->totalDistance);
    Serial.print("X: ");
    Serial.print(xAccel);
    Serial.print("   Y: ");
    Serial.print(yAccel);
    Serial.print("   Z: ");
    Serial.println(zAccel);
    aData->timeInMS = millis();
}
