#include <Arduino.h>
#include "Bcontroller.h"

#include "drive.h"
//#include "arm.h"


BluetoothController BC;
Robot perry;
//CanCommunicator ESPcan;
//MPU9250 mpu; 
//Arm arm;



void setup() {

    Serial.begin(115200);
    //Wire.begin();
    //delay(2000);
    //mpu.setup(0x68);
   // mpu.calibrateAccelGyro();
    BC.connect();
    //ESPcan.setup(500E3);
    perry.setup();
  //  arm.setup();
    


  

}

void loop() {
  
  perry.update(); //should be called as much as possible.
 // arm.update();
}