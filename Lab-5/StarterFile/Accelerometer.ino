#include "Accelerometer.h"


float X_CALIBRATION = 0;
float Y_CALIBRATION = 0;
float Z_CALIBRATION = 0;
float X_MAX_DRIFT = 0;
float Y_MAX_DRIFT = 0;
float Z_MAX_DRIFT = 0;
float TOTAL_MAX_DRIFT;

float getMeasurement(AccelerometerValue *axis, float CALIBRATION_VALUE) {
    /****************
    Function name: getMeasurement
    Function inputs: axis: pointer to accelerometer axis struct, calibration_value: the calibration value for that axis
    Function outputs: returns the current acceleration (non rolling)
    Function description: converts a newly read analog measurement to acceleration units and shifts it into the rolling average array
    Authors:    Long Nguyen / Chase Arline
    *****************/
    float reading;
    for(int i=0; i<40; i++) {
        reading+= ((analogRead(axis->pin)+CALIBRATION_VALUE)/675.0 *3.3 /0.80 );  //converts raw ADC to value in g(acceleration) with 800mV/g of sensitivity
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
    /****************
    Function name: calculateAverage
    Function inputs: arr: array to calculate average of
    Function outputs: float: average value of the array values
    Function description: calculates average value of the array given
    Authors:    Long Nguyen / Chase Arline
    *****************/
    float sum = 0;
    for(int i=0; i<SAMPLE_SIZE; i++) {
        sum+=arr[i];
    }
    return sum/SAMPLE_SIZE;
}

float getDegrees(float accelV, float magnitude) {
    /****************
    Function name: getDegrees
    Function inputs: accelV: acceleration of a single axis, magnitude: total acceleration on the device
    Function outputs: float: angle in degrees
    Function description: calculates the angle of an axis relative to gravity
    Authors:    Long Nguyen / Chase Arline
    *****************/
    return acos(accelV/magnitude) *57.2958;
}

float calculateMagnitude(float aX, float aY, float aZ) {
    /****************
    Function name: calculateMagnitude
    Function inputs: aX,aY,aZ: magnitude values for each axis
    Function outputs: the overall magnitude felt across all axes
    Function description: calculates the overall acceleration magnitude felt on the device
    Authors:    Long Nguyen / Chase Arline
    *****************/
    return sqrt(aX*aX+aY*aY+aZ*aZ);
}


void calibrateAccelerometer(int xPin, int yPin, int zPin) {
    /****************
    Function name: calibrateAccelerometer
    Function inputs: xPin,yPin,zPin: pins for each analog input
    Function outputs: void return
    Function description: sets the calibration values for the accelerometer (takes about 20-30 seconds total after starting)
    Authors:    Long Nguyen / Chase Arline
    *****************/
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
    return;
}

void calibrateMaxDrift(int xPin, int yPin, int zPin) {
    /****************
    Function name: calibrateMaxDrift
    Function inputs: xPin,yPin,zPin: pins used for analog  inputs
    Function outputs: void return
    Function description: sets the max drifting values read when sitting still for long periods of time: defines deadzone
    Authors:    Long Nguyen / Chase Arline
    *****************/
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
    TOTAL_MAX_DRIFT = calculateMagnitude(X_MAX_DRIFT, Y_MAX_DRIFT, 0);
    return;
}

int signum(float x) {
    /****************
    Function name: signum
    Function inputs: x: value to be checked for sign
    Function outputs: 1 * the sign of the input
    Function description: generic signum function
    Authors:    Long Nguyen / Chase Arline
    *****************/
    int ret;
    if(x>=0) {
        ret = 1;
    }
    else {
        ret=-1;
    }
    return ret;
}

/****************
    Function name: polynomialIntegrate
    Function inputs: secondLastPeak, lastPeak, currentPeak: the three most recent values to integrate from, dt: the delta-time between last two samples
    Function outputs: the integrated value
    Function description: performs second order simpson polynomial integration and returns the result
    Authors:    Long Nguyen / Chase Arline
    *****************/
float polynomialIntegrate(float secondLastPeak, float lastPeak, float currentPeak, float dt) {
    return dt*(secondLastPeak+4*lastPeak+currentPeak)/6;
}


void updateVelocity(float *secondLastVelocity, float *lastVelocity, float *velocity, float secondLastRolling, float lastRolling, float rollingAccel, float dt, float drift) {
    /****************
    Function name: updateVelocity
    Function inputs: secondLastVelocity, lastVelocity, velocity: last three velocities, secondLastRolling, lastrolling, rollingAccel: last three accelerations, dt: delta time, drift: maximum drift on this axis
    Function outputs: void return
    Function description: updates the current velocity on the device using integration
    Authors:    Long Nguyen / Chase Arline
    *****************/
    *secondLastVelocity = *lastVelocity;
    *lastVelocity = *velocity;
    if(rollingAccel>drift|| rollingAccel<-drift) {
        *velocity += 9.8*100*polynomialIntegrate(secondLastRolling, lastRolling, rollingAccel, dt);  //convert g to cm/s
    }
    if(rollingAccel <drift && rollingAccel > -drift) {
        *velocity=0;
    }
    return;
}

void updateDistance(float *distance, float secondLastVelocity,  float lastVelocity, float velocity, float dt) {
    /****************
    Function name: updateDistance
    Function inputs: distance: displacement/distance on the axis, secondLastVelocity,lastVelocity,velocity: last three velocity data points, dt: delta time
    Function outputs: void return
    Function description: uses integration to calculate the current distance based on velocity points
    Authors:    Long Nguyen / Chase Arline
    *****************/
    *distance += polynomialIntegrate(secondLastVelocity, lastVelocity, velocity, dt);
    return;
}

void accelerometerTask(void* taskData) {
    /****************
    Function name: accelerometerTask
    Function inputs: taskData: the AccelerometerTaskData
    Function outputs: void return
    Function description: performs all acceleration task operations and updates all values used
    Authors:    Long Nguyen / Chase Arline
    *****************/
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
    return;
}
