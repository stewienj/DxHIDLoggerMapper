#pragma once

#include "FileLoggerJoystick.h"

#define JOY_COUNT 2

class MapperJoystickToKeyboard : public FileLoggerJoystick
{
private:
  // Key can be activated/deactivated more that once. Count up and down to work out when to toggle the key state (at zero).
  int _keyActivatedCount[256];
public:
  MapperJoystickToKeyboard(const TCHAR* filename, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
public:
  static void KeyDown(int scanCode);
  static void KeyUp(int scanCode);

};

