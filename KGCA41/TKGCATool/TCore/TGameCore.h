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
	virtual bool		Init() {return true;}; // �ʱ�ȭ
	virtual bool		PreProcess() { return true; }; // �ǽð� ������
	virtual bool		PreFrame() { return true; }; // �ǽð� ���	
	virtual bool		Frame() { return true; }; // �ǽð� ���
	virtual bool		PostFrame() { return true; }; // �ǽð� ������
	virtual bool		PreRender(ID3D11DeviceContext* pContext) { return true; }; // �ǽð� ���	
	virtual bool		Render(ID3D11DeviceContext* pContext) { return true; }; // �ǽð� ������
	virtual bool		PostRender(ID3D11DeviceContext* pContext) { return true; }; // �ǽð� ������
	virtual bool		PostProcess() { return true; }; // �ǽð� ������
	virtual bool		Release() { return true; }; // �Ҹ� �� ����

	virtual bool		UIRender(ID3D11DeviceContext* pContext);
public:
	bool		TCoreInit(); // �ʱ�ȭ
	bool		TCoreFrame();// �ǽð� ���
	bool		TCorePreRender();
	bool		TCoreRender();// �ǽð� ������
	bool		TCorePostRender();
	bool		TCoreRelease();// �Ҹ� �� ����	

public:
	bool		Run() override;
	bool        ToolRun();
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();
};

