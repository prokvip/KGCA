/**
@mainpage   TBasis3D Sample 메인페이지
@section intro 소개
- 소개      :   Tech Basis Sample(DirectX 11 Graphic)
@section developer 개발자
- 김명균
@section   Program 프로그램명
- 프로그램명  :   TBasis3D11 Sample.
- 프로그램내용    :   게임 기본 코어 라이브러리 및 샘플. 
@section info 개발목적

- DirectX SDK 11 교육 샘플 소스

@section advenced 추가정보

- 글머리는 '-' 태그를 사용하면 되며

- 탭으로 들여쓸경우 하위 항목이 된다.

-# 번호매기기는 '-#' 방식으로 할수 있다.

-# 위와 같이 탭으로 들여쓸경우 하위 항목이 된다.

-# 두번째 하위 항목

-이런식으로 그림을 넣을수도 있다.

\image html gom.jpg


@section  INOUTPUT    입출력자료
- INPUT           :   없음.
- OUTPUT      :   Hello World 화면출력.
@section  CREATEINFO      작성정보
- 작성자      :   prokvip
- 작성일      :   2017/11/10
@section  MODIFYINFO      수정정보
- 수정자/수정일   : 수정내역
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
