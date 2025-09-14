#include <Arduino.h>
#include "camera.h"
#include <ESP32-TWAI-CAN.hpp>
#include "hal/can_data.h"

Camera cam;

CanFrame rxFrame;

//esp32 camera board.
void setup() {
  ESP32Can.setPins(1, 3); //
    ESP32Can.begin(ESP32Can.convertSpeed(500E3));




  //cam.setup();

}

void loop() {
 if ( ESP32Can.readFrame(rxFrame)) {
    if(rxFrame.identifier == 0x223) {  //line position info
      cam::getBlockPosition();
    }
    if (rxFrame.identifier == 0x224) { //block position info 
      cam::getLinePosition();
    }
 }  
}





