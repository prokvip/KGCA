#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
class TGameCore : public TDevice
{
public:
	TTimer		m_Timer;
	TInput		m_Input;
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
	bool		Run();
};

