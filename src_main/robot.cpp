#include "robot.h"
#include "hal/can_data.h"
#include <math.h>
#include <cstdint> 

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

            data.rx = static_cast<int16_t>(BC->LastRJoyStickX);
            data.ry = static_cast<int16_t>(BC->LastRJoyStickY);
            data.lx = static_cast<int16_t>(BC->LastLJoyStickX);
            data.ly = static_cast<int16_t>(BC->LastLJoyStickY);

            //old calculations
            //data.targetAngleSpeed = sqrt(rx*rx + ry*ry) / 128.0;
            //data.targetRelAngle = (BC->LastLJoyStickX != 0.0) ? atan(ry / rx) : data.targetRelAngle = 3.1415 / 2.0;
            //data.targetRelSpeed = sqrt(lx*lx + ly*ly) / 128.0;

            can->updateSwerve(data);
            BC->commandSent=true;
        }
    }
}