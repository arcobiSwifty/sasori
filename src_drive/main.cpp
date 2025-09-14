#include <Arduino.h>
#include "swerve_controller.h"
#include "hal/stm32_can_manager.h"
#include "imu.h"


SwerveController swerve;
SimpleCan can(-1,-1);
//Adafruit_MPU6050 mpu;

void can_message_handler(FDCAN_RxHeaderTypeDef rxHeader, uint8_t *rxData)
{
  if (rxHeader.Identifier == 0x123 ) {
        SwerveUpdateData receivedData;
        memcpy(&receivedData, rxData, sizeof(SwerveUpdateData));
        swerve.updateTarget(receivedData);
    }
}
SimpleCan::RxHandler rxHandler(8, can_message_handler);

void setup() {
  swerve.InitMotorPins(PA0,PA1,PA2,PA3);
  swerve.InitSensorPins(PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7);
  swerve.setup();   // setup arduino pins 
  swerve.reset(); //reset motor orientation to match hall effect sensor
  can.init(CanSpeed::Kbit500);
  FDCAN_FilterTypeDef filterDef;
  filterDef.IdType = FDCAN_STANDARD_ID;       
  filterDef.FilterIndex = 0;                   
  filterDef.FilterType = FDCAN_FILTER_MASK;    
  filterDef.FilterConfig = FDCAN_FILTER_REMOTE; 
  filterDef.FilterID1 = 0x000;                  // Not used 
  filterDef.FilterID2 = 0x000;  
  can.activateNotification(&rxHandler);
  can.start();
 
}

void loop() {
    // imu -> get position 
    // magnet -> get position 
    // use model to estimate position 
    // update swerve from new data
    swerve.update();
}


