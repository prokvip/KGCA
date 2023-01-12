#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "TGameCore.h"
#include "TNetwork.h"

class Sample : public TGameCore
{
	
	TNetwork	m_Net;
	HWND		m_hEdit;
	HWND		m_hListbox;
	HWND		m_hSendBtn;
	bool		m_bNameSend = false;
public:
	void	ChatMsg(UPACKET& t);	
	void    NameReq(UPACKET& t);
public:
	/*typedef void (Sample::* CallFuction) (UPACKET& t);
	typedef std::map<DWORD, CallFuction>::iterator FunIter;
	std::map<DWORD, CallFuction> m_fnExecutePacket;*/	
public:
	void		PRINT(std::list<std::wstring> myArguments);
	void		Print(const WCHAR* fmt, ...);	
	bool		Run() override;
	LRESULT		MsgProc(HWND hWnd,	UINT message,WPARAM wParam,LPARAM lParam)override;
	bool	Init() override;
	bool	PreProcess()override;
		bool	PreFrame()override;
		bool	Frame()override;
		bool	Render()override;
		bool	PostRender()override;
	bool	PostProcess()override;
	bool	Release()override;
	void    ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);
};

