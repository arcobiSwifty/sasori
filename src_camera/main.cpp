#include <Arduino.h>
#include "camera.h"
//esp32 camera board.
void setup() {
  //establish can connection with main chip. 
  //initialize with values sent over via CAN communication
  //provide field orientation for main controller

  Serial.begin(9600);
}

void loop() {
  
  //Robot.update() should be called as much as possible.
    Serial.println("Hello world"); 
    delay(500);
  
}
