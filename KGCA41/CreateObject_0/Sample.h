#pragma once
#include "TDevice.h"
#include "TBaseObject.h"
class Sample : public TDevice
{
	std::vector<TBaseObject*> m_pObjectList;	
	int m_iTexIndex = 0;
public:
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
};

