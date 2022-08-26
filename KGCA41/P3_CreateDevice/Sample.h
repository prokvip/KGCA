#pragma once
#include "TWindow.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class Sample : public TWindow
{
	// 인터페이스-> 하드웨어 직접 제어 -> 획득
	ID3D11Device*			m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext*	m_pImmediateContext = nullptr;
	IDXGIFactory*			m_pGIFactory = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;

	ID3D11RenderTargetView* m_pRTV = nullptr;
public:
	bool		Init(); // 초기화
	bool		Frame();// 실시간 계산
	bool		Render();// 실시간 랜더링
	bool		Release();// 소멸 및 삭제
};

