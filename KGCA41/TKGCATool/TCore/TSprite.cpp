#include "pch.h"
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
bool TSpriteTexture::Load(ID3D11Device* pd3dDevice,// 디바이스 객체
	ID3D11DeviceContext* pImmediateContext, std::wstring name)
{
	m_pTexArray.resize(m_texArray.size());
	for (int iTex=0; iTex < m_texArray.size(); iTex++)
	{
		m_pTexArray[iTex] = I_Tex.Load(m_texArray[iTex]);
	}
	return TSprite::Load(pd3dDevice, pImmediateContext, name);	
}