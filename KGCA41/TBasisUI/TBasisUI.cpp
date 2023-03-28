#include "TBasisUI.h"
#include "TInputUI.h"
#include "TTimerUI.h"
#include "TWriter.h"
#include "TDxStateUI.h"
#include "TTextureUIManager.h"
#include "TShaderUIManager.h"
#include "TSpriteManager.h"
namespace TUI
{
	HWND g_hWnd;
	RECT g_rtClient;

	bool TBasisUI::Init()
	{		
		I_Sprite.Load(L"../../data/script/ui.txt");
		return true;
	}
	bool TBasisUI::Frame(double fTime, float fElapsedTime)
	{
		I_InputUI.Frame();
		I_TimerUI.Frame();		
		I_Writer.Frame();
	/*	for (auto data = m_pPriorityQueue.rbegin(); data != m_pPriorityQueue.rend(); data++)
		{
			data->second->Frame();
		}*/
		return true;
	}
	bool TBasisUI::Render(double fTime, float fElapsedTime)
	{
		//for (auto data = m_pPriorityQueue.rbegin(); data != m_pPriorityQueue.rend(); data++)
		//{
		//	data->second->Render();
		//}
		return true;
	}
	bool TBasisUI::Release()
	{		
		I_InputUI.Release();
		I_TimerUI.Release();
		I_Writer.Release();
		I_TexUI.Release();
		I_ShaderUI.Release();
		I_Sprite.Release();
		TDxStateUI::Release();

		//for (auto data : m_pPriorityQueue)
		//{
		//	data.second->Release();
		//	delete data.second;
		//}
		return true;
	}
	void  TBasisUI::Set(HWND hWnd, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
						IDXGISwapChain* pSwapChain)
	{		
		g_hWnd = m_hWnd = hWnd;
		m_pd3dDevice = pd3dDevice;
		m_pImmediateContext = pImmediateContext;
		m_pSwapChain = pSwapChain;

		HRESULT hr;
		m_pSwapChain->GetDesc(&m_SwapChainDesc);
		g_rtClient.right = m_SwapChainDesc.BufferDesc.Width;
		g_rtClient.bottom = m_SwapChainDesc.BufferDesc.Height;

		TUI::TDxStateUI::SetState(m_pd3dDevice);

		I_TexUI.SetDevice(m_pd3dDevice, m_pImmediateContext);
		I_ShaderUI.SetDevice(m_pd3dDevice, m_pImmediateContext);
		I_Sprite.SetDevice(m_pd3dDevice, m_pImmediateContext);

		I_InputUI.Init();
		I_TimerUI.Init();
		I_Writer.Init();		
	}
	void TBasisUI::CreateDevice(IDXGISurface1* dxgiSurface)
	{		
		I_Writer.Set(dxgiSurface);
	}
	void TBasisUI::ResetDevice()
	{
		I_Writer.ResetDevice();
	}	
	void TBasisUI::LostDevice()
	{
	}
	void TBasisUI::DestroyDevice()
	{
		I_Writer.Release();
	}
	LRESULT TBasisUI::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	void TBasisUI::KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown)
	{
	}
	TBasisUI::TBasisUI()
	{
	}
	TBasisUI::~TBasisUI()
	{
		Release();
	}
}
// 내보낸 변수의 예제입니다.
TBASISUI_API int nTUnionEngine = 0;

// 내보낸 함수의 예제입니다.
TBASISUI_API int fnTUnionEngine(void)
{
	return 42;
}

TBASISUI_API TUI::TBasisUI* GetTBasisUIPtr()
{
	return new TUI::TBasisUI();
}