#pragma once
#include "TPlaneObj.h"
class TPlayer : public TPlaneObj
{
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

