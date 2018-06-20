#include "MapperJoystickToKeyboard.h"
#include "MapperJoystickToKeyboardConfig.h"
#include "HIDMapperInterface.h"
#include <tchar.h>
#include <strsafe.h>
#include <Windows.h>

MapperJoystickToKeyboard::MapperJoystickToKeyboard(int deviceNo, HIDMapperDLL::HIDMapperInterface^ loggerInterface) 
  : MapperJoystick(deviceNo, loggerInterface) 
{
  memset(_keyActivatedCount, 0, sizeof(_keyActivatedCount));
  _config = gcnew HIDMapperDLL::MapperJoystickToKeyboardConfig();
  _config->ConfigureForFortnite();
}

void MapperJoystickToKeyboard::Log(const DIJOYSTATE2& joyState)
{
  if (_loggerInterface->SuppressMapping)
  {
    MapperJoystick::Log(joyState);
    return;
  }

  int previousKeyActivatedCount[KEY_COUNT];
  memcpy(previousKeyActivatedCount, _keyActivatedCount, sizeof(_keyActivatedCount));


  // Check the buttons first
  int buttonCount = sizeof(joyState.rgbButtons);
  for (int buttonNo = 0; buttonNo < buttonCount; ++buttonNo) 
  {
    auto keySequence = _config->ButtonToKeySequence[buttonNo];
    if (keySequence != nullptr && keySequence->Length > 0)
    {
      // Just use the first key at the moment, key combos can come later
      auto key = keySequence[0];
      if (joyState.rgbButtons[buttonNo] > 0 && _lastState.rgbButtons[buttonNo] == 0)
      {
        // On
        _keyActivatedCount[key]++;
      }
      if (joyState.rgbButtons[buttonNo] == 0 && _lastState.rgbButtons[buttonNo] >0)
      {
        // Off
        _keyActivatedCount[key]--;
      }
    }
  }

  // Check each linear axis position
  for each(auto combo in _config->X)
  {
    CheckAxis(joyState.lX, _lastState.lX, combo.Threshold, combo.Key);
  }
  for each(auto combo in _config->Y)
  {
    CheckAxis(joyState.lY, _lastState.lY, combo.Threshold, combo.Key);
  }
  for each(auto combo in _config->Z)
  {
    CheckAxis(joyState.lZ, _lastState.lZ, combo.Threshold, combo.Key);
  }

  // Check each rotation axis position
  for each(auto combo in _config->RX)
  {
    CheckAxis(joyState.lRx, _lastState.lRx, combo.Threshold, combo.Key);
  }
  for each(auto combo in _config->RY)
  {
    CheckAxis(joyState.lRy, _lastState.lRy, combo.Threshold, combo.Key);
  }
  for each(auto combo in _config->RZ)
  {
    CheckAxis(joyState.lRz, _lastState.lRz, combo.Threshold, combo.Key);
  }


  for (int i = 0; i < KEY_COUNT; ++i)
  {
    if (previousKeyActivatedCount[i] == 0 && _keyActivatedCount[i] > 0)
    {
      MapperJoystickToKeyboard::KeyDown(i);
    }
    if (previousKeyActivatedCount[i] > 0 && _keyActivatedCount[i] == 0)
    {
      MapperJoystickToKeyboard::KeyUp(i);
    }
  }

  MapperJoystick::Log(joyState);
}

void MapperJoystickToKeyboard::CheckAxis(int current, int previous, int threshold, int key)
{
  if (threshold < 0)
  {
    if (current < threshold && previous >= threshold)
    {
      _keyActivatedCount[key]++;
    }
    if (current >= threshold && previous < threshold)
    {
      _keyActivatedCount[key]--;
    }
  }
  else if (threshold >= 0)
  {
    if (current > threshold && previous <= threshold)
    {
      _keyActivatedCount[key]++;
    }
    if (current <= threshold && previous > threshold)
    {
      _keyActivatedCount[key]--;
    }
  }
}


// For key codes
// https://www.codeproject.com/Articles/7305/Keyboard-Events-Simulation-using-keybd-event-funct
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646271(v=vs.85).aspx
// https://stackoverflow.com/questions/22419038/how-to-use-sendinput-function-c
// https://stackoverflow.com/questions/3644881/simulating-keyboard-with-sendinput-api-in-directinput-applications/3647975#3647975

void MapperJoystickToKeyboard::KeyDown(int scanCode)
{
  // Send keyboard state using the scan code
  INPUT inputData = { 0 };
  inputData.type = INPUT_KEYBOARD;
  inputData.ki.wScan = scanCode;
  inputData.ki.dwFlags = KEYEVENTF_SCANCODE;
  SendInput(1, &inputData, sizeof(INPUT));
}
void MapperJoystickToKeyboard::KeyUp(int scanCode)
{
  // Send keyboard state using the scan code
  INPUT inputData = { 0 };
  inputData.type = INPUT_KEYBOARD;
  inputData.ki.wScan = scanCode;
  inputData.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
  SendInput(1, &inputData, sizeof(INPUT));
}

