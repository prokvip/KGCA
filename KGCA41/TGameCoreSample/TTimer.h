#pragma once
#include "TStd.h"
class TTimer : public TSingleton<TTimer>
{
public:
	float m_fGameTimer = 0.0f;
	float m_fElapseTimer = 10.0f;
	UINT  m_iFPS = 0;
	std::wstring m_szTimer;
private:
	DWORD m_dwBeforeTime; //(1000: 1��)
	UINT  m_iFPSCounter = 0;
	float m_fFPSTimer = 0.0f;
public:
	virtual bool		Init(); // �ʱ�ȭ
	virtual bool		Frame();// �ǽð� ���
	virtual bool		Render();// �ǽð� ������
	virtual bool		Release();// �Ҹ� �� ����
};

#define I_Timer TTimer::GetInstance()