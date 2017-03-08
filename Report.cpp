// Report.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "Report.h"
#include "reportdef.h"
#include "qdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReport

IMPLEMENT_DYNCREATE(CReport, CFormView)

CReport::CReport()
	: CFormView(CReport::IDD)
{
	//{{AFX_DATA_INIT(CReport)
    m_Date = NULL;
    m_Report = NULL;
    m_ReportDef = NULL;
	//}}AFX_DATA_INIT
}

CReport::~CReport()
{
	if(m_Date)
		delete m_Date;
	if(m_Report)
		delete m_Report;
	if(m_ReportDef)
		delete m_ReportDef;
}

void CReport::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReport)
	DDX_Control(pDX, IDC_REPORT_LIST, m_SS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReport, CFormView)
	//{{AFX_MSG_MAP(CReport)
	ON_COMMAND(ID_REPORT_SETUP, OnReportSetup)
	ON_CBN_SELCHANGE(IDC_REPORT_DATE_COMBO, OnSelchangeReportDateCombo)
	ON_CBN_SELCHANGE(IDC_REPORT_REPORT_COMBO, OnSelchangeReportReportCombo)
	ON_CBN_SELCHANGE(IDC_REPORT_DEF_COMBO, OnSelchangeReportDefCombo)
	ON_COMMAND(ID_REPORT_SELECT_BLOCK, OnReportSelectBlock)
	ON_UPDATE_COMMAND_UI(ID_REPORT_SELECT_BLOCK, OnUpdateReportSelectBlock)
	ON_COMMAND(ID_REPORT_COPY, OnReportCopy)
	ON_UPDATE_COMMAND_UI(ID_REPORT_COPY, OnUpdateReportCopy)
	ON_COMMAND(ID_REPORT_RETRIEVE, OnReportRetrieve)
	ON_UPDATE_COMMAND_UI(ID_REPORT_RETRIEVE, OnUpdateReportRetrieve)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReport diagnostics

#ifdef _DEBUG
void CReport::AssertValid() const
{
	CFormView::AssertValid();
}

void CReport::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CReport::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReport message handlers

void CReport::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();

	GetDocument()->GetData().LoadDates();
	m_Data.SetOraLoader(GetDocument()->GetData().GetOraLoader());
	m_Data.SetUserIni(GetDocument()->GetData().GetOraUser());
	if(m_Data.GetAvail().GetSize() <= 0)
		m_Data.LoadReportArray();

    m_Date = new COptComboBox(this, IDC_REPORT_DATE_COMBO);
    m_Report = new COptComboBox(this, IDC_REPORT_REPORT_COMBO);
    m_ReportDef = new COptComboBox(this, IDC_REPORT_DEF_COMBO);
	
	m_SS.SetVisibleCols(11);
	m_SS.SetVisibleRows(21);
	m_SS.SetSheetRows(0);

	for(int i = 0; i < m_Data.GetAvail().GetReportArray().GetSize(); i ++)
		m_Report->AddString(m_Data.GetAvail().GetReportArray().GetAt(i)->GetAt(0));

	GetDocument()->SetViewTitle("REPORT");
	EndWaitCursor();
}

void CReport::OnReportSetup() 
{
	CReportDef Dlg;

	Dlg.m_ReportData = &m_Data;
	Dlg.DoModal();
}

void CReport::OnSelchangeReportDateCombo() 
{
	int Index;

	Index = m_Date->GetCurSel();
	if(Index < 0)
		return;
}

void CReport::OnSelchangeReportReportCombo() 
{
	int Index;
	CQData QData;
	
	Index = m_Report->GetCurSel();
	if(Index < 0)
		return;

	BeginWaitCursor();
	m_Data.SetTableName(m_Data.GetAvail().GetReportArray().GetAt(Index)->GetAt(1));
	m_Data.SetReportID(m_Data.GetAvail().GetReportArray().GetAt(Index)->GetAt(2));
	m_Data.SetWithUnion(m_Data.GetAvail().GetReportArray().GetAt(Index)->GetAt(3));
	m_Data.LoadReportDefArray();
	
	m_ReportDef->ResetContent();

	QData.CopyDBRecArrayToComboBox(m_Data.GetSel().GetReportArray(), *m_ReportDef, 0, FALSE);
	m_ReportDef->InsertString(0, m_Data.GetAvail().GetReportArray().GetAt(Index)->GetAt(0));
	m_Data.GetSel().RemoveAll();
	m_Data.LoadDate(m_Date);

	if(m_ReportDef->GetCount() == 1)
		m_ReportDef->SetCurSel(0);
	if(m_Date->GetCount() == 1)
		m_Date->SetCurSel(0);
	EndWaitCursor();
}

void CReport::OnSelchangeReportDefCombo() 
{
	int Index;
	
	Index = m_ReportDef->GetCurSel();
	if(Index < 0)
		return;
	
	BeginWaitCursor();
	if(Index == 0)
	{
		m_Data.SetDefID(EMPTYSTRING);
		m_Data.GetSel().RemoveAll();
	}
	else
	{
		m_Data.SetDefID(m_Data.GetSel().GetReportArray().GetAt(Index - 1)->GetAt(1));
		m_Data.LoadSelFieldArray();
	}

	EndWaitCursor();
}

void CReport::OnReportSelectBlock() 
{
	m_SS.SetSelection(1, 1, m_SS.GetSheetCols(), m_SS.GetSheetRows());
}

void CReport::OnUpdateReportSelectBlock(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SS.GetSheetRows() > 0);
}

void CReport::OnReportCopy() 
{
	m_SS.ClipboardCopy();
}

void CReport::OnUpdateReportCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SS.GetIsBlockSelected());
}

void CReport::OnReportRetrieve()
{
	CString Date;

	m_Date->GetSelString(Date);
	if(Date.IsEmpty() || m_ReportDef->GetCurSel() < 0)
		return;

	BeginWaitCursor();
	m_Data.RunReport(m_SS, Date);
	EndWaitCursor();
}

void CReport::OnUpdateReportRetrieve(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Report->GetCurSel() >= 0 && m_ReportDef->GetCurSel() >= 0 &&
					m_Date->GetCurSel() >= 0);
}
