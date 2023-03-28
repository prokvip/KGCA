#pragma once
#include "TDevice.h"

class TWindow : public TDevice
{
public:
	//--------------------------------------------------------------------------------------
	// Global Variables
	//--------------------------------------------------------------------------------------
	HINSTANCE               m_hInstance;		// �ν��Ͻ� �ڵ�
	HWND                    m_hWnd;				// ������ �ڵ�
	DWORD					m_dwWindowStyle;	// ������ ��Ÿ��
	RECT					m_rcWindowBounds;   // ������ ����
	RECT					m_rcWindowClient;   // Ŭ���̾�Ʈ ����	
	UINT					m_iWindowWidth;		// Ŭ���̾�Ʈ ���� ����ũ��
	UINT					m_iWindowHeight;	// �����̾�Ʈ ���� ����ũ��
public:
	// ������ ����
	bool				InitWindow(		HINSTANCE hInstance, 
										int nCmdShow, 
										const TCHAR* strWindowTitle = _T("TBasis Sample"), 										
										int iWidth = 800,
										int iHeight=600,
										BOOL IsFullScreen = FALSE);	
	// ������ ��ġ ȭ�� �߾����� �̵�
	void				CenterWindow(HWND hwnd);
	bool				CheckWindowSize();
	
public:
	virtual int			WndProc( HWND, UINT, WPARAM, LPARAM );
			LRESULT		WindowMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
public:
	TWindow(void);
	virtual ~TWindow(void);
};