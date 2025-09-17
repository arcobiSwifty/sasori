#ifndef DRIVE
#define DRIVE

#include "Arduino.h"
#include "MPU9250.h"

extern MPU9250 mpu; 

class Robot {
    public:
        void update();
        void setup();
        void move(); // numbers between 0 and 1, of max speed
        void moveFieldCentric();


        volatile float target_vx;
        volatile float target_vy; 
        float vx; 
        float vy;
        
        volatile float target_yaw;
        volatile float yaw;
};

extern Robot perry;

#endif