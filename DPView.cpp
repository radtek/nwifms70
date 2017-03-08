// DPView.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "DPView.h"
#include "rletter.h"
#include "qdata.h"
#include "datedlg.h"
#include "runreportdlg.h"
#include "bulkcopy.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDPView

IMPLEMENT_DYNCREATE(CDPView, CEditView)

CDPView::CDPView()
{
}

CDPView::~CDPView()
{
}


BEGIN_MESSAGE_MAP(CDPView, CEditView)
	//{{AFX_MSG_MAP(CDPView)
	ON_COMMAND(ID_DP_CUSTODY, OnDpCustody)
	ON_COMMAND(ID_DP_RUNREPORT, OnDpRunReport)	
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_DP_BULKCOPY, OnDpBulkcopy)
	ON_UPDATE_COMMAND_UI(ID_DP_BULKCOPY, OnUpdateDpBulkcopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDPView drawing

void CDPView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDPView diagnostics

#ifdef _DEBUG
void CDPView::AssertValid() const
{
	CEditView::AssertValid();
}

void CDPView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CNWIFMS70Doc* CDPView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDPView message handlers

void CDPView::OnDpCustody() 
{
	CString Path;
	CQData QData;
	CDateDlg InDlg;
	CCustodyLetter CustodyLetter;
	LPCTSTR TempFile = "Letter Template File";
	LPCTSTR Template = "Template";

	GetData().GetOraLoader().Today(InDlg.m_Data);
	CustodyLetter.m_RunDate = InDlg.m_Data;

	if(InDlg.DoModal() != IDOK)
		return;

	Path = "Template Files (*.DOC) | *.DOC | All Files (*.*) | *.* ||";

	if(!QData.GetFileName(AfxGetApp(), Path, Template, TempFile, "DOC"))
		return;

	BeginWaitCursor();
	CustodyLetter.m_Date = InDlg.m_Data;
	CustodyLetter.m_Template = Path;

	CustodyLetter.Generate(GetData().GetOraLoader());
	EndWaitCursor();
}

void CDPView::OnDpRunReport()
{
	CRunReportDlg Dlg;

	Dlg.m_pOraLoader = &GetData().GetOraLoader();

	Dlg.DoModal();
}

void CDPView::OnKillFocus(CWnd* pNewWnd) 
{
	CEditView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}

void CDPView::OnDpBulkcopy() 
{
	CBulkCopy Dlg;

	Dlg.m_pOraLoader = &GetData().GetOraLoader();
	Dlg.DoModal();
}

void CDPView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
	GetDocument()->GetData().LoadDates();
}

void CDPView::OnUpdateDpBulkcopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}
