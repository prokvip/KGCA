#pragma once
#include "TStd.h"
class TViewDS
{
public:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11DepthStencilView* m_pDepthStencilView;
public:
	ID3D11Texture2D* CreateTexture(UINT Width,UINT Height);
	HRESULT CreateDepthStencilView(UINT Width,UINT Height);	
	bool	Release();
};

