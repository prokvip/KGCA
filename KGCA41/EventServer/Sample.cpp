#include "Sample.h"

// printf("%s %d %f",a,b,c);
void   Sample::Print(const WCHAR* fmt, ...)
{
	va_list arg;
		va_start(arg, fmt);
		WCHAR msg[256] = L"";
		_vsntprintf(msg, 256, fmt, arg);
		SendMessage(m_hListbox, LB_ADDSTRING, 0, (LPARAM)msg);
	va_end(arg);
}

//
LRESULT Sample::MsgProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {  
	case WM_CREATE:
	{
		m_hEdit =CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			500, 10, 200, 25, hWnd, (HMENU)1000, m_hInstance, NULL);

		m_hSendBtn = CreateWindow(L"button", L"Send", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
			700, 10, 50, 25, hWnd, (HMENU)1001, m_hInstance, NULL);

		m_hListbox = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL,
			0, 0, 500, 600, hWnd, (HMENU)1002, m_hInstance, NULL);

		std::wstring fmt = L"%s";
		Print(fmt.c_str(), L"Chatting Start");
	}break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case 1001:
			{
				WCHAR szBuffer[255] = L"";
				GetWindowText(m_hEdit, szBuffer, 255);
				OutputDebugString(szBuffer);
				if (m_bNameSend == false)
				{
					std::string msg = to_wm(szBuffer);	
					m_Net.AddSend(m_Net.m_Sock,	msg.c_str(), msg.size(),
						PACKET_NAME_REQ);
					m_bNameSend = true;					
				}
				else
				{
					std::string msg = to_wm(szBuffer);
					m_Net.AddSend(m_Net.m_Sock,	msg.c_str(), msg.size(),
						PACKET_CHAR_MSG);
				}
			}break;
		}
	}break;
    case WM_DESTROY:
        PostQuitMessage(0); // 메세지큐에 직접 WM_QUIT
        break;
    }
    // 메세지 내가 처리 불가 니가 대신 해줘.
    return  DefWindowProc(hWnd, message, wParam, lParam);
}
bool		Sample::Run()
{
	Init();
	while (m_bGameRun)
	{
		PreProcess();
		if (TWindow::Run() == true)
		{
			Frame();
			Render();
		}
		else
		{
			m_bGameRun = false;
		}
		PostProcess();
	}
	Release();
	return true;
}
bool	Sample::Init()
{
	/*m_fnExecutePacket[PACKET_CHATNAME_REQ] = &Sample::ChatNameReq;
	m_fnExecutePacket[PACKET_CHAR_MSG] = &Sample::ChatMsg;
	m_fnExecutePacket[PACKET_JOIN_USER] = &Sample::JoinUser;
	m_fnExecutePacket[PACKET_NAME_ACK] = &Sample::NameAck;*/

	m_Net.m_fnExecutePacket[PACKET_CHAR_MSG] = std::bind(&Sample::ChatMsg, this, std::placeholders::_1);
	m_Net.m_fnExecutePacket[PACKET_NAME_REQ] = std::bind(&Sample::NameReq, this, std::placeholders::_1);

	std::wstring fmt = L"IP[%s]:PORT[%d] %s";
	if (m_Net.NetStart("192.168.0.12", 10000))
	{		
		Print(fmt.c_str(), L"192.168.0.12", 10000, L"서버 시작");
	}
	else
	{
		Print(fmt.c_str(), L"192.168.0.12", 10000, L"서버 실폐");
	}
	
	return true;
}
bool	Sample::PreProcess()
{
	m_Net.PacketProcess();
	return true;
}
bool	Sample::PreFrame()
{	
	m_Net.Frame();	
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
bool	Sample::PostRender()
{
	m_Net.Render();	
	return true;
}
bool	Sample::PostProcess()
{
	m_Net.SendPrecess();
	return true;
}
bool	Sample::Release() 
{
	m_Net.Release();
	return true;
}
GAME_RUN(EventSelectServer, 800, 600)