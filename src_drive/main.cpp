#include <Arduino.h>
#include "swerve_controller.h"
#include "hal/stm32_can_manager.h"
#include "imu.h"


SwerveController swerve;
STMCanCommunicator can;
//Adafruit_MPU6050 mpu;

void setup() {
  swerve.InitMotorPins(PA0,PA1,PA2,PA3);
  swerve.InitSensorPins(PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7);
  swerve.setup();   // setup arduino pins 
  swerve.reset(); //reset motor orientation to match hall effect sensor
  
  can.setup(500E3);  //establish can connection with main chip. 
}

void loop() {
    // imu -> get position 
    // magnet -> get position 
    // use model to estimate position 
    // update swerve from new data
    swerve.update();
}
