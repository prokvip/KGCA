#include "TPlaneObj.h"
bool  TPlaneObj::Load(
    ID3D11Device* pDevice,
    ID3D11DeviceContext* pContext,
    TVector3 p,
    TVector3 s,
    W_STR texFile, W_STR shaderFile)
{
    Set(pDevice, pContext);
    SetPos(p);
    SetScale(s);
    return Create(texFile, shaderFile);
}
bool  TPlaneObj::CreateVertexBuffer()
{
    m_VertexList.resize(6);
    m_VertexList[0].t.x = 0.0f; m_VertexList[0].t.y = 0.0f;
    m_VertexList[1].t.x = 1.0f; m_VertexList[1].t.y = 0.0f;
    m_VertexList[2].t.x = 0.0f; m_VertexList[2].t.y = 1.0f;
    m_VertexList[3].t.x = 0.0f; m_VertexList[3].t.y = 1.0f;
    m_VertexList[4].t.x = 1.0f; m_VertexList[4].t.y = 0.0f;
    m_VertexList[5].t.x = 1.0f; m_VertexList[5].t.y = 1.0f;

    m_VertexList[0].p.x = -1.0f; m_VertexList[0].p.y = 1.0f;  m_VertexList[0].p.z = 0.0f;
    m_VertexList[1].p.x = 1.0f; m_VertexList[1].p.y = 1.0f;  m_VertexList[1].p.z = 0.0f;
    m_VertexList[2].p.x = -1.0f; m_VertexList[2].p.y = -1.0f;  m_VertexList[2].p.z = 0.0f;
    m_VertexList[3] = m_VertexList[2];
    m_VertexList[4] = m_VertexList[1];
    m_VertexList[5].p.x = 1.0f; m_VertexList[5].p.y = -1.0f;  m_VertexList[5].p.z = 0.0f;

    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(PT_Vertex) * m_VertexList.size();
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA InitialData;
    ZeroMemory(&InitialData, sizeof(InitialData));
    InitialData.pSysMem = &m_VertexList.at(0);

    HRESULT hr = m_pDevice->CreateBuffer(
        &Desc,
        &InitialData,
        &m_pVertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }


    return true;
}

bool TPlaneObj::Frame()
{
    UpdateMatrix();
    return true;
}