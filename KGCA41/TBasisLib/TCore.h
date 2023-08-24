#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TCamera.h"


class TCore : public TDevice
{
public:
    TTimer          m_GameTimer;   
    TCamera         m_MainCamera;
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

