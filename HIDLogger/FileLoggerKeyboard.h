#pragma once
#include "FileLogger.h"

struct DIKBSTATE {
  char keys[256];
};

class FileLoggerKeyboard : public FileLogger<DIKBSTATE>
{

public:
  FileLoggerKeyboard(const TCHAR* directory, HIDLogger::HIDLoggerInterface^ loggerInterface);
  void Log(const DIKBSTATE& kbState) override;
  HRESULT GetInputState(DWORD size, void* newState) override;
};

