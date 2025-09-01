 #ifndef CAN 
#define CAN

//#include <ESP32-TWAI-CAN.hpp>
#include "hal/can_data.h"
#include <ESP32-TWAI-CAN.hpp>
//const int clockFrequency; 
const int rx = 4;
const int tx = 5;

//packet id list 
// update swerve target speed 0x123
// update robot position 

const auto swerveFrameIdentifier = 0x123;


// all 3 modules should use the same CAN CONTROLLER and access different functions to communicate


struct CameraUpdateData {};

struct CameraPosData {};

class CanCommunicator {
    public: 
        void setup(int clockFrequency);

        void updateSwerve(SwerveUpdateData data);

        void onReceiveSwerve(CanFrame &frame);

        void updateCamera(CameraUpdateData data);

        CameraPosData receiveCamera();

        void reset();

};

#endif