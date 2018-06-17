#pragma once
using namespace System;
using namespace System::Threading::Tasks;
#include <map>
#include "HIDStateChangeArgs.h"
#include "LimitSingleInstance.h"

class MapperBase;

namespace HIDMapperDLL {
  public ref class HIDMapperInterface {
  private:
    bool _stopMonitoring;
    LPDIRECTINPUT8 _pDI;
    Task^ _deviceMonitoringTask;
    std::map<const GUID, MapperBase*>* _guidToLogger;
    int _joystickNo;
    bool _keyboardMonitoringEnabled;

    LPDIRECTINPUTDEVICE8 CreateJoystickDevice(GUID guid);
    LPDIRECTINPUTDEVICE8 CreateMouseDevice(GUID guid);
    LPDIRECTINPUTDEVICE8 CreateKeyboardDevice(GUID guid);

    void MonitorDevices();
    HRESULT InitDirectInput();
    void FreeDirectInput();
    void FireDeviceChanged(const TCHAR* deviceName, DeviceInfo::InfoType action);
    void FireStartedOrStopped(StartStopStatus::StartStopType startedOrStopped);

    static CLimitSingleInstance* g_SingleInstanceObj = new CLimitSingleInstance(TEXT("Global\\{E5CF13DB-AEB5-43F9-ADB3-773A4A675A82}"));
  public:
    HIDMapperInterface(void);
    !HIDMapperInterface(void);
    ~HIDMapperInterface(void);

    void CheckDeviceIsMonitored(const DIDEVICEINSTANCE* pdidInstance);
    void StartMonitoring();
    void StopMonitoring();
    void ResetKeyboardState();
    property bool KeyboardMonitoringEnabled
    {
      bool get() {
        return _keyboardMonitoringEnabled;
      }
      void set(bool keyboardLoggingEnabled) {
        _keyboardMonitoringEnabled = keyboardLoggingEnabled;
      }
    }

    void OnHIDStateChanged(TCHAR* device, TCHAR* control, long state, long previousState);
    const DIJOYSTATE2& GetJoystickState();
    static bool OtherInstanceRunning();
    void Error(TCHAR* deviceName, String^ message);

    event EventHandler<HIDStateChangeArgs^>^ HIDStateChanged;
    event EventHandler<DeviceInfo^>^ DeviceChanged;
    event EventHandler<StartStopStatus^>^ StartedOrStopped;

  };

}
