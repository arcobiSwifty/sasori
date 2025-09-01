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
        if (BC->commandSent != true) {
            SwerveUpdateData data;

            double rx = static_cast<double>(BC->LastRJoyStickX);
            double ry = static_cast<double>(BC->LastRJoyStickY);
            double lx = static_cast<double>(BC->LastLJoyStickX);
            double ly = static_cast<double>(BC->LastLJoyStickY);

            data.targetAngleSpeed = sqrt(rx*rx + ry*ry) / 128.0;
            if (BC->LastLJoyStickX != 0.0) {
            data.targetRelSpeed = atan(ry/rx);
            } else {
                data.targetRelAngle = 3.1415 / 2.0;
            }
            data.targetRelSpeed = sqrt(lx*lx + ly*ly) / 128.0;
            can->updateSwerve(data);
            BC->commandSent=true;
        }
    }
}