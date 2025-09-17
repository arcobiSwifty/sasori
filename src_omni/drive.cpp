#include "Arduino.h"
//#include "MPU9250.h"
#include "drive.h"
#include <math.h>
#include <algorithm>


#define MOTA_DIR 4
#define MOTA_PWM 15
#define MOTB_DIR 22
#define MOTB_PWM 23
#define MOTC_DIR 19
#define MOTC_PWM 18



Robot::Robot() {
    
}

void Robot::update() {
    // get data from sensors
    //if (mpu.update()) {
       // float measuredYaw = mpu.getYaw();
      //  Serial.println(mpu.getYaw()); 
       // perry.yaw = measuredYaw;
   // }
    //pid on values
    perry.move();
}
void Robot::setup() {
    pinMode(MOTA_DIR, OUTPUT);
  //  pinMode(MOTB_DIR, OUTPUT);
   // pinMode(MOTC_DIR, OUTPUT);
    pinMode(MOTA_PWM, OUTPUT);
  //  pinMode(MOTB_PWM, OUTPUT);
   // pinMode(MOTC_PWM, OUTPUT);
   lastMove = millis();
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
void calculate_new_coordinate(const float A[3][2], const float B[2][1], float C[3][1]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1; ++j) {
            C[i][j] = 0.0; 
            for (int k = 0; k < 2; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
void multiply_vec(const float A[2][2], const float B[2][1], float C[2][1]) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 1; ++j) {
            C[i][j] = 0.0; 
            for (int k = 0; k < 2; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

float dot_product(const float A[1][2], const float B[1][2]) {
    return A[0][0]*B[0][0] + A[0][1]*B[0][1];
}

void get_abs(const float A[3][1], float B[3][1]) {
    for (int i = 0; i < 3; i++) {
        B[i][0] = abs(A[i][0]);
    }
}

void Robot::move() {

    unsigned long current_time = millis();
    if (current_time - this->lastMove < 10) {
        return;
    } else {
        lastMove = current_time;
    }

    if ((abs(target_vx) > 0.0) || (abs(target_vy) > 0.0)) {
    
    const float SQRT3_DIV_2 = 0.86602540378;
    //float w = target_yaw - yaw / 2.0;
    float w = 0.1;
    Serial.print(target_vx);
    Serial.print("x");
    Serial.print(target_vy);
    Serial.println("y");


    float v1_t = target_vy +  w;
    float v2_t = -SQRT3_DIV_2 * target_vx - 0.5 * target_vy +  w;
    float v3_t = SQRT3_DIV_2 * target_vx - 0.5 * target_vy +   w;
    Serial.print(v1_t);
    Serial.print(", ");
    Serial.print(v3_t);
    Serial.print(", ");
    Serial.println(v3_t);
    

    float abs_v1 = abs(v1_t);
    float abs_v2 = abs(v2_t);
    float abs_v3 = abs(v3_t);
    float biggest = max({abs_v1, abs_v2, abs_v3});
    if (biggest > 0) {
        v1_t /= biggest;
        v2_t /= biggest; 
        v3_t /= biggest;
        abs_v1 /= biggest;
        abs_v2 /= biggest;
        abs_v3 /= biggest;
    } else {
        v1_t = 0; 
        v2_t = 0;
        v3_t = 0;
        abs_v1 = 0;
        abs_v2 = 0;
        abs_v3=0;
    }
    Serial.print(v1_t);
    Serial.print(", ");
    Serial.print(v3_t);
    Serial.print(", ");
    Serial.println(v3_t);

    digitalWrite(MOTA_DIR, (v1_t > 0) ? 1 : 0);
    digitalWrite(MOTB_DIR, (v2_t > 0) ? 1 : 0);
    digitalWrite(MOTC_DIR, (v3_t > 0) ? 1 : 0);
    analogWrite(MOTA_PWM, abs_v1*255);
    analogWrite(MOTB_PWM, abs_v2*255);
    analogWrite(MOTC_PWM, abs_v3*255);
    } else {
        analogWrite(MOTA_PWM, 0);
    analogWrite(MOTB_PWM, 0);
    analogWrite(MOTC_PWM, 0);
    }

}




void Robot::moveFieldCentric() {
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

    float rotation[2][2] = {{cos(relAngle), -sin(relAngle)}, {sin(relAngle), cos(relAngle)}};
    float omni[3][2] = {{1,0}, {-0.5, -sqrt(3)/2.0}, {-0.5, sqrt(3)/2.0}};
    float motorOrientations[3][2];
    float targetDir[2][1] = {{this->target_vx}, {this->target_vy}};
    float perpDir[2][1];

    float ninetyRot[2][2] = {{0,-1},{1,0}};
    multiply_vec(ninetyRot,targetDir,perpDir);

    float newCoord_x[3][1];
    float newCoord_y[3][1];
    calculate_new_coordinate(omni, targetDir, newCoord_x);
    calculate_new_coordinate(omni, perpDir, newCoord_y);

    // find max 



     

    // omni is now a 3x2 vector of how much each vector contributes 
    // solve for 

    float yawError = this->target_yaw - this->yaw; 


}