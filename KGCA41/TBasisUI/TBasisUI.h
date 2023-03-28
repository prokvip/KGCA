#pragma once
#include "TWidget.h"
#include "TWriter.h"
namespace TUI
{	
	class TBASISUI_API TBasisUI
	{	
	public:
		HWND				 m_hWnd = NULL;
		ID3D11Device*		 m_pd3dDevice = nullptr;		
		ID3D11DeviceContext* m_pImmediateContext = nullptr;
		IDXGISwapChain*		 m_pSwapChain = nullptr;
		DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;	
		TPriorityMap		 m_pPriorityQueue;
	public:
		virtual void	Set(HWND hWnd, ID3D11Device* pd3dDevice,ID3D11DeviceContext* pImmediateContext, IDXGISwapChain* pSwapChain);
		
		virtual bool	Init();
		virtual bool	Frame(double fTime,float fElapsedTime);
		virtual bool	Render(double fTime,	float fElapsedTime);
		virtual bool	Release();

		virtual void	LostDevice();
		virtual void	DestroyDevice();
		virtual LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void	KeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown);
		virtual void	CreateDevice(IDXGISurface1* dxgiSurface);
		virtual void	ResetDevice();
	public:
		TBasisUI(void);
		virtual ~TBasisUI();		
	};
}

#ifdef __cplusplus
extern "C" {
#endif

	TBASISUI_API TUI::TBasisUI* GetTBasisUIPtr();

#ifdef __cplusplus
}
#endif

