#include <Arduino.h>
#include <vector>
#include "robot.h"
#include "swerve.h"
#include "Bcontroller.h"
#include "hal/can_manager.h"



Robot perry;


void setup() {
  perry = Robot();
  perry.setup();

  BluetoothController::connect();
  Serial.begin(115200);
}

void loop() {
  // on receive position updates from stm32 -> update internal 
  // position x y and orientation.

  // bluetooth class: last sent command, current command
  // on change bluetooth:
  // update current command
  // check if current command changed 
  //movement command-> send via CAN
  //
  
  perry.update(); //should be called as much as possible.
  
}

