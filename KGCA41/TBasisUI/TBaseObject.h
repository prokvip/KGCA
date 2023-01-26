#pragma once
#include "TBasisUIStd.h"
#include "TTextureUIManager.h"
#include "TShaderUIManager.h"
#include "TCollisionUI.h"
#include "TDxStateUI.h"
//TBASISUI_TEMPLATE template class TBASISUI_API std::vector<TUI::SimpleVertex>;
namespace TUI
{
	struct SimpleVertex
	{
		TUI::TVector    p;
		TUI::TVector4D  c;
		TUI::TVector2D  t;
	};

	class TBASISUI_API  TBaseObject
	{
	public:
		ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
		ID3D11DeviceContext* m_pImmediateContext = nullptr;
		ID3D11Buffer* m_pVertexBuffer = nullptr;
		ID3D11Buffer* m_pIndexBuffer = nullptr;
		ID3D11InputLayout* m_pVertexLayout = nullptr;
		ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
		ID3D11VertexShader* m_pVS = nullptr;
		ID3D11PixelShader* m_pPS = nullptr;
		ID3DBlob* m_pVSCode = nullptr;
		ID3DBlob* m_pPSCode = nullptr;
	public:
		TShader* m_pShader = nullptr;
		TTexture* m_pTexture = nullptr;
		std::wstring				m_szShaderName;
		std::wstring				m_szTextureName;
		std::vector<SimpleVertex>   m_VertexList;
		std::vector<SimpleVertex>   m_InitVertexList;
		std::vector<DWORD>			m_IndexList;
		TRect						m_rtCollision;
	public:
		virtual bool		Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
			const std::wstring shadername = {}, const std::wstring texturename = {});
		virtual bool		Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
			TShader* pShader, TTexture* pTexture);
		virtual void		CreateVertexData();
		virtual void		CreateIndexData();
		virtual HRESULT		CreateVertexBuffer();
		virtual HRESULT		CreateIndexBuffer();
		virtual bool		CreateShader(std::wstring filename);
		virtual HRESULT     CreateVertexShader(std::wstring filename);
		virtual HRESULT     CreatePixelShader(std::wstring filename);
		virtual HRESULT     CreateVertexLayout();
		virtual void		UpdateVertexBuffer();
	public:
		virtual bool		Init();
		virtual bool		Frame();
		virtual bool		PreRender();
		virtual bool		Render();
		virtual bool		PostRender();
		virtual bool		Release();
	public:
		TBaseObject();
		virtual ~TBaseObject();
	};

};