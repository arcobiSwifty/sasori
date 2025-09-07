#include "hal/stm32_can_manager.h"
#include "swerve_controller.h"

STMCanCommunicator::STMCanCommunicator() {}

void STMCanCommunicator::setup(int frequency) {
    CAN.begin(500E3);
    CAN.onReceive(staticOnReceive);
}

void STMCanCommunicator::staticOnReceive(int packetSize) {
    can.onReceiveSwerve(packetSize);

}

void STMCanCommunicator::onReceiveSwerve (int packetSize) {
    if (CAN.packetId() == 0x123 && packetSize == sizeof(SwerveUpdateData)) {
        SwerveUpdateData receivedData;
        CAN.readBytes((uint8_t*)&receivedData, sizeof(receivedData));
        swerve.updateTarget(receivedData);
    }
}

void STMCanCommunicator::updatePosition(double dx, double dy) {
    
}