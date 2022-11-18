#pragma once
#include "MonitorBase.h"

class MonitorMouse : public HIDMonitor<DIMOUSESTATE2>
{

public:
  MonitorMouse(GUID deviceGuid, HIDMonitorDLL::HIDMonitorInterface^ loggerInterface);
  void Log(const DIMOUSESTATE2& mouseState) override;
};

