#pragma once
#include "TWindow.h"
class TDevice : public TWindow
{
public:
	// 인터페이스-> 하드웨어 직접 제어 -> 획득
	ID3D11Device*			m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext*	m_pImmediateContext = nullptr;
	IDXGIFactory*			m_pGIFactory = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRTV = nullptr;
public:
	// 1)디바이스 생성
	HRESULT CreateDevice();
	// 2)팩토리 생성
	HRESULT CreateDXGIDevice();
	// 3)스왑체인 생성
	HRESULT CreateSwapChain();
	// 4)랜더타켓뷰 생성
	HRESULT CreateRenderTargetView();
	// 5)뷰포트 설정
	void  CreateViewport();
public:
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제
private:
	bool		TCoreInit() override; // 초기화
	bool		TCoreFrame()override; // 실시간 계산
	bool		TCorePreRender();// 실시간 랜더링
	bool		TCoreRender()override; ;// 실시간 랜더링
	bool		TCorePostRender();// 실시간 랜더링
	bool		TCoreRelease()override;// 소멸 및 삭제
};

