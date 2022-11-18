#pragma once
#include "MonitorBase.h"

struct DIKBSTATE {
  char keys[256];
};

class MonitorKeyboard : public HIDMonitor<DIKBSTATE>
{
public:
  MonitorKeyboard(GUID deviceGuid, HIDMonitorDLL::HIDMonitorInterface^ loggerInterface);
  void Log(const DIKBSTATE& kbState) override;
};

