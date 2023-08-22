#include "TObject.h"
#include "DirectXTex.h" // ±âÅ¸
#include "TShaderMgr.h"
#include "TTextureMgr.h"
void   TObject::SetRect(TVector2& c, float w, float h)
{
    m_tRT.Set(c, w, h);
}
void  TObject::SetPos(TVector3 p)
{
    m_vPos = p;
}
void  TObject::SetScale(TVector3 s)
{
    m_vScale = s;
}
void  TObject::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{
    if (matWorld != nullptr)
    {
        m_matWorld = *matWorld;
    }
    if (matView != nullptr)
    {
        m_matView = *matView;
    }
    if (matProj != nullptr)
    {
        m_matProj = *matProj;
    }
    m_cbData.matWorld = m_matWorld.Transpose();
    m_cbData.matView = m_matView.Transpose();
    m_cbData.matProj = m_matProj.Transpose();
    m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_cbData, 0, 0);

}
bool  TObject::Create(std::wstring texFilename,
                      std::wstring shaderFilename)
{
    CreateConstantBuffer();
    CreateVertexBuffer();
    m_pShader = I_Shader.Load(shaderFilename);
    CreateInputLayout();
    m_pTex = I_Tex.Load(texFilename);   
    return true;
}

bool  TObject::Init()
{   
    TDxObject::Init();
    return true;
}
bool  TObject::Frame()
{
    TDxObject::Frame();
    return true;
}
bool  TObject::Render()
{
    TDxObject::Render();
    return true;
}
bool  TObject::Release()
{
    TDxObject::Release();
    return true;
}
TObject::TObject()
{
    m_vPos      = TVector3(0,0,0);
    m_vScale    = TVector3(1, 1, 1);
    m_vRotation = TVector3(0, 0, 0);
}
