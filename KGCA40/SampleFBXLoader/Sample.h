#pragma once
#include "TCore.h"
#include "TFbxObj.h"
class Sample :   public TCore
{
	TFbxObj		m_FbxObj;
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

