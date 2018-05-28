#include "FileLogger.h"
#include "HIDLoggerInterface.h"
#include <tchar.h>
#include <strsafe.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

FileLoggerBase::FileLoggerBase(const TCHAR* directory, const TCHAR* suffix, int deviceNo, HIDLogger::HIDLoggerInterface^ loggerInterface) {
  _loggerInterface = loggerInterface;
  // initialize class variables
  _canExit = true;
  _keepRunning = true;
  _outputMovement = NULL;
  _outputButton = NULL;
  _pDevice = NULL;
  _deviceLost = false;
  _writeToFile = directory != NULL;
  StringCchPrintf(_deviceType, 128, TEXT("%s%d"), suffix, deviceNo);
  StringCchPrintf(_directory, 1024, _writeToFile ? directory : TEXT(""));
  DWORD size = sizeof(_machineName);
  GetComputerName(_machineName, &size);

  try {
    if (_writeToFile)
    {
      CreateDirectory(directory, NULL);
    }
  }
  catch (...) {
    _loggerInterface->Error(_deviceName, "Error Creating Log Directory");
  }
}

FileLoggerBase::~FileLoggerBase(void) {
  Stop();
  SAFE_RELEASE(_pDevice);
}

void FileLoggerBase::Stop() {
  _keepRunning = false;
  while (!_canExit) {
    Sleep(10);
  }
  if (_outputMovement != NULL) {
    _outputMovement->Close();
    delete _outputMovement;
    _outputMovement = NULL;
  }
  if (_outputButton != NULL) {
    _outputButton->Close();
    delete _outputButton;
    _outputButton = NULL;
  }
  if (_pDevice != NULL)
    _pDevice->Unacquire();
}

void FileLoggerBase::SetDevice(LPDIRECTINPUTDEVICE8 device) {
  _pDevice = device;
  DIDEVICEINSTANCE info = { 0 };
  info.dwSize = sizeof(DIDEVICEINSTANCE);
  if (S_OK == _pDevice->GetDeviceInfo(&info))
    wcscpy_s(_deviceName, info.tszProductName);
}

void FileLoggerBase::StartLoggingThread() {
  while (_keepRunning) {
    try {
      Sleep(15);
      UpdateInputState();
    } catch (...) {
    }
  }
  _canExit = true;
}

TextFile* FileLoggerBase::CreateFileWithHeader(TCHAR* prefix)
{
  TCHAR tempFilename[4096] = { 0 };
  TextFile* textFile = NULL;
  if (_writeToFile)
  {
    try {

      TCHAR timeString[128];
      SYSTEMTIME time;
      GetLocalTime(&time);
      StringCchPrintf(timeString, 128, TEXT("%04d%02d%02d %02d%02d%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
      StringCchPrintf(tempFilename, 4096, TEXT("%s\\%s %s %s %s %s.tsv"), _directory, prefix, timeString, _deviceType, _deviceName, _machineName);
      textFile = new TextFile();
      textFile->Open(tempFilename, TF_WRITE, TF_UTF8, 4096);
    }
    catch (...) {
      _loggerInterface->Error(_deviceName, "Error Creating Log File");
      return NULL;
    }

    TCHAR buffer[4096] = { 0 };
    BOOL test;

    try {
      StringCchCat(buffer, 4096, _TEXT("Time\t"));
      StringCchCat(buffer, 4096, _TEXT("Machine Name\t"));
      StringCchCat(buffer, 4096, _TEXT("Device ID\t"));
      StringCchCat(buffer, 4096, _TEXT("Control ID\t"));
      StringCchCat(buffer, 4096, _TEXT("Control Value\t"));

      textFile->WriteString(&test, buffer);
      textFile->WriteChar(&test, '\n');
    }
    catch (...) {
      _loggerInterface->Error(_deviceName, "Error Writing to Log File");
    }
  }
  return textFile;
}
void FileLoggerBase::Start() {
  if (_pDevice != NULL) {
    _canExit = false;
    // Don't acquire here, there is now a dedicated thread for managing device acquisition
    //_pDevice->Acquire();
    CreateThread(NULL, 0, &ThreadProc, this, 0, NULL);
  }
}

// All acquires must be called from the same thread else you get deadlocks.
// Checks if an error has occurred and if so then reaquires the device
HRESULT FileLoggerBase::UIThreadCheckIsValid() {
  if (_deviceLost) {
    return _pDevice->Acquire();
  } else {
    return S_OK;
  }
}

void FileLoggerBase::LogBaseMovement(TCHAR* controlID, LONG state, LONG previousState) {
  if (state != previousState) {
    if (_outputMovement == NULL) {
      _outputMovement = CreateFileWithHeader(TEXT("hid_axis"));
    }
    LogBase(controlID, state, previousState, _outputMovement);
  }
}
void FileLoggerBase::LogBaseButton(TCHAR* controlID, LONG state, LONG previousState) {
  if (state != previousState && state != -1) {
    if (_outputButton == NULL) {
      _outputButton = CreateFileWithHeader(TEXT("hid_button"));
    }
    LogBase(controlID, state, previousState, _outputButton);
  }
}

void FileLoggerBase::LogBase(TCHAR* controlID, LONG state, LONG previousState, TextFile* output) {
  TCHAR sz[128];
  SYSTEMTIME time;
  GetLocalTime(&time);
  TCHAR buffer[4096] = { 0 };
  BOOL test;

  if (_writeToFile)
  {
    try {
      StringCchPrintf(sz, 128, TEXT("%04d-%02d-%02d %02d:%02d:%02d.%03d\t"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
      StringCchCat(buffer, 4096, sz);
      StringCchPrintf(sz, 128, TEXT("%s\t"), _machineName);
      StringCchCat(buffer, 4096, sz);
      StringCchPrintf(sz, 128, TEXT("%s\t"), _deviceName);
      StringCchCat(buffer, 4096, sz);
      StringCchPrintf(sz, 128, TEXT("%s\t"), controlID);
      StringCchCat(buffer, 4096, sz);
      StringCchPrintf(sz, 128, TEXT("%ld\t"), state);
      StringCchCat(buffer, 4096, sz);
      output->WriteString(&test, buffer);
      output->WriteChar(&test, '\n');
      output->Flush();
    }
    catch (...) {
      _loggerInterface->Error(_deviceName, "Error Writing to Log File");
    }
  }
  _loggerInterface->LoggedInfo(_machineName, _deviceName, controlID, state, previousState);
}


//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT FileLoggerBase::GetInputState(DWORD size, void* newState) {
  HRESULT hr;
  TCHAR strText[512] = { 0 }; // Device state text

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
