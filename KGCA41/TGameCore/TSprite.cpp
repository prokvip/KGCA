#include "TSprite.h"
bool TSprite::Load(ID3D11Device* pd3dDevice,// 디바이스 객체
	ID3D11DeviceContext* pImmediateContext, std::wstring name)
{
	TTexture* pMaskTex = I_Tex.Load(m_szMaskTexturePath);

	if (!Create(pd3dDevice,	pImmediateContext,	
				m_szShaderPath,
		        m_szTexturePath))
	{
		return false;
	}
	SetMask(pMaskTex);
	m_fSpeed = 300.0f;
	return true;
}