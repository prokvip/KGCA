#pragma once
#include "TGameCore.h"
#include "TUser2D.h"
#include "TNpc2D.h"

class Sample : public TGameCore
{
	TBaseObject*	m_pMap;
	std::vector<TBaseObject*> m_pNpcList;	
	TUser2D*		m_pUser;
	std::vector<TBaseObject*> m_pObjectList;
public:
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
};

