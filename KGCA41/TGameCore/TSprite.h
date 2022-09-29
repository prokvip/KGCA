#pragma once
#include "TObject2D.h"
typedef std::vector<RECT>  RECT_ARRAY;
class TSprite : public TObject2D
{
public:
	RECT_ARRAY		m_uvArray;
	std::vector<TSprite*>  m_pChild;
public:
	virtual bool Load(	ID3D11Device* pd3dDevice,
						ID3D11DeviceContext* pImmediateContext, 
						std::wstring name);	
};

