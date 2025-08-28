#include "Bcontroller.h"
#include <ps4Controller.h>

BluetoothController* BluetoothController::instance = nullptr;

BluetoothController::BluetoothController() {
    this->instance = this;
}

void BluetoothController::connect() {
    
    ps4.attachOnConnect(onConnect);
    ps4.attachOnDisconnect(onDisconnect);
    ps4.attach(onEvent);
    ps4.begin();
    
}

void BluetoothController::onConnect() { // should not call serial inside of this function
    
}

void BluetoothController::onDisconnect(){
    
}

void BluetoothController::onEvent() {
    if (instance != nullptr) {
        instance->handleEvent();
    }
}

void BluetoothController::handleEvent() {
    if (ps4.event.button_down.cross) {

        ps4.setRumble(50, 50);
        ps4.sendToController();
    }
}
