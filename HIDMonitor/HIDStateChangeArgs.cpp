#include "StdAfx.h"
#include "HIDStateChangeArgs.h"

namespace HIDMonitorDLL {
  HIDStateChangeArgs::HIDStateChangeArgs(TCHAR* device, HIDMonitorDLL::DeviceType deviceType, GUID deviceGuid, TCHAR* control, int state, int previousState) {
    Device = gcnew String(device);
    Control = gcnew String(control);
    DeviceGuid = Guid(deviceGuid.Data1, deviceGuid.Data2, deviceGuid.Data3,
      deviceGuid.Data4[0], deviceGuid.Data4[1],
      deviceGuid.Data4[2], deviceGuid.Data4[3],
      deviceGuid.Data4[4], deviceGuid.Data4[5],
      deviceGuid.Data4[6], deviceGuid.Data4[7]);
    State = state;
    PreviousState = previousState;
  }

  DeviceInfo::DeviceInfo(const TCHAR* device, HIDMonitorDLL::DeviceType deviceType, GUID deviceGuid, DeviceInfo::InfoType infoAction) {
    Device = gcnew String(device);
    DeviceType = deviceType;
    DeviceGuid = Guid(deviceGuid.Data1, deviceGuid.Data2, deviceGuid.Data3,
      deviceGuid.Data4[0], deviceGuid.Data4[1],
      deviceGuid.Data4[2], deviceGuid.Data4[3],
      deviceGuid.Data4[4], deviceGuid.Data4[5],
      deviceGuid.Data4[6], deviceGuid.Data4[7]);
    InfoAction = infoAction;
  }
  DeviceInfo::~DeviceInfo() {
  }

  DeviceInfoError::DeviceInfoError(const TCHAR* device, HIDMonitorDLL::DeviceType deviceType, GUID deviceGuid, String^ message) : DeviceInfo(device, deviceType, deviceGuid, DeviceInfo::InfoType::Error) {
    ErrorMessage = message;
  }
  DeviceInfoError::~DeviceInfoError() {
  }

}
