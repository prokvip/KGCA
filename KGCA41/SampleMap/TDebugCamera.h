#pragma once
#include "TCamera.h"
class TDebugCamera : public TCamera
{
public:
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
};

