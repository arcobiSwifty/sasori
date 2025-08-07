#ifndef CAN 
#define CAN

#include "driver/twai.h"

const int clockFrequency; 
const int rx; 
const int tx;


struct SwerveCommandData {
    float targetSpeed;
    float targetAngle;

};

class CanCommunicator {
    void setup(int clockFrequency);

    bool updateSwerve();
    //create pointer to swerve command data. set 8 bits at a time;

    void receiveSwerve();

    void updateCamera();

    void receiveCamera();

    void reset();

};

#endif