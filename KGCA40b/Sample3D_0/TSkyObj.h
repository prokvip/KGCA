#pragma once
#include "TObject3D.h"
class TSkyObj : public TObject3D
{
public:
	TMatrix				m_matViewSky;
	TTexture*			m_pTexArray[6];
public:
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
	virtual bool		LoadTexture(const TCHAR* szColorFileName,
							const TCHAR* szMaskFileName);
	bool	PostRender();
public:
	TSkyObj();
	virtual ~TSkyObj();
};

