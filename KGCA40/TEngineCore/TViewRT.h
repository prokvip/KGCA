#pragma once
#include "TStd.h"
class TViewRT
{
public:
	ID3D11Texture2D* m_pDSTexture;
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11RenderTargetView* m_pRenderTargetView;// ���� ����Ÿ�� ��
public:
	ID3D11Texture2D*	CreateTexture(UINT Width, UINT Height);
	HRESULT				SetRenderTargetView(ID3D11Texture2D* pTexture);
	HRESULT CreateRenderTargetView(UINT Width, UINT Height);
	bool	Release();
};

