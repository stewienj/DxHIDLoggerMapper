#pragma once

#include "FileLogger.h"

#define JOY_COUNT 2

class FileLoggerJoystick : public FileLogger<DIJOYSTATE2>
{
public:
  FileLoggerJoystick(const TCHAR* filename, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
};

