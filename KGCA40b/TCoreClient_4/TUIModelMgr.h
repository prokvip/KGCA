#pragma once
#include "TBaseMgr.h"
#include "TObject2D.h"
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
	TStatePlayData(TTexture* a, TSound* b)
	{
		pTex = a;
		pSound = b;
		DisplayText("TStatePlayData\n");
	}
};

// 0번 - 배경이미지 타입 -> Red
// 1번 - 대화상자(패널)  -> Green
// 2번 - 버튼타입        -> Blue
class TUIModel : public TObject2D
{

public:
	std::vector<TStatePlayData> m_pStatePlayList;
	RECT	m_rtOffset;
	RECT	m_rtOffsetTex;
public:
	virtual TUIModel*	Clone() {	return nullptr;	};	

	virtual void  UpdateData(){}
};
class TUIModelComposed : public TUIModel
{
public:
	std::list< TUIModel*> m_Components;
public:
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
	virtual void	Add(TUIModel* pObj);
	virtual TUIModel* Clone();	
	virtual bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
		return true;
	};
	virtual void  UpdateData();
};
class TUIModelMgr : public TBaseMgr<TUIModel, TUIModelMgr>
{
	friend class TSingleton<TUIModelMgr>;
private:
	TUIModelMgr() {};
public:
	~TUIModelMgr() {};
};
#define I_UI TUIModelMgr::Get()

