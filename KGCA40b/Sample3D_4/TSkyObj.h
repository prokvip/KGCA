#pragma once
#include "TObject3D.h"
class TSkyObj : public TObject3D
{
public:	
	TTexture*			m_pTexArray[6];
	TTexture*			m_pTexCube;
	ComPtr<ID3D11ShaderResourceView> m_pSRVArray[6];
public:
	virtual void		SetMatrix(T::TMatrix* matWorld,T::TMatrix* matView, T::TMatrix* matProj);
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
	virtual bool		LoadTexture(const TCHAR* szColorFileName,
							const TCHAR* szMaskFileName);
	bool	Render();
	bool	PostRender();
public:
	TSkyObj();
	virtual ~TSkyObj();
};

