#include "hal/stm32_can_manager.h"

void STMCanCommunicator::onReceiveSwerve (int packetSize) {
    if (CAN.packetId() == 0x123 && packetSize == sizeof(SwerveUpdateData)) {
        
    }
}