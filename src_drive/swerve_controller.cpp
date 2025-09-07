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

void SwerveController::update() { 

    updateMotor();
    updatePos();
}

void SwerveController::updateMotor() {
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

void SwerveController::updatePos() {
    int rawValue = analogRead(wheelRot);
    //float angle = map(rawValue, 102, 921, 0, 360);
    float angle = map(rawValue, 0, 1024, 0, 360);
    double radians = static_cast<double>(angle) * 2 * 3.14159 / 360.0;
    double travelledDistance = radians * WHEEL_RADIUS;

    double temp_dx = travelledDistance * cos(angle);
    double temp_dy = travelledDistance * sin(angle); 

    
    //update timeLastPolled
    //update average and instantaneous speed


    //error correction necessary

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

    double rx = static_cast<double>(data.rx);
    double ry = static_cast<double>(data.ry);
    double lx = static_cast<double>(data.lx);
    double ly = static_cast<double>(data.ly);


    this->targetSpeed =  sqrt(lx*lx + ly*ly) / 128.0;
    this->targetAngleSpeed = sqrt(rx*rx+ry*ry) / 256.0 + sqrt(lx*lx + ly*ly) / 256.0;
    this->targetAngle = (lx != 0.0) ? atan(ry / rx) : 3.1415 / 4.0;
}