#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWriter.h"
class TGameCore : public TDevice
{
public:
	bool m_bGameRun = true;
	TWriter				m_Writer;
	IDXGISurface1* m_pBackBuffer;
public:
	virtual bool		Init() {return true;}; // 초기화
	virtual bool		Frame() { return true; }; // 실시간 계산
	virtual bool		Render() { return true; }; // 실시간 랜더링
	virtual bool		Release() { return true; }; // 소멸 및 삭제
private:
	bool		TCoreInit(); // 초기화
	bool		TCoreFrame();// 실시간 계산
	bool		TCorePreRender();
	bool		TCoreRender();// 실시간 랜더링
	bool		TCorePostRender();
	bool		TCoreRelease();// 소멸 및 삭제	
public:
	bool		Run();
};

