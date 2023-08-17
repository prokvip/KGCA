#include "TShaderMgr.h"
bool  TShader::Release()
{
    if(m_VertexShaderCode) m_VertexShaderCode->Release();
    if (m_pVS) m_pVS->Release();
    if (m_pPS) m_pPS->Release();
    if (m_pDS) m_pDS->Release();
    if (m_pHS) m_pHS->Release();
    if (m_pGS) m_pGS->Release();
    if (m_pCS) m_pCS->Release();
    m_VertexShaderCode = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
    m_pDS = nullptr;
    m_pHS = nullptr;
    m_pGS = nullptr;
    m_pCS = nullptr;
    return true;
}
bool  TShader::Load(ID3D11Device* pDevice, std::wstring filename)
{
    if (LoadVertexShader(pDevice, filename)==false)
    {
        
    }   
    if (LoadPixelShader(pDevice, filename)==false)
    {

    }
    return true;
}
bool  TShader::LoadVertexShader(ID3D11Device* pDevice, std::wstring filename)
{
    ID3DBlob* ErrorCode;
    // 쉐이더 컴파일
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
        nullptr,
        nullptr,
        "VS",
        "vs_5_0",
        0,
        0,
        &m_VertexShaderCode,
        &ErrorCode);
    if (FAILED(hr))
    {
        //ErrorCode
        TCHAR pMessage[500];
        mbstowcs(pMessage, (CHAR*)ErrorCode->GetBufferPointer(), 500);
        MessageBox(NULL, pMessage, L"ERROR", MB_OK);
        if (ErrorCode) ErrorCode->Release();
        return false;
    }
    if (ErrorCode) ErrorCode->Release();

    hr = pDevice->CreateVertexShader(
        m_VertexShaderCode->GetBufferPointer(),
        m_VertexShaderCode->GetBufferSize(),
        nullptr,
        &m_pVS);

    if (FAILED(hr))
    {        
        return false;
    }    
    return true;
}
bool  TShader::LoadPixelShader(ID3D11Device* pDevice, std::wstring filename)
{
    ID3DBlob* ShaderCode;
    ID3DBlob* ErrorCode;
    // 쉐이더 컴파일
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
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
        TCHAR pMessage[500];
        mbstowcs(pMessage, (CHAR*)ErrorCode->GetBufferPointer(), 500);
        MessageBox(NULL, pMessage, L"ERROR", MB_OK);
        if (ErrorCode) ErrorCode->Release();
        return false;
    }
    if (ErrorCode) ErrorCode->Release();

    hr = pDevice->CreatePixelShader(
        ShaderCode->GetBufferPointer(),
        ShaderCode->GetBufferSize(),
        nullptr,
        &m_pPS);

    if (ShaderCode) ShaderCode->Release();
    if (FAILED(hr))
    {        
        return false;
    }    
    return true;
}

void  TShaderMgr::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext)
{
    m_pDevice = pDevice;
    m_pImmediateContext = pImmediateContext;
}
const TShader* TShaderMgr::Load(std::wstring szPullfilePath)
{
    std::size_t found = szPullfilePath.find_last_of(L"/");
    std::wstring path = szPullfilePath.substr(0, found + 1);
    std::wstring key = szPullfilePath.substr(found + 1);
    const TShader* data = GetPtr(key);
    if (data != nullptr)
    {
        return data;
    }
    TShader* newData = new TShader;
    if (newData->Load(m_pDevice, szPullfilePath))
    {
        m_list.insert(std::make_pair(key, newData));
        return newData;
    }
    delete newData;
    return nullptr;
}
const TShader* TShaderMgr::GetPtr(std::wstring key)
{
    auto iter = m_list.find(key);
    if (m_list.end() == iter)
    {
        return nullptr;
    }
    return iter->second;
}
bool TShaderMgr::Get(std::wstring key, TShader& ret)
{
    auto iter = m_list.find(key);
    if (m_list.end() == iter)
    {
        return false;
    }
    ret = *(iter->second);
    return true;
}
TShaderMgr::TShaderMgr()
{

}
TShaderMgr::~TShaderMgr()
{
    for (auto& data : m_list)
    {
        data.second->Release();
        delete data.second;
    }
    m_list.clear();
}