#pragma once
#include "TNetwork.h"
#define NETWORK_MSG WM_USER+1
class TAsyncSelect :   public TNetwork
{
public:
	bool	m_bConnect = false;
public:
	bool     Connect(HWND hWnd, int protocol, int iport, const char* ip);
	LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

