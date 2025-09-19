#include "Arduino.h"
#include "arm.h"
#include <math.h>
#include <TMCStepper.h>
#include "FastAccelStepper.h"
#include <HardwareSerial.h>

#define PRIMARY_DIR 17
#define SECONDARY_DIR 18
#define TURRET_DIR 21

#define PRIMARY_STEP 16
#define SECONDARY_STEP 19
#define TURRET_STEP 6


#define PRIMARY_UART 17

#define ARM_LENGTH 300.0

#define DRIVER_RX_PIN 3 
#define DRIVER_TX_PIN 1

 
HardwareSerial &mySerial = Serial1;
TMC2209Stepper driver(&mySerial,0.11f,0);

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepperPrimary = NULL;
FastAccelStepper *stepperSecondary = NULL;
FastAccelStepper *stepperTurret = NULL;


void Arm::setup() {

    pinMode(PRIMARY_DIR, OUTPUT);
    pinMode(SECONDARY_DIR, OUTPUT);
    pinMode(TURRET_DIR, OUTPUT);

    pinMode(PRIMARY_STEP, OUTPUT);
    pinMode(SECONDARY_STEP, OUTPUT);
    pinMode(TURRET_STEP, OUTPUT);

    pinMode(PRIMARY_UART, OUTPUT);


    mySerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

    driver.begin();
  
    driver.I_scale_analog(false); 
  
  // Set driver current in milliamps (mA)
  // IRUN is the running current, IHOLD is the holding current
    driver.rms_current(1800, 0.1); // 800mA run current, 10% (80mA) hold current

  // Enable StealthChop for silent operation
    driver.toff(4); // Recommended value for StealthChop

   driver.pdn_disable(true);

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


  // Initialize the single engine for all steppers
  engine.init();

  stepperPrimary = engine.stepperConnectToPin(PRIMARY_STEP);
  if (stepperPrimary) {
    stepperPrimary->setDirectionPin(PRIMARY_DIR);
    stepperPrimary->setAcceleration(500); // steps/s^2
    stepperPrimary->setSpeedInHz(1000);    // steps/s
  }


  stepperSecondary = engine.stepperConnectToPin(SECONDARY_STEP);
  if (stepperSecondary) {
    stepperSecondary->setDirectionPin(SECONDARY_DIR);
    stepperSecondary->setAcceleration(500);
    stepperSecondary->setSpeedInHz(1000);
  }

  // --- Configure Stepper Z ---
  stepperTurret = engine.stepperConnectToPin(TURRET_STEP);
  if (stepperTurret) {
    stepperTurret->setDirectionPin(TURRET_DIR);
    stepperTurret->setAcceleration(200); // Z-axis might be heavier
    stepperTurret->setSpeedInHz(500);
  }

  stepperPrimary->setCurrentPosition(457);
  stepperSecondary->setCurrentPosition(-49);
  stepperTurret->setCurrentPosition(0);

}

const float ratio1 = 450;
const float ratio2 = 30.0 / 12.0;
void Arm::goTo(const float rho, const float phi, const float r)
{
    this->targetRho = rho;
    this->targetPhi = phi;
    this->targetR = r;

    float primaryAngle = acos(r / (2.0 * ARM_LENGTH)) + rho / 2.0;
    float secondaryAngle = acos(r / (2.0 * ARM_LENGTH)) - rho / 2.0;
    float turretAngle = phi;

    this->targetPosTurret = static_cast<int>(turretAngle / 360.0 * 200.0);
    this->targetPosPrimary = static_cast<int>(primaryAngle / 360.0 * 200.0);
    this->targetPosSecondary = static_cast<int>(secondaryAngle / 360.0 * 200.0);
    stepperPrimary->moveTo(targetPosPrimary * ratio1);
    stepperSecondary->moveTo(targetPosSecondary * ratio2);
    stepperTurret->moveTo(targetPosTurret * ratio2);
}

void Arm::relativeMove(const float drho, const float dphi, const float dr)
{
    float primaryAngle = acos(dr / (2.0 * ARM_LENGTH)) + drho / 2.0;
    float secondaryAngle = acos(dr / (2.0 * ARM_LENGTH)) - drho / 2.0;
    float turretAngle = dphi;

    stepperPrimary->moveTo(stepperPrimary->getCurrentPosition() + targetPosPrimary * ratio1);
    stepperSecondary->moveTo(stepperSecondary->getCurrentPosition() + targetPosSecondary * ratio2);
    stepperTurret->moveTo(stepperTurret->getCurrentPosition() + targetPosTurret * ratio2);
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
  
}

void Arm::unpause() {

}