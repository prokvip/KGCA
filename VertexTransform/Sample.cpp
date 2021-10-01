#include "Sample.h"
#include "TVector.h"
TBASIS_RUN(SampleLibrary)
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
    _stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

    int iNumVertex = 0;
    _fgetts(buffer, 256, fp);
    _stscanf_s(buffer, _T("%d"), &iNumVertex);

    int index = 0;
    for (int iLine = 0; iLine < iNumVertex; iLine++)
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
    m_pSpeed = 3.0f;
    m_vCameraPos = { 3,3, -5.0f };
    m_vCameraTarget = { 0,0,0.0f };
    m_pVertexBuffer = nullptr;
    m_pVertexLayout = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
}
HRESULT Sample::CreateConstantBuffer()
{
    HRESULT hr = S_OK;   
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_cbData;
    hr = m_pd3dDevice->CreateBuffer(&bd, &data, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;
    return hr;
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
    bd.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_VertexList.at(0);
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
    LoadObject(L"ObjectData.txt");
    CreateConstantBuffer();
    CreateVertexBuffer();
    LoadShader();
    CreateVertexLayout();    
    return false;
}

bool Sample::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos.z += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos.z -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos.x -= m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_vCameraPos.x += m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_pSpeed * g_fSecPerFrame;
    }
    // -1 ~ +1 -> 0 ~ 1 => -1*0.5f+0.5f;
    // 0 ~ +1 -> -1 ~ 1 => 0.5f*2.0f-1.0f;
    // D3D11_USAGE_DEFAULT
    //m_cbData.matWorld._11 = cosf(g_fGameTimer)*0.5f+0.5f;
    m_cbData.matWorld = TMatrix::RotationZ(g_fGameTimer);

    TVector3 vUp = { 0,1,0.0f };
    m_cbData.matView = TMatrix::ViewLookAt(
        m_vCameraPos, m_vCameraTarget, vUp);
    m_cbData.matProj = TMatrix::PerspectiveFovLH(1.0f, 
        100.0f, TBASIS_PI * 0.5f, 
        (float)g_rtClient.right / (float)g_rtClient.bottom);

    m_cbData.matWorld = m_cbData.matWorld.Transpose();
    m_cbData.matView = m_cbData.matView.Transpose();
    m_cbData.matProj = m_cbData.matProj.Transpose();
    m_pImmediateContext->UpdateSubresource(
        m_pConstantBuffer,0,NULL, &m_cbData, 0,0);


    return false;
}

bool Sample::Render()
{
    m_pImmediateContext->VSSetConstantBuffers(
        0, 1, &m_pConstantBuffer   );
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
    m_pConstantBuffer->Release();
    m_pVS->Release();
    m_pPS->Release();
    return false;
}
