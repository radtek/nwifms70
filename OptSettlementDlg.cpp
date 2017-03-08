// OptSettlementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "OptSettlementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "qdata.h"

/////////////////////////////////////////////////////////////////////////////
// COptSettlementDlg dialog


COptSettlementDlg::COptSettlementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptSettlementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptSettlementDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptSettlementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptSettlementDlg)
	DDX_Control(pDX, IDC_OPTSET_LIST, m_SS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptSettlementDlg, CDialog)
	//{{AFX_MSG_MAP(COptSettlementDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptSettlementDlg message handlers

BEGIN_EVENTSINK_MAP(COptSettlementDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(COptSettlementDlg)
	ON_EVENT(COptSettlementDlg, IDC_OPTSET_LIST, 11 /* DblClick */, OnDblClickOptSetList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL COptSettlementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();
	m_SS.SetVisibleCols(3);
	m_SS.SetVisibleRows(10);

	m_OptSetEdit.Setup(this, IDC_OPTSET_SET_EDIT);
	m_ZoneEdit.Setup(this, IDC_OPTSET_ZONE_EDIT);
	m_TimeEdit.Setup(this, IDC_OPTSET_TIME_EDIT);

	if(m_OraLoader.Open("SELECT OPT_SET_CODE, OPT_SET_ZONE, OPT_SET_TIME "
						"FROM SEMAM.NW_OPT_SETTLEMENT ORDER BY 2, 3"))
		m_OraLoader.LoadDBSheet(m_SS, TRUE);
	GetDlgItem(IDOK)->EnableWindow(m_OptSetEdit.GetWindowTextLength() > 0 ? TRUE :FALSE);

	if(!m_SetCode.IsEmpty())
	{
		for(int i = 1; i <= m_SS.GetSheetRows(); i++)
		{
			if(m_SetCode == m_SS.GetSheetText(1, i))
				OnDblClickOptSetList(1, i);
		}
	}

	SetWindowText(m_Title);
	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptSettlementDlg::OnDblClickOptSetList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_OptSetEdit.SetWindowText(m_SS.GetSheetText(1, Row));
		m_ZoneEdit.SetWindowText(m_SS.GetSheetText(2, Row));
		m_TimeEdit.SetWindowText(m_SS.GetSheetText(3, Row));
		m_SS.SetSheetCurRow(Row);
	}
	else
	{
		m_OptSetEdit.SetWindowText("");
		m_ZoneEdit.SetWindowText("");
		m_TimeEdit.SetWindowText("");
	}

	GetDlgItem(IDOK)->EnableWindow(m_OptSetEdit.GetWindowTextLength() > 0 ? TRUE :FALSE);
}

void COptSettlementDlg::OnOK() 
{
	m_OptSetEdit.GetWindowText(m_SetCode);

	CDialog::OnOK();
}

