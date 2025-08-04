#include <Arduino.h>
#include <Eigen/Dense>


void setup() {
  Serial.begin(9600);
}

void loop() {
  
  //Robot.update() should be called as much as possible.
    Serial.println("Hello world"); 
    delay(500);
  
}
