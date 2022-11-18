#pragma once
using namespace System;
using namespace System::Threading::Tasks;
#include <map>
#include "HIDStateChangeArgs.h"
#include "LimitSingleInstance.h"

class MonitorBase;

namespace HIDMonitorDLL {
  ref class MonitorJoystickToKeyboardConfig;

  public ref class HIDMonitorInterface {
  private:
    bool _stopMonitoring;
    LPDIRECTINPUT8 _pDI;
    Task^ _deviceMonitoringTask;
    std::map<const GUID, MonitorBase*>* _guidToMonitor;
    bool _keyboardMonitoringEnabled;
    bool _mouseMonitoringEnabled;
    bool _joystickMonitoringEnabled;

    LPDIRECTINPUTDEVICE8 CreateJoystickDevice(GUID guid);
    LPDIRECTINPUTDEVICE8 CreateMouseDevice(GUID guid);
    LPDIRECTINPUTDEVICE8 CreateKeyboardDevice(GUID guid);

    void MonitorDevices();
    HRESULT InitDirectInput();
    void FreeDirectInput();
    void FireDeviceChanged(const TCHAR* deviceName, HIDMonitorDLL::DeviceType deviceType, GUID deviceGuid, DeviceInfo::InfoType action);
    void FireStartedOrStopped(StartStopStatus::StartStopType startedOrStopped);

    static CLimitSingleInstance* g_SingleInstanceObj = new CLimitSingleInstance(TEXT("Global\\{E5CF13DB-AEB5-43F9-ADB3-773A4A675A82}"));
  public:
    HIDMonitorInterface(void);
    !HIDMonitorInterface(void);
    ~HIDMonitorInterface(void);

    void CheckDeviceIsMonitored(const DIDEVICEINSTANCE* pdidInstance);
    void StartMonitoring();
    void StopMonitoring();
    void ResetKeyboardState();

    property bool KeyboardMonitoringEnabled
    {
      bool get() {
        return _keyboardMonitoringEnabled;
      }
      void set(bool keyboardMonitoringEnabled) {
        _keyboardMonitoringEnabled = keyboardMonitoringEnabled;
      }
    }
    property bool MouseMonitoringEnabled
    {
      bool get()
      {
        return _mouseMonitoringEnabled;
      }
      void set(bool mouseMonitoringEnabled)
      {
        _mouseMonitoringEnabled = mouseMonitoringEnabled;
      }
    }
    property bool JoystickMonitoringEnabled
    {
      bool get()
      {
        return _joystickMonitoringEnabled;
      }
      void set(bool joystickMonitoringEnabled)
      {
        _joystickMonitoringEnabled = joystickMonitoringEnabled;
      }
    }

    void OnHIDStateChanged(TCHAR* device, DeviceType deviceType, GUID deviceGuid, TCHAR* control, long state, long previousState);
    const DIJOYSTATE2& GetJoystickState();
    static bool OtherInstanceRunning();
    void Error(TCHAR* deviceName, HIDMonitorDLL::DeviceType deviceType, GUID deviceGuid, String^ message);

    event EventHandler<HIDStateChangeArgs^>^ HIDStateChanged;
    event EventHandler<DeviceInfo^>^ DeviceChanged;
    event EventHandler<StartStopStatus^>^ StartedOrStopped;

  };

}

