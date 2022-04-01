#pragma once
#include "TObject3D.h"
class TBoxObj : public TObject3D
{
public:
	TTexture* m_pTexCube;
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
	bool	PostRender();
public:
	TBoxObj();
	virtual ~TBoxObj();
};

