#include <Arduino.h>
#include "Bcontroller.h"
#include "hal/can_manager.cpp"
#include "drive.h"


BluetoothController BC;
Robot perry;
CanCommunicator ESPcan;
MPU9250 mpu; 


void setup() {

    Serial.begin(115200);
    Wire.begin();
    delay(2000);
    mpu.setup(0x68);
    mpu.calibrateAccelGyro();
    BC.connect();
    ESPcan.setup(500E3);
    
  

}

void loop() {
  
  perry.update(); //should be called as much as possible.
  
}