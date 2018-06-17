#include "HIDStateChangeArgs.h"

namespace HIDMapperDLL {
  HIDStateChangeArgs::HIDStateChangeArgs(TCHAR* device, HIDMapperDLL::DeviceType deviceType, TCHAR* control, int state, int previousState) {
    Device = gcnew String(device);
    Control = gcnew String(control);
    State = state;
    PreviousState = previousState;
  }

  DeviceInfo::DeviceInfo(const TCHAR* device, HIDMapperDLL::DeviceType deviceType, DeviceInfo::InfoType infoAction) {
    Device = gcnew String(device);
    DeviceType = deviceType;
    InfoAction = infoAction;
  }
  DeviceInfo::~DeviceInfo() {
  }

  DeviceInfoError::DeviceInfoError(const TCHAR* device, HIDMapperDLL::DeviceType deviceType, String^ message) : DeviceInfo(device, deviceType, DeviceInfo::InfoType::Error) {
    ErrorMessage = message;
  }
  DeviceInfoError::~DeviceInfoError() {
  }

}