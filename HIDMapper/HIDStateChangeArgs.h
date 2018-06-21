#pragma once

#include <windows.h> 
#include <tchar.h>
using namespace System;

namespace HIDMapperDLL {

  public enum class DeviceType
  {
    Unknown,
    Keyboard,
    Joystick,
    Mouse
  };

  public ref class HIDStateChangeArgs : public EventArgs {
  public:
    HIDStateChangeArgs(TCHAR* device, DeviceType deviceType, GUID deviceGuid, TCHAR* control, int state, int previousState);
    String^ Device;
    String^ Control;
    DeviceType DeviceType;
    Guid DeviceGuid;
    int State;
    int PreviousState;

  };

  public ref class DeviceInfo : public EventArgs {
  public:
    enum class InfoType {Added, Removed, Checked, Updated, Error};
    DeviceInfo(const TCHAR* device, DeviceType deviceType, GUID deviceGuid, DeviceInfo::InfoType infoAction);
    ~DeviceInfo();
    InfoType InfoAction;
    String^ Device;
    DeviceType DeviceType;
    Guid DeviceGuid;
  };

  public ref class DeviceInfoError : public DeviceInfo {
  public:
    DeviceInfoError(const TCHAR* device, HIDMapperDLL::DeviceType deviceType, GUID deviceGuid, String^ message);
    ~DeviceInfoError();
    String^ ErrorMessage;
  };

  public ref class StartStopStatus : public EventArgs {
  public:
    enum class StartStopType {Started, Stopped};
    StartStopStatus(StartStopType startedOrStopped) {
      StartedOrStopped = startedOrStopped;
    }
    StartStopType StartedOrStopped;
  };
}

