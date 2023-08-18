#pragma once
#include "TDevice.h"
#include "TTimer.h"
class TCore : public TDevice
{
    TTimer        m_GameTimer;
public:
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
private:
    virtual bool  EngineInit();
    virtual bool  EngineFrame();
    virtual bool  EngineRender();
    virtual bool  EngineRelease();
public:
    bool  Run();
};

