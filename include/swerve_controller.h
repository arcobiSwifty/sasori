#ifndef SWERVE
#define SWERVE

#include "hal/can_data.h"
// from joystick imputs, calculates each modules desidered movement. communicates with swerve modules and updates central position. 
//sends update to modules about desired speed / angle


// how it works 

// step 1) receive target speed and angle
// step 2) receive 


class SwerveController {
    public:
        SwerveController();

        float absX;
        float absY; //used to update main controller about current position

        volatile float targetAngleSpeed; // volatile because updated by ISR events triggered by CAN packet receival 
        volatile float targetSpeed;
        volatile float targetAngle;

        float angle;
        float speed;

        void update();
        void setup();
        void updateTarget(SwerveUpdateData& data);

 
    private:
        int encMotorPID();
        int fwdMotorPID();


};

#endif