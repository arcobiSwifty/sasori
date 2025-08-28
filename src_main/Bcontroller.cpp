#include "Bcontroller.h"
#include <ps4Controller.h>


BluetoothController* BluetoothController::instance = nullptr;

BluetoothController::BluetoothController() {
    this->instance = this;
    commandSent = true;
    LastLJoyStickX = 0;
    LastLJoyStickY = 0;
    LastRJoyStickX = 0;
    LastRJoyStickY = 0;
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
  int currentLStickX = ps4.LStickX();
  int currentLStickY = ps4.LStickY();
  int currentRStickX = ps4.RStickX();
  int currentRStickY = ps4.RStickY();


  if (abs(currentLStickX - LastLJoyStickX) > 0 || abs(currentLStickY - LastLJoyStickY) > 0) {
    
    int LprocessedX = (abs(currentLStickX) < 15) ? 0 : currentLStickX;
    int LprocessedY = (abs(currentLStickY) < 15) ? 0 : currentLStickY;

    if (LastLJoyStickX != currentLStickX || LastLJoyStickY != currentLStickY) {
        LastLJoyStickX = currentLStickX;
        LastLJoyStickY = currentLStickY;
        commandSent = false;
    }

    if (abs(currentRStickX - LastRJoyStickX) > 0 || abs(currentRStickY - LastRJoyStickY) > 0) {
    
    int RprocessedX = (abs(currentRStickX) < 15) ? 0 : currentRStickX;
    int RprocessedY = (abs(currentRStickY) < 15) ? 0 : currentRStickY;

    LastRJoyStickX = currentLStickX;
    LastRJoyStickY = currentLStickY;
  }
  
  // You can add similar logic for the right stick if needed
  if (abs(currentRStickX - LastRJoyStickX) > 0 || abs(currentRStickY - LastRJoyStickY) > 0) {
    // Logic for right stick change...
    LastRJoyStickX = currentRStickX;
    LastRJoyStickY = currentRStickY;
  }
  if (LastRJoyStickX != currentRStickX || LastRJoyStickY != currentRStickY) {
        LastRJoyStickX = currentRStickX;
        LastRJoyStickY = currentRStickY;
        commandSent = false;
    }
  }
}
