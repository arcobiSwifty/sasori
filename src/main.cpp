#include <Arduino.h>
#include "Blink.h"
#include <AccelStepper.h>

// --- Microstepping Configuration ---
#define MICROSTEPS 16 // Using 1/16th step for smooth motion
#define FULL_STEP 1   // Using full step for holding torque

// Define the motor interface type. 1 = DRIVER (STEP/DIR)
#define motorInterfaceType 1

// --- Motor 1 Pin Definitions ---
#define DIR_PIN_1  2
#define STEP_PIN_1 3
#define MS_PIN_1   6  // Single pin for all 3 MS inputs on driver 1

// --- Motor 2 Pin Definitions ---
#define DIR_PIN_2  4
#define STEP_PIN_2 5
#define MS_PIN_2   7  // Single pin for all 3 MS inputs on driver 2

// Create two AccelStepper objects
AccelStepper motor1(motorInterfaceType, STEP_PIN_1, DIR_PIN_1);
AccelStepper motor2(motorInterfaceType, STEP_PIN_2, DIR_PIN_2);

// State tracking to avoid redundant pin changes
bool motor1IsMoving = false;
bool motor2IsMoving = false;

// For sending data back to the GUI periodically
unsigned long lastSendTime = 0;
const long sendInterval = 100; // Send data to GUI every 100 ms

void sendStatus();
void handleSerialCommands();
void setMicrostepMode(int motorIndex, int mode);


void setup() {
  Serial.begin(9600);

  // --- Configure MS Pins as Outputs ---
  pinMode(MS_PIN_1, OUTPUT);
  pinMode(MS_PIN_2, OUTPUT);
  
  // Set initial mode to full step for max holding torque at startup
  setMicrostepMode(0, FULL_STEP);
  setMicrostepMode(1, FULL_STEP);

  // --- Configure Motor 1 ---
  // NOTE: Speed and acceleration are in MICROSTEPS per second!
  motor1.setMaxSpeed(1000 * MICROSTEPS);
  motor1.setAcceleration(500 * MICROSTEPS);
  motor1.setCurrentPosition(0);

  // --- Configure Motor 2 ---
  motor2.setMaxSpeed(1000 * MICROSTEPS);
  motor2.setAcceleration(500 * MICROSTEPS);
  motor2.setCurrentPosition(0);
}

void loop() {
  handleSerialCommands();

  // --- Dynamic Microstepping Logic ---
  // Motor 1: Check if it should be moving and set mode accordingly
  if (motor1.distanceToGo() != 0) {
    if (!motor1IsMoving) {
      setMicrostepMode(0, MICROSTEPS); // Switch to microstepping
      motor1IsMoving = true;
    }
  } else { // Motor is stopped
    if (motor1IsMoving) {
      setMicrostepMode(0, FULL_STEP); // Switch to full step for holding torque
      motor1IsMoving = false;
    }
  }

  // Motor 2: Logic is identical
  if (motor2.distanceToGo() != 0) {
    if (!motor2IsMoving) {
      setMicrostepMode(1, MICROSTEPS);
      motor2IsMoving = true;
    }
  } else {
    if (motor2IsMoving) {
      setMicrostepMode(1, FULL_STEP);
      motor2IsMoving = false;
    }
  }

  // The library's run() function MUST be called as often as possible.
  motor1.run();
  motor2.run();

  sendStatus();
}

/**
 * Sets the microstepping mode for a given motor using a single control pin.
 * motorIndex: 0 for motor1, 1 for motor2
 * mode: The microstep resolution (e.g., 1 for full, 16 for 1/16th)
 */
void setMicrostepMode(int motorIndex, int mode) {
  int msPin = (motorIndex == 0) ? MS_PIN_1 : MS_PIN_2;

  // For A4988: LOW = Full Step, HIGH = 1/16th Step
  if (mode == MICROSTEPS) {
    digitalWrite(msPin, HIGH); // Set to 1/16th microstepping
  } else {
    digitalWrite(msPin, LOW);  // Set to Full Step
  }
}

// No changes to the functions below this line
void handleSerialCommands() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('>');
    
    if (command.startsWith("<")) {
      command.remove(0, 1);
      int commaIndex = command.indexOf(',');
      if (commaIndex != -1) {
        int motorIndex = command.substring(0, commaIndex).toInt();
        long targetFullStepPos = command.substring(commaIndex + 1).toInt();
        long targetMicrostepPos = targetFullStepPos * MICROSTEPS;

        if (motorIndex == 0) {
          motor1.moveTo(targetMicrostepPos);
        } else if (motorIndex == 1) {
          motor2.moveTo(targetMicrostepPos);
        }
      }
    }
  }
}

void sendStatus() {
  if (millis() - lastSendTime > sendInterval) {
    lastSendTime = millis();

    long pos1_full = motor1.currentPosition() / MICROSTEPS;
    float speed1_full_sps = motor1.speed() / MICROSTEPS;
    long pos2_full = motor2.currentPosition() / MICROSTEPS;
    float speed2_full_sps = motor2.speed() / MICROSTEPS;

    Serial.print(pos1_full);
    Serial.print(",");
    Serial.print(speed1_full_sps);
    Serial.print(",");
    Serial.print(pos2_full);
    Serial.print(",");
    Serial.print(speed2_full_sps);
    Serial.println();
  }
}