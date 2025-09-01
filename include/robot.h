#ifndef ROBOT_H
#define ROBOT_H

#include "arm.h"
#include "hal/can_manager.h"
#include "Bcontroller.h"
#include "hal/can_manager.h"
#include <ESP32-TWAI-CAN.hpp>

//handles main update loop and setup. delegates tasks to the swerve controller arm and logger components.

class Robot {
    //properties
    public:

        Robot();

        Arm arm;
        CanCommunicator* can;

        void setup(BluetoothController* human);
        void update();


        void moveToPos(float x, float y);
        void rotateToAngle(float angle);
        float getAbsPosX();
        float getAbsPosY();

        BluetoothController* BC;

    private: 
        float absPosX; //units in mm
        float absPosY; //units in mm
        float absAngle; //units in degrees

    //methods:
    //robot. init (calls all submodule's init)

    //change speed
    //change angular speed 

    //reset 

    //log
};



#endif