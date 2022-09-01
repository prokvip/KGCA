#pragma once
#include "TDevice.h"
#include "TBaseObject.h"
class Sample : public TDevice
{
	std::vector<TBaseObject*> m_pObjectList;
public:
	bool		Init(); // 초기화
	bool		Frame();// 실시간 계산
	bool		Render();// 실시간 랜더링
	bool		Release();// 소멸 및 삭제
};

