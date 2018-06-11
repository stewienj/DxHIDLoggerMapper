#pragma once

#include "MapperJoystick.h"
//#include "MapperJoystickToKeyboardConfig.h"

namespace HIDMapperDLL
{
  ref class MapperJoystickToKeyboardConfig;
}

#define KEY_COUNT 256

class MapperJoystickToKeyboard : public MapperJoystick
{
private:
  // Key can be activated/deactivated more that once. Count up and down to work out when to toggle the key state (at zero).
  int _keyActivatedCount[KEY_COUNT];
  gcroot<HIDMapperDLL::MapperJoystickToKeyboardConfig^> _config;
public:
  MapperJoystickToKeyboard(int deviceNo, HIDMapperDLL::HIDMapperInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
protected:
  void CheckAxis(int current, int previous, int threshold, int key);
  static void KeyDown(int scanCode);
  static void KeyUp(int scanCode);
};

