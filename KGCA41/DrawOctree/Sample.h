#pragma once
#include "TDevice.h"
#include "TBaseObject.h"
class Sample : public TDevice
{
	TBaseObject		m_object;
public:
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
};

