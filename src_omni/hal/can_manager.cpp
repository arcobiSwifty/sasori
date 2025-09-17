

//#include "hal/can_manager.h"
//#include <ESP32-TWAI-CAN.hpp>
/*
void CanCommunicator::setup(int clockFrequency) {
    ESP32Can.setPins(tx, rx); //
    ESP32Can.begin(ESP32Can.convertSpeed(clockFrequency));

    
}

void CanCommunicator::updateSwerve(SwerveUpdateData data) {
    CanFrame txFrame = {0};
    txFrame.extd = 0; 
    txFrame.data_length_code = sizeof(data);

    txFrame.identifier = 0x123;
    memcpy(txFrame.data, &data, sizeof(data));
    ESP32Can.writeFrame(txFrame);
}

// swerve module is not an esp32 so this is not needed
void CanCommunicator::onReceiveSwerve(CanFrame &frame) {
   // if (frame.identifier == swerveFrameIdentifier && frame.data_length_code == sizeof(SwerveUpdateData)) ;

}
*/