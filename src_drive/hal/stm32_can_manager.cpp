#include "hal/stm32_can_manager.h"
#include "swerve_controller.h"

STMCanCommunicator::STMCanCommunicator() {}

STMCanCommunicator::STMCanCommunicator(SwerveController* c)  {
    this->_controller = c;
}

void STMCanCommunicator::setup(int frequency) {
    CAN.begin(500E3);
    _instance = this;
    CAN.onReceive(staticOnReceive);
}

void STMCanCommunicator::staticOnReceive(int packetSize) {
    if (_instance != nullptr) {
        _instance->onReceiveSwerve(packetSize);
    }
}

void STMCanCommunicator::onReceiveSwerve (int packetSize) {
    if (CAN.packetId() == 0x123 && packetSize == sizeof(SwerveUpdateData)) {
        SwerveUpdateData receivedData;
        CAN.readBytes((uint8_t*)&receivedData, sizeof(receivedData));
        this->_controller->updateTarget(receivedData);
    }
}