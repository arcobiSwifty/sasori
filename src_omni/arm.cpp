#include "Arduino.h"
#include "arm.h"
#include <math.h>
#include <TMCStepper.h>
#include <SoftwareSerial.h> 

#define PRIMARY_DIR 1
#define SECONDARY_DIR 2
#define TURRET_DIR 3

#define PRIMARY_STEP 4
#define SECONDARY_STEP 5
#define TURRET_STEP 6

#define ENABLE_PIN 7 

#define PRIMARY_UART 8
#define SECONDARY_UART 9
#define TURRET_UART 10

#define ARM_LENGTH 300.0

#define SW_RX     10 // SoftwareSerial RX -> Driver TX
#define SW_TX     11 // SoftwareSerial TX -> Driver RX

// Use SoftwareSerial or a free HardwareSerial port
SoftwareSerial mySerial(SW_TX, SW_RX); 

// Create the driver object
TMC2209Stepper driver(&mySerial);


void Arm::setup() {
    pinMode(PRIMARY_DIR, OUTPUT);
    pinMode(SECONDARY_DIR, OUTPUT);
    pinMode(TURRET_DIR, OUTPUT);

    pinMode(PRIMARY_STEP, OUTPUT);
    pinMode(SECONDARY_STEP, OUTPUT);
    pinMode(TURRET_STEP, OUTPUT);

    pinMode(PRIMARY_UART, OUTPUT);
    pinMode(SECONDARY_UART, OUTPUT);
    pinMode(PRIMARY_DIR, OUTPUT);

    pinMode(ENABLE_PIN, OUTPUT);

     mySerial.begin(115200); // TMC2209 default baud rate
  Serial.begin(115200);   // For debugging

  driver.begin();
  
  driver.I_scale_analog(false); 
  
  // Set driver current in milliamps (mA)
  // IRUN is the running current, IHOLD is the holding current
  driver.rms_current(800, 0.1); // 800mA run current, 10% (80mA) hold current

  // Enable StealthChop for silent operation
  driver.toff(4); // Recommended value for StealthChop

  driver.stealth = false;

// Set the StallGuard threshold. This value (0-255) needs tuning.
// A lower value is more sensitive. Start around 10-20.
driver.SGTHRS(15);

// Configure CoolStep settings
// seimin(0) -> minimum current is 50%
// seimin(1) -> minimum current is 25%
driver.semin(1); // Set minimum current to 25% of IRUN
driver.semax(2); // Set max current adjustment steps
driver.sedn(1); // Set current down-step speed

// Enable CoolStep by setting a non-zero hysteresis
driver.TCOOLTHRS(0xFFFFF); // Set a threshold to activate CoolStep
}

void Arm::goTo(const float rho, const float phi, const float r) {
    this->targetRho = rho;
    this->targetPhi = phi;
    this->targetR = r;

    float primaryAngle = acos(r / (2.0*ARM_LENGTH)) + rho / 2.0;
    float secondaryAngle = acos(r / (2.0*ARM_LENGTH)) - rho / 2.0;
    float turretAngle = phi;

    this->targetPosTurret = static_cast<int>(turretAngle / 360.0 * 200.0);
    this->targetPosPrimary = static_cast<int>(primaryAngle / 360.0 * 200.0);
    this->targetPosSecondary = static_cast<int>(secondaryAngle / 360.0 * 200.0);
}

void Arm::update() { 
    unsigned long currentTime = micros();
    if ((currentTime - lastPoll) < 100) {
        return;
    } else {
        lastPoll = currentTime;
    }
    if (posTurret - targetPosTurret != 0) {

    }
    if (posPrimary - targetPosPrimary != 0) {

    } 
    if (posSecondary - targetPosSecondary != 0) {}


}


void Arm::pause() { 
    digitalWrite(ENABLE_PIN, HIGH);
}

void Arm::unpause() {
    digitalWrite(ENABLE_PIN, LOW);
}