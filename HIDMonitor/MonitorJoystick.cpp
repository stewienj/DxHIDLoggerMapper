#include "StdAfx.h"
#include "MonitorJoystick.h"

MonitorJoystick::MonitorJoystick(GUID deviceGuid, HIDMonitorDLL::HIDMonitorInterface^ loggerInterface) : HIDMonitor(HIDMonitorDLL::DeviceType::Joystick, deviceGuid, loggerInterface) {
}

void MonitorJoystick::Log(const DIJOYSTATE2& joyState)
{
  TCHAR sz[32];

  for (int povNo = 0; povNo < 4; povNo++) {
    StringCchPrintf(sz, 32, TEXT("POV%d"), 1 + povNo);
    NotifyStateChangeButton(sz, joyState.rgdwPOV[povNo], _lastState.rgdwPOV[povNo]);
  }

  int buttonCount = sizeof(joyState.rgbButtons);
  for (int buttonNo = 0; buttonNo < buttonCount; ++buttonNo) {
    StringCchPrintf(sz, 32, TEXT("Button%d"), 1 + buttonNo);
    NotifyStateChangeButton(sz, joyState.rgbButtons[buttonNo], _lastState.rgbButtons[buttonNo]);
  }

  NotifyStateChangeAxis(TEXT("X"), joyState.lX, _lastState.lX);
  NotifyStateChangeAxis(TEXT("Y"), joyState.lY, _lastState.lY);
  NotifyStateChangeAxis(TEXT("Z"), joyState.lZ, _lastState.lZ);

  NotifyStateChangeAxis(TEXT("Rx"), joyState.lRx, _lastState.lRx);
  NotifyStateChangeAxis(TEXT("Ry"), joyState.lRy, _lastState.lRy);
  NotifyStateChangeAxis(TEXT("Rz"), joyState.lRz, _lastState.lRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("Slider%d"), 1 + sliderNo);
    NotifyStateChangeAxis(sz, joyState.rglSlider[sliderNo], _lastState.rglSlider[sliderNo]);
  }

  NotifyStateChangeAxis(TEXT("VX"), joyState.lVX, _lastState.lVX);
  NotifyStateChangeAxis(TEXT("VY"), joyState.lVY, _lastState.lVY);
  NotifyStateChangeAxis(TEXT("VZ"), joyState.lVZ, _lastState.lVZ);

  NotifyStateChangeAxis(TEXT("VRx"), joyState.lVRx, _lastState.lVRx);
  NotifyStateChangeAxis(TEXT("VRy"), joyState.lVRy, _lastState.lVRy);
  NotifyStateChangeAxis(TEXT("VRz"), joyState.lVRz, _lastState.lVRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("VSlider%d"), 1 + sliderNo);
    NotifyStateChangeAxis(sz, joyState.rglVSlider[sliderNo], _lastState.rglVSlider[sliderNo]);
  }

  NotifyStateChangeAxis(TEXT("AX"), joyState.lAX, _lastState.lAX);
  NotifyStateChangeAxis(TEXT("AY"), joyState.lAY, _lastState.lAY);
  NotifyStateChangeAxis(TEXT("AZ"), joyState.lAZ, _lastState.lAZ);

  NotifyStateChangeAxis(TEXT("ARx"), joyState.lARx, _lastState.lARx);
  NotifyStateChangeAxis(TEXT("ARy"), joyState.lARy, _lastState.lARy);
  NotifyStateChangeAxis(TEXT("ARz"), joyState.lARz, _lastState.lARz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("ASlider%d"), 1 + sliderNo);
    NotifyStateChangeAxis(sz, joyState.rglASlider[sliderNo], _lastState.rglASlider[sliderNo]);
  }

  NotifyStateChangeAxis(TEXT("FX"), joyState.lFX, _lastState.lFX);
  NotifyStateChangeAxis(TEXT("FY"), joyState.lFY, _lastState.lFY);
  NotifyStateChangeAxis(TEXT("FZ"), joyState.lFZ, _lastState.lFZ);

  NotifyStateChangeAxis(TEXT("FRx"), joyState.lFRx, _lastState.lFRx);
  NotifyStateChangeAxis(TEXT("FRy"), joyState.lFRy, _lastState.lFRy);
  NotifyStateChangeAxis(TEXT("FRz"), joyState.lFRz, _lastState.lFRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("FSlider%d"), 1 + sliderNo);
    NotifyStateChangeAxis(sz, joyState.rglFSlider[sliderNo], _lastState.rglFSlider[sliderNo]);
  }
}

