
// NWIFMS70View.cpp : implementation of the CNWIFMS70View class
//

#include "stdafx.h"
#include "NWIFMS70.h"

#include "NWIFMS70Doc.h"
#include "NWIFMS70View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNWIFMS70View

IMPLEMENT_DYNCREATE(CNWIFMS70View, CView)

BEGIN_MESSAGE_MAP(CNWIFMS70View, CView)
END_MESSAGE_MAP()

// CNWIFMS70View construction/destruction

CNWIFMS70View::CNWIFMS70View()
{
	// TODO: add construction code here

}

CNWIFMS70View::~CNWIFMS70View()
{
}

BOOL CNWIFMS70View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNWIFMS70View drawing

void CNWIFMS70View::OnDraw(CDC* /*pDC*/)
{
	CNWIFMS70Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CNWIFMS70View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNWIFMS70View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CNWIFMS70View diagnostics

#ifdef _DEBUG
void CNWIFMS70View::AssertValid() const
{
	CView::AssertValid();
}

void CNWIFMS70View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNWIFMS70Doc* CNWIFMS70View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNWIFMS70Doc)));
	return (CNWIFMS70Doc*)m_pDocument;
}
#endif //_DEBUG


// CNWIFMS70View message handlers
