#include "MapperJoystickToKeyboard.h"
#include "MapperJoystickToKeyboardConfig.h"
#include "textfile.h"
#include <tchar.h>
#include <strsafe.h>
#include <Windows.h>

MapperJoystickToKeyboard::MapperJoystickToKeyboard(const TCHAR* filename, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface) 
  : FileLoggerJoystick(filename, deviceNo, loggerInterface) 
{
}

void MapperJoystickToKeyboard::Log(const DIJOYSTATE2& joyState)
{
  for (int povNo = 0; povNo < 4; povNo++) 
  {
    // joyState.rgdwPOV[povNo], _lastState.rgdwPOV[povNo]
  }

  int buttonCount = sizeof(joyState.rgbButtons);
  for (int buttonNo = 0; buttonNo < buttonCount; ++buttonNo) 
  {
    // joyState.rgbButtons[buttonNo], _lastState.rgbButtons[buttonNo]

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
  FileLoggerJoystick::Log(joyState);
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

