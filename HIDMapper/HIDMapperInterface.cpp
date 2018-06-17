#include "MapperJoystickToKeyboard.h"
#include "MapperJoystick.h"
#include "MapperMouse.h"
#include "MapperKeyboard.h"
#include "HIDMapperInterface.h"
#include "LimitSingleInstance.h"
#include <vector>
#include <gcroot.h>
#include <tchar.h>
#include <strsafe.h>
using namespace System::Threading::Tasks;

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// The one and only CLimitSingleInstance object.
// Change what is passed to constructor. GUIDGEN Tool may be of help.
//CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{E5CF13DB-AEB5-43F9-ADB3-773A4A675A81}"));

inline bool operator<(const GUID & lhs, const GUID & rhs) {
  return memcmp(&lhs, &rhs, sizeof(GUID)) < 0;
}

namespace HIDMapperDLL {
  HIDMapperInterface::HIDMapperInterface(void) {
    _guidToLogger = new std::map<const GUID, MapperBase*>();
    _joystickNo = 0;
    _keyboardMonitoringEnabled = true;
  }

  // Finalizer cleans up unmanaged resources.
  // Destructor or garbage collector will
  // clean up managed resources
  HIDMapperInterface::!HIDMapperInterface(void) {
    delete _guidToLogger;
  }

  // Destructor cleans up all resources, called explicily, suppresses finalizer
  HIDMapperInterface::~HIDMapperInterface(void) {
    this->!HIDMapperInterface();
  }

  bool HIDMapperInterface::OtherInstanceRunning() {
    return g_SingleInstanceObj->IsAnotherInstanceRunning() != 0;
  }

  void HIDMapperInterface::ResetKeyboardState() {
    for (int keyNo = 0; keyNo < 256; ++keyNo) {
      keybd_event(0, keyNo, 0x0002, 0);
    }
  }

