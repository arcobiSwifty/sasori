#include "robot.h"
#include "hal/can_data.h"
#include <math.h>

void Robot::setup(BluetoothController* bc) {
    this->arm.setup();
    this->BC = bc;
}

Robot::Robot() {
    BC = nullptr;
    can = nullptr;
}

void Robot::update() {
    this->updateSwerve();
    this->updateArm();
}

void Robot::updateArm() {

}

void Robot::updateSwerve() {
    if (BC != nullptr && can != nullptr) {
        if (BC->commandSent == false) {
            SwerveUpdateData data;

            data.rx = static_cast<char>(BC->LastRJoyStickX);
            data.ry = static_cast<char>(BC->LastRJoyStickY);
            data.lx = static_cast<char>(BC->LastLJoyStickX);
            data.ly = static_cast<char>(BC->LastLJoyStickY);

            //old calculations
            //data.targetAngleSpeed = sqrt(rx*rx + ry*ry) / 128.0;
            //data.targetRelAngle = (BC->LastLJoyStickX != 0.0) ? atan(ry / rx) : data.targetRelAngle = 3.1415 / 2.0;
            //data.targetRelSpeed = sqrt(lx*lx + ly*ly) / 128.0;

            can->updateSwerve(data);
            BC->commandSent=true;
        }
    }
}