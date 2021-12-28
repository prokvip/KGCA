#pragma once
#include "TCore.h"
#include "TDxObject.h"
class Sample : public TCore
{
	std::vector<TDxObject> m_ObjectList;
public:
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
public:
	Sample();
	~Sample();
};

