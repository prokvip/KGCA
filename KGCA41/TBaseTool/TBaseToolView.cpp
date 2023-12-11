
// TBaseToolView.cpp: CTBaseToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "TBaseTool.h"
#endif

#include "TBaseToolDoc.h"
#include "TBaseToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTBaseToolView

IMPLEMENT_DYNCREATE(CTBaseToolView, CView)

BEGIN_MESSAGE_MAP(CTBaseToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTBaseToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTBaseToolView 생성/소멸

CTBaseToolView::CTBaseToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTBaseToolView::~CTBaseToolView()
{
}

BOOL CTBaseToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTBaseToolView 그리기

void CTBaseToolView::OnDraw(CDC* /*pDC*/)
{
	CTBaseToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTBaseToolView 인쇄


void CTBaseToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTBaseToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTBaseToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTBaseToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CTBaseToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTBaseToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTBaseToolView 진단

#ifdef _DEBUG
void CTBaseToolView::AssertValid() const
{
	CView::AssertValid();
}

void CTBaseToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTBaseToolDoc* CTBaseToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTBaseToolDoc)));
	return (CTBaseToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CTBaseToolView 메시지 처리기
