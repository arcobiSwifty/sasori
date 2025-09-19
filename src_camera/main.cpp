#include <Arduino.h>
#include "camera.h"
#include <ESP32-TWAI-CAN.hpp>
#include "hal/can_data.h"

Camera cam;


//esp32 camera board.
void setup() {



  //cam.setup();

}

void loop() {
 //line position info
      cam.getBlockPosition(1);

 
}





