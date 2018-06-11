#pragma once
#include "MapperBase.h"

struct DIKBSTATE {
  char keys[256];
};

class MapperKeyboard : public HIDMapper<DIKBSTATE>
{
public:
  MapperKeyboard(HIDMapperDLL::HIDMapperInterface^ loggerInterface);
  void Log(const DIKBSTATE& kbState) override;
  HRESULT GetInputState(DWORD size, void* newState) override;
};

