#include "HIDStateChangeArgs.h"

namespace HIDMapperDLL {
  HIDStateChangeArgs::HIDStateChangeArgs(TCHAR* device, TCHAR* control, int state, int previousState) {
    Device = gcnew String(device);
    Control = gcnew String(control);
    State = state;
    PreviousState = previousState;
  }

  DeviceInfo::DeviceInfo(const TCHAR* device, DeviceInfo::InfoType infoAction) {
    Device = gcnew String(device);
    InfoAction = infoAction;
  }
  DeviceInfo::~DeviceInfo() {
  }

  DeviceInfoError::DeviceInfoError(const TCHAR* device, String^ message) : DeviceInfo(device, DeviceInfo::InfoType::Error) {
    ErrorMessage = message;
  }
  DeviceInfoError::~DeviceInfoError() {
  }

}