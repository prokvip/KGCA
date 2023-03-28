#pragma once
#include "TObject2D.h"
namespace TUI
{
	
	typedef std::vector<RECT>  RECT_ARRAY;
	class TBASISUI_API TSprite : public TObject2D
	{
		//TSprite(const TSprite&) = delete;
		//TSprite& operator=(const TSprite&) = delete;
		//TSprite(TSprite&&) = delete;
		//TSprite& operator=(TSprite&&) = delete;
	public:
		RECT_ARRAY				m_uvArray;
		std::vector<W_STR>		m_texArray;
		std::vector<TTexture*>	m_pTexArray;
		std::vector<TSprite*>	m_pChild;
	public:
		virtual bool Load(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, std::wstring name);
	public:
		TSprite() {}
		~TSprite() {}
	};
	class TSpriteTexture : public TSprite
	{
	public:
		virtual bool Load(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pImmediateContext,std::wstring name);
	};
};