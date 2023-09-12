#pragma once
#include "TStd.h"
#include "TPlaneObj.h"
#include "TWriter.h"
#include "TSpriteObj.h"
enum TSelectState
{
	T_DEFAULT = 0, // ���콺�� ���� ���� ��
	T_HOVER = 1,   // ���콺�� ���� ���� ��
	T_FOUCS = 2,   // T_ACTIVE���� �ٸ� ������ ������ ��
	T_ACTIVE = 4,  // ���콺 ��ư�� ������ ���� ��
	T_SELECTED = 8,// T_ACTIVE���� ������ ������ ��
};
using CollisionFunction = std::function<void(TObject*, DWORD)>;
using SelectFunction = std::function<void(TVector2, DWORD)>;

class TUserInterface : public TPlaneObj
{
public:
	virtual void    MouseOverlap(TVector2 pt, DWORD state)
	{
		//m_iSelectState = state;
		std::wstring msg = this->m_szName;
		msg += std::to_wstring(pt.x);
		msg += std::to_wstring(pt.y);
		msg += std::to_wstring(state);
		I_Writer.AddText(msg, 0, 300, D2D1::ColorF(1, 0, 0, 1));
	}
};

class TBtnObj : public TUserInterface
{
public:
	int			m_iSelectState = 0;
	TSpriteObj  m_SprteObj;
public:
	virtual void    MouseOverlap(TVector2 pt, DWORD state)
	{		
		if (state == T_HOVER)
		{
			// �̹��� ��ü
		}
		m_iSelectState = state;
		std::wstring msg = this->m_szName;
		msg += std::to_wstring(pt.x);
		msg += std::to_wstring(pt.y);
		msg += std::to_wstring(state);
		I_Writer.AddText(msg, 0, 300, D2D1::ColorF(1, 0, 0, 1));
	}
};

class TScene
{
	using U_TObject = std::unique_ptr<TUserInterface>;

	int m_iExecuteCollisionID = 0;
	int m_iExecuteSelectID = 0;
public:
	U_TObject m_pMapObj = nullptr;
	std::map<int, TObject*> m_SelectObjectList;
	std::map<int, SelectFunction > m_fnSelectExecute;
	void AddSelectExecute(TObject* owner, SelectFunction func);
	void DeleteSelectExecute(int iID);
public:
	W_STR          m_szBackround;
public:
	virtual bool   Init();
	virtual bool   Frame();
	virtual bool   Render();
	virtual bool   Release();
	virtual void   SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
public:
	virtual ~TScene() {}
};

class TSceneTitle : public TScene
{	
	std::unique_ptr<TUserInterface> m_pStartBtn = nullptr;
public:
	virtual bool Init() override;
	virtual bool Frame()override;
	virtual bool Render()override;
	virtual bool Release()override;
public:
	virtual ~TSceneTitle() {}
};
class TSceneLobby : public TScene
{
public:
	virtual ~TSceneLobby() {}
};
class TSceneInGame : public TScene
{
public:
	virtual ~TSceneInGame() {}
};
class TSceneResult : public TScene
{
public:
	virtual ~TSceneResult() {}
};
