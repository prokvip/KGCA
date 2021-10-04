#pragma once
#include <TCore.h>
#include "TShape.h"
#include "TMap.h"
#include "TCamera.h"
class Sample : public TCore
{
public:
	TBoxShape			m_BoxObj[2];
	TMap				m_Map;
	TDebugCamera		m_Camera;
public:
	bool   Init();
	bool   Frame();
	bool   Render();
	bool   Release();
public:
	Sample();
	virtual ~Sample();
};

