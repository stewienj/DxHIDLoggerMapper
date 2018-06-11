#include "MapperMouse.h"
#include <tchar.h>
#include <strsafe.h>

MapperMouse::MapperMouse(HIDMapperDLL::HIDMapperInterface^ loggerInterface) : HIDMapper(TEXT("Mouse"), 0, loggerInterface) {
}

void MapperMouse::Log(const DIMOUSESTATE2& mouseState) {
  TCHAR sz[32];

  NotifyStateChange(TEXT("X"), mouseState.lX, _lastState.lX);
  NotifyStateChange(TEXT("Y"), mouseState.lY, _lastState.lY);
  NotifyStateChange(TEXT("Z"), mouseState.lZ, _lastState.lZ);
  for (int buttonNo = 0; buttonNo < 8; ++buttonNo) {
    StringCchPrintf(sz, 32, TEXT("Button%d"), 1 + buttonNo);
    NotifyStateChange(sz, mouseState.rgbButtons[buttonNo], _lastState.rgbButtons[buttonNo]);
  }
}
