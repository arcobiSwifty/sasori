#include <Arduino.h>
#include "Bcontroller.h"

#include <ps4Controller.h>
#include "drive.h"
#include <math.h>
#include "arm.h"

BluetoothController::BluetoothController()
{
  commandSent = true;
  LastLJoyStickX = 0;
  LastLJoyStickY = 0;
  LastRJoyStickX = 0;
  LastRJoyStickY = 0;
}

void BluetoothController::connect()
{
  ps4.begin("00:1f:e2:a0:04:32");
  ps4.attachOnConnect(onConnect);
  ps4.attachOnDisconnect(onDisconnect);
  ps4.attach(onEvent);
}

void BluetoothController::onConnect()
{ // should not call serial inside of this function
}

void BluetoothController::onDisconnect()
{
}

void BluetoothController::onEvent()
{
    BC.handleEvent();
}


// do not call any functions from here! this function should execute as quickly as possible to free the cpu
void BluetoothController::handleEvent()
{
  
  unsigned long current_time = millis();
  if (current_time - last_poll < 20) {
    return;
  } else {
    last_poll = current_time;
  }

  if (ps4.Cross())
  {

  }
  if (ps4.event.button_down.circle)
  {
  }
  if (ps4.event.button_down.square)
  {
  }
  if (ps4.event.button_down.triangle)
  {
  }
  if (ps4.event.button_down.l1)
  {
  }
  if (ps4.event.button_up.l2)
  {
    arm.relativeMove(10, 0, 0);
  }
  if (ps4.event.button_down.l3)
  {
  }
  if (ps4.event.button_down.r1)
  {
  }
  if (ps4.event.button_up.r2)
  {
    arm.relativeMove(-10, 0, 0);
  }
  if (ps4.event.button_down.r3)
  {
  }
  if (ps4.event.button_up.left)
  {
    arm.relativeMove(0, -10.0, 0);
  }
  if (ps4.event.button_up.right)
  {
    arm.relativeMove(0, 10.0, 0);
  }
  if (ps4.event.button_up.down)
  {
    arm.relativeMove(0, 0, 10);
  }
  if (ps4.event.button_down.up)
  {
    arm.relativeMove(0, 0, 10);
  }

  int currentLStickX = ps4.LStickX();
  int currentLStickY = ps4.LStickY();
  int currentRStickX = ps4.RStickX();
  int currentRStickY = ps4.RStickY();

  int LprocessedX = (abs(currentLStickX) < 15) ? 0 : currentLStickX; 
  int LprocessedY = (abs(currentLStickY) < 15) ? 0 : currentLStickY;

  perry.target_vx = static_cast<float>(LprocessedX) / 128.0; 
  perry.target_vy = static_cast<float>(LprocessedY) / 128.0;
    
  int RprocessedX = (abs(currentRStickX) < 15) ? 0 : currentRStickX; 
  int RprocessedY = (abs(currentRStickY) < 15) ? 0 : currentRStickY;

  perry.target_yaw = atan(static_cast<float>(RprocessedY) / static_cast<float>(RprocessedX) );
}

//in error state, set led to red
void BluetoothController::setErrorState()
{
  ps4.setLed(250, 0, 0);
  ps4.sendToController();
}

void BluetoothController::setNormalState() {
  ps4.setLed(0, 250, 0);
  ps4.sendToController();
}

void BluetoothController::setObjectDetectedState() {
  ps4.setLed(250, 250, 0);
  ps4.sendToController();
}