#pragma once
#include "TDevice.h"
class TGameCore : public TDevice
{
public:
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
};

