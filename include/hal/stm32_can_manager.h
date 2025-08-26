#ifndef CANSTM
#define CANSTM

#include "can_data.h"
#include <CAN.h>
#include "swerve_controller.h"


const auto STMSwerveFrameIdentifier = 0x123;

class STMCanCommunicator {
    public:
        STMCanCommunicator(SwerveController* controller);
        STMCanCommunicator();
        void setup (int frequency);
        void onReceiveSwerve(int packetSize);

    private:
        static void staticOnReceive(int packetSize); 
        static STMCanCommunicator* _instance; 
        SwerveController* _controller;
};



#endif