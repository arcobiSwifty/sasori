#include "hal/stm32_can_manager.h"
#include "hal/can_data.h"
#include "swerve_controller.h"
#include <Arduino.h>

STMCanCommunicator::STMCanCommunicator() {}

void STMCanCommunicator::setup(int frequency) {
    CAN.setPins(PA11, PA12); // pa11 rx, pa12 tx
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
    CAN.beginPacket(0x100);
    SwerveModulePositionChange data;
    data.dx =dx;
    data.dy = dy;
    CAN.write((uint8_t*)&data, sizeof(data));
    CAN.endPacket();
}