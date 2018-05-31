#pragma once

#include "FileLogger.h"

class FileLoggerJoystick : public FileLogger<DIJOYSTATE2>
{
public:
  FileLoggerJoystick(const TCHAR* filename, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
};

