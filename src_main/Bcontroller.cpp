#include <Arduino.h>
//#include <PS4controller.h>
#include "Bcontroller.h"

#include <ps4Controller.h>

void BluetoothController::connect() {
    
    ps4.attachOnConnect(BluetoothController::onConnect);
    ps4.attachOnDisconnect(BluetoothController::onDisconnect);
    ps4.attach(BluetoothController::onEvent);
    ps4.begin();
    
}

void BluetoothController::onConnect() { // should not call serial inside of this function
    Serial.println("connected");
}

void BluetoothController::onDisconnect(){
    Serial.println("disconnected");
}

void BluetoothController::onEvent() {
    
    if (ps4.event.button_down.cross) {
    Serial.println("Cross button was pressed.");
    ps4.setRumble(50, 50);
    ps4.sendToController();
  }
    
}
