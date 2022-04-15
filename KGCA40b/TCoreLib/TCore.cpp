#include "TCore.h"
#include "TObjectMgr.h"
#include "TSoundMgr.h"

TBoxObj* g_pBoxDebug = nullptr;
bool	TCore::CoreInit()
{
	m_GameTimer.Init();	
	TInput::Get().Init();
	if( SUCCEEDED(InitDeivice()))
	{
		I_Shader.Set(m_pd3dDevice.Get());
		I_Texture.Set(m_pd3dDevice.Get());
		TDxState::SetState(m_pd3dDevice.Get());
		if (m_dxWrite.Init())
		{
			IDXGISurface1* pSurface = nullptr;
			HRESULT hr = m_pSwapChain->GetBuffer(0, 
				__uuidof(IDXGISurface1),
				(void**)&pSurface);
			if (SUCCEEDED(hr))
			{
				m_dxWrite.SetRenderTarget(pSurface);
			}
			if (pSurface) pSurface->Release();
		}
	}

	m_DefaultCamera.Init();
	m_DefaultCamera.CreateViewMatrix(T::TVector3(0, 500.0f, -100.0f),
		T::TVector3(0, 0.0f, 0));
	m_DefaultCamera.CreateProjMatrix(XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 5000.0f);
	m_DefaultCamera.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_DefaultCamera.m_pVShader = I_Shader.CreateVertexShader(
		m_pd3dDevice.Get(), L"../../data/shader/Box.hlsl", "VSColor");;
	m_DefaultCamera.m_pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice.Get(), L"../../data/shader/Box.hlsl", "PSColor");;
	m_DefaultCamera.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_DefaultCamera.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return false;
	}

	m_pMainCamera = &m_DefaultCamera;


	m_SkyObj.Init();
	m_SkyObj.SetPosition(T::TVector3(0.0f, 0.0f, 0.0f));

	if (!m_SkyObj.Create(m_pd3dDevice.Get(),
		m_pImmediateContext.Get(),
		L"../../data/shader/sky.hlsl",
		L"../../data/sky/skybox02.dds"))
	{
		return false;
	}


	DrawDebugInit(m_pd3dDevice.Get(),m_pImmediateContext.Get());
	g_pBoxDebug = &m_BoxDebug;

	Init();

	return true;
}
bool	TCore::GameRun()
{
	CoreInit();
	while(WinRun())
	{
		CoreFrame();
		CoreRender();
	}
	CoreRelease();

	//MemoryReporting();
	return true;
}
bool	TCore::CoreFrame()
{
	if (TInput::Get().GetKey('V')== KEY_PUSH)
	{
		m_bWireFrame = !m_bWireFrame;
	}
	m_GameTimer.Frame();
	TInput::Get().Frame();
	m_pMainCamera->Frame();
	m_SkyObj.Frame();
	I_ObjectMgr.Frame();
	I_Sound.Frame();
	Frame();
	m_dxWrite.Frame();
	return true;
}
bool	TCore::CoreRender()
{	
	float color[4] = { 0.1543f, 0.23421f, 0.4323f,1.0f };
	//float color[4] = { 1, 0, 0,1.0f };
	m_pImmediateContext->ClearRenderTargetView(
		m_pRenderTargetView.Get(),
		color);
	m_pImmediateContext->ClearDepthStencilView(
		m_pDepthStencilView.Get(), 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->OMSetRenderTargets(1, 
		m_pRenderTargetView.GetAddressOf(),
		m_pDepthStencilView.Get());

	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);	
	m_pImmediateContext->PSSetSamplers(1, 1, &TDxState::m_pSSPoint);

	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDSSDepthEnable, 0x00);
	if( m_bWireFrame)
		m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullWireFrame);
	else
		m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullSolid);

	m_SkyObj.m_matView = m_pMainCamera->m_matView;
	m_SkyObj.m_matView._41 = 0;
	m_SkyObj.m_matView._42 = 0;
	m_SkyObj.m_matView._43 = 0;
	T::TMatrix matRotation, matScale;
	/*T::D3DXMatrixScaling(&matScale, 3000.0f, 3000.0f, 3000.0f);
	T::D3DXMatrixRotationY(&matRotation, g_fGameTimer * 0.00f);
	m_SkyObj.m_matWorld = matScale * matRotation;*/
	m_SkyObj.SetMatrix(NULL, &m_SkyObj.m_matView, &m_pMainCamera->m_matProj);
	m_pImmediateContext->RSSetState(TDxState::g_pRSNoneCullSolid);
	//m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	//m_pImmediateContext->PSSetSamplers(1, 1, &TDxState::m_pSSPoint);
	m_SkyObj.Render();
	m_pImmediateContext->RSSetState(TDxState::g_pRSBackCullSolid);
	// ����ۿ� ������ �Ѵ�.
	Render();

	m_GameTimer.Render();
	TInput::Get().Render();

	m_dxWrite.Render();
	m_pSwapChain->Present(0, 0);
	return true;
}

bool	TCore::CoreRelease()
{
	Release();
	m_BoxDebug.Release();
	m_SkyObj.Release();
	m_DefaultCamera.Release();
	TDxState::Release();
	m_dxWrite.Release();
	m_GameTimer.Release();
	TInput::Get().Release();
	CleapupDevice();	
	return true;
}
void     TCore::ResizeDevice(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == nullptr) return;
	DeleteResizeDevice(iWidth, iHeight);

	m_dxWrite.DeleteDeviceResize();
		
		TDevice::ResizeDevice(iWidth, iHeight);
	
	IDXGISurface1* pSurface = nullptr;
	HRESULT hr = m_pSwapChain->GetBuffer(0,
		__uuidof(IDXGISurface1),
		(void**)&pSurface);
	if (SUCCEEDED(hr))
	{
		m_dxWrite.SetRenderTarget(pSurface);
	}
	if (pSurface) pSurface->Release();

	CreateResizeDevice(iWidth, iHeight);
}

void TCore::DrawDebugInit(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pContext)
{
	m_BoxDebug.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_BoxDebug.m_pVShader = I_Shader.CreateVertexShader(
		pd3dDevice, L"Box.hlsl", "VSColor");
	m_BoxDebug.m_pPShader = I_Shader.CreatePixelShader(
		pd3dDevice, L"Box.hlsl", "PSColor");
	m_BoxDebug.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_BoxDebug.Create(pd3dDevice, pContext))
	{
		return;
	}
}


TCore::TCore()
{

}
TCore::~TCore()
{
	
}