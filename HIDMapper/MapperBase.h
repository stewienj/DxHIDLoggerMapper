#pragma once
#ifndef STRICT
#define STRICT 1
#endif
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "vcclr.h"
#include "HIDStateChangeArgs.h"

namespace HIDMapperDLL {
  ref class HIDMapperInterface;
}

class MapperBase
{
private:
  bool _canExit;
  bool _keepRunning;
  bool _deviceLost;
  HIDMapperDLL::DeviceType _deviceType;
  int _deviceNo;
  WCHAR _deviceName[MAX_PATH];
  LPDIRECTINPUTDEVICE8 _pDevice;

  // Create Thread requires a static function
  static DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    ((MapperBase*)lpParameter)->StartLoggingThread();
    return 0;
  }
  void StartLoggingThread();
  // This must not be pure virtual as it can be called while in the destructor from the logging thread
  virtual HRESULT UpdateInputState() { return S_OK; }
protected:
  gcroot<HIDMapperDLL::HIDMapperInterface^> _loggerInterface;
  virtual HRESULT GetInputState(DWORD size, void* newState);
  void NotifyStateChangeButton(TCHAR* controlID, LONG state, LONG previousState);
  void NotifyStateChangeAxis(TCHAR* controlID, LONG state, LONG previousState);
public:
  MapperBase(HIDMapperDLL::DeviceType deviceType, int deviceNo, HIDMapperDLL::HIDMapperInterface^ loggerInterface);
  virtual ~MapperBase(void);
  void Start();
  void Stop();
  void SetDevice(LPDIRECTINPUTDEVICE8 device);
  const WCHAR* GetDeviceName() { return _deviceName; }
  const HIDMapperDLL::DeviceType GetDeviceType() { return _deviceType; }
  HRESULT UIThreadCheckIsValid();
};

template <typename T>
class HIDMapper : public MapperBase {
protected:
  T _lastState;
public:
	HIDMapper(HIDMapperDLL::DeviceType deviceType, int deviceNo, HIDMapperDLL::HIDMapperInterface^ loggerInterface) : MapperBase(deviceType, deviceNo, loggerInterface) {
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



