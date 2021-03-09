

#ifndef ACCEL_H_
#define ACCEL_H_

extern int X_CALIBRATION;
extern int Y_CALIBRATION;
extern int Z_CALIBRATION;

typedef struct  AccelerometerValueStruct {
    float* distance;
    float* angle;
    byte pin;
    float velocity;
    float lastAccel;
    float lastVelocity;
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
void calibrateAccelerometer(int xPin, int yPin, int zPin);

#endif
