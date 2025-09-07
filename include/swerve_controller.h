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
        double ENC_PULSES_PER_ROTATION = 102.1;
        double ENC_MOT_GEAR_RATIO = 15.0 / 50.0;

        SwerveController();

        int fwdDir;
        int fwdPwm;
        int encDir;
        int encPwm;

        int resetHS; //reset hall sensor pin 
        int wheelRot; 
        int enc1;
        int enc2;
        
        int gyroSCL;
        int gyroSDA;
        int gyroSDO;
        int gyroNCS;

        void InitMotorPins(int fwdDir, int fwdPwm, int encDir, int encPwm);
        void InitSensorPins(int rst, int wrot, int enc1, int enc2, int gyroSCL, int gyroSDA, int gyroSDO, int gyroNCS);


        double absX;
        double absY; //used to update main controller about current position

        volatile double targetAngleSpeed; // volatile because updated by ISR events triggered by CAN packet receival 
        volatile double targetSpeed;
        volatile double targetAngle; //target angle relative to robot angle;
        volatile double targetRobotAngle;
        volatile double robotAngle;

        volatile float robotCenterX;
        volatile float robotCenterY;

        volatile double angle;
        volatile double speed;

        volatile double lastWheelAngle;
        double instantaneousSpeed; 
        double averageSpeed;
        double timeLastPolled; //change data type

        void update();
        void setup();
        void reset();

        void updateMotor();
        void updateTarget(SwerveUpdateData& data);

        //sensors 

        

        void updatePos();

    private:
        float encMotorPID();
        float fwdMotorPID();

};

void encMotChange();

extern SwerveController swerve;


#endif