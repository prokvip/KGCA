#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TCamera.h"
#include "TWriter.h"
#include "TShaderMgr.h"
#include "TTextureMgr.h"

class TCore : public TDevice
{
public:
    TTimer          m_GameTimer;   
    std::shared_ptr<TCamera>    m_pMainCamera=nullptr;
    ID3D11BlendState* m_AlphaBlend = nullptr;
public:
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
    void  CreateBlendState();
private:
    virtual bool  EngineInit();
    virtual bool  EngineFrame();
    virtual bool  EngineRender();
    virtual bool  EngineRelease();
public:
    bool  Run();
};

