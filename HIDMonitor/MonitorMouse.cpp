#include "StdAfx.h"
#include "MonitorMouse.h"

MonitorMouse::MonitorMouse(GUID deviceGuid, HIDMonitorDLL::HIDMonitorInterface^ loggerInterface) : HIDMonitor(HIDMonitorDLL::DeviceType::Mouse, deviceGuid, loggerInterface) {
}

void MonitorMouse::Log(const DIMOUSESTATE2& mouseState) {
  TCHAR sz[32];

  NotifyStateChangeAxis(TEXT("X"), mouseState.lX, _lastState.lX);
  NotifyStateChangeAxis(TEXT("Y"), mouseState.lY, _lastState.lY);
  NotifyStateChangeAxis(TEXT("Z"), mouseState.lZ, _lastState.lZ);
  for (int buttonNo = 0; buttonNo < 8; ++buttonNo) {
    StringCchPrintf(sz, 32, TEXT("Button%d"), 1 + buttonNo);
    NotifyStateChangeButton(sz, mouseState.rgbButtons[buttonNo], _lastState.rgbButtons[buttonNo]);
  }
}
