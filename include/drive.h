#ifndef DRIVE
#define DRIVE

#include "MPU9250.h"

extern MPU9250 mpu; 

class Robot {
    public:
        void update();
        void setup();
        void move(); // numbers between 0 and 1, of max speed


        volatile float target_vx;
        volatile float target_vy; 
        float vx; 
        float vy;
        
        volatile float target_yaw;
        volatile float yaw;
};

extern Robot perry;

#endif