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
	virtual void  Rotation();	
};
class TButton :public TInterface
{
public:

public:
	virtual bool  Init() override;
	virtual bool  Frame() override;
	virtual bool  Render() override;
	virtual bool  Release() override;
public:
	TButton()
	{
		m_CurrentState = UI_NORMAL;
	}
};
class TListControl : public TButton
{
public:
	std::vector< TButton*> m_btnList;
public:
	virtual bool  Init() override;
	virtual bool  Frame() override;
	virtual bool  Render() override;
	virtual bool  Release() override;
	void  UpdateVertexBuffer() override;
	void  SetRect(TRect rt)override;
	void  ScreenToNDC()override;
};
class TDialog : public TInterface
{
public:
	std::vector< TListControl*> m_listCtlList;
	std::vector< TButton*> m_btnList;
};