#include "MapperBase.h"
#include "HIDMapperInterface.h"
#include <tchar.h>
#include <strsafe.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

MapperBase::MapperBase(HIDMapperDLL::DeviceType deviceType, int deviceNo, HIDMapperDLL::HIDMapperInterface^ loggerInterface) {
  _loggerInterface = loggerInterface;
  // initialize class variables
  _canExit = true;
  _keepRunning = true;
  _pDevice = NULL;
  _deviceLost = false;
  _deviceType = deviceType;
  _deviceNo = deviceNo;
}

MapperBase::~MapperBase(void) {
  Stop();
  SAFE_RELEASE(_pDevice);
}

void MapperBase::Stop() {
  _keepRunning = false;
  while (!_canExit) {
    Sleep(10);
  }
  if (_pDevice != NULL)
    _pDevice->Unacquire();
}

void MapperBase::SetDevice(LPDIRECTINPUTDEVICE8 device) {
  _pDevice = device;
  DIDEVICEINSTANCE info = { 0 };
  info.dwSize = sizeof(DIDEVICEINSTANCE);
  if (S_OK == _pDevice->GetDeviceInfo(&info))
    wcscpy_s(_deviceName, info.tszProductName);
}

void MapperBase::StartLoggingThread() {
  while (_keepRunning) {
    try {
      Sleep(10);
      UpdateInputState();
    } catch (...) {
    }
  }
  _canExit = true;
}

void MapperBase::Start() {
  if (_pDevice != NULL) {
    _canExit = false;
    // Don't acquire here, there is now a dedicated thread for managing device acquisition
    //_pDevice->Acquire();
    CreateThread(NULL, 0, &ThreadProc, this, 0, NULL);
  }
}

// All acquires must be called from the same thread else you get deadlocks.
// Checks if an error has occurred and if so then reaquires the device
HRESULT MapperBase::UIThreadCheckIsValid() {
  if (_deviceLost) {
    return _pDevice->Acquire();
  } else {
    return S_OK;
  }
}

void MapperBase::NotifyStateChangeButton(TCHAR* controlID, LONG state, LONG previousState) {
  if (state != previousState && state != -1) {
	_loggerInterface->OnHIDStateChanged(_deviceName, _deviceType, controlID, state, previousState);
  }
}

void MapperBase::NotifyStateChangeAxis(TCHAR* controlID, LONG state, LONG previousState) {
  if (state != previousState && state != -1) {
    _loggerInterface->OnHIDStateChanged(_deviceName, _deviceType, controlID, state, previousState);
  }
}


//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT MapperBase::GetInputState(DWORD size, void* newState) {
  HRESULT hr;

  if (NULL == _pDevice)
    return S_OK;

  // Poll the device to read the current state
  hr = _pDevice->Poll();

  if (hr == DIERR_NOTACQUIRED || hr == DIERR_INPUTLOST) {
    _deviceLost = true;
  } else {
    _deviceLost = false;
  }

  // Device shouldn't be acquired on this thread, causes lockout
  //if (FAILED(hr)) {
  //  // DInput is telling us that the input stream has been
  //  // interrupted. We aren't tracking any state between polls, so
  //  // we don't have any special reset that needs to be done. We
  //  // just re-acquire and try again.
  //  if (FAILED(hr = _pDevice->Acquire())) {
  //    // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
  //    // may occur when the app is minimized or in the process of 
  //    // switching, so just try again later 
  //    return hr;
  //  }
  //  hr = _pDevice->Poll();
  //}

  if (FAILED(hr)) {
    return hr;
  }

  // Get the input's device state
  if (FAILED(hr = _pDevice->GetDeviceState(size, newState)))
    return hr; // The device should have been acquired during the Poll()

  return S_OK;
}
