#pragma once
#include "TBasisUIStd.h"

namespace TUI
{
	class TBASISUI_API TTexture
	{
	public:
		W_STR		m_strFileName;
		ID3D11Texture2D* m_pTexture = nullptr;
		ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
		D3D11_TEXTURE2D_DESC		m_Desc;
	public:
		HRESULT Load(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, std::wstring name);
		void Apply(ID3D11DeviceContext* pImmediateContext, UINT iSlot = 0);
		bool Release();
	};
};

