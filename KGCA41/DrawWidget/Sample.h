#pragma once
#include "TBasisLib.h"
#include "TShape.h"
#include "TBasisUI.h"
#include "TEditBox.h"
//#include "TWidgetControl.h"
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct TWidgetCmp
{
	bool operator()(TUI::TWidget* a, TUI::TWidget* b) const
	{
		return a->GetPriority()> b->GetPriority();
	}
};

class Sample : public TBasisLib
{
public:
	std::shared_ptr<TUI::TEditBox>		m_pEditBox;
	std::shared_ptr<TUI::TComboBox>		m_pComboBox;
	std::shared_ptr<TUI::TWidget>		m_pListBox;
	std::shared_ptr<TUI::TText>			m_pText;
	std::shared_ptr<TUI::TWidget>		m_Dlg;
	std::vector<TUI::TWidget*>			m_pWidgets;
	std::priority_queue<TUI::TWidget*,std::vector<TUI::TWidget*>, TWidgetCmp>   m_DrawWidgets;
public:	
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	//--------------------------------------------------------------------------------------
	// 변경된 클라이언트 영역를 재설정을 위한 소멸 및 생성
	//--------------------------------------------------------------------------------------
	HRESULT		CreateResource();
	HRESULT		DeleteResource();	
	int		WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void		CreateDlg();
	void		CreateWidget();
public:
	Sample(void);
	virtual ~Sample(void);
};
