#pragma once
#include "TGameCore.h"
#include "TBaseObject.h"
class Sample : public TGameCore
{
	std::vector<TBaseObject*> m_pObjectList;
public:
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
};

