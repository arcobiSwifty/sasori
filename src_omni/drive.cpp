#include "MPU9250.h"
#include "drive.h"
#include "Arduino.h"
#include <math.h>

#define MOTA_DIR 1
#define MOTA_PWM 2
#define MOTB_DIR 3 
#define MOTB_PWM 4 
#define MOTC_DIR 5
#define MOTC_PWM 6



void Robot::update() {
    // get data from sensors
    if (mpu.update()) {
        float measuredYaw = mpu.getYaw();
        Serial.println(mpu.getYaw()); 
        perry.yaw = measuredYaw;
    }
    //pid on values
    perry.moveBySpeed()
}
void Robot::setup() {
    pinMode(MOTA_DIR, OUTPUT);
    pinMode(MOTB_DIR, OUTPUT);
    pinMode(MOTC_DIR, OUTPUT);
    pinMode(MOTA_PWM, OUTPUT);
    pinMode(MOTB_PWM, OUTPUT);
    pinMode(MOTC_PWM, OUTPUT);
}

void multiply(const float A[3][2], const float B[2][2], float C[3][2]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            C[i][j] = 0.0; 
            for (int k = 0; k < 2; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void Robot::move() {
    // yaw : direction i am pointing 
    // vector angle: direction i want to go 

    //step one: calculate 

    //      A
    //    C   B
    //

    // vector A   1,     0
    // vector B  -1/2, -sqrt(3)/2
    // vector C  =1/2   sqrt(3)/2

    // step 2: calculate unit vector of relAngle and perpedicular, 
    // parallel        sin a cos a
    // perpendicular   cos a -sin a

    // dot products gives the new vectors

    float vectorAngle = atan(this->target_vy/this->target_vx) * (2*PI) / 360.0;
    float relAngle = vectorAngle - this->yaw;

    float rotation[2][2] = {{cos(relAngle), sin(relAngle)}, {-sin(relAngle), cos(relAngle)}};
    float omni[3][2] = {{1,0}, {-0.5, -sqrt(3)/2.0}, {-0.5, sqrt(3)/2.0}};

    // omni is now a 3x2 vector of how much each vector contributes 

}