#include "MapperJoystickToKeyboard.h"
#include "textfile.h"
#include <tchar.h>
#include <strsafe.h>
#include <Windows.h>

MapperJoystickToKeyboard::MapperJoystickToKeyboard(int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface) : FileLogger(NULL, TEXT("Joy"), deviceNo, loggerInterface) {
}

void MapperJoystickToKeyboard::Log(const DIJOYSTATE2& joyState)
{
  TCHAR sz[32];

  for (int povNo = 0; povNo < 4; povNo++) {
    //StringCchPrintf(sz, 32, TEXT("POV%d"), 1 + povNo);
    //LogBaseButton(sz, joyState.rgdwPOV[povNo], _lastState.rgdwPOV[povNo]);
  }

  int buttonCount = sizeof(joyState.rgbButtons);
  for (int buttonNo = 0; buttonNo < buttonCount; ++buttonNo) {
    //StringCchPrintf(sz, 32, TEXT("Button%d"), 1 + buttonNo);
    //LogBaseButton(sz, joyState.rgbButtons[buttonNo], _lastState.rgbButtons[buttonNo]);

    if (buttonNo == 1)
    {
      if (joyState.rgbButtons[buttonNo] > 0 && _lastState.rgbButtons[buttonNo] == 0)
      {
        // On
        MapperJoystickToKeyboard::KeyDown(0x11);
      }
      if (joyState.rgbButtons[buttonNo] == 0 && _lastState.rgbButtons[buttonNo] >0)
      {
        // Off
        MapperJoystickToKeyboard::KeyUp(0x11);
      }

    }
  }

  //LogBaseMovement(TEXT("Steering"), joyState.lX, _lastState.lX);
  //LogBaseMovement(TEXT("Accelerator"), joyState.lY, _lastState.lY);
  //LogBaseMovement(TEXT("Brake"), joyState.lRz, _lastState.lRz);
  //LogBaseMovement(TEXT("Gears"), joyState.rglSlider[0], _lastState.rglSlider[0]);
  //LogBaseMovement(TEXT("Clutch"), joyState.rglSlider[1], _lastState.rglSlider[1]);

  //LogBaseMovement(TEXT("X"), joyState.lX, _lastState.lX);
  //LogBaseMovement(TEXT("Y"), joyState.lY, _lastState.lY);
  //LogBaseMovement(TEXT("Z"), joyState.lZ, _lastState.lZ);

  //LogBaseMovement(TEXT("Rx"), joyState.lRx, _lastState.lRx);
  //LogBaseMovement(TEXT("Ry"), joyState.lRy, _lastState.lRy);
  //LogBaseMovement(TEXT("Rz"), joyState.lRz, _lastState.lRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    //StringCchPrintf(sz, 32, TEXT("Slider%d"), 1 + sliderNo);
    //LogBaseMovement(sz, joyState.rglSlider[sliderNo], _lastState.rglSlider[sliderNo]);
  }

  //LogBaseMovement(TEXT("VX"), joyState.lVX, _lastState.lVX);
  //LogBaseMovement(TEXT("VY"), joyState.lVY, _lastState.lVY);
  //LogBaseMovement(TEXT("VZ"), joyState.lVZ, _lastState.lVZ);

  //LogBaseMovement(TEXT("VRx"), joyState.lVRx, _lastState.lVRx);
  //LogBaseMovement(TEXT("VRy"), joyState.lVRy, _lastState.lVRy);
  //LogBaseMovement(TEXT("VRz"), joyState.lVRz, _lastState.lVRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    //StringCchPrintf(sz, 32, TEXT("VSlider%d"), 1 + sliderNo);
    //LogBaseMovement(sz, joyState.rglVSlider[sliderNo], _lastState.rglVSlider[sliderNo]);
  }

  //LogBaseMovement(TEXT("AX"), joyState.lAX, _lastState.lAX);
  //LogBaseMovement(TEXT("AY"), joyState.lAY, _lastState.lAY);
  //LogBaseMovement(TEXT("AZ"), joyState.lAZ, _lastState.lAZ);

  //LogBaseMovement(TEXT("ARx"), joyState.lARx, _lastState.lARx);
  //LogBaseMovement(TEXT("ARy"), joyState.lARy, _lastState.lARy);
  //LogBaseMovement(TEXT("ARz"), joyState.lARz, _lastState.lARz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    //StringCchPrintf(sz, 32, TEXT("ASlider%d"), 1 + sliderNo);
    //LogBaseMovement(sz, joyState.rglASlider[sliderNo], _lastState.rglASlider[sliderNo]);
  }

 /* LogBaseMovement(TEXT("FX"), joyState.lFX, _lastState.lFX);
  LogBaseMovement(TEXT("FY"), joyState.lFY, _lastState.lFY);
  LogBaseMovement(TEXT("FZ"), joyState.lFZ, _lastState.lFZ);

  LogBaseMovement(TEXT("FRx"), joyState.lFRx, _lastState.lFRx);
  LogBaseMovement(TEXT("FRy"), joyState.lFRy, _lastState.lFRy);
  LogBaseMovement(TEXT("FRz"), joyState.lFRz, _lastState.lFRz);*/

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    //StringCchPrintf(sz, 32, TEXT("FSlider%d"), 1 + sliderNo);
    //LogBaseMovement(sz, joyState.rglFSlider[sliderNo], _lastState.rglFSlider[sliderNo]);
  }
}

// For key codes
// https://www.codeproject.com/Articles/7305/Keyboard-Events-Simulation-using-keybd-event-funct
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646271(v=vs.85).aspx
// https://stackoverflow.com/questions/22419038/how-to-use-sendinput-function-c
// https://stackoverflow.com/questions/3644881/simulating-keyboard-with-sendinput-api-in-directinput-applications/3647975#3647975

void MapperJoystickToKeyboard::KeyDown(int scanCode)
{
  INPUT inputData = { 0 };
  inputData.type = INPUT_KEYBOARD;
  //inputData.ki.wVk = 0x57;
  inputData.ki.wScan = scanCode;
  inputData.ki.dwFlags = KEYEVENTF_SCANCODE;
  SendInput(1, &inputData, sizeof(INPUT));
}
void MapperJoystickToKeyboard::KeyUp(int scanCode)
{
  INPUT inputData = { 0 };
  inputData.type = INPUT_KEYBOARD;
  //inputData.ki.wVk = 0x57;
  //inputData.ki.wScan = 0x91;
  inputData.ki.wScan = scanCode;
  inputData.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
  SendInput(1, &inputData, sizeof(INPUT));
}

