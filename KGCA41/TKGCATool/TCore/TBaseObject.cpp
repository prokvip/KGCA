#include "pch.h"
#include "TBaseObject.h"
namespace TDX
{
    ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice,
        void* pDataAddress,
        UINT iNumVertex,
        UINT iVertexSize)
    {
        HRESULT hr;
        ID3D11Buffer* pVB = nullptr;
        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = iVertexSize * iNumVertex; // ����Ʈ �뷮
        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = pDataAddress;
        hr = pd3dDevice->CreateBuffer(
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
            &pVB);
        return pVB;
    }
    void		TBaseObject::CreateConstantData()
    {
        D3DXMatrixIdentity(&m_cbData.matWorld);
        D3DXMatrixIdentity(&m_cbData.matView);
        D3DXMatrixIdentity(&m_cbData.matProj);
        D3DXMatrixIdentity(&m_cbData.matWorldInverse);
        m_cbData.fTimer = 0.0f;
        D3DXMatrixTranspose(&m_cbData.matWorld, &m_cbData.matWorld);
        D3DXMatrixTranspose(&m_cbData.matView, &m_cbData.matView);
        D3DXMatrixTranspose(&m_cbData.matProj, &m_cbData.matProj);
        D3DXMatrixTranspose(&m_cbData.matWorldInverse, &m_cbData.matWorldInverse);
    }
    HRESULT		TBaseObject::CreateConstantBuffer()
    {
        HRESULT hr;
        CreateConstantData();
        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(VS_CONSTANT_BUFFER) * 1; // ����Ʈ �뷮
        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_cbData;
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
            &m_pConstantBuffer);
        return hr;
    }
    void    TBaseObject::CreateVertexData()
    {
        if (m_VertexList.size() > 0)
        {
            m_InitVertexList = m_VertexList;
            return;
        }
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
        if (m_IndexList.size() > 0)return;
        // �������ۿ� �ε���
        m_IndexList.resize(6);
        m_IndexList[0] = 0;
        m_IndexList[1] = 1;
        m_IndexList[2] = 2;
        m_IndexList[3] = 2;
        m_IndexList[4] = 1;
        m_IndexList[5] = 3;

        m_dwFace = m_IndexList.size() / 3;
    }
    HRESULT TBaseObject::CreateVertexBuffer()
    {
        HRESULT hr;
        CreateVertexData();
        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size(); // ����Ʈ �뷮
        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_VertexList.at(0);
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
            &m_pVertexBuffer);
        return hr;
    }
    HRESULT TBaseObject::CreateIndexBuffer()
    {
        HRESULT hr=S_OK;

        CreateIndexData();
        if (m_IndexList.size() == 0) return hr;

        D3D11_BUFFER_DESC       bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth = sizeof(DWORD) * m_IndexList.size(); // ����Ʈ �뷮
        // GPU �޸𸮿� �Ҵ�
        bd.Usage = D3D11_USAGE_DEFAULT; // ������ �Ҵ� ��� ������ ���ۿ뵵
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA  sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = &m_IndexList.at(0);
        hr = m_pd3dDevice->CreateBuffer(
            &bd, // ���� �Ҵ�
            &sd, // �ʱ� �Ҵ�� ���۸� ü��� CPU�޸� �ּ�
            &m_pIndexBuffer);
        return hr;
    }
    bool TBaseObject::CreateShader(std::wstring filename)
    {
        m_pShader = I_Shader.Load(filename);
        if (m_pShader)
        {
            m_pVS = m_pShader->m_pVS;
            m_pPS = m_pShader->m_pPS;
            m_pGS = m_pShader->m_pGS.Get();
            m_pHS = m_pShader->m_pHS.Get();
            m_pDS = m_pShader->m_pDS.Get();
            m_pCS = m_pShader->m_pCS.Get();
            m_pVSCode = m_pShader->m_pVSCode;
            return true;
        }
        return false;
    }
    HRESULT TBaseObject::CreateVertexShader(std::wstring filename)
    {
        HRESULT hr;
        // �������̴� ������ 
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
        // �Ƚ����̴� ������  
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
        if (m_pVSCode == nullptr)
        {
            return E_FAIL;
        }
        // �������̾ƿ��� �������̴� ������ ����.
        // �������̾ƿ� ������ ������ �������̴� ������ �ʿ��ϴ�.
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,24,D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0,40,D3D11_INPUT_PER_VERTEX_DATA, 0},
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
    bool	TBaseObject::Create(
        ID3D11Device* pd3dDevice,// ����̽� ��ü
        ID3D11DeviceContext* pImmediateContext,
        std::wstring shadername, std::wstring texturename)
    {
        m_pd3dDevice = pd3dDevice;
        m_pImmediateContext = pImmediateContext;
        m_szShaderName = shadername;
        m_szTextureName = texturename;
        if (FAILED(CreateConstantBuffer()))
        {
            return false;
        }
        if (FAILED(CreateVertexBuffer()))
        {
            return false;
        }
        if (FAILED(CreateIndexBuffer()))
        {
            return false;
        }
        if (FAILED(CreateShader(shadername)))
        {
            return false;
        }
        /*if (FAILED(CreateVertexShader(shadername)))
        {
            return false;
        }
        if (FAILED(CreatePixelShader(shadername)))
        {
            return false;
        }*/
        if (FAILED(CreateVertexLayout()))
        {
            return false;
        }
        if (LoadTexture(texturename))
        {
            m_pTextureSRV = m_pTexture->m_pTextureSRV;
        }

        if (!Init())
        {
            return false;
        }
        return true;
    }
    bool    TBaseObject::LoadTexture(W_STR texturename)
    {
        m_pTexture = I_Tex.Load(texturename);
        if (m_pTexture != nullptr)
        {
            m_pTextureSRV = m_pTexture->m_pTextureSRV;
            return true;
        }
        return false;
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
    void   TBaseObject::UpdateConstantBuffer()
    {
        m_cbData.matWorld = m_matWorld.Transpose();
        m_cbData.matView = m_matView.Transpose();
        m_cbData.matProj = m_matProj.Transpose();
        m_cbData.matWorldInverse = m_matWorld.Invert();
        m_pImmediateContext->UpdateSubresource( m_pConstantBuffer, 0, nullptr,    &m_cbData, 0, 0);
    }
    void	TBaseObject::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
    {
        if (matWorld != nullptr)
        {
            m_matWorld = *matWorld;
        }
        if (matView != nullptr)
        {
            m_matView = *matView;
        }
        if (matProj != nullptr)
        {
            m_matProj = *matProj;
        }

        UpdateConstantBuffer();
    }
    bool TBaseObject::RenderShadow(ID3D11DeviceContext* pContext)
    {
        return true;
    }
    bool TBaseObject::Render(ID3D11DeviceContext* pContext)
    {
        PreRender(pContext);
        PostRender(pContext);
        return true;
    }
    bool TBaseObject::PreRender(ID3D11DeviceContext* pContext)
    {
        pContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
        pContext->IASetInputLayout(m_pVertexLayout);
        pContext->VSSetShader(m_pVS, NULL, 0);
        pContext->PSSetShader(m_pPS, NULL, 0);
        pContext->GSSetShader(m_pGS, NULL, 0);
        pContext->HSSetShader(m_pHS, NULL, 0);
        pContext->DSSetShader(m_pDS, NULL, 0);
        pContext->CSSetShader(m_pCS, NULL, 0);
        UINT stride = sizeof(PNCT_VERTEX); // ����1���� ����Ʈ�뷮
        UINT offset = 0; // �������ۿ��� �������(����Ʈ)
        pContext->IASetVertexBuffers(0, 1,&m_pVertexBuffer, &stride, &offset);
        pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
        pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
        pContext->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);
        pContext->DSSetConstantBuffers(0, 1, &m_pConstantBuffer);
        return true;
    }
    bool TBaseObject::PostRender(ID3D11DeviceContext* pContext)
    {
        if (m_pIndexBuffer == nullptr)
            pContext->Draw(m_VertexList.size(), 0);
        else
            pContext->DrawIndexed(m_dwFace * 3, 0, 0);

        return true;
    }
    bool TBaseObject::Release()
    {
        m_VertexList.clear();
        m_InitVertexList.clear();
        m_IndexList.clear();

        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        if (m_pIndexBuffer) m_pIndexBuffer->Release();
        if (m_pConstantBuffer) m_pConstantBuffer->Release();
        if (m_pVertexLayout) m_pVertexLayout->Release();

        m_pVS = nullptr;
        m_pPS = nullptr;
        m_pHS = nullptr;
        m_pDS = nullptr;
        m_pVertexBuffer = nullptr;
        m_pIndexBuffer = nullptr;
        m_pConstantBuffer = nullptr;
        m_pVertexLayout = nullptr;       
        m_pTextureSRV = nullptr;
        return true;
    }

};