#pragma once

#include "MapperBase.h"

class MapperJoystick : public HIDMapper<DIJOYSTATE2>
{
public:
  MapperJoystick(GUID deviceGuid, HIDMapperDLL::HIDMapperInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
};

