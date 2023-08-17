#pragma once
#include "TStd.h"
#include "TTextureMgr.h"
#include "TShaderMgr.h"

struct PT_Vertex
{
    float x, y, z;
    float u, v;
};


class TObject
{    
    //TMatrix       m_matWorld;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
    ID3D11Buffer*           m_pVertexBuffer = nullptr;    
    ID3D11InputLayout*      m_pVertexLayout = nullptr;

    const TShader*          m_pShader = nullptr;
    const TTexture*         m_pTex = nullptr;
    std::vector< PT_Vertex> m_VertexList;
public:
    void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
    bool CreateVertexBuffer();
    bool  LoadVertexShader();
    bool  LoadPixelShader();
    bool  CreateInputLayout();
    bool  LoadTextureFile(std::wstring filename);
public:
    bool  Create(TTextureMgr& texMgr, std::wstring shaderFilename,
                 TShaderMgr& shaderMgr, std::wstring texFilename);
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

