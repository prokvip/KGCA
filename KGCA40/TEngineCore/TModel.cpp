#include "TModel.h"
void		TModel::SetMatrix(
    TMatrix* pMatWorld,
    TMatrix* pMatView, TMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        m_cbData.matWorld = pMatWorld->Transpose();
    }
    if (pMatView != nullptr)
    {
        m_cbData.matView = pMatView->Transpose();
    }
    if (pMatProj != nullptr)
    {
        m_cbData.matProj = pMatProj->Transpose();
    }
}
bool  TModel::LoadObject(std::wstring filename)
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
        PNCT_VERTEX v;
        _fgetts(buffer, 256, fp);
        _stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
            &index,
            &v.pos.x, &v.pos.y, &v.pos.z,
            &v.color.x, &v.color.y, &v.color.z, &v.color.w);
        m_pVertexList.push_back(v);
    }
    fclose(fp);
    return true;
}

HRESULT TModel::CreateConstantBuffer()
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
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}
HRESULT TModel::CreateVertexBuffer()
{
    HRESULT hr = S_OK;
    if (m_pVertexList.size() <= 0) return hr;
    // 로칼->월드->뷰->투영좌표계(NDC)->화면
    //  x = -1.0f ~ 1.0f
    //  y = -1.0f ~ 1.0f
    //  z = 0 ~ 1
    // 시스템 메모리   

     // 그래픽 카드 메모리로 보내야 한다.
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = m_iVertexSize * m_pVertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_pVertexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}
HRESULT TModel::CreateIndexBuffer()
{
    HRESULT hr = S_OK;
    if (m_IndexList.size() <= 0) return hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_IndexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
    if (FAILED(hr)) return hr;

    m_iNumIndex = m_IndexList.size();
    return hr;
}
HRESULT TModel::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout),
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    m_pVSBlob->Release();
    return hr;
}
HRESULT TModel::LoadShader(std::wstring vs, std::wstring ps)
{
    HRESULT hr = S_OK;
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(
        vs.c_str(),
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

    hr = g_pd3dDevice->CreateVertexShader(
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        NULL, &m_pVS);
    if (FAILED(hr)) return hr;

    ID3DBlob* PSBlob = nullptr;
    hr = D3DCompileFromFile(
        ps.c_str(),
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &PSBlob,
        nullptr);
    if (FAILED(hr)) return hr;
    hr = g_pd3dDevice->CreatePixelShader(
        PSBlob->GetBufferPointer(),
        PSBlob->GetBufferSize(),
        NULL, &m_pPS);
    if (FAILED(hr)) return hr;
    PSBlob->Release();
    return hr;
}
bool TModel::CreateVertexData()
{
    if (m_pVertexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool TModel::CreateIndexData()
{
    if (m_IndexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool TModel::Init()
{     
    return false;
}
bool TModel::Create(std::wstring vsFile, std::wstring psFile)
{
    CreateConstantBuffer();
    if (CreateVertexData())
    {
        CreateVertexBuffer();
    }
    if (CreateIndexData())
    {
        CreateIndexBuffer();
    }
    if (SUCCEEDED(LoadShader(vsFile, psFile)))
    {
        if (SUCCEEDED(CreateVertexLayout()))
        {
            return true;
        }        
    }
    return false;
}
bool TModel::Frame()
{     
    m_cbData.vValue.z = g_fGameTimer;
    return true;
}
bool TModel::Render(ID3D11DeviceContext* pContext)
{
    if (PreRender(pContext) ==false) return false;
    if (PostRender(pContext, m_iNumIndex) == false) return false;
    return true;
}
bool TModel::PreRender(ID3D11DeviceContext* pContext)
{
    if (m_pVertexList.size() <= 0) return true;

    pContext->UpdateSubresource(
        m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
    pContext->VSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
    pContext->VSSetShader(m_pVS, NULL, 0);
    pContext->PSSetShader(m_pPS, NULL, 0);
    pContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = m_iVertexSize;
    UINT pOffsets = 0;
    pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    pContext->IASetIndexBuffer(m_pIndexBuffer,
        DXGI_FORMAT_R32_UINT, 0);
    return true;
}
bool TModel::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
    if (iNumIndex > 0)
    {        
        pContext->DrawIndexed(iNumIndex, 0, 0);
    }
    else
    {
        pContext->Draw(m_pVertexList.size(), 0);
    }
    return false;
}
bool TModel::Release()
{
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pVertexLayout);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pConstantBuffer);
    SAFE_RELEASE(m_pVS);
    SAFE_RELEASE(m_pPS);    
    return false;
}
TModel::TModel()
{
    SAFE_ZERO(m_pVertexBuffer);
    SAFE_ZERO(m_pIndexBuffer);
    SAFE_ZERO(m_pVertexLayout);
    SAFE_ZERO(m_pConstantBuffer);
    SAFE_ZERO(m_pVS);
    SAFE_ZERO(m_pPS);
    m_iVertexSize = sizeof(PNCT_VERTEX);
    m_iNumIndex = 0;    
}
TModel::~TModel()
{

}
