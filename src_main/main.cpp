#include <Arduino.h>
#include <vector>
#include "robot.h"
#include "swerve.h"
#include "Bcontroller.h"
#include "hal/can_manager.h"


static BluetoothController human;
static Robot perry;
static CanCommunicator c;

void setup() {
  int LED_BUILTIN = 2;

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  c.setup(500E3);
  human.connect();
  perry.setup(&human);
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

