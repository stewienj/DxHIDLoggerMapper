#pragma once
using namespace System;
using namespace System::Threading::Tasks;
#include <map>
#include "LoggedDataArgs.h"
#include "LimitSingleInstance.h"

class FileLoggerBase;

namespace HIDLogger {
  public ref class HIDLoggerInterface {
  private:
    bool _stopMonitoring;
    LPDIRECTINPUT8 _pDI;
    Task^ _deviceMonitoringTask;
    std::map<const GUID, FileLoggerBase*>* _guidToLogger;
    WCHAR* _directoryName;
    int _joystickNo;
    bool _keyboardLoggingEnabled;

    LPDIRECTINPUTDEVICE8 CreateJoystickDevice(GUID guid);
    LPDIRECTINPUTDEVICE8 CreateMouseDevice(GUID guid);
    LPDIRECTINPUTDEVICE8 CreateKeyboardDevice(GUID guid);

    void MonitorDevices();
    HRESULT InitDirectInput();
    void FreeDirectInput();
    void FireDeviceChanged(const TCHAR* deviceName, DeviceInfo::InfoType action);
    void FireStartedOrStopped(StartStopStatus::StartStopType startedOrStopped);

    static CLimitSingleInstance* g_SingleInstanceObj = new CLimitSingleInstance(TEXT("Global\\{E5CF13DB-AEB5-43F9-ADB3-773A4A675A81}"));
  public:
    HIDLoggerInterface(void);
    !HIDLoggerInterface(void);
    ~HIDLoggerInterface(void);

    void CheckDeviceIsMonitored(const DIDEVICEINSTANCE* pdidInstance);
    void StartLogging(String^ directoryName);
    void StartLogging(const WCHAR* directoryName);
    void StopLogging();
    void ResetKeyboardState();
    property bool KeyboardLoggingEnabled
    {
      bool get() {
        return _keyboardLoggingEnabled;
      }
      void set(bool keyboardLoggingEnabled) {
        _keyboardLoggingEnabled = keyboardLoggingEnabled;
      }
    }

    void LoggedInfo(TCHAR* machine, TCHAR* device, TCHAR* control, long state, long previousState);
    const DIJOYSTATE2& GetJoystickState();
    static bool OtherInstanceRunning();
    void Error(TCHAR* deviceName, String^ message);

    event EventHandler<LoggedDataArgs^>^ InfoLogged;
    event EventHandler<DeviceInfo^>^ DeviceChanged;
    event EventHandler<StartStopStatus^>^ StartedOrStopped;

  };

}

