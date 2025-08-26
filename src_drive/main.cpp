#include <Arduino.h>
#include <Eigen/Dense>
#include "swerve_controller.h"
#include "hal/stm32_can_manager.h"

SwerveController swerve;
STMCanCommunicator can;


void setup() {
  //establish can connection with main chip. 
  //initialize with values send over via CAN communication
  //reset motor orientation to match hall effect sensor
  swerve = SwerveController();
  swerve.setup();
  can = STMCanCommunicator(&swerve);
  can.setup(500E3);
}

void loop() {
    // imu -> get position 
    // magnet -> get position 
    // use model to estimate position 
    // update swerve from new data
    swerve.update();
}
