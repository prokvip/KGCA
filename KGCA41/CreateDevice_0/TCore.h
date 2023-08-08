#pragma once
#include "TWindow.h"
class TCore : public TWindow
{
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

