#pragma once
#include "TCore.h"
#include "TAsyncSelect.h"
#include "TDxObject.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

class Sample : public TCore
{
	ID3D11ShaderResourceView*	m_pSRV0;
	ID3D11Resource*				m_pTexture0;
	ID3D11ShaderResourceView*	m_pSRV1;
	ID3D11Resource*				m_pTexture1;
public:
	std::vector<TDxObject> m_ObjectList;
	TAsyncSelect		m_Net;
	HWND				m_hEdit;
	HWND				m_hButton;
	HWND				m_hListBox;
	int					m_iChatCnt;
public:
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

