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
