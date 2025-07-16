#include <Arduino.h>
#include "Blink.h"

int blinkPin;

void setupBlink(int pin) {
    blinkPin = pin; 
    pinMode(blinkPin, OUTPUT);
}

void performBlink() {
    digitalWrite(blinkPin,HIGH);
    delay(500);
    digitalWrite(blinkPin, LOW);
    delay(500);
}