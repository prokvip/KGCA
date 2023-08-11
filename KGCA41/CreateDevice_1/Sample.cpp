#include "Sample.h"
bool  Sample::CreateVertexBuffer()
{
    m_VertexList.resize(6);
    m_VertexList[0].x = -1.0f; m_VertexList[0].y = 1.0f; m_VertexList[0].z = 0.5f;
    m_VertexList[1].x = 1.0f; m_VertexList[1].y = 1.0f; m_VertexList[1].z = 0.5f;
    m_VertexList[2].x = -1.0f; m_VertexList[2].y = -1.0f; m_VertexList[2].z = 0.5f;

    m_VertexList[3].x = -1.0f; m_VertexList[3].y = -1.0f; m_VertexList[3].z = 0.5f;
    m_VertexList[4].x = 1.0f; m_VertexList[4].y = 1.0f; m_VertexList[4].z = 0.5f;
    m_VertexList[5].x = 1.0f; m_VertexList[5].y = -1.0f; m_VertexList[5].z = 0.5f;


    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(P_Vertex) * m_VertexList.size();
    Desc.Usage= D3D11_USAGE_DEFAULT;
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
bool  Sample::LoadVertexShader()
{
    ID3DBlob* ErrorCode;
    // 쉐이더 컴파일
    HRESULT hr= D3DCompileFromFile(
        L"VertexShader.vsh",
        nullptr,
        nullptr,
        "VS_KGCA",
        "vs_5_0",
        0,
        0,
        &m_VertexShaderCode,
        &ErrorCode);
    if (FAILED(hr))
    {
        //ErrorCode
    }
    //ID3D11VertexShader* m_pVS
    hr = m_pDevice->CreateVertexShader(
        m_VertexShaderCode->GetBufferPointer(),
        m_VertexShaderCode->GetBufferSize(),
        nullptr,
        &m_pVS);
   
    if (ErrorCode) ErrorCode->Release();
    return true;
}
bool  Sample::LoadPixelShader()
{
    ID3DBlob* ShaderCode;
    ID3DBlob* ErrorCode;
    // 쉐이더 컴파일
    HRESULT hr = D3DCompileFromFile(
        L"PixelShader.psh",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &ShaderCode,
        &ErrorCode);
    if (FAILED(hr))
    {
        //ErrorCode
    }
    //ID3D11VertexShader* m_pVS
    hr = m_pDevice->CreatePixelShader(
        ShaderCode->GetBufferPointer(),
        ShaderCode->GetBufferSize(),
        nullptr,
        &m_pPS);
    
    if (ShaderCode) ShaderCode->Release();
    if (ErrorCode) ErrorCode->Release();
    return true;
}
bool  Sample::CreateInputLayout()
{
    const D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },        
    };
    UINT iNumCount = sizeof(layout) / sizeof(layout[0]);
    HRESULT hr = m_pDevice->CreateInputLayout(
         layout,
        iNumCount,
        m_VertexShaderCode->GetBufferPointer(),
        m_VertexShaderCode->GetBufferSize(),
        &m_pVertexLayout);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool  Sample::Init() 
{   
    CreateVertexBuffer();
    LoadVertexShader();
    LoadPixelShader();
    CreateInputLayout();
    return true; 
}
bool  Sample::Frame() 
{ 
    return true; 
}
bool  Sample::Render() 
{    
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    m_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
    m_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
    UINT stride = sizeof(P_Vertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(m_VertexList.size(), 0);
    return true; 
}
bool  Sample::Release()
{   
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pVertexLayout) m_pVertexLayout->Release();
    if (m_pVS) m_pVS->Release();
    if (m_pPS) m_pPS->Release();
    return true; 
}

TGAME(L"kgca", 800, 600)