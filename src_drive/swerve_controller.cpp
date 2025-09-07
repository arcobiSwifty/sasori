#include "swerve_controller.h"
#include <Arduino.h>
#include <math.h>


SwerveController::SwerveController() {}


void SwerveController::InitMotorPins(int fwdDir, int fwdPwm, int encDir, int encPwm) {
    this->fwdDir = fwdDir;
    this->fwdPwm = fwdPwm;
    this->encDir = encDir;
    this->encPwm = encPwm;
}

void SwerveController::InitSensorPins(int rst, int wrot, int enc1, int enc2, int gyroSCL, int gyroSDA, int gyroSDO, int gyroNCS) {
    this->resetHS = rst;
    this->wheelRot = wrot;
    this->enc1 = enc1;
    this->enc2 = enc2; 
    this->gyroSCL = gyroSCL;
    this->gyroSDA = gyroSDA;
    this->gyroSDO = gyroSDO;
    this->gyroNCS = gyroNCS;
}


void SwerveController::setup() {
    pinMode(fwdDir, OUTPUT);
    pinMode(fwdPwm, OUTPUT);
    pinMode(encDir, OUTPUT);
    pinMode(encPwm, OUTPUT);

    pinMode(resetHS, INPUT);
    pinMode(wheelRot, INPUT);
    pinMode(enc1, INPUT_PULLUP);
    pinMode(enc2, INPUT_PULLUP);
    pinMode(gyroSCL, INPUT);
    pinMode(gyroSDA, INPUT);
    pinMode(gyroSDO, INPUT);

    attachInterrupt(digitalPinToInterrupt(enc1), encMotChange);


}

void SwerveController::reset() { 

}

void SwerveController::update() { 

    updateMotor();
    updatePos();
}

void SwerveController::updateMotor() {
    float PID_fwd = this->fwdMotorPID();
    float PID_enc = this->encMotorPID();
    // give command to motor based on values
    digitalWrite(fwdDir, (PID_fwd > 0.1) ? HIGH : LOW);
    analogWrite(fwdPwm, (PID_enc > 0.1) ? PID_fwd*255 : 0);

    
    if (PID_enc>0.01) { //place holder value, to tune
        digitalWrite(encDir, HIGH);
        analogWrite(encPwm, PID_enc*255); // maybe choose a different max speed?
    }  else if (PID_enc < -0.01) {
        digitalWrite(encDir, LOW);
        analogWrite(encPwm, PID_enc*255*(-1));
    }else{
        digitalWrite(fwdDir, LOW);
        analogWrite(encPwm, 0);
    }
}

void SwerveController::updatePos() {
    int rawValue = analogRead(wheelRot);
    //float angle = map(rawValue, 102, 921, 0, 360);
    float angle = map(rawValue, 0, 1024, 0, 360);
    double radians = static_cast<double>(angle) * 2 * 3.14159 / 360.0;
    double travelledDistance = (radians - lastWheelAngle) * WHEEL_RADIUS;
    this->lastWheelAngle = radians;

    double temp_dx = travelledDistance * cos(angle);
    double temp_dy = travelledDistance * sin(angle); 

    can.updatePosition(temp_dx, temp_dy);

    this->absX += temp_dx;
    this->absY += temp_dy;

    
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

void encMotChange() {
    if (digitalRead(swerve.enc2) == HIGH) {
        swerve.angle += 2 * 3.14159 * swerve.ENC_MOT_GEAR_RATIO / swerve.ENC_PULSES_PER_ROTATION; 
    } else {
        swerve.angle -= 2 * 3.14159 * swerve.ENC_MOT_GEAR_RATIO / swerve.ENC_PULSES_PER_ROTATION;
    }
}