/**
@mainpage   TBasis3D Sample ����������
@section intro �Ұ�
- �Ұ�      :   Tech Basis Sample(DirectX 11 Graphic)
@section developer ������
- ����
@section   Program ���α׷���
- ���α׷���  :   TBasis3D11 Sample.
- ���α׷�����    :   ���� �⺻ �ھ� ���̺귯�� �� ����. 
@section info ���߸���

- DirectX SDK 11 ���� ���� �ҽ�

@section advenced �߰�����

- �۸Ӹ��� '-' �±׸� ����ϸ� �Ǹ�

- ������ �鿩����� ���� �׸��� �ȴ�.

-# ��ȣ�ű��� '-#' ������� �Ҽ� �ִ�.

-# ���� ���� ������ �鿩����� ���� �׸��� �ȴ�.

-# �ι�° ���� �׸�

-�̷������� �׸��� �������� �ִ�.

\image html gom.jpg


@section  INOUTPUT    ������ڷ�
- INPUT           :   ����.
- OUTPUT      :   Hello World ȭ�����.
@section  CREATEINFO      �ۼ�����
- �ۼ���      :   prokvip
- �ۼ���      :   2017/11/10
@section  MODIFYINFO      ��������
- ������/������   : ��������
- prokvip/2017.1110    :   "doxgen Build"
*/




#pragma once
#include "TWindow.h"
#include "TTimer.h"
#include "TInput.h"
#include "TDirectWrite.h"
#include "TShape.h"
#include "TCamera.h"
#include "TLog.h"
#include "TBasisUI.h"
class TBasisLib : public TWindow
{
	HMODULE   m_hLibraryUI;
public:
	TUI::TBasisUI* m_pTBasisUI=nullptr;
	bool	DllLoads();	
public:
	TTimer					m_Timer;
	TDirectWrite			m_Font;
	TDirectionLineShape		m_AxisLine;
	TCamera*				m_pGameCamera;
public:
	UINT					m_iPrimitiveType;
	UINT					m_iCullMode;
	UINT					m_iSamplerMode;
	bool					m_bWireFrameRender;
	bool					m_bDebugPrint;
	bool					m_bUseEngineStateCtl;		
	TCHAR					m_szDefaultDataPath[MAX_PATH] = { 0, };
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual int			WndProc( HWND, UINT, WPARAM, LPARAM );
	virtual bool		PreInit();	
	virtual bool		PostInit();	 		
	virtual bool		PreFrame();		
	virtual bool		PostFrame();	 
	virtual bool		Update(ID3D11DeviceContext*    pContext);
	virtual	bool		PreRender();
	virtual bool		DrawDebug();
	virtual bool		DrawDebugRect(RECT* rcDest = NULL, const TCHAR* pString = 0, TBASIS_EX::TColor color = TBASIS_EX::TColor( 1.0f, 1.0f, 1.0f, 1.0f) );
	virtual	bool		PostRender();
	virtual bool		ToolInit(HWND hWnd, HINSTANCE hInstance);
	virtual	HRESULT		CreateResource();
	virtual	HRESULT		DeleteResource();
public:
	bool				TInit();
	bool				TFrame();
	bool				TRender();	
	bool				Run();		 
	bool				ToolRun();
	bool				TRelease();
	HRESULT				CreateDxResource();
	HRESULT				DeleteDxResource();	
public:
	TBasisLib(void);
	virtual ~TBasisLib(void);
};
