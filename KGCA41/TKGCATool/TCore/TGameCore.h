#pragma once
#include "TDevice.h"
#include "TInput.h"
#include "TTimer.h"
#include "TWriter.h"
#include "TDxState.h"
#include "TSoundManager.h"
#include "TTextureManager.h"
#include "TShaderManager.h"
#include "TSpriteManager.h"
#include "TRenderTarget.h"
class TGameCore : public TDevice
{
public:
	TWriter				m_Writer;
	TBaseObject			m_BG;
	TRenderTarget		m_RT;
	bool				m_bUsedRT=false;
	bool				m_bGameRun = true;

public:
	virtual bool		Init() {return true;}; // 초기화
	virtual bool		PreProcess() { return true; }; // 실시간 랜더링
	virtual bool		PreFrame() { return true; }; // 실시간 계산	
	virtual bool		Frame() { return true; }; // 실시간 계산
	virtual bool		PostFrame() { return true; }; // 실시간 랜더링
	virtual bool		PreRender(ID3D11DeviceContext* pContext) { return true; }; // 실시간 계산	
	virtual bool		Render(ID3D11DeviceContext* pContext) { return true; }; // 실시간 랜더링
	virtual bool		PostRender(ID3D11DeviceContext* pContext) { return true; }; // 실시간 랜더링
	virtual bool		PostProcess() { return true; }; // 실시간 랜더링
	virtual bool		Release() { return true; }; // 소멸 및 삭제

	virtual bool		UIRender(ID3D11DeviceContext* pContext);
public:
	bool		TCoreInit(); // 초기화
	bool		TCoreFrame();// 실시간 계산
	bool		TCorePreRender();
	bool		TCoreRender();// 실시간 랜더링
	bool		TCorePostRender();
	bool		TCoreRelease();// 소멸 및 삭제	

public:
	bool		Run() override;
	bool        ToolRun();
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();
};

