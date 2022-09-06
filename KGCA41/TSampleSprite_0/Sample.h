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
	bool		Init(); // 초기화
	bool		Frame();// 실시간 계산
	bool		Render();// 실시간 랜더링
	bool		Release();// 소멸 및 삭제
};

