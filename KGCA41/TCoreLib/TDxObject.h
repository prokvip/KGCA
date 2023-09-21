#pragma once
#include "TStd.h"
#include "TTextureMgr.h"
#include "TShaderMgr.h"
struct CB_Data
{
    TBASIS_EX::TMatrix matWorld;
    TBASIS_EX::TMatrix matView;
    TBASIS_EX::TMatrix matProj;
};
struct PNCT_Vertex
{
    TVector3 p;
    TVector3 n;
    TVector4 c;
    TVector2 t;
};

class TDxObject
{
public:
    ID3D11Device*           m_pDevice = nullptr;
    ID3D11DeviceContext*    m_pImmediateContext = nullptr;
    ID3D11Buffer*           m_pVertexBuffer = nullptr;
    ID3D11Buffer*           m_pIndexBuffer = nullptr;
    ID3D11Buffer*           m_pConstantBuffer = nullptr;
    ID3D11InputLayout*      m_pVertexLayout = nullptr;
    const TShader*          m_pShader = nullptr;
    const TTexture*         m_pTex = nullptr;
    CB_Data                 m_cbData;
    std::vector< PNCT_Vertex> m_VertexList;
    std::vector< DWORD>     m_IndexList;
public:
    void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
    virtual bool  CreateVertexData()
    {
        return true;
    }
    virtual bool  CreateIndexData()
    {
        return true;
    }
    virtual bool  CreateVertexBuffer();
    virtual bool  CreateIndexBuffer();
    virtual bool  CreateConstantBuffer();
    virtual bool  CreateInputLayout();
public:
    virtual bool    Init();
    virtual bool    Frame();
    virtual bool    PreRender();
    virtual bool    Render();
    virtual bool    PostRender();
    virtual bool    Release();
public:
    virtual ~TDxObject() {};
};