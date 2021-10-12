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
	TQuadtree			m_Quadtree;
public:
	bool   Init() override;
	bool   Frame()override;
	bool   Render()override;
	bool   Release()override;
public:
	Sample();
	virtual ~Sample();
};

