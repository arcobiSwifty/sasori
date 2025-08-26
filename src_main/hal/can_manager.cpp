#include "hal/can_manager.h"

void CanCommunicator::setup(int clockFrequency) {
    ESP32Can.setPins(tx, rx);
    ESP32Can.begin(ESP32Can.convertSpeed(clockFrequency));
}

void CanCommunicator::updateSwerve(SwerveUpdateData data) {
    CanFrame txFrame = {0};
    txFrame.identifier = 0x123;
    txFrame.extd = 0; 
    txFrame.data_length_code = sizeof(data);

    memcpy(txFrame.data, &data, sizeof(data));
}

// swerve module is not an esp32 so this is not needed
void CanCommunicator::onReceiveSwerve(CanFrame &frame) {
   // if (frame.identifier == swerveFrameIdentifier && frame.data_length_code == sizeof(SwerveUpdateData)) ;

}