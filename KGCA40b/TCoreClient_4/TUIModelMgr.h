#pragma once
#include "TBaseMgr.h"
#include "TObject2D.h"

// 0번 - 배경이미지 타입 -> Red
// 1번 - 대화상자(패널)  -> Green
// 2번 - 버튼타입        -> Blue
class TUIModel : public TObject2D
{
public:
	TUIModel* m_pParent = nullptr;
public:
	virtual TUIModel*	Clone() {	return nullptr;	};	
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
		return true;
	};
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
	virtual void Add(TUIModel* pObj);
	virtual TUIModel* Clone();	
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
		return true;
	};
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

