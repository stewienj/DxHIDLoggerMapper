#pragma once
#include "FileLogger.h"

class FileLoggerMouse : public FileLogger<DIMOUSESTATE2>
{

public:
  FileLoggerMouse(const TCHAR* directory, HIDLogger::HIDLoggerInterface^ loggerInterface);
  void Log(const DIMOUSESTATE2& mouseState) override;
};

