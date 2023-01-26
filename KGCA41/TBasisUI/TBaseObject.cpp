#include "TBaseObject.h"
namespace TUI
{
    void    TBaseObject::CreateVertexData()
    {
        m_VertexList.resize(4);
        m_VertexList[0].p = { -1.0f, 1.0f, 0.0f };
        m_VertexList[1].p = { +1.0f, 1.0f,  0.0f };
        m_VertexList[2].p = { -1.0f, -1.0f, 0.0f };
        m_VertexList[3].p = { 1.0f, -1.0f, 0.0f };

        m_VertexList[0].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[1].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[2].c = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_VertexList[3].c = { 1.0f, 1.0f, 1.0f, 1.0f };

        m_VertexList[0].t = { 0.0f, 0.0f };
        m_VertexList[1].t = { 1.0f, 0.0f };
        m_VertexList[2].t = { 0.0f, 1.0f };
        m_VertexList[3].t = { 1.0f, 1.0f };

        m_InitVertexList = m_VertexList;
    }
    void    TBaseObject::CreateIndexData()
    {
        // 정점버퍼에 인덱스
        m_IndexList.resize(6);
        m_IndexList[0] = 0;
        m_IndexList[1] = 1;
        m_IndexList[2] = 2;
        m_IndexList[3] = 2;
        m_IndexList[4] = 1;
        m_IndexList[5] = 3;
    }
    HRESULT TBaseObject::CreateVertexBuffer()
    {
        HRESULT hr;
        CreateVertexData();
        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = (UINT)(sizeof(SimpleVertex) * m_VertexList.size()); // 바이트 용량
        // GPU 메모리에 할당
        bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_VertexList.at(0);
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // 버퍼 할당
            &sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
            &m_pVertexBuffer);
        return hr;
    }
    HRESULT TBaseObject::CreateIndexBuffer()
    {
        HRESULT hr;

        CreateIndexData();

        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = (UINT)(sizeof(DWORD) * m_IndexList.size()); // 바이트 용량
        // GPU 메모리에 할당
        bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_IndexList.at(0);
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // 버퍼 할당
            &sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
            &m_pIndexBuffer);
        return hr;
    }
    bool TBaseObject::CreateShader(std::wstring filename)
    {
        m_pShader = I_ShaderUI.Load(filename);
        if (m_pShader)
        {
            m_pVS = m_pShader->m_pVS;
            m_pPS = m_pShader->m_pPS;
            m_pVSCode = m_pShader->m_pVSCode;
            return true;
        }
        return false;
    }
    HRESULT TBaseObject::CreateVertexShader(std::wstring filename)
    {
        HRESULT hr;
        // 정점쉐이더 컴파일 
        ID3DBlob* pErrorCode = nullptr;
        hr = D3DCompileFromFile(
            filename.c_str(),
            NULL,
            NULL,
            "VS",
            "vs_5_0",
            0,
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
            return false;
        }
        hr = m_pd3dDevice->CreateVertexShader(
            m_pVSCode->GetBufferPointer(),
            m_pVSCode->GetBufferSize(),
            NULL,
            &m_pVS);
        return hr;
    }
    HRESULT TBaseObject::CreatePixelShader(std::wstring filename)
    {
        HRESULT hr;
        ID3DBlob* pErrorCode = nullptr;
        // 픽쉘쉐이더 컴파일  
        hr = D3DCompileFromFile(
            filename.c_str(),
            NULL,
            NULL,
            "PS",
            "ps_5_0",
            0,
            0,
            &m_pPSCode,
            &pErrorCode);
        if (FAILED(hr))
        {
            if (pErrorCode)
            {
                OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
                pErrorCode->Release();
            }
            return false;
        }
        hr = m_pd3dDevice->CreatePixelShader(
            m_pPSCode->GetBufferPointer(),
            m_pPSCode->GetBufferSize(),
            NULL,
            &m_pPS);
        if (FAILED(hr))
        {
            if (pErrorCode)
            {
                OutputDebugStringA((char*)pErrorCode->GetBufferPointer());
                pErrorCode->Release();
            }
            return false;
        }
        return hr;
    }
    HRESULT TBaseObject::CreateVertexLayout()
    {
        HRESULT hr;
        // 정점레이아웃은 정점쉐이더 밀접한 관련.
        // 정점레이아웃 생성시 사전에 정점쉐이더 생성이 필요하다.
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0,28,D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        UINT NumElements = sizeof(ied) / sizeof(ied[0]);
        hr = m_pd3dDevice->CreateInputLayout(
            ied,
            NumElements,
            m_pVSCode->GetBufferPointer(),
            m_pVSCode->GetBufferSize(),
            &m_pVertexLayout);

        return hr;
    }
    bool	TBaseObject::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        const std::wstring shadername, const std::wstring texturename)
    {
        _ASSERT(pd3dDevice);
        _ASSERT(pImmediateContext);

        m_pd3dDevice = pd3dDevice;
        m_pImmediateContext = pImmediateContext;
        m_szShaderName = shadername;
        m_szTextureName = texturename;
        if (FAILED(CreateVertexBuffer()))
        {
            return false;
        }
        if (FAILED(CreateIndexBuffer()))
        {
            return false;
        }
        if (!m_szShaderName.empty() && FAILED(CreateShader(shadername)))
        {
            return false;
        }
        /*if (!m_szShaderName.empty() && FAILED(CreateVertexShader(shadername)))
        {
            return false;
        }
        if (!m_szShaderName.empty() && FAILED(CreatePixelShader(shadername)))
        {
            return false;
        }*/
        if (m_pVSCode != nullptr && FAILED(CreateVertexLayout()))
        {
            return false;
        }

        if (!texturename.empty())
        {
            m_pTexture = I_TexUI.Load(texturename);
            if (m_pTexture != nullptr)
            {
                m_pTextureSRV = m_pTexture->m_pTextureSRV;
            }
        }
        return true;
    }
    bool	TBaseObject::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
        TShader* pShader, TTexture* pTexture)
    {
        _ASSERT(pd3dDevice);
        _ASSERT(pImmediateContext);
        _ASSERT(pShader);
        _ASSERT(pTexture);
        m_pd3dDevice = pd3dDevice;
        m_pImmediateContext = pImmediateContext;
        m_szShaderName = pShader->m_strFileName;
        m_szTextureName = pTexture->m_strFileName;
        if (FAILED(CreateVertexBuffer()))
        {
            return false;
        }
        if (FAILED(CreateIndexBuffer()))
        {
            return false;
        }
        m_pShader = pShader;
        if (m_pShader)
        {
            m_pVS = m_pShader->m_pVS;
            m_pPS = m_pShader->m_pPS;
            m_pVSCode = m_pShader->m_pVSCode;
        }
        if (m_pVSCode != nullptr && FAILED(CreateVertexLayout()))
        {
            return false;
        }
        m_pTexture = pTexture;
        if (m_pTexture != nullptr)
        {
            m_pTextureSRV = m_pTexture->m_pTextureSRV;
        }
        return true;
    }
    bool	TBaseObject::Init()
    {
        return true;
    }
    bool TBaseObject::Frame()
    {

        return true;
    }
    void   TBaseObject::UpdateVertexBuffer()
    {
        m_pImmediateContext->UpdateSubresource(
            m_pVertexBuffer, 0, nullptr,
            &m_VertexList.at(0), 0, 0);
    }

    bool TBaseObject::Render()
    {
        PreRender();
        PostRender();
        return true;
    }
    bool TBaseObject::PreRender()
    {
        m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
        m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
        m_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
        m_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
        UINT stride = sizeof(SimpleVertex); // 정점1개의 바이트용량
        UINT offset = 0; // 정점버퍼에서 출발지점(바이트)
        m_pImmediateContext->IASetVertexBuffers(0, 1,&m_pVertexBuffer, &stride, &offset);
        m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer,  DXGI_FORMAT_R32_UINT, 0);
        m_pImmediateContext->OMSetDepthStencilState(TDxStateUI::g_pDSSDepthDisable, 0xff);
        return true;
    }
    bool TBaseObject::PostRender()
    {
        if (m_pIndexBuffer == nullptr)
            m_pImmediateContext->Draw((UINT)m_VertexList.size(), 0);
        else
            m_pImmediateContext->DrawIndexed((UINT)m_IndexList.size(), 0, 0);

        return true;
    }
    bool TBaseObject::Release()
    {
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        if (m_pIndexBuffer) m_pIndexBuffer->Release();
        if (m_pVertexLayout) m_pVertexLayout->Release();
        return true;
    }

    TBaseObject::TBaseObject()
    {}
    TBaseObject::~TBaseObject()
    {
    }
};