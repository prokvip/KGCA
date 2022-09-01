#pragma once
#include "TStd.h"
class TTexture
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
	ID3D11Resource* m_Texture = nullptr;
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

	HRESULT Load(
		ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pImmediateContext,
		std::wstring name);
};

