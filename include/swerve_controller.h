#ifndef SWERVE
#define SWERVE

#include "hal/can_data.h"
#include "hal/stm32_can_manager.h"
// from joystick imputs, calculates each modules desidered movement. communicates with swerve modules and updates central position. 
//sends update to modules about desired speed / angle


// how it works 

// step 1) receive target speed and angle
// step 2) receive 


// PORT LIST 
/* 
    Hall sensor (reset)
    AS5600 
    Gyroscope 
        CLK
        other port 
        encMot DIR
        encMot Pwm 
        fwdMot DIR
        fwdMot Pwm
    CAN
        rx A11 
        tx A12
    ENC MOT

*/


class SwerveController {
    public:
        double WHEEL_RADIUS = 10.0;

        SwerveController();
        SwerveController(int fwdDir, int fwdPwm, int encDir, int encPwm);

        int fwdDir;
        int fwdPwm;
        int encDir;
        int encPwm;

        double absX;
        double absY; //used to update main controller about current position

        volatile float targetAngleSpeed; // volatile because updated by ISR events triggered by CAN packet receival 
        volatile float targetSpeed;
        volatile float targetAngle; //target angle relative to robot angle;
        volatile float targetRobotAngle;
        volatile float robotAngle;

        volatile float robotCenterX;
        volatile float robotCenterY;

        volatile float angle;
        volatile float speed;

        volatile double lastWheelAngle;
        double instantaneousSpeed; 
        double averageSpeed;
        double timeLastPolled; //change data type

        void update();
        void setup();

        void updateMotor();
        void updateTarget(SwerveUpdateData& data);

        //sensors 

        int resetHS; //reset hall sensor pin 
        int wheelRot = 7; 
        int gyro;

        void updatePos();

    private:
        float encMotorPID();
        float fwdMotorPID();

};

extern SwerveController swerve;


#endif