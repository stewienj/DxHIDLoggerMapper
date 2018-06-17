#include "MapperJoystick.h"
#include <tchar.h>
#include <strsafe.h>

MapperJoystick::MapperJoystick(int deviceNo, HIDMapperDLL::HIDMapperInterface^ loggerInterface) : HIDMapper(HIDMapperDLL::DeviceType::Joystick, deviceNo, loggerInterface) {
}

void MapperJoystick::Log(const DIJOYSTATE2& joyState)
{
  TCHAR sz[32];

  for (int povNo = 0; povNo < 4; povNo++) {
    StringCchPrintf(sz, 32, TEXT("POV%d"), 1 + povNo);
    NotifyStateChange(sz, joyState.rgdwPOV[povNo], _lastState.rgdwPOV[povNo]);
  }

  int buttonCount = sizeof(joyState.rgbButtons);
  for (int buttonNo = 0; buttonNo < buttonCount; ++buttonNo) {
    StringCchPrintf(sz, 32, TEXT("Button%d"), 1 + buttonNo);
    NotifyStateChange(sz, joyState.rgbButtons[buttonNo], _lastState.rgbButtons[buttonNo]);
  }

  NotifyStateChange(TEXT("X"), joyState.lX, _lastState.lX);
  NotifyStateChange(TEXT("Y"), joyState.lY, _lastState.lY);
  NotifyStateChange(TEXT("Z"), joyState.lZ, _lastState.lZ);

  NotifyStateChange(TEXT("Rx"), joyState.lRx, _lastState.lRx);
  NotifyStateChange(TEXT("Ry"), joyState.lRy, _lastState.lRy);
  NotifyStateChange(TEXT("Rz"), joyState.lRz, _lastState.lRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("Slider%d"), 1 + sliderNo);
    NotifyStateChange(sz, joyState.rglSlider[sliderNo], _lastState.rglSlider[sliderNo]);
  }

  NotifyStateChange(TEXT("VX"), joyState.lVX, _lastState.lVX);
  NotifyStateChange(TEXT("VY"), joyState.lVY, _lastState.lVY);
  NotifyStateChange(TEXT("VZ"), joyState.lVZ, _lastState.lVZ);

  NotifyStateChange(TEXT("VRx"), joyState.lVRx, _lastState.lVRx);
  NotifyStateChange(TEXT("VRy"), joyState.lVRy, _lastState.lVRy);
  NotifyStateChange(TEXT("VRz"), joyState.lVRz, _lastState.lVRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("VSlider%d"), 1 + sliderNo);
    NotifyStateChange(sz, joyState.rglVSlider[sliderNo], _lastState.rglVSlider[sliderNo]);
  }

  NotifyStateChange(TEXT("AX"), joyState.lAX, _lastState.lAX);
  NotifyStateChange(TEXT("AY"), joyState.lAY, _lastState.lAY);
  NotifyStateChange(TEXT("AZ"), joyState.lAZ, _lastState.lAZ);

  NotifyStateChange(TEXT("ARx"), joyState.lARx, _lastState.lARx);
  NotifyStateChange(TEXT("ARy"), joyState.lARy, _lastState.lARy);
  NotifyStateChange(TEXT("ARz"), joyState.lARz, _lastState.lARz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("ASlider%d"), 1 + sliderNo);
    NotifyStateChange(sz, joyState.rglASlider[sliderNo], _lastState.rglASlider[sliderNo]);
  }

  NotifyStateChange(TEXT("FX"), joyState.lFX, _lastState.lFX);
  NotifyStateChange(TEXT("FY"), joyState.lFY, _lastState.lFY);
  NotifyStateChange(TEXT("FZ"), joyState.lFZ, _lastState.lFZ);

  NotifyStateChange(TEXT("FRx"), joyState.lFRx, _lastState.lFRx);
  NotifyStateChange(TEXT("FRy"), joyState.lFRy, _lastState.lFRy);
  NotifyStateChange(TEXT("FRz"), joyState.lFRz, _lastState.lFRz);

  for (int sliderNo = 0; sliderNo < 2; sliderNo++) {
    StringCchPrintf(sz, 32, TEXT("FSlider%d"), 1 + sliderNo);
    NotifyStateChange(sz, joyState.rglFSlider[sliderNo], _lastState.rglFSlider[sliderNo]);
  }
}

