#include "Bcontroller.h"
#include <ps4Controller.h>

BluetoothController *BluetoothController::instance = nullptr;

BluetoothController::BluetoothController()
{
  this->instance = this;
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
  if (instance != nullptr)
  {
    instance->handleEvent();
  }
}

void BluetoothController::handleEvent()
{

  if (ps4.event.button_down.cross)
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
  if (ps4.event.button_down.l2)
  {
  }
  if (ps4.event.button_down.l3)
  {
  }
  if (ps4.event.button_down.r1)
  {
  }
  if (ps4.event.button_down.r2)
  {
  }
  if (ps4.event.button_down.r3)
  {
  }
  if (ps4.event.button_down.left)
  {
  }
  if (ps4.event.button_down.right)
  {
  }
  if (ps4.event.button_down.down)
  {
  }
  if (ps4.event.button_down.up)
  {
  }

  int currentLStickX = ps4.LStickX();
  int currentLStickY = ps4.LStickY();
  int currentRStickX = ps4.RStickX();
  int currentRStickY = ps4.RStickY();

  // check if left stick moved
  if (abs(currentLStickX - LastLJoyStickX) > 0 || abs(currentLStickY - LastLJoyStickY) > 0)
  {

    int LprocessedX = (abs(currentLStickX) < 15) ? 0 : currentLStickX; // deadzone check
    int LprocessedY = (abs(currentLStickY) < 15) ? 0 : currentLStickY;

    if (LastLJoyStickX != LprocessedX || LastLJoyStickY != LprocessedY)
    {
      LastLJoyStickX = LprocessedX;
      LastLJoyStickY = LprocessedY;
      commandSent = false;
    }
  }
  // check if right stick moved
  if (abs(currentRStickX - LastRJoyStickX) > 0 || abs(currentRStickY - LastRJoyStickY) > 0)
  {

    int RprocessedX = (abs(currentRStickX) < 15) ? 0 : currentRStickX; // dead zone check
    int RprocessedY = (abs(currentRStickY) < 15) ? 0 : currentRStickY;

    if (LastRJoyStickX != RprocessedX || LastRJoyStickY != RprocessedY)
    {
      LastRJoyStickX = RprocessedX;
      LastRJoyStickY = RprocessedY;
      commandSent = false;
    }
  }
}

void BluetoothController::setErrorState()
{
  ps4.setLed(250, 0, 0);
  ps4.sendToController();
}