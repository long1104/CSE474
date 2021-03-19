#include "Accelerometer.h"


float X_CALIBRATION = 0;
float Y_CALIBRATION = 0;
float Z_CALIBRATION = 0;
float X_MAX_DRIFT = 0;
float Y_MAX_DRIFT = 0;
float Z_MAX_DRIFT = 0;
float TOTAL_MAX_DRIFT;

float getMeasurement(AccelerometerValue *axis, float CALIBRATION_VALUE) {
    float reading;
    for(int i=0; i<40; i++) {
        reading+= ((analogRead(axis->pin)+CALIBRATION_VALUE)/675.0 *3.3 /0.80 );
    }
    if(axis->movingAveragePos>=SAMPLE_SIZE) {
        axis->movingAveragePos=0;
    }
    reading=reading/40;
    axis->accelerations[axis->movingAveragePos] = reading;
    axis->secondLastRolling = axis->lastRolling;
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
    return acos(accelV/magnitude) *57.2958;
}

float calculateMagnitude(float aX, float aY, float aZ) {
    return sqrt(aX*aX+aY*aY+aZ*aZ);
}


void calibrateAccelerometer(int xPin, int yPin, int zPin) {
    uint64_t xSum=0;
    uint64_t ySum=0;
    uint64_t zSum=0;
    unsigned long startMs = millis();
    for(int i=0; i<500; i++) {
        while((millis()-startMs)<5);
        xSum+=analogRead(xPin);
        ySum+=analogRead(yPin);
        zSum+=analogRead(zPin);
        startMs=millis();
    }
    X_CALIBRATION=-(xSum/500.0);
    Y_CALIBRATION=-(ySum/500.0);
    Z_CALIBRATION=-(zSum/500.0) + 163.84;
    Serial.println(X_CALIBRATION);
    Serial.println(Y_CALIBRATION);
    Serial.println(Z_CALIBRATION);
    calibrateMaxDrift(xPin,yPin,zPin);
}

void calibrateMaxDrift(int xPin, int yPin, int zPin) {
    int pins[] = {xPin,yPin,zPin};
    float *xyzMax[] = {&X_MAX_DRIFT, &Y_MAX_DRIFT, &Z_MAX_DRIFT};
    float xyzCal[] = {X_CALIBRATION, Y_CALIBRATION, Z_CALIBRATION};
    Serial.println("before loop");
    for(unsigned int i=0; i<40000; i++) {
        for(int x=0; x<3; x++) {
            float reading = ((analogRead(pins[x])+xyzCal[x])/675.0 *3.3 /0.80 );
            if(reading>*xyzMax[x] || reading < -*xyzMax[x]) {
                *xyzMax[x] = reading*signum(reading);
            }
        }
    }
    X_MAX_DRIFT +=0.01;
    Y_MAX_DRIFT +=0.01;
    Z_MAX_DRIFT-=1;
    Z_MAX_DRIFT +=0.0015;
    Serial.println(X_MAX_DRIFT, 4);
    Serial.println(Y_MAX_DRIFT, 4);
    Serial.println(Z_MAX_DRIFT, 4);
    TOTAL_MAX_DRIFT = calculateMagnitude(X_MAX_DRIFT, Y_MAX_DRIFT, 0);
    Serial.println(TOTAL_MAX_DRIFT,4);
}

int signum(float x){
    if(x>=0){
        return 1;
    }
    else{
        return -1;
    }
}

float polynomialIntegrate(float secondLastPeak, float lastPeak, float currentPeak, float dt) {
    return dt*(secondLastPeak+4*lastPeak+currentPeak)/6;
}

float trapezoidIntegrate(float lastPeak, float currentPeak, float dt) {
    float area = lastPeak*dt;
    area+=(currentPeak-lastPeak)*dt/2.0;
    return  area;
}


void updateVelocity(float *secondLastVelocity, float *lastVelocity, float *velocity, float secondLastRolling, float lastRolling, float rollingAccel, float dt, float drift) {
    *secondLastVelocity = *lastVelocity;
    *lastVelocity = *velocity;
    if(rollingAccel>drift|| rollingAccel<-drift) {
        *velocity += 9.8*100*polynomialIntegrate(secondLastRolling, lastRolling, rollingAccel, dt);
    }
    if(rollingAccel <drift && rollingAccel > -drift) {
        *velocity=0;
    }
}

void updateDistance(float *distance, float secondLastVelocity,  float lastVelocity, float velocity, float dt) {
    *distance += polynomialIntegrate(secondLastVelocity, lastVelocity, velocity, dt);
}

void accelerometerTask(void* taskData) {
    AccelerometerTaskData *aData = (AccelerometerTaskData*)(taskData);
    float xAccel= getMeasurement(&aData->x, X_CALIBRATION);
    float yAccel= getMeasurement(&aData->y, Y_CALIBRATION);
    float zAccel= getMeasurement(&aData->z, Z_CALIBRATION);
    float accelMagDist = calculateMagnitude(aData->x.rollingAccel, aData->y.rollingAccel ,0);
    float accelMagDeg = calculateMagnitude(aData->x.rollingAccel, aData->y.rollingAccel ,aData->z.rollingAccel);
    double s = (millis()-aData->timeInMS)/1000.0;
    updateVelocity(&aData->x.secondLastVelocity,&aData->x.lastVelocity, &aData->x.velocity, aData->x.secondLastRolling, aData->x.lastRolling, aData->x.rollingAccel, s, X_MAX_DRIFT);
    updateDistance(aData->x.distance, aData->x.secondLastVelocity, aData->x.lastVelocity, aData->x.velocity, s);
    updateVelocity(&aData->y.secondLastVelocity,&aData->y.lastVelocity, &aData->y.velocity, aData->y.secondLastRolling,aData->y.lastRolling, aData->y.rollingAccel, s, Y_MAX_DRIFT);
    updateDistance(aData->y.distance, aData->y.secondLastVelocity, aData->y.lastVelocity, aData->y.velocity, s);
    updateVelocity(&aData->secondLastVelocity,&aData->lastTotalVelocity, &aData->totalVelocity, aData->secondLastAccel,aData->lastTotalAccel, accelMagDist, s, TOTAL_MAX_DRIFT);
    updateDistance(aData->totalDistance,  aData->secondLastVelocity, aData->lastTotalVelocity, aData->totalVelocity, s);
    aData->secondLastAccel = aData->lastTotalAccel;
    aData->lastTotalAccel = accelMagDist;
    *aData->x.angle = getDegrees(aData->x.rollingAccel, accelMagDeg);
    *aData->y.angle = getDegrees(aData->y.rollingAccel, accelMagDeg);
    *aData->z.angle = getDegrees(aData->z.rollingAccel, accelMagDeg);
    aData->timeInMS = millis();
}
