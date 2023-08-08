#include "Sample.h"
bool  Sample::Init() 
{ 
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));    
    SwapChainDesc.BufferDesc.Width = 800;
    SwapChainDesc.BufferDesc.Height = 600;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator=1;
    SwapChainDesc.BufferDesc.Format= DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.SampleDesc.Count = 1;  
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = m_hWnd;
    SwapChainDesc.Windowed = true;

    D3D_DRIVER_TYPE DriverType= D3D_DRIVER_TYPE_HARDWARE;
    UINT Flags = 0;
    D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
    // 1) ����̽�
    D3D11CreateDeviceAndSwapChain(
        NULL,
        DriverType,
        NULL,
        Flags,
        &pFeatureLevels,     1,
        D3D11_SDK_VERSION,
        &SwapChainDesc,

        &m_pSwapChain,   // ������������̽�
        &m_pDevice,      // dx �������̽�( ���� )
        NULL,
        &m_pImmediateContext); // dx �������̽�( ���� )
    // 2) ����� ��
    // 3) ����Ÿ�� �����ϰ�
    // 4) �ÿ� �Ѵ�.
    return true; 
}
bool  Sample::Frame() 
{ 
    return true; 
}
bool  Sample::Render() 
{ 
    return true; 
}
bool  Sample::Release()
{ 
    m_pSwapChain->Release();
    m_pDevice->Release();
    m_pImmediateContext->Release();
    return true; 
}

TGAME(L"kgca", 300, 300)