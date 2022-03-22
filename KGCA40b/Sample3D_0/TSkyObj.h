#pragma once
#include "TObject3D.h"
class TSkyObj : public TObject3D
{
public:
	TMatrix				m_matViewSky;
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
	bool	PostRender();
public:
	TSkyObj();
	virtual ~TSkyObj();
};

