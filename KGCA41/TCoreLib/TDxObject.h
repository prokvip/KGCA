#pragma once
#include "TStd.h"
#include "TTextureMgr.h"
#include "TShaderMgr.h"
struct CB_Data
{
    TMatrix matWorld;
    TMatrix matView;
    TMatrix matProj;
};
struct PT_Vertex
{
    TVector3 p;
    TVector2 t;
};

class TDxObject
{
public:
    ID3D11Device*           m_pDevice = nullptr;
    ID3D11DeviceContext*    m_pImmediateContext = nullptr;
    ID3D11Buffer*           m_pVertexBuffer = nullptr;
    ID3D11Buffer*           m_pConstantBuffer = nullptr;
    ID3D11InputLayout*      m_pVertexLayout = nullptr;
    const TShader*          m_pShader = nullptr;
    const TTexture*         m_pTex = nullptr;
    CB_Data                 m_cbData;
    std::vector< PT_Vertex> m_VertexList;
public:
    void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
    virtual bool  CreateVertexBuffer();
    virtual bool  CreateConstantBuffer();
    virtual bool  CreateInputLayout();
public:
    virtual bool    Init();
    virtual bool    Frame();
    virtual bool    Render();
    virtual bool    Release();
public:
    virtual ~TDxObject() {};
};