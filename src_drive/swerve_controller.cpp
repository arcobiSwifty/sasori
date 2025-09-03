#include "swerve_controller.h"
#include <Arduino.h>
#include <math.h>

SwerveController::SwerveController() {}
SwerveController::SwerveController(int fwdDir, int fwdPwm, int encDir, int encPwm) {
    this->fwdDir = fwdDir;
    this->fwdPwm = fwdPwm;
    this->encDir = encDir;
    this->encPwm = encPwm;
}

void SwerveController::setup() {
    pinMode(fwdDir, OUTPUT);
    pinMode(fwdPwm, OUTPUT);
    pinMode(encDir, OUTPUT);
    pinMode(encPwm, OUTPUT);
}

void SwerveController::update() { // TODO refactor
    // self update speed and angle from sensors
    // get pid values
    float PID_fwd = this->fwdMotorPID();
    float PID_enc = this->encMotorPID();
    // give command to motor based on values
    if (PID_fwd>0.1) { //place holder value
        digitalWrite(fwdDir, HIGH);
        digitalWrite(fwdPwm, PID_fwd*256); // maybe choose a different max speed?
    }  else {
        digitalWrite(fwdDir, LOW);
        digitalWrite(fwdPwm, LOW);
    }
    if (PID_enc>0.01) { //place holder value, to tune
        digitalWrite(encDir, HIGH);
        digitalWrite(fwdPwm, PID_enc*256); // maybe choose a different max speed?
    }  else if (PID_enc < -0.01) {
        digitalWrite(encDir, LOW);
        digitalWrite(encPwm, PID_enc*256);
    }else{
        digitalWrite(fwdDir, LOW);
        digitalWrite(fwdPwm, LOW);
    }
}
// these functions are just proportional controls for now. TODO: make pid
float SwerveController::fwdMotorPID() {
    float dv = (targetSpeed - speed);
    float k = 1.0; //to do: tune k
    return dv*k;
}
float SwerveController::encMotorPID() {
    float dtheta = (targetAngle -angle);
    float k = 1.0;
    return dtheta*targetAngleSpeed*k;
}
void SwerveController::updateTarget(SwerveUpdateData& data) {

    //data.targetAngleSpeed = ;
    //data.targetRelAngle = (BC->LastLJoyStickX != 0.0) ? atan(ry / rx) : data.targetRelAngle = 3.1415 / 2.0;
     //data.targetRelSpeed = sqrt(lx*lx + ly*ly) / 128.0;


    this->targetSpeed = data.targetRelSpeed;
    this->targetAngleSpeed = sqrt(data.rx*data.rx + data.ry*data.ry) / 256.0 + sqrt(data);
    this->targetAngle = data.targetRelAngle;
}