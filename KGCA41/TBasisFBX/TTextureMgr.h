#pragma once
#include "TBaseMgr.h"
namespace TBasisFBX
{
	class TBASISFBX_API TTexture
	{
	public:
		int				m_iIndex;
		std::wstring	m_csName;
		std::wstring	m_szMsg;
		TCHAR			m_szBuffer[256];
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
		D3D11_TEXTURE2D_DESC		m_TextureDesc;
	public:
		bool	Init();
		bool	Frame();
		bool	Render();
		bool	Release();
		bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
	public:
		TTexture();
		virtual ~TTexture();
	};

	class TBASISFBX_API TTextureMgr : public TBaseMgr<TTexture, TTextureMgr>
	{
		friend class TSingleton<TTextureMgr>;
	public:
		static HRESULT SaveFile(ID3D11DeviceContext* pContext, ID3D11Texture2D* pRes, T_STR name);
		static HRESULT SaveFile(ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain, T_STR name);
	private:
		TTextureMgr() {};
	public:
		~TTextureMgr() {};
	};
#define I_TextureFBX TTextureMgr::Get()
};