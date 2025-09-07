#include <Arduino.h>
#include <Eigen/Dense>
#include "swerve_controller.h"
#include "hal/stm32_can_manager.h"
#include "imu.h"


SwerveController swerve(PA0,PA1,PA2,PA3);
STMCanCommunicator can;

void setup() {
  
  swerve.setup();   // setup arduino pins 
  can.setup(500E3);  //establish can connection with main chip. 

  swerve.reset(); //reset motor orientation to match hall effect sensor
}

void loop() {
    // imu -> get position 
    // magnet -> get position 
    // use model to estimate position 
    // update swerve from new data
    swerve.update();
}
