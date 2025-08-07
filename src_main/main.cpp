#include <Arduino.h>
#include <vector>
#include "robot.h"
#include "swerve.h"
#include "controller.h"


Robot perry;

void setup() {
  //define robot. initialize robot.
  perry = Robot();
  perry.setup();
}

void loop() {
  
  perry.update() //should be called as much as possible.
  
}

