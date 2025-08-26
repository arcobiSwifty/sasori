#include "swerve_controller.h"

SwerveController::SwerveController() {}

void SwerveController::setup() {}

void SwerveController::update() {
    int PID_fwd = this->fwdMotorPID();
    int PID_enc = this->encMotorPID();
}

int SwerveController::fwdMotorPID() {}
int SwerveController::encMotorPID() {}

void SwerveController::updateTarget(SwerveUpdateData& data) {
    this->targetSpeed = data.targetRelSpeed;
    this->targetAngleSpeed = data.targetAngleSpeed;
    this->targetAngle = data.targetRelAngle;
}