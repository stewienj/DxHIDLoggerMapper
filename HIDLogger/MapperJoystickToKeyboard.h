#pragma once

#include "FileLoggerJoystick.h"
//#include "MapperJoystickToKeyboardConfig.h"

namespace HIDLogger
{
  ref class MapperJoystickToKeyboardConfig;
}

#define KEY_COUNT 256

class MapperJoystickToKeyboard : public FileLoggerJoystick
{
private:
  // Key can be activated/deactivated more that once. Count up and down to work out when to toggle the key state (at zero).
  int _keyActivatedCount[KEY_COUNT];
  gcroot<HIDLogger::MapperJoystickToKeyboardConfig^> _config;
public:
  MapperJoystickToKeyboard(const TCHAR* filename, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
public:
  static void KeyDown(int scanCode);
  static void KeyUp(int scanCode);

};

