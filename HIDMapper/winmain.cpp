//-----------------------------------------------------------------------------
// File: Joystick.cpp
//
// Desc: Demonstrates an application which receives immediate 
//       Joystick data in exclusive mode via a dialog timer.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define _CRT_SECURE_NO_DEPRECATE
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <dinput.h>
#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#include "LimitSingleInstance.h"

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT UpdateInputState(HWND hDlg);

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#include "HIDMapperInterface.h"
#include "vcclr.h"

gcroot<HIDMapperDLL::HIDMapperInterface^> g_loggerInterface;

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point for the application.  Since we use a simple dialog for 
//       user interaction we don't need to pump messages.
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
  if (g_loggerInterface->OtherInstanceRunning()) {
    return 0;
  }

  InitCommonControls();

  WCHAR* strCmdLine;
  int nNumArgs;
  LPWSTR* pstrArgList = CommandLineToArgvW(GetCommandLineW(), &nNumArgs);
  for (int iArg = 1; iArg < nNumArgs; iArg++) {
    strCmdLine = pstrArgList[iArg];

    // Handle flag args
    if (*strCmdLine == L'/' || *strCmdLine == L'-') {
      strCmdLine++;

      //int nArgLen = ( int )wcslen( L"noxinput" );
      //if( _wcsnicmp( strCmdLine, L"noxinput", nArgLen ) == 0 && strCmdLine[nArgLen] == 0 )
      //{
      //	g_bFilterOutXinputDevices = true;
      //	continue;
      //}
    }
  }
  LocalFree(pstrArgList);

  // Display the main dialog box.
  DialogBox(hInst, MAKEINTRESOURCE(IDD_JOYST_IMM), NULL, MainDlgProc);

  return 0;
}

//-----------------------------------------------------------------------------
// Name: MainDialogProc
// Desc: Handles dialog messages
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);

  switch (msg) {
  case WM_INITDIALOG:
    // Set a timer to go off 30 times a second. At every timer message
    // the input device will be read

    g_loggerInterface = gcnew HIDMapperDLL::HIDMapperInterface();
    g_loggerInterface->StartMonitoring();

    SetTimer(hDlg, 0, 1000 / 100, NULL);

    return TRUE;

  case WM_ACTIVATE:
    if (WA_INACTIVE != wParam) {
    }
    return TRUE;

  case WM_TIMER:
    // Update the input device every timer message
    if (FAILED(UpdateInputState(hDlg))) {
      KillTimer(hDlg, 0);
      MessageBox(NULL, TEXT("Error Reading Input State. ") \
        TEXT("The sample will now exit."), TEXT("DirectInput Sample"),
        MB_ICONERROR | MB_OK);
      EndDialog(hDlg, TRUE);
    }
    return TRUE;

  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDCANCEL:
      EndDialog(hDlg, 0);
      return TRUE;
    }

  case WM_DESTROY:
    // Cleanup everything
    KillTimer(hDlg, 0);
    g_loggerInterface->StopMonitoring();
    g_loggerInterface = nullptr;
    return TRUE;
  }

  return FALSE; // Message not handled 
}


//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT UpdateInputState(HWND hDlg) {
  TCHAR strText[512] = { 0 }; // Device state text
  DIJOYSTATE2 js;           // DInput Joystick state 

  js = g_loggerInterface->GetJoystickState();
  // Display Joystick state to dialog

  // Axes
  StringCchPrintf(strText, 512, TEXT("%ld"), js.lX);
  SetWindowText(GetDlgItem(hDlg, IDC_X_AXIS), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), 1000 - js.lY);
  SetWindowText(GetDlgItem(hDlg, IDC_Y_AXIS), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), js.lZ);
  SetWindowText(GetDlgItem(hDlg, IDC_Z_AXIS), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), js.lRx);
  SetWindowText(GetDlgItem(hDlg, IDC_X_ROT), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), js.lRy);
  SetWindowText(GetDlgItem(hDlg, IDC_Y_ROT), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), 1000 - js.lRz);
  SetWindowText(GetDlgItem(hDlg, IDC_Z_ROT), strText);

  // Slider controls
  StringCchPrintf(strText, 512, TEXT("%ld"), js.rglSlider[0]);
  SetWindowText(GetDlgItem(hDlg, IDC_SLIDER0), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), 1000 - js.rglSlider[1]);
  SetWindowText(GetDlgItem(hDlg, IDC_SLIDER1), strText);

  // Points of view
  StringCchPrintf(strText, 512, TEXT("%ld"), js.rgdwPOV[0]);
  SetWindowText(GetDlgItem(hDlg, IDC_POV0), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), js.rgdwPOV[1]);
  SetWindowText(GetDlgItem(hDlg, IDC_POV1), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), js.rgdwPOV[2]);
  SetWindowText(GetDlgItem(hDlg, IDC_POV2), strText);
  StringCchPrintf(strText, 512, TEXT("%ld"), js.rgdwPOV[3]);
  SetWindowText(GetDlgItem(hDlg, IDC_POV3), strText);

  // Fill up text with which buttons are pressed
  StringCchCopy(strText, 512, TEXT(""));
  for (int i = 0; i < 128; i++) {
    if (js.rgbButtons[i] & 0x80) {
      TCHAR sz[128];
      StringCchPrintf(sz, 128, TEXT("%02d "), i + 1);
      StringCchCat(strText, 512, sz);
    }
  }

  SetWindowText(GetDlgItem(hDlg, IDC_BUTTONS), strText);

  return S_OK;
}

