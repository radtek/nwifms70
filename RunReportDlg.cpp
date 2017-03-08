// RunReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "RunReportDlg.h"
#include "runreport.h"
#include "reportdate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunReportDlg dialog


CRunReportDlg::CRunReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunReportDlg)
	m_PrintFlag = NULL;	
	//}}AFX_DATA_INIT
}


CRunReportDlg::~CRunReportDlg()
{
	if(m_PrintFlag)
		delete m_PrintFlag;
}

void CRunReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunReportDlg)
	DDX_Control(pDX, IDC_RUNREPORT_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunReportDlg, CDialog)
	//{{AFX_MSG_MAP(CRunReportDlg)
	ON_BN_CLICKED(IDC_RUNREPORT_RUN_BUTTON, OnRunreportRunButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRunReportDlg::LoadReport()
{
	if(m_pOraLoader->Open("SELECT REPORT, RUN_FILE, PRINT_FILE, PRINT_DEF, THE_DATE, F_DATE, "
							"L_DATE,  F_NAV_DATE, F_ACCR_DATE, F_FUND_DATE, PREV_YEAR_END, "
							"REPORT_TYPE FROM SEMAM.NW_RUN_REPORTS ORDER BY 1 "))
		m_pOraLoader->LoadRowCtrl(m_Row, TRUE, TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// CRunReportDlg message handlers
BEGIN_EVENTSINK_MAP(CRunReportDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CRunReportDlg)
	ON_EVENT(CRunReportDlg, IDC_RUNREPORT_LIST, 11 /* DblClick */, OnDblClickRunreportList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CRunReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(15);
	m_SS.SetVisibleCols(1);
	
	m_PrintFlag = (CCheckBox*) new CCheckBox(this, IDC_RUNREPORT_CHECK);
	m_ReportEdit.Setup(this, IDC_RUNREPORT_EDIT);

	m_Row.Add(&m_ReportEdit, &m_Report);
	m_Row.Add(&m_RunFile);
	m_Row.Add(&m_PrintFile);
	m_Row.Add(&m_PrintDef);
	m_Row.Add(&m_TheDate);
	m_Row.Add(&m_FDate);
	m_Row.Add(&m_LDate);	
	m_Row.Add(&m_FNavDate);
	m_Row.Add(&m_AccrDate);
	m_Row.Add(&m_FundDate);
	m_Row.Add(&m_PrevYearEnd);
	m_Row.Add(&m_Cmd);
	m_Row.Setup(&m_SS);

	LoadReport();
	
	if(m_SS.GetSheetRows() > 0)
		m_SS.EnableWindow(TRUE);

	GetDlgItem(IDC_RUNREPORT_RUN_BUTTON)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunReportDlg::OnRunreportRunButton() 
{
	if(m_RunFile.IsEmpty())
		return;
	
	if(!m_PrintFlag->IsWindowEnabled() || m_PrintFlag->GetCheck() != BST_CHECKED)
		m_PrintFile.Empty();

	CRunReport RunRp;
	if(atoi(m_Cmd) == PRC_REPORT_TYPE)
		RunRp.Setup(m_Cmd, m_RunFile, m_PrintFile, m_PrintDef);
	else
	{
		CReportDate RpDate;

		RpDate.m_Report = m_Report;
		m_pOraLoader->Today(RpDate.m_Today);
		RpDate.AddDateItem(m_PrevYearEnd, "PREV_YEAR_DATE", "Previous Year End", DATE_PREV_YEAR_END);
		RpDate.AddDateItem(m_FNavDate, "F_NAV_DATE", "Previous Nav Date", DATE_PREV_MONTH_END);
		RpDate.AddDateItem(m_AccrDate, "F_ACCR_DATE", "Accrual Start Date", DATE_MONTH_START);
		RpDate.AddDateItem(m_FundDate, "F_FUND_DATE", "Fund Start Date", DATE_FUND_START);
		RpDate.AddDateItem(m_FDate, "F_DATE", "Report Start Date", DATE_MONTH_START);
		RpDate.AddDateItem(m_LDate, "L_DATE", "Report End Date", DATE_TODAY);
		RpDate.AddDateItem(m_TheDate, "THE_DATE", "Report Date", DATE_TODAY);

		if(RpDate.DoModal() == IDOK)
		{
			CReportDateItem *pItem;

			RunRp.Setup(m_Cmd, m_RunFile, m_PrintFile, m_PrintDef);
			for(int i = 0; i < RpDate.m_DateArray.GetSize(); i ++)
			{
				pItem = RpDate.m_DateArray.GetAt(i);
				if(pItem)
					RunRp.SetDate(pItem->GetName(), pItem->GetDate());
			}
		}
	}

	RunRp.RunReport();
}

void CRunReportDlg::OnDblClickRunreportList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Row.SetCurrentRow(Row);		
	else
		m_Row.SetCurrentRow(-1);

	m_PrintFlag->EnableWindow(!m_PrintFile.IsEmpty());

	m_PrintFlag->SetCheck(0);

	GetDlgItem(IDC_RUNREPORT_RUN_BUTTON)->EnableWindow(m_Row.GetCurrentRow() > 0);
}
