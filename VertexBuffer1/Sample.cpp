#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"
#include "TVector.h"
TBASIS_RUN(SampleLibrary)
//This function or variable may be unsafe.
//Consider using _wfopen_s instead.To disable deprecation,
//use _CRT_SECURE_NO_WARNINGS.See online help for details.
bool  Sample::LoadObject(std::wstring filename)
{
    FILE* fp = nullptr;
    _tfopen_s(&fp, filename.c_str(), _T("rt"));
    if (fp == NULL)
    {
        return false;
    }
    TCHAR buffer[256] = { 0, };
    int iVersion = 0;
    _fgetts(buffer, 256, fp);
    TCHAR tmp[256] = { 0, };
    _stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp) ,&iVersion);

    int iNumVertex = 0;
    _fgetts(buffer, 256, fp);
    _stscanf_s(buffer, _T("%d"), &iNumVertex);

    int index = 0;
    for( int iLine=0; iLine < iNumVertex; iLine++)
    {
        SimpleVertex v;
        _fgetts(buffer, 256, fp);
         _stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
            &index,
            &v.pos.x, &v.pos.y, &v.pos.z,
            &v.color.x, &v.color.y, &v.color.z, &v.color.w);
        m_VertexList.push_back(v);
    }
    fclose(fp);
    return true;
}
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

     // 그래픽 카드 메모리로 보내야 한다.
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth    = sizeof(SimpleVertex)* m_VertexList.size();
    bd.Usage    = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem= &m_VertexList.at(0);
    hr = m_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT Sample::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numLayout = sizeof(layout) / sizeof(layout[0]);
    hr = m_pd3dDevice->CreateInputLayout(layout, numLayout,
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
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(
        L"VertexShader.txt",
        nullptr,
        nullptr,
        "VS",
        "vs_5_0",
        0,
        0,
        &m_pVSBlob,
        &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, 
            (char*)error->GetBufferPointer(), 
            "error", MB_OK);
        return hr;
    }

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
        &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }
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
    LoadObject(L"ObjectData.txt");
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
    m_pImmediateContext->Draw(m_VertexList.size(), 0);
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