  //-----------------------------------------------------------------------------
  // Name: InitDirectInput()
  // Desc: Initialize the DirectInput variables.
  //-----------------------------------------------------------------------------
  HRESULT HIDMapperInterface::InitDirectInput() {
    HWND hDlg = NULL;
    HRESULT hr;
    LPDIRECTINPUT8 pDI;

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
    if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
      IID_IDirectInput8, (VOID**)&pDI, NULL))) {
      _pDI = NULL;
      return hr;
    } else {
      _pDI = pDI;
      return S_OK;
    }
  }

  //-----------------------------------------------------------------------------
  // Name: EnumObjectsCallback()
  // Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
  //       Joystick. This function enables user interface elements for objects
  //       that are found to exist, and scales axes min/max values.
  //-----------------------------------------------------------------------------
  BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if (pdidoi->dwType & DIDFT_AXIS) {
      DIPROPRANGE diprg;
      diprg.diph.dwSize = sizeof(DIPROPRANGE);
      diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
      diprg.diph.dwHow = DIPH_BYID;
      diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
      diprg.lMin = -1000;
      diprg.lMax = +1000;

      // Set the range for the axis
      if (FAILED(((LPDIRECTINPUTDEVICE8)pContext)->SetProperty(DIPROP_RANGE, &diprg.diph)))
        return DIENUM_STOP;
    }
    return DIENUM_CONTINUE;
  }

  LPDIRECTINPUTDEVICE8 HIDMapperInterface::CreateJoystickDevice(GUID guid) {
    LPDIRECTINPUTDEVICE8 pJoystick;
    HRESULT hr = S_OK;

    hr = _pDI->CreateDevice(guid, &pJoystick, NULL);

    // Set the data format to "simple Joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    hr = pJoystick->SetDataFormat(&c_dfDIJoystick2);

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    hr = pJoystick->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    // Enumerate the Joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    hr = pJoystick->EnumObjects(EnumObjectsCallback, (VOID*)pJoystick, DIDFT_ALL);

    return pJoystick;
  }

  LPDIRECTINPUTDEVICE8 HIDMapperInterface::CreateKeyboardDevice(GUID guid) {
    HRESULT hr = S_OK;
    LPDIRECTINPUTDEVICE8 pKeyboard = NULL;
  
    // Retrieve the system mouse
    if (FAILED(_pDI->CreateDevice(guid, &pKeyboard, NULL))) {
      return NULL;
    }

  
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a MouseState structure to IDirectInputDevice::GetDeviceState().
    if (FAILED(hr = pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
      return NULL;
  
    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if (FAILED(hr = pKeyboard->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
      return NULL;
  
    return pKeyboard;
  }

  LPDIRECTINPUTDEVICE8 HIDMapperInterface::CreateMouseDevice(GUID guid) {
    HRESULT hr = S_OK;
    LPDIRECTINPUTDEVICE8 pMouse;

    // Retrieve the system mouse
    if (FAILED(_pDI->CreateDevice(guid, &pMouse, NULL))) {
      return NULL;
    }

    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a MouseState structure to IDirectInputDevice::GetDeviceState().
    if (FAILED(hr = pMouse->SetDataFormat(&c_dfDIMouse2)))
      return NULL;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if (FAILED(hr = pMouse->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
      return NULL;

    return pMouse;

  }

  void HIDMapperInterface::CheckDeviceIsMonitored(const DIDEVICEINSTANCE* pdidInstance) {

    auto foundLogger = _guidToLogger->find(pdidInstance->guidInstance);

    if (foundLogger != _guidToLogger->end()) {

      FireDeviceChanged((*foundLogger).second->GetDeviceName(), (*foundLogger).second->GetDeviceType(), DeviceInfo::InfoType::Checked);

    } else {

      // Create a new device and add a logger for it
      LPDIRECTINPUTDEVICE8 pDevice = NULL;
	  MapperBase* logger = NULL;

      switch (pdidInstance->dwDevType & 0xFF) {
      case DI8DEVTYPE_MOUSE:
        if (pdidInstance->guidInstance == GUID_SysMouse) {
          if ((pDevice = CreateMouseDevice(pdidInstance->guidInstance)) != NULL) {
            logger = new MapperMouse(this);
          }
        }
        break;
      case DI8DEVTYPE_KEYBOARD:
        if (pdidInstance->guidInstance == GUID_SysKeyboard && _keyboardMonitoringEnabled) {
          if ((pDevice = CreateKeyboardDevice(pdidInstance->guidInstance)) != NULL) {
            logger = new MapperKeyboard(this);
          }
        }
        break;
      case DI8DEVTYPE_JOYSTICK:
      case DI8DEVTYPE_GAMEPAD:
      case DI8DEVTYPE_DRIVING:
      case DI8DEVTYPE_FLIGHT:
      case DI8DEVTYPE_1STPERSON:
        _joystickNo++;
        if ((pDevice = CreateJoystickDevice(pdidInstance->guidInstance)) != NULL) {
          logger = new MapperJoystickToKeyboard(_joystickNo, this);
        }
        break;
      }

      // Check that logger was created and if so then add it to the map, fire off
      // an event signalling a new logger was create, and start the logger.
      if (logger != NULL) {
        logger->SetDevice(pDevice);
        (*_guidToLogger)[pdidInstance->guidInstance] = logger;
        FireDeviceChanged(logger->GetDeviceName(), logger->GetDeviceType(), DeviceInfo::InfoType::Added);
        logger->Start();
      }
    }
  }

  //-----------------------------------------------------------------------------
  // Name: EnumJoysticksCallback()
  // Desc: Called once for each enumerated Joystick. If we find one, create a
  //       device interface on it so we can play with it.
  //-----------------------------------------------------------------------------
  BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
    
    HIDMapperInterface^ hid = ((gcroot<HIDMapperInterface^>*)pContext)->operator HIDMapperDLL::HIDMapperInterface^();
    hid->CheckDeviceIsMonitored(pdidInstance);
    return DIENUM_CONTINUE;
  }

  // This monitors the devices to ensure that they
  // remain acquired.
  void HIDMapperInterface::MonitorDevices() {

    // This should be keeping a list of all the devices attached.
    // Should enumerate all devices each pass, if any new ones then add them
    // in and should be removing old devices.
    // Have a dictionary of devices by guidInstance (but not for keyboard or mouse, use system GUIDs for these)
    try {
      _stopMonitoring = false;
      while (!_stopMonitoring) {
        Sleep(200);

        // Remove logger for any removed devices
        std::vector<std::pair<const GUID, MapperBase*>> loggerToRemove;
        for (std::pair<const GUID, MapperBase*> logger : *_guidToLogger) {
          if (logger.second->UIThreadCheckIsValid() != S_OK) {
            loggerToRemove.push_back(logger);
          }
        }
        for (std::pair<const GUID, MapperBase*> logger : loggerToRemove) {
          _guidToLogger->erase(logger.first);
          logger.second->Stop();
          FireDeviceChanged(logger.second->GetDeviceName(), logger.second->GetDeviceType(), DeviceInfo::InfoType::Removed);
          delete logger.second;
        }

        gcroot<HIDMapperInterface^>* pointerToThis = new gcroot<HIDMapperInterface^>(this);
        // Look for devices on this machine
        // Had a problem with the keyboard getting stuck, so only poll for mice and game controllers
        _pDI->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCallback, (LPVOID)pointerToThis, DIEDFL_ATTACHEDONLY);
        //_pDI->EnumDevices(DI8DEVCLASS_POINTER, EnumDevicesCallback, (LPVOID)pointerToThis, DIEDFL_ATTACHEDONLY);
        //_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, (LPVOID)pointerToThis, DIEDFL_ATTACHEDONLY);
        delete pointerToThis;
      }
    } catch (...) {

    }
    FireStartedOrStopped(StartStopStatus::StartStopType::Stopped);
  }

  // Call this from the GUI Thread
  void HIDMapperInterface::StartMonitoring() {
    StopMonitoring();

	// Initialize direct input
    if (FAILED(InitDirectInput())) {
      return;
    }
    FireStartedOrStopped(StartStopStatus::StartStopType::Started);
    // Start a hardware monitoring task that looks for new joysticks, keyboards, and mice
    _deviceMonitoringTask = Task::Factory->StartNew(gcnew Action(this, &HIDMapperInterface::MonitorDevices), TaskCreationOptions::LongRunning);
  }

  static DIJOYSTATE2 g_nothingState;
  const DIJOYSTATE2& HIDMapperInterface::GetJoystickState() {
    for (std::pair<const GUID, MapperBase*> logger : *_guidToLogger) {
      MapperJoystick* joyLogger = dynamic_cast<MapperJoystick*>(logger.second);
      if (joyLogger != NULL) {
        return joyLogger->GetLastState();
      }
    }
    return g_nothingState;
  }

  void HIDMapperInterface::FireStartedOrStopped(StartStopStatus::StartStopType startedOrStopped) {
    try {
      StartedOrStopped(this, gcnew StartStopStatus(startedOrStopped));
    } catch (...) {
    }
  }

  void HIDMapperInterface::FireDeviceChanged(const TCHAR* deviceName, HIDMapperDLL::DeviceType deviceType, DeviceInfo::InfoType action) {
    try {
      DeviceChanged(this, gcnew DeviceInfo(deviceName, deviceType, action));
    } catch (...) {

    }
  }

  void HIDMapperInterface::Error(TCHAR* deviceName, HIDMapperDLL::DeviceType deviceType, String^ message) {
    try {
      DeviceChanged(this, gcnew DeviceInfoError(deviceName, deviceType, message));
    } catch (...) {

    }
  }

  void HIDMapperInterface::OnHIDStateChanged(TCHAR* device, DeviceType deviceType, TCHAR* control, long state, long previousState) {
    try {
		HIDStateChanged(this, gcnew HIDStateChangeArgs(device, deviceType, control, state, previousState));
    } catch (...) {

    }
  }

  void HIDMapperInterface::StopMonitoring() {
    _stopMonitoring = true;

    // Stop monitoring the hardware for changes
    if (_deviceMonitoringTask != nullptr) {
      _deviceMonitoringTask->Wait();
      _deviceMonitoringTask = nullptr;
    }

    // Stop all the loggers and clean up the devices
    for (std::pair<const GUID, MapperBase*> logger : *_guidToLogger) {
      logger.second->Stop();
      delete logger.second;
    }
    _guidToLogger->clear();

    // Disconnect from direct input
    FreeDirectInput();
  }

  //-----------------------------------------------------------------------------
  // Name: FreeDirectInput()
  // Desc: Initialize the DirectInput variables.
  //-----------------------------------------------------------------------------
  void HIDMapperInterface::FreeDirectInput() {
    SAFE_RELEASE(_pDI);
  }
}