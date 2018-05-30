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
        StringCchPrintf(sz, 32, TEXT("%s (0x%x)"),keyName, keyNo);
      } else {
        switch (keyNo) {
        case 0xD2:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Insert"), keyNo);
          break;
        case 0xD3:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Delete"), keyNo);
          break;
        case 0xCB:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Left Arrow"), keyNo);
          break;
        case 0xC7:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Home"), keyNo);
          break;
        case 0xCF:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("End"), keyNo);
          break;
        case 0xC8:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Up Arrow"), keyNo);
          break;
        case 0xD0:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Down Arrow"), keyNo);
          break;
        case 0xC9:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Page Up"), keyNo);
          break;
        case 0xD1:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Page Down"), keyNo);
          break;
        case 0xCD:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Right Arrow"), keyNo);
          break;
        case 0x9D:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Right Control"), keyNo);
          break;
        case 0xB8:
          StringCchPrintf(sz, 32, TEXT("%s (0x%x)"), TEXT("Right Alt"), keyNo);
          break;
        default:
          StringCchPrintf(sz, 32, TEXT("Unknown Key (0x%x)"), keyNo);
          break;
        }
      }
      LogBaseButton(sz, kbState.keys[keyNo], _lastState.keys[keyNo]);
    }
  }
}
