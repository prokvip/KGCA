#include "pch.h"
#include "TShader.h"
#include "TShaderManager.h"
bool	TShader::Init()
{
    return true;
}
bool	TShader::Frame() {
    return true;
}
bool	TShader::Render(ID3D11DeviceContext* pContext) {
    return true;
}
bool	TShader::Release() {
    if (m_pVS) m_pVS->Release();
    if (m_pPS) m_pPS->Release();
    if (m_pVSCode) m_pVSCode->Release();
    if (m_pPSCode) m_pPSCode->Release();

    m_pVS = nullptr;
    m_pPS = nullptr;
    m_pVSCode = nullptr;
    m_pPSCode = nullptr;
    return true;
}
HRESULT TShader::Load(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,std::wstring filename)
{
    HRESULT hr;
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pImmediateContext;

    DWORD dwShaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
#if defined( _DEBUG ) 
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    // �������̴� ������ 
    const D3D_SHADER_MACRO defines[] =
    {
        "EXAMPLE_DEFINE", "1",
        NULL, NULL
    };

    // �ʼ� (VS & PS)
    ID3DBlob* pErrorCode = nullptr;
    hr = D3DCompileFromFile(
        filename.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS",
        "vs_5_0",
        dwShaderFlags,
        0,
        &m_pVSCode,
        &pErrorCode);
    if (FAILED(hr))
    {
        if (pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            pErrorCode->Release();
        }
        return hr;
    }
    hr = m_pd3dDevice->CreateVertexShader(m_pVSCode->GetBufferPointer(),m_pVSCode->GetBufferSize(),NULL, &m_pVS);
    // �Ƚ����̴� ������  
    hr = D3DCompileFromFile(
        filename.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS",
        "ps_5_0",
        dwShaderFlags,
        0,
        &m_pPSCode,
        &pErrorCode);
    if (FAILED(hr))
    {
        if (pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            MessageBoxA(NULL, (char*)pErrorCode->GetBufferPointer(), "VS Compile", MB_OK);
            pErrorCode->Release();
        }
        return hr;
    }
    hr = m_pd3dDevice->CreatePixelShader(m_pPSCode->GetBufferPointer(),m_pPSCode->GetBufferSize(),NULL, &m_pPS);
    if (FAILED(hr))
    {
        if (pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            MessageBoxA(NULL, (char*)pErrorCode->GetBufferPointer(), "PS Compile", MB_OK);
            pErrorCode->Release();
        }
        return hr;
    }

    // ����( HS & DS)
    ComPtr<ID3DBlob> m_pBlob;
    hr = D3DCompileFromFile(
        filename.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "HS",
        "hs_5_0",
        dwShaderFlags,
        0,
        m_pBlob.GetAddressOf(),
        &pErrorCode);
    if (SUCCEEDED(hr))
    {
        hr = m_pd3dDevice->CreateHullShader(m_pBlob->GetBufferPointer(),m_pBlob->GetBufferSize(),NULL,m_pHS.GetAddressOf());
        if (FAILED(hr) && pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            MessageBoxA(NULL, (char*)pErrorCode->GetBufferPointer(), "HS Compile", MB_OK);
            pErrorCode->Release();
            return hr;
        }       
    }  

    m_pBlob.Reset();
    hr = D3DCompileFromFile(
        filename.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "DS",
        "ds_5_0",
        dwShaderFlags,
        0,
        m_pBlob.GetAddressOf(),
        &pErrorCode);
    if (SUCCEEDED(hr))
    {
        hr = m_pd3dDevice->CreateDomainShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), NULL, m_pDS.GetAddressOf());
        if (FAILED(hr) && pErrorCode)
        {
            OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
            MessageBoxA(NULL, (char*)pErrorCode->GetBufferPointer(), "DS Compile", MB_OK);
            pErrorCode->Release();
            return hr;
        }
    }  
    return S_OK;
}