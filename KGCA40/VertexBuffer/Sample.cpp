#include "Sample.h"
#include "XVector.h"
TBASIS_RUN(SampleLibrary)

Sample::Sample()
{
    m_pVertexBuffer = nullptr;
    m_pVertexLayout = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
}
HRESULT Sample::CreateVertexBuffer()
{
    HRESULT hr = S_OK;
    // 로칼->월드->뷰->투영좌표계(NDC)->화면
    //  x = -1.0f ~ 1.0f
    //  y = -1.0f ~ 1.0f
    //  z = 0 ~ 1
    // 시스템 메모리
    SimpleVertex v[6];
    v[0].pos.x = -0.5f; v[0].pos.y = 0.5f; v[0].pos.z = 0.5f;
    v[1].pos.x =  0.5f; v[1].pos.y = 0.5f; v[1].pos.z = 0.5f;
    v[2].pos.x =  0.5f; v[2].pos.y = -0.5f; v[2].pos.z = 0.5f;

    v[3].pos.x = -0.5f; v[3].pos.y = 0.5f; v[3].pos.z = 0.5f;
    v[4].pos.x = 0.5f; v[4].pos.y = -0.5f; v[4].pos.z = 0.5f;
    v[5].pos.x = -0.5f; v[5].pos.y = -0.5f; v[5].pos.z = 0.5f;
    UINT numVertex = sizeof(v) / sizeof(v[0]);
    // 그래픽 카드 메모리로 보내야 한다.
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth    = sizeof(SimpleVertex)* numVertex;
    bd.Usage    = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem= v;
    hr = m_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT Sample::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[2];
    ZeroMemory(layout, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);
    layout[0].SemanticName = "POSITION";
    layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    layout[0].AlignedByteOffset=0;
    layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[1].SemanticName = "COLOR";
    layout[1].Format= DXGI_FORMAT_R32G32B32A32_FLOAT;
    layout[1].AlignedByteOffset=12;
    layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    hr = m_pd3dDevice->CreateInputLayout(layout,2,
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    m_pVSBlob->Release();
    return hr;
}

HRESULT Sample::LoadShader()
{
    HRESULT hr = S_OK;
  
    hr = D3DCompileFromFile(
        L"VertexShader.txt",
        nullptr,
        nullptr,
        "VS",
        "vs_5_0",
        0,
        0,
        &m_pVSBlob,
        nullptr);
    if (FAILED(hr)) return hr;

    hr = m_pd3dDevice->CreateVertexShader(
        m_pVSBlob->GetBufferPointer(), 
        m_pVSBlob->GetBufferSize(),
        NULL, &m_pVS);
    if (FAILED(hr)) return hr;

    ID3DBlob* PSBlob = nullptr;
    hr = D3DCompileFromFile(
        L"PixelShader.txt",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &PSBlob,
        nullptr);
    if (FAILED(hr)) return hr;
    hr = m_pd3dDevice->CreatePixelShader(
        PSBlob->GetBufferPointer(),
        PSBlob->GetBufferSize(),
        NULL, &m_pPS);
    if (FAILED(hr)) return hr;
    PSBlob->Release();
    return hr;
}

bool Sample::Init()
{
    CreateVertexBuffer();
    LoadShader();
    CreateVertexLayout();    
    return false;
}

bool Sample::Frame()
{
    return false;
}

bool Sample::Render()
{
    m_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
    m_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = sizeof(SimpleVertex);
    UINT pOffsets = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    m_pImmediateContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(6, 0);
    return false;
}

bool Sample::Release()
{
    m_pVertexBuffer->Release();
    m_pVertexLayout->Release();
    m_pVS->Release();
    m_pPS->Release();
    return false;
}
