#ifndef DRIVE
#define DRIVE

#include "Arduino.h"
//#include "MPU9250.h"

//extern MPU9250 mpu; 

class Robot {
    public:
        Robot();
        void update();
        void setup();
        void move(); // numbers between 0 and 1, of max speed
        void moveFieldCentric();


        volatile float target_vx = 0;
        volatile float target_vy = 0; 
        float vx = 0; 
        float vy = 0;
        
        volatile float target_yaw = 0;
        volatile float yaw = 0;

        bool is_jiritu = false;
        unsigned long jiritu_start;
        int fase = 5;

        unsigned long lastMove;
};

extern Robot perry;

#endif