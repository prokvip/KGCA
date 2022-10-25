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
class TGameCore : public TDevice
{
public:
	TWriter				m_Writer;
	TBaseObject			m_BG;
	bool m_bGameRun = true;
public:
	virtual bool		Init() {return true;}; // �ʱ�ȭ
	virtual bool		Frame() { return true; }; // �ǽð� ���
	virtual bool		Render() { return true; }; // �ǽð� ������
	virtual bool		Release() { return true; }; // �Ҹ� �� ����
private:
	bool		TCoreInit(); // �ʱ�ȭ
	bool		TCoreFrame();// �ǽð� ���
	bool		TCorePreRender();
	bool		TCoreRender();// �ǽð� ������
	bool		TCorePostRender();
	bool		TCoreRelease();// �Ҹ� �� ����	
public:
	bool		Run() override;
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();
};

