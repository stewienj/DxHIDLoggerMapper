#pragma once

#include "FileLogger.h"

#define JOY_COUNT 2

class MapperJoystickToKeyboard : public FileLogger<DIJOYSTATE2>
{
public:
  MapperJoystickToKeyboard(int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
public:
  static void KeyDown(int scanCode);
  static void KeyUp(int scanCode);

};

