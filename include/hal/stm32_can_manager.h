#ifndef CANSTM
#define CANSTM

#include "can_data.h"
#include <CAN.h>


const auto STMSwerveFrameIdentifier = 0x123;

class STMCanCommunicator {
    void setup (int frequency);
    void onReceiveSwerve(int packetSize);


};




#endif