#pragma once

#include "MapperBase.h"

class MapperJoystick : public HIDMapper<DIJOYSTATE2>
{
public:
  MapperJoystick(int deviceNo, HIDMapperDLL::HIDMapperInterface^ loggerInterface);
	void Log(const DIJOYSTATE2& joyState) override;
};

