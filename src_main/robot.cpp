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

void Robot::update () {
    if (BC != nullptr) {
        if (BC->commandSent != true) {
            SwerveUpdateData data;
            data.targetAngleSpeed = sqrt(BC->LastRJoyStickX*BC->LastRJoyStickX + BC->LastRJoyStickY*BC->LastRJoyStickY) / 128;
            if (BC->LastLJoyStickX != 0) {
            data.targetRelSpeed = atan(BC->LastRJoyStickY/BC->LastRJoyStickX);
            } else {
                data.targetRelAngle = 3.1415 / 2.0;
            }
            data.targetRelSpeed = sqrt(BC->LastRJoyStickX*BC->LastRJoyStickX + BC->LastRJoyStickY*BC->LastRJoyStickY) / 128;
            can->updateSwerve(data);\
            BC->commandSent=true;
        }
        
    }
}