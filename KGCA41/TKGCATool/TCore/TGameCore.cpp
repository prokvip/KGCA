#include "pch.h"
#include "TGameCore.h"
HRESULT TGameCore::CreateDXResource()
{
	m_Writer.Init();
	IDXGISurface1* pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1),
		(void**)&pBackBuffer);
	m_Writer.Set(pBackBuffer);
	pBackBuffer->Release();
	return S_OK;
}
HRESULT TGameCore::DeleteDXResource()
{
	m_Writer.DeleteDXResource();
	return S_OK;
}
bool		TGameCore::TCoreInit()
{
	TDevice::Init();
	TDxState::SetState(m_pd3dDevice.Get());
	I_Sprite.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	I_Tex.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	I_Shader.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	I_Sound.Init();
	I_Input.Init();
	I_Timer.Init();
	m_Writer.Init();

	ComPtr<IDXGISurface1> pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1),(void**)&pBackBuffer);
	m_Writer.Set(pBackBuffer.Get());

	std::wstring shaderfilename = L"DefaultRT.hlsl";
	m_BG.Create(m_pd3dDevice.Get(),m_pImmediateContext.Get(), shaderfilename, L"../../data/kgcabk.bmp");
	m_RT.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), 4096*2, 4096*2);
	return Init();
}
bool		TGameCore::TCoreFrame()
{
	I_Input.Frame();
	I_Timer.Frame();
	I_Sound.Frame();
	m_Writer.Frame();
	PreFrame();
    Frame();
	PostFrame();
	return true;
}
bool		TGameCore::TCorePreRender()
{
	m_pImmediateContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(),m_pDepthStencilView.Get());
	float color[4] = { 0.34324f,0.52342f,0.798320f,1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRTV.Get(), color);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(),D3D11_CLEAR_DEPTH| D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::g_pDefaultSSMirror);
	m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pImmediateContext->RSSetViewports(1, &m_vp);
	m_pImmediateContext->RSSetState(TDxState::g_pDefaultRSSolid);
	m_pImmediateContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDefaultDepthStencil,0xff);
    return true;
}
bool		TGameCore::TCoreRender()
{  	
	TCorePreRender();

	PreRender(m_pImmediateContext.Get());
	if (m_bUsedRT)
	{
		// ����Ÿ�� ����
		m_RT.m_pOldRTV = m_pRTV.Get();
		m_RT.m_pOldDSV = m_pDepthStencilView.Get();
		m_RT.m_vpOld[0] = m_vp;
		//ID3D11DeviceContext::OMSetRenderTargets: Resource being set to OM RenderTarget slot 0 is still bound on input
		if (m_RT.Begin(m_pImmediateContext.Get()))
		{
			Render(m_pImmediateContext.Get());
			m_RT.End(m_pImmediateContext.Get());
		}
		if (m_RT.m_pSRV)
		{
			//m_BG.m_pTextureSRV = m_RT.m_pDsvSRV.Get();
			m_BG.m_pTextureSRV = m_RT.m_pSRV.Get();
		}
	}
	else
	{
		Render(m_pImmediateContext.Get());
	}
	TCorePostRender();
    return true;
}
bool		TGameCore::UIRender(ID3D11DeviceContext* pContext)
{
	return true;
}
bool		TGameCore::TCorePostRender()
{
	PostRender(m_pImmediateContext.Get());
	if (m_bUsedRT)
	{
		m_pImmediateContext.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_BG.SetMatrix(nullptr, nullptr, nullptr);
		m_BG.Render(m_pImmediateContext.Get());
	}

	UIRender(m_pImmediateContext.Get());

	I_Input.Render(m_pImmediateContext.Get());
	I_Timer.Render(m_pImmediateContext.Get());
	m_Writer.m_szDefaultText = I_Timer.m_szTimer;
	m_Writer.Render(m_pImmediateContext.Get());

	m_pSwapChain->Present(0, 0);

    return true;
}
bool		TGameCore::TCoreRelease()
{   
	m_RT.Release();
	m_BG.Release();
	Release();
	I_Input.Release();
	I_Timer.Release();
	m_Writer.Release();
	I_Sound.Release();
	TDxState::Release();
	TDevice::Release();
    return true;
}
bool        TGameCore::Run()
{    
	TCoreInit();	
	while (m_bGameRun)
	{
		PreProcess();
		if (TWindow::Run() == true)
		{
			TCoreFrame();
			TCoreRender();			
		}
		else
		{
			m_bGameRun = false;
		}
		PostProcess();
	}
	TCoreRelease();
	return true;
}
bool        TGameCore::ToolRun()
{
	//TCoreInit();
	//while (m_bGameRun)
	{
		if( !TCoreFrame() || !TCoreRender() )
		{
			m_bGameRun = false;
			return false;
		}
	}
	//TCoreRelease();
	return true;
}