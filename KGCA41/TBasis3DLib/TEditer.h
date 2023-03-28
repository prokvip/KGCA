#pragma once
#include "TBasisStd.h"

class TEditer
{
protected:	
	INT											m_iShaderIndex;
	bool											m_bReload;
	HWND										m_hWnd;
	PROCESS_INFORMATION			m_NotePad_PI;
	T_STR							m_strFileName;
	_WIN32_FILE_ATTRIBUTE_DATA	m_fileAttData;	

public:
	bool			IsReload();
	bool			Update();
	BOOL		SafeTerminateProcess(HANDLE hProcess, UINT uExitCode);
	bool			Create(T_STR ShaderPath, HWND hWnd );
	int				MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

public:
	bool	Frame();
	bool	Release();
	bool	Init();
public:
	TEditer(void);
	virtual ~TEditer(void);
};
