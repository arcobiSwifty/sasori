#include "hal/can_manager.h"
#include <ESP32-TWAI-CAN.hpp>

void CanCommunicator::setup(int clockFrequency) {
    ESP32Can.setPins(tx, rx); //
    ESP32Can.begin(ESP32Can.convertSpeed(clockFrequency));

}


void onReceiveCamera();