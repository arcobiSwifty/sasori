#include <Arduino.h>
#include <Eigen/Dense>


void setup() {
  //establish can connection with main chip. 
  //initialize with values send over via CAN communication
  //reset motor orientation to match hall effect sensor

  Serial.begin(9600);
  pinMode(PA5, OUTPUT);
  digitalWrite(PA5, HIGH);
}

void loop() {
  
  //Robot.update() should be called as much as possible.
    Serial.println("Hello world"); 
    delay(500);
  
}
