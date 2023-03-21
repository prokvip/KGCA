#pragma once
#include "TSpriteManager.h"
#include "TSoundManager.h"
//https://www.vectorstock.com/
enum TUIState
{
	UI_NORMAL,
	UI_HOVER,	
	UI_PUSH,
	UI_DIS,
	UI_SELECT,
	UI_MAXSTATE,
};
struct TUIEvent
{
	std::vector<TTexture*>  m_pStateList;
	std::vector<TSound*>    m_pSoundStateList;
	TSprite* m_pSprite;
	TSprite* m_pEffect;
	TUIEvent()
	{
		m_pStateList.resize(UI_MAXSTATE);
		m_pSprite = nullptr;
		m_pEffect = nullptr;
	}
};
class TInterface :public TObject2D
{
public:
	TUIState				m_CurrentState;
	TUIEvent				m_EventState;
	TTexture*				m_pCurrentTex = nullptr;	
	TSprite*				m_pSprite = nullptr;
public:
	std::vector<TInterface*> m_rtDrawList;
	std::vector<TInterface*> m_pChildList;
public:
	virtual bool  Frame() override;
public:
	virtual void FadeInOut(float fAlpha);
	virtual void SetEvent(TUIEvent& event)
	{
		m_EventState = event;
	};
	virtual void AddChild(TInterface* pUI);
	virtual bool SetTextueState(const std::vector<W_STR>& texStateList);
	virtual bool SetAttribute(TVector2 vPos, TRect rt, TSprite* pSprite);
	virtual bool SetAttribute(TVector2 vPos, TRect rt, const std::vector<W_STR>& texStateList = {});
	virtual bool SetAttribute(TVector2 vPos, const std::vector<W_STR>& texStateList = std::vector<W_STR>());
	virtual bool  SetDrawList(
		float fScaleX0, float fScaleX1,
		float fScaleY0, float fScaleY1,
		float fScaleU0, float fScaleU1,
		float fScaleV0, float fScaleV1);
public:
	TInterface()
	{
		m_CurrentState = UI_NORMAL;
		
	}
};
class TButton :public TInterface
{
public:
	virtual bool  Init() override;
	virtual bool  Frame() override;
	virtual bool  Render() override;
	virtual bool  Release() override;
	virtual void  SetRect(TRect rt) override;
};
class TListControl : public TInterface
{
public:
	virtual bool  Init() override;
	virtual bool  Frame() override;
	virtual bool  Render() override;
	virtual bool  Release() override;
	virtual void  SetRect(TRect rt)override;
};
class TDialog : public TInterface
{	
public:
	virtual bool  Init() override;
	virtual bool  Frame() override;
	virtual bool  Render() override;
	virtual bool  Release() override;
};