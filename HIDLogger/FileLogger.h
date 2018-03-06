#pragma once
#ifndef STRICT
#define STRICT 1
#endif
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "textfile.h"
#include "vcclr.h"

namespace HIDLogger {
  ref class HIDLoggerInterface;
}

class FileLoggerBase
{
private:
  bool _canExit;
  bool _keepRunning;
  TextFile* _outputMovement;
  TextFile* _outputButton;
  TCHAR _directory[1024];
  TCHAR _machineName[128];
  TCHAR _deviceType[128];
  WCHAR _deviceName[MAX_PATH];
  LPDIRECTINPUTDEVICE8 _pDevice;
  bool _deviceLost;

  // Create Thread requires a static function
  static DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    ((FileLoggerBase*)lpParameter)->StartLoggingThread();
    return 0;
  }
  void StartLoggingThread();
  TextFile* CreateFileWithHeader(TCHAR* prefix);
  // This must not be pure virtual as it can be called while in the destructor from the logging thread
  virtual HRESULT UpdateInputState() { return S_OK; }
  void LogBase(TCHAR* controlID, LONG state, LONG previousState, TextFile* output);
protected:
  gcroot<HIDLogger::HIDLoggerInterface^> _loggerInterface;

  void LogBaseMovement(TCHAR* controlID, LONG state, LONG previousState);
  void LogBaseButton(TCHAR* controlID, LONG state, LONG previousState);
  virtual HRESULT GetInputState(DWORD size, void* newState);
public:
  FileLoggerBase(const TCHAR* directory, const TCHAR* suffix, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface);
  virtual ~FileLoggerBase(void);
  void Start();
  void Stop();
  void SetDevice(LPDIRECTINPUTDEVICE8 device);
  const WCHAR* GetDeviceName() { return _deviceName; }
  HRESULT UIThreadCheckIsValid();
};

template <typename T>
class FileLogger : public FileLoggerBase {
protected:
  T _lastState;
public:
  FileLogger(const TCHAR* directory, const TCHAR* suffix, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface) : FileLoggerBase(directory, suffix, deviceNo, loggerInterface) {
    memset(&_lastState, 0, sizeof(T));
  }
  virtual void Log(const T& deviceState) = 0;
  HRESULT UpdateInputState() override {
    T deviceState;
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



