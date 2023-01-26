#include "TTextureUI.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#ifdef _DEBUG
#pragma comment(lib, "DirectXTK_D.lib")
#else
#pragma comment(lib, "DirectXTK_R.lib")
#endif


namespace TUI
{
	HRESULT TTexture::Load(ID3D11Device* pd3dDevice,// 디바이스 객체
		ID3D11DeviceContext* pImmediateContext, std::wstring name)
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(
			pd3dDevice,
			pImmediateContext,
			name.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pTextureSRV);
		if (FAILED(hr))
		{
			hr = DirectX::CreateDDSTextureFromFile(
				pd3dDevice,
				pImmediateContext,
				name.c_str(),
				(ID3D11Resource**)&m_pTexture,
				&m_pTextureSRV);
		}
		if (m_pTexture)
		{
			m_strFileName = name;
			m_pTexture->GetDesc(&m_Desc);
		}
		return hr;
	}
	void TTexture::Apply(ID3D11DeviceContext* pImmediateContext,
		UINT iSlot)
	{
		pImmediateContext->PSSetShaderResources(iSlot, 1, &m_pTextureSRV);
	}
	bool TTexture::Release()
	{
		if (m_pTexture)m_pTexture->Release();
		if (m_pTextureSRV)m_pTextureSRV->Release();
		return true;
	}
};