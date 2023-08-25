#pragma once
#include "TObject.h"
class TPlaneObj : public TObject
{
public:
	virtual bool  Load(
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pImmediateContext,
		TVector3,
		TVector3,
		W_STR, W_STR);
	virtual bool	CreateVertexBuffer() override;
	virtual bool    Frame();
};

