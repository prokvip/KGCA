#pragma once
#include "TObject3D.h"
class TBoxObj : public TObject3D
{
public:
	TTexture* m_pTexCube;
	virtual bool	SetVertexData();
	virtual bool	SetIndexData();
	virtual bool	PostRender();
	virtual void    DrawDebugRender(TBox* pBox);
public:
	TBoxObj();
	virtual ~TBoxObj();
};

