#include "FileLoggerMouse.h"
#include <tchar.h>
#include <strsafe.h>

FileLoggerMouse::FileLoggerMouse(const TCHAR* directory, HIDLogger::HIDLoggerInterface^ loggerInterface) : FileLogger(directory, TEXT("Mouse"), 0, loggerInterface) {
}

void FileLoggerMouse::Log(const DIMOUSESTATE2& mouseState) {
  TCHAR sz[32];

  LogBaseMovement(TEXT("X"), mouseState.lX, _lastState.lX);
  LogBaseMovement(TEXT("Y"), mouseState.lY, _lastState.lY);
  LogBaseMovement(TEXT("Z"), mouseState.lZ, _lastState.lZ);
  for (int buttonNo = 0; buttonNo < 8; ++buttonNo) {
    StringCchPrintf(sz, 32, TEXT("Button%d"), 1 + buttonNo);
    LogBaseButton(sz, mouseState.rgbButtons[buttonNo], _lastState.rgbButtons[buttonNo]);
  }
}
