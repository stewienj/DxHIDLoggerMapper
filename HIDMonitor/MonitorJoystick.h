#pragma once

#include "MonitorBase.h"

class MonitorJoystick : public HIDMonitor<DIJOYSTATE2>
{
public:
  MonitorJoystick(GUID deviceGuid, HIDMonitorDLL::HIDMonitorInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
};

