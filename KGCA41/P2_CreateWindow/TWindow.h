#pragma once
#include <windows.h>
class TWindow
{
public:
	HINSTANCE   m_hInstance;	// ������ȣ
	HWND		m_hWnd;			// ����ȣ
public:
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
	bool		Run();
public:
	ATOM		MyRegisterClass(HINSTANCE hInstance);
	BOOL		InitInstance(HINSTANCE hInstance);
};

