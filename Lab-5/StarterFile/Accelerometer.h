

#ifndef ACCEL_H_
#define ACCEL_H_
#define SAMPLE_SIZE 5

extern int X_CALIBRATION;
extern int Y_CALIBRATION;
extern int Z_CALIBRATION;


typedef struct  AccelerometerValueStruct {
    float* distance;
    float* angle;
    byte pin;
    float accelerations[SAMPLE_SIZE];
    float velocity;
    int movingAveragePos;
    float rollingAccel;
    float lastRolling;
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
float addMeasurement(AccelerometerValue *axis, float CALIBRATION_VALUE);
float getDegrees(float val, float magnitude);
void updateTotalDistance(float magnitude, float *totalDistance);
float calculateMagnitude(float aX, float aY, float aZ);
void calibrateAccelerometer(int xPin, int yPin, int zPin);

#endif