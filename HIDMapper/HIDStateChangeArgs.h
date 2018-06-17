#pragma once

#include <tchar.h>
using namespace System;

namespace HIDMapperDLL {

  public ref class HIDStateChangeArgs : public EventArgs {
  public:
    HIDStateChangeArgs(TCHAR* device, TCHAR* control, int state, int previousState);
    String^ Device;
    String^ Control;
    int State;
    int PreviousState;
  };

  public ref class DeviceInfo : public EventArgs {
  public:
    enum class InfoType {Added, Removed, Checked, Updated, Error};
    DeviceInfo(const TCHAR* device, DeviceInfo::InfoType infoAction);
    ~DeviceInfo();
    InfoType InfoAction;
    String^ Device;
  };

  public ref class DeviceInfoError : public DeviceInfo {
  public:
    DeviceInfoError(const TCHAR* device, String^ message);
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

