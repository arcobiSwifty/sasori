#include "Arduino.h"
#include "arm.h"
#include <math.h>
#include <TMCStepper.h>
#include "AccelStepper.h"
#include <HardwareSerial.h>

#define PRIMARY_DIR 17
#define SECONDARY_DIR 18
#define TURRET_DIR 21

#define PRIMARY_STEP 16
#define SECONDARY_STEP 5
#define TURRET_STEP 19

#define PRIMARY_UART 27


#define ARM_LENGTH 30.0

#define DRIVER_RX_PIN 25
#define DRIVER_TX_PIN 26

const int MICRO_STEP_FACTOR = 1;

//HardwareSerial &mySerial = Serial1;
//
//TMC2209Stepper driver(&mySerial, 0.11f, 0);

/*
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepperPrimary = NULL;
FastAccelStepper *stepperSecondary = NULL;
FastAccelStepper *stepperTurret = NULL;
*/



#define DRIVER_INTERFACE AccelStepper::DRIVER

AccelStepper STEPPER_P(DRIVER_INTERFACE, PRIMARY_STEP, PRIMARY_DIR);
AccelStepper STEPPER_S(DRIVER_INTERFACE, SECONDARY_STEP, SECONDARY_DIR);
AccelStepper STEPPER_T(DRIVER_INTERFACE, TURRET_STEP, TURRET_DIR);


void Arm::setup()
{

  pinMode(PRIMARY_DIR, OUTPUT);
  pinMode(SECONDARY_DIR, OUTPUT);
  pinMode(TURRET_DIR, OUTPUT);

  pinMode(PRIMARY_STEP, OUTPUT);
  pinMode(SECONDARY_STEP, OUTPUT);
  pinMode(TURRET_STEP, OUTPUT);

  pinMode(PRIMARY_UART, OUTPUT);
/*
  mySerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);

  driver.begin();

  driver.I_scale_analog(false);

  // Set driver current in milliamps (mA)
  // IRUN is the running current, IHOLD is the holding current
  driver.rms_current(2000, 0.1); // 800mA run current, 10% (80mA) hold current

   driver.en_spreadCycle(false);

  // Enable StealthChop for silent operation
  driver.toff(4); // Recommended value for StealthChop

  driver.pdn_disable(true);

  // Set the StallGuard threshold. This value (0-255) needs tuning.
  // A lower value is more sensitive. Start around 10-20.
  driver.SGTHRS(100);

  // Configure CoolStep settings
  // seimin(0) -> minimum current is 50%
  // seimin(1) -> minimum current is 25%
  driver.semin(1); // Set minimum current to 25% of IRUN
  driver.semax(2); // Set max current adjustment steps
  driver.sedn(1);  // Set current down-step speed

  // Enable CoolStep by setting a non-zero hysteresis
  driver.TCOOLTHRS(100); // Set a threshold to activate CoolStep

 uint8_t result = driver.test_connection();
  if (result == 0) {
    Serial.println("TMC2209 driver is connected and responding!");
    Serial.print("Version: 0x");
    Serial.println(driver.version(), HEX);
  } else {
    Serial.println("Error: TMC2209 driver not responding. Check wiring and power.");

  }

  // Initialize the single engine for all steppers
  engine.init();

  stepperPrimary = engine.stepperConnectToPin(PRIMARY_STEP);
  if (stepperPrimary)
  {
    stepperPrimary->setDirectionPin(PRIMARY_DIR);
    stepperPrimary->setAcceleration(500 * MICRO_STEP_FACTOR); // steps/s^
    stepperPrimary->setSpeedInHz(300 * MICRO_STEP_FACTOR);   // steps/s
  } else {
    Serial.println("error");
  }

  stepperSecondary = engine.stepperConnectToPin(SECONDARY_STEP);
  if (stepperSecondary)
  {
    stepperSecondary->setDirectionPin(SECONDARY_DIR);
    stepperPrimary->setAcceleration(500 * MICRO_STEP_FACTOR); // steps/s^
    stepperPrimary->setSpeedInHz(500 * MICRO_STEP_FACTOR); 
  } else {
    Serial.println("error2");
  }

  // --- Configure Stepper Z ---
  stepperTurret = engine.stepperConnectToPin(TURRET_STEP);
  if (stepperTurret)
  {
    stepperTurret->setDirectionPin(TURRET_DIR);
    stepperPrimary->setAcceleration(100 * MICRO_STEP_FACTOR); // steps/s^
    stepperPrimary->setSpeedInHz(300 * MICRO_STEP_FACTOR); 
  }

  stepperPrimary->setCurrentPosition(457);
  stepperSecondary->setCurrentPosition(-49);
  stepperTurret->setCurrentPosition(0);
  */
  STEPPER_P.setMaxSpeed(500);
  STEPPER_P.setAcceleration(200);
  STEPPER_S.setMaxSpeed(500);
  STEPPER_S.setAcceleration(200);
  STEPPER_T.setMaxSpeed(500);
  STEPPER_T.setAcceleration(200);
}

const float ratio1 = 4;
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
 // stepperPrimary->moveTo(targetPosPrimary * ratio1);
  //stepperSecondary->moveTo(targetPosSecondary * ratio2);
 // stepperTurret->moveTo(targetPosTurret * ratio2);
/*
  static unsigned long last_print_time = 0;
  if (millis() - last_print_time > 100) {
    Serial.print("StallGuard Result (sg_result): ");
    Serial.println(driver.SG_RESULT()); // This is the value you need to see
    last_print_time = millis();
  }*/
}

void Arm::relativeMove(const float drho, const float dphi, const float dr)
{
  float primaryAngle = acos(dr / (2.0 * ARM_LENGTH)) + drho / 2.0;
  float secondaryAngle = acos(dr / (2.0 * ARM_LENGTH)) - drho / 2.0;
  float turretAngle = dphi;

  this->targetPosTurret = static_cast<int>(turretAngle / 360.0 * 200.0);
  this->targetPosPrimary = static_cast<int>(primaryAngle / 360.0 * 200.0);
  this->targetPosSecondary = static_cast<int>(secondaryAngle / 360.0 * 200.0);
/*
  Serial.println(stepperSecondary->getCurrentPosition());


  stepperPrimary->moveTo(stepperPrimary->getCurrentPosition() + targetPosPrimary * ratio1);
  stepperSecondary->moveTo(stepperSecondary->getCurrentPosition() + targetPosSecondary * ratio2);
  stepperTurret->moveTo(stepperTurret->getCurrentPosition() + targetPosTurret * ratio2);
  */
 STEPPER_P.moveTo(STEPPER_P.currentPosition() + targetPosPrimary * ratio1);
 STEPPER_S.moveTo(STEPPER_P.currentPosition() + targetPosSecondary * ratio1);
 STEPPER_T.moveTo(STEPPER_P.currentPosition() + targetPosTurret * ratio1);
}

void Arm::update()

{
  STEPPER_P.run();
  STEPPER_S.run();
  STEPPER_T.run();
 
}

void Arm::pause()
{
}

void Arm::unpause()
{
}