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
    std::shared_ptr<TCamera>    m_pDefaultCamera=nullptr;

    ID3D11BlendState*           m_AlphaBlend = nullptr;
    ComPtr<ID3D11SamplerState> m_pSamplerState;
    ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
    ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateDisable;
public:
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
    void  CreateBlendState();
    void  CreateSamplerState();
    void  CreateDepthStencilState();
    virtual void ResizeDevice(UINT width, UINT height) override;
private:
    virtual bool  EngineInit();
    virtual bool  EngineFrame();
    virtual bool  EngineRender();
    virtual bool  EngineRelease();
public:
    // resize
    virtual bool  DeleteDxResource();
    virtual bool  CreateDxResource();
  
public:
    bool  Run();
};
