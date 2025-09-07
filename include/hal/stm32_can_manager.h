#ifndef CANSTM
#define CANSTM

#include "can_data.h"
#include <CAN.h>


const auto STMSwerveFrameIdentifier = 0x123;

class STMCanCommunicator {
    public:
        STMCanCommunicator();
        void setup (int frequency);
        void onReceiveSwerve(int packetSize);
        void updatePosition(double dx, double dy);

    private:
        static void staticOnReceive(int packetSize); 
};

extern STMCanCommunicator can;



#endif