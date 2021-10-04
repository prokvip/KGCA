#pragma once
#include <TCore.h>
#include "TShape.h"
#include "TMap.h"
class Sample : public TCore
{
	float				m_pSpeed;
	TVector3			m_vCameraPos;
	TVector3			m_vCameraTarget;
	CB_DATA				m_cbData;
public:
	TBoxShape			m_BoxObj[2];
	TMap				m_Map;
public:
	bool   Init();
	bool   Frame();
	bool   Render();
	bool   Release();
public:
	Sample();
};

