

#ifndef ACCEL_H_
#define ACCEL_H_


typedef struct  AccelerometerValueStruct {
    float* distance;
    float* angle;
    byte pin;
    float velocity;
} AccelerometerValue;


typedef struct AccelerometerTaskDataStruct {
    AccelerometerValue x;
    AccelerometerValue y;
    AccelerometerValue z;
    float *totalDistance;
    unsigned long timeInMS;
    float totalVelocity;
} AccelerometerTaskData;



void accelerometerTask(void* taskData);
float getMeasurement(byte pin, float CALIBRATION_VALUE);
float getDegrees(float val, float magnitude);
void updateTotalDistance(float magnitude, float *totalDistance);
float calculateMagnitude(float aX, float aY, float aZ);

#endif
