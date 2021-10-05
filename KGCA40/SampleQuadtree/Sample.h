#pragma once
#include <TCore.h>
#include "TShape.h"
#include "TMap.h"
#include "TCamera.h"
#include "TQuadtree.h"
class Sample : public TCore
{
public:
	//TBoxShape			m_BoxObj[2];
	TMap				m_Map;
	TDebugCamera		m_Camera;
	TQuadtree			m_Quadtree;
	float m_fYaw =0.0f;
	float m_fPitch=0.0f;
public:
	bool   Init() override;
	bool   Frame()override;
	bool   Render()override;
	bool   Release()override;
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)override;
public:
	Sample();
	virtual ~Sample();
};

