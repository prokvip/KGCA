#pragma once
#include "TStd.h"
#include "TTextureMgr.h"
#include "TShaderMgr.h"

struct PT_Vertex
{
    TVector3 p;
    TVector2 t;
};
struct CB_Data
{
    TMatrix matWorld;
    TMatrix matView;
    TMatrix matProj;
};

class TObject
{    
public:
    TMatrix       m_matWorld;
    TMatrix       m_matView;
    TMatrix       m_matProj;
    CB_Data       m_cbData;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
    TVector3      m_vPos;
    TVector3      m_vScale;
    TVector3      m_vRotation;
    void          SetPos(TVector3 p);
public:
    ID3D11Buffer*           m_pVertexBuffer = nullptr;    
    ID3D11Buffer*           m_pConstantBuffer = nullptr;
    ID3D11InputLayout*      m_pVertexLayout = nullptr;

    const TShader*          m_pShader = nullptr;
    const TTexture*         m_pTex = nullptr;
    std::vector< PT_Vertex> m_VertexList;
public:
    void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
    virtual bool  CreateVertexBuffer();
    virtual bool  CreateConstantBuffer();
    virtual bool  CreateInputLayout();
public:
    virtual bool  Create(TTextureMgr& texMgr, std::wstring shaderFilename,
                 TShaderMgr& shaderMgr, std::wstring texFilename);
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
    virtual void  SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
public:
    TObject();
    virtual ~TObject() {}
};

