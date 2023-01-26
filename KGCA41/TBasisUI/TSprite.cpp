#include "TSprite.h"
namespace TUI
{
	bool TSprite::Load(ID3D11Device* pd3dDevice,// 디바이스 객체
		ID3D11DeviceContext* pImmediateContext, std::wstring name)
	{
		TTexture* pMaskTex = I_TexUI.Load(m_szMaskTexturePath);

		if (!Create(pd3dDevice, pImmediateContext, m_szShaderPath, m_szTexturePath))
		{
			return false;
		}
		if (m_pTexture)
		{
			m_ptImageSize.x = m_pTexture->m_Desc.Width;
			m_ptImageSize.y = m_pTexture->m_Desc.Height;
		}
		SetMask(pMaskTex);
		m_fSpeed = 300.0f;
		return true;
	}
	bool TSpriteTexture::Load(ID3D11Device* pd3dDevice,// 디바이스 객체
		ID3D11DeviceContext* pImmediateContext, std::wstring name)
	{
		m_pTexArray.resize(m_texArray.size());
		for (int iTex = 0; iTex < m_texArray.size(); iTex++)
		{
			m_pTexArray[iTex] = I_TexUI.Load(m_texArray[iTex]);
		}
		return TSprite::Load(pd3dDevice, pImmediateContext, name);
	}
};