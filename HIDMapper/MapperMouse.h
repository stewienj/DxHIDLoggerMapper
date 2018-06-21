#pragma once
#include "MapperBase.h"

class MapperMouse : public HIDMapper<DIMOUSESTATE2>
{

public:
  MapperMouse(GUID deviceGuid, HIDMapperDLL::HIDMapperInterface^ loggerInterface);
  void Log(const DIMOUSESTATE2& mouseState) override;
};

