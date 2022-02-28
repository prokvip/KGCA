#pragma once
#include "TObjectMgr.h"
#include "TUIModelMgr.h"
#include "TSoundMgr.h"
struct TStatePlayData
{
	TTexture* pTex;
	TSound* pSound;
	TStatePlayData(const TStatePlayData& data)
	{
		pTex = data.pTex;
		pSound = data.pSound;
		DisplayText("TStatePlayDataCopy\n");
	}
	TStatePlayData()
	{
		DisplayText("TStatePlayData\n");
	}
	TStatePlayData(TTexture* a,TSound* b) 
	{
		pTex = a;
		pSound = b;
		DisplayText("TStatePlayData\n");
	}
};
class TUIObject : public TUIModel
{
	TUIModel* Clone()
	{
		TUIModel* pCopy = new TUIObject(*this);
		pCopy->CreateVertexBuffer();
		pCopy->CreateIndexBuffer();
		pCopy->CreateConstantBuffer();
		pCopy->CreateInputLayout();
		D3D11_BLEND_DESC  blenddesc;
		ZeroMemory(&blenddesc, sizeof(D3D11_BLEND_DESC));
		/*bd.AlphaToCoverageEnable;
		bd.IndependentBlendEnable;*/
		blenddesc.RenderTarget[0].BlendEnable = TRUE;
		blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//// A 연산 저장
		blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blenddesc.RenderTarget[0].RenderTargetWriteMask =
			D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlend);

		blenddesc.RenderTarget[0].BlendEnable = FALSE;
		hr = m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlendDisable);
		return pCopy;
	};
	void  UpdateData()
	{
		m_rtCollision = TRect(m_vPos, m_fWidth, m_fHeight);
		I_ObjectMgr.AddCollisionExecute(this,
			std::bind(&TBaseObject::HitOverlap, this,
				std::placeholders::_1,
				std::placeholders::_2));
		I_ObjectMgr.AddSelectExecute(this,
			std::bind(&TBaseObject::HitSelect, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
public:
	std::vector<TStatePlayData> m_pStatePlayList;
	RECT	m_rtOffset;
	RECT	m_rtOffsetTex;
public:	
	bool	Frame() override;
	bool	Render()override;
	bool    SetVertexData()override;
	bool    SetIndexData()override;
	TUIObject()
	{
		m_rtOffsetTex.left = 0;
		m_rtOffsetTex.top = 0;
		m_rtOffsetTex.right = 1;
		m_rtOffsetTex.bottom = 1;
	}
};
class TImageObject : public TUIObject
{
	TUIModel* Clone()
	{
		TUIModel* pCopy = new TImageObject(*this);
		pCopy->CreateVertexBuffer();
		pCopy->CreateIndexBuffer();
		pCopy->CreateConstantBuffer();
		pCopy->CreateInputLayout();
		D3D11_BLEND_DESC  blenddesc;
		ZeroMemory(&blenddesc, sizeof(D3D11_BLEND_DESC));
		/*bd.AlphaToCoverageEnable;
		bd.IndependentBlendEnable;*/
		blenddesc.RenderTarget[0].BlendEnable = TRUE;
		blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//// A 연산 저장
		blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blenddesc.RenderTarget[0].RenderTargetWriteMask =
			D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlend);

		blenddesc.RenderTarget[0].BlendEnable = FALSE;
		hr = m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlendDisable);
		return pCopy;
	};
	void  UpdateData()
	{
		m_rtCollision = TRect(m_vPos, m_fWidth, m_fHeight);
		I_ObjectMgr.AddCollisionExecute(this,
			std::bind(&TBaseObject::HitOverlap, this,
				std::placeholders::_1,
				std::placeholders::_2));
		I_ObjectMgr.AddSelectExecute(this,
			std::bind(&TBaseObject::HitSelect, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
public:
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
};
class TButtonObject : public TUIObject
{
public:
	TUIModel* Clone()
	{
		TUIModel* pCopy = new TButtonObject(*this);
		pCopy->CreateVertexBuffer();
		pCopy->CreateIndexBuffer();
		pCopy->CreateConstantBuffer();
		pCopy->CreateInputLayout();
		D3D11_BLEND_DESC  blenddesc;
		ZeroMemory(&blenddesc, sizeof(D3D11_BLEND_DESC));
		/*bd.AlphaToCoverageEnable;
		bd.IndependentBlendEnable;*/
		blenddesc.RenderTarget[0].BlendEnable = TRUE;
		blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//// A 연산 저장
		blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blenddesc.RenderTarget[0].RenderTargetWriteMask =
			D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlend);

		blenddesc.RenderTarget[0].BlendEnable = FALSE;
		hr = m_pd3dDevice->CreateBlendState(&blenddesc, &m_AlphaBlendDisable);
		return pCopy;
	};
	void  UpdateData()
	{
		m_rtCollision = TRect(m_vPos, m_fWidth, m_fHeight);
		I_ObjectMgr.AddCollisionExecute(this,
			std::bind(&TBaseObject::HitOverlap, this,
				std::placeholders::_1,
				std::placeholders::_2));
		I_ObjectMgr.AddSelectExecute(this,
			std::bind(&TBaseObject::HitSelect, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}

	//TButtonObject(const TButtonObject& copy)
	//{
	//	TDxObject::CreateVertexBuffer();
	//}
	virtual void	HitSelect(TBaseObject* pObj, DWORD dwState) override;
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
public:
	TButtonObject() {}
	virtual ~TButtonObject() {}
};
