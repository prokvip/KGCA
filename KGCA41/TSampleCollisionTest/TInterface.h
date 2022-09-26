#pragma once
#include "TObject2D.h"
enum TUIState
{
	UI_NORMAL,
	UI_HOVER,	
	UI_PUSH,
	UI_DIS,
	UI_SELECT,
};
class TInterface :public TObject2D
{
public:
	TUIState		m_CurrentState;
	std::vector<TTexture*>  m_pStateList;
	TTexture* m_pCurrentTex = nullptr;	
public:
	virtual void AddChild(TInterface* pUI) { };
	virtual bool  SetAttribute(	TVector2D vPos, TRect rt,
								std::vector<W_STR>& texStateList);
	virtual void  Rotation();	
	virtual bool  SetDrawList(
		float fScaleX0, float fScaleX1,
		float fScaleY0, float fScaleY1,
		float fScaleU0, float fScaleU1,
		float fScaleV0, float fScaleV1) {
		return true;
	};
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
};
class TListControl : public TInterface
{
public:
	std::vector<TInterface*> m_pChildList;
public:
	virtual bool  Init() override;
	virtual bool  Frame() override;
	virtual bool  Render() override;
	virtual bool  Release() override;
	virtual void  SetRect(TRect rt)override;
	virtual void AddChild(TInterface* pUI) { 
		m_pChildList.push_back(pUI);
	};
};
class TDialog : public TInterface
{
public:
	std::vector<TInterface*> m_rtDrawList;
	std::vector<TInterface*> m_pChildList;
	virtual void AddChild(TInterface* pUI)
	{
		m_pChildList.push_back(pUI);
	};
public:
	virtual bool  Frame() override;
	virtual bool  Render() override;
	virtual void  SetRect(TRect rt)override;
	virtual bool  SetDrawList(
		float fScaleX0, float fScaleX1,
		float fScaleY0, float fScaleY1,
		float fScaleU0, float fScaleU1,
		float fScaleV0, float fScaleV1 );
};