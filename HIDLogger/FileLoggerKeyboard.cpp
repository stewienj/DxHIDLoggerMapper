#include "FileLoggerKeyboard.h"
#include "HIDLoggerInterface.h"
#include <tchar.h>
#include <strsafe.h>

FileLoggerKeyboard::FileLoggerKeyboard(const TCHAR* directory, HIDLogger::HIDLoggerInterface^ loggerInterface) : FileLogger(directory, TEXT("Keyboard"), 0, loggerInterface) {
}

HRESULT FileLoggerKeyboard::GetInputState(DWORD size, void* newState) {

  if (!_loggerInterface->KeyboardLoggingEnabled) {
    return S_OK;
  } else {
    return FileLoggerBase::GetInputState(size, newState);
  }
}

void FileLoggerKeyboard::Log(const DIKBSTATE& kbState) {
  TCHAR keyName[256];
  memset(keyName, 0, 256);
  TCHAR sz[32];
  for (int keyNo = 0; keyNo < 256; ++keyNo) {

    if (kbState.keys[keyNo] != _lastState.keys[keyNo]) {
      unsigned long sc = keyNo;
      sc <<= 16;
      //sc |= 1 << 24; this screws things up for keys like delete
      int test = GetKeyNameText(sc, keyName, 256);
      if (test) {
        StringCchPrintf(sz, 32, keyName);
      } else {
        switch (keyNo) {
        case 0xD2:
          StringCchPrintf(sz, 32, TEXT("Insert"));
          break;
        case 0xD3:
          StringCchPrintf(sz, 32, TEXT("Delete"));
          break;
        case 0xCB:
          StringCchPrintf(sz, 32, TEXT("Left Arrow"));
          break;
        case 0xC7:
          StringCchPrintf(sz, 32, TEXT("Home"));
          break;
        case 0xCF:
          StringCchPrintf(sz, 32, TEXT("End"));
          break;
        case 0xC8:
          StringCchPrintf(sz, 32, TEXT("Up Arrow"));
          break;
        case 0xD0:
          StringCchPrintf(sz, 32, TEXT("Down Arrow"));
          break;
        case 0xC9:
          StringCchPrintf(sz, 32, TEXT("Page Up"));
          break;
        case 0xD1:
          StringCchPrintf(sz, 32, TEXT("Page Down"));
          break;
        case 0xCD:
          StringCchPrintf(sz, 32, TEXT("Right Arrow"));
          break;
        default:
          StringCchPrintf(sz, 32, TEXT("Unknown Key %d"), keyNo);
          break;
        }
      }
      LogBaseButton(sz, kbState.keys[keyNo], _lastState.keys[keyNo]);
    }
  }
}
