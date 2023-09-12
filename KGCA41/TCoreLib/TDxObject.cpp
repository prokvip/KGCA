#include "TDxObject.h"
void  TDxObject::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext)
{
    m_pDevice = pDevice;
    m_pImmediateContext = pImmediateContext;
}
bool  TDxObject::CreateIndexBuffer()
{
    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(DWORD) * m_IndexList.size();
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA InitialData;
    ZeroMemory(&InitialData, sizeof(InitialData));
    InitialData.pSysMem = &m_IndexList.at(0);

    HRESULT hr = m_pDevice->CreateBuffer(
        &Desc,
        &InitialData,
        &m_pIndexBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool  TDxObject::CreateVertexBuffer()
{   
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
bool  TDxObject::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(CB_Data);
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = m_pDevice->CreateBuffer(
        &Desc,
        nullptr,
        &m_pConstantBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool  TDxObject::CreateInputLayout()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT iNumCount = sizeof(layout) / sizeof(layout[0]);

    if (m_pShader)
    {
        HRESULT hr = m_pDevice->CreateInputLayout(
            layout,
            iNumCount,
            m_pShader->GetBufferPointer(),
            m_pShader->GetBufferSize(),
            &m_pVertexLayout);
        if (FAILED(hr))
        {
            return false;
        }
    }
    return true;
}
bool  TDxObject::Init()
{
    return true;
}
bool  TDxObject::Frame()
{
    return true;
}
bool  TDxObject::PreRender()
{
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    if (m_pTex)
    {
        m_pTex->Apply(m_pImmediateContext, 0);
    }
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    if (m_pShader)
    {
        m_pShader->Apply(m_pImmediateContext, 0);
    }

    UINT stride = sizeof(PT_Vertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    return true;
}
bool  TDxObject::Render()
{
    PreRender();
    PostRender();
    return true;
}
bool  TDxObject::PostRender()
{
    if(m_pIndexBuffer== nullptr)
        m_pImmediateContext->Draw(m_VertexList.size(), 0);
    else
    {
        m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
    }
    return true;
}
bool  TDxObject::Release()
{
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pIndexBuffer)m_pIndexBuffer->Release();
    if (m_pVertexLayout) m_pVertexLayout->Release();
    if (m_pConstantBuffer)m_pConstantBuffer->Release();
    return true;
}
