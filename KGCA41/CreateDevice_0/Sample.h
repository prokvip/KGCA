#pragma once
#include "TCore.h"
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib") // d3d11.dll
class Sample : public TCore
{
    IDXGISwapChain*     m_pSwapChain = nullptr;
    ID3D11Device*       m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
    ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};

