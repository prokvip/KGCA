#include "Sample.h"
LRESULT Sample::MsgProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {  

    case WM_DESTROY:
        PostQuitMessage(0); // �޼���ť�� ���� WM_QUIT
        break;
    }
    // �޼��� ���� ó�� �Ұ� �ϰ� ��� ����.
    return  DefWindowProc(hWnd, message, wParam, lParam);
}
bool		Sample::Run()
{
	Init();
	while (m_bGameRun)
	{
		if (TWindow::Run() == true)
		{
			Frame();
			Render();
		}
		else
		{
			m_bGameRun = false;
		}
	}
	Release();
	return true;
}
bool	Sample::Init()
{
	return true;
}
bool	Sample::Frame() 
{
	return true;
}
bool	Sample::Render() 
{
	return true;
}
bool	Sample::Release() 
{
	return true;
}
GAME_RUN(SocketWin, 800, 600)