#pragma once
#ifndef STRICT
#define STRICT 1
#endif
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "vcclr.h"
#include "HIDStateChangeArgs.h"

namespace HIDMonitorDLL {
  ref class HIDMonitorInterface;
}

class MonitorBase
{
private:
  bool _canExit;
  bool _keepRunning;
  bool _deviceLost;
  HIDMonitorDLL::DeviceType _deviceType;
  GUID _deviceGuid;
  WCHAR _deviceName[MAX_PATH];
  LPDIRECTINPUTDEVICE8 _pDevice;

  // Create Thread requires a static function
  static DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    ((MonitorBase*)lpParameter)->StartLoggingThread();
    return 0;
  }
  void StartLoggingThread();
  // This must not be pure virtual as it can be called while in the destructor from the logging thread
  virtual HRESULT UpdateInputState() { return S_OK; }
protected:
  gcroot<HIDMonitorDLL::HIDMonitorInterface^> _loggerInterface;
  virtual HRESULT GetInputState(DWORD size, void* newState);
  void NotifyStateChangeButton(TCHAR* controlID, LONG state, LONG previousState);
  void NotifyStateChangeAxis(TCHAR* controlID, LONG state, LONG previousState);
public:
  MonitorBase(HIDMonitorDLL::DeviceType deviceType, GUID deviceGuid, HIDMonitorDLL::HIDMonitorInterface^ loggerInterface);
  virtual ~MonitorBase(void);
  void Start();
  void Stop();
  void SetDevice(LPDIRECTINPUTDEVICE8 device);
  const WCHAR* GetDeviceName() { return _deviceName; }
  const HIDMonitorDLL::DeviceType GetDeviceType() { return _deviceType; }
  const GUID GetDeviceGuid() { return _deviceGuid; }
  HRESULT UIThreadCheckIsValid();
};

template <typename T>
class HIDMonitor : public MonitorBase {
protected:
  T _lastState;
public:
	HIDMonitor(HIDMonitorDLL::DeviceType deviceType, GUID deviceGuid, HIDMonitorDLL::HIDMonitorInterface^ loggerInterface) : MonitorBase(deviceType, deviceGuid, loggerInterface) {
    _lastState = { 0 };
  }
  virtual void Log(const T& deviceState) = 0;
  HRESULT UpdateInputState() override {
    T deviceState = { 0 };
    HRESULT hr = GetInputState(sizeof(deviceState), &deviceState);
    // Check if it has changed since last time
    if (memcmp(&_lastState, &deviceState, sizeof(T)) != 0) {
      Log(deviceState);
      _lastState = deviceState;
    }
    return hr;
  }
  const T& GetLastState() {
    return _lastState;
  }
};



