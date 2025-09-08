#include <Arduino.h>
#include <vector>
#include "robot.h"
#include "swerve.h"
#include "Bcontroller.h"
#include "hal/can_manager.h"
#include "arm.h"


BluetoothController BC;
Robot perry;
CanCommunicator ESPcan;
Arm arm;

void setup() {
  
  ESPcan.setup(500E3);
  BC.connect();
  perry.setup();
  arm.setup();

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

