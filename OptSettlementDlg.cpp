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

void COptSettlementDlg::LoadData()
{
	if(m_OraLoader.Open("SELECT OPT_SET_CODE, OPT_SET_ZONE, OPT_SET_TIME, OPT_SET_TIME_24, OPT_COUNTRY "
						"FROM SEMAM.NW_OPT_SETTLEMENT ORDER BY 2, 3") > 0)
		m_OraLoader.LoadDBSheet(m_SS, TRUE);

	GetDlgItem(IDOK)->EnableWindow(m_OptSetEdit.GetWindowTextLength() > 0 ? TRUE :FALSE);
}

BEGIN_MESSAGE_MAP(COptSettlementDlg, CDialog)
	//{{AFX_MSG_MAP(COptSettlementDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPTSET_CLEAR_BUTTON, &COptSettlementDlg::OnBnClickedOptsetClearButton)
	ON_BN_CLICKED(IDC_OPTSET_SAVE_BUTTON, &COptSettlementDlg::OnBnClickedOptsetSaveButton)
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
	m_SS.SetVisibleCols(5);
	m_SS.SetVisibleRows(10);

	m_OptSetEdit.Setup(this, IDC_OPTSET_SET_EDIT);
	m_ZoneEdit.Setup(this, IDC_OPTSET_ZONE_EDIT);
	m_TimeEdit.Setup(this, IDC_OPTSET_TIME_EDIT);
	m_Time24Edit.Setup(this, IDC_OPTSET_TIME24_EDIT);
	m_Country.Setup(this, IDC_OPTSET_COUNTRY_EDIT);

	LoadData();

	if(m_SS.GetSheetRows() > 0)
		OnDblClickOptSetList(1, 1);

	SetWindowText(m_Title);
	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptSettlementDlg::OnDblClickOptSetList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);
		m_OptSetEdit.SetData(m_SS.GetSheetText(1, Row));
		m_ZoneEdit.SetData(m_SS.GetSheetText(2, Row));
		m_TimeEdit.SetData(m_SS.GetSheetText(3, Row));
		m_Time24Edit.SetData(m_SS.GetSheetText(4, Row));
		m_Country.SetData(m_SS.GetSheetText(5, Row));
	}
	else
	{
		m_OptSetEdit.SetData("");
		m_ZoneEdit.SetData("");
		m_TimeEdit.SetData("");
		m_Time24Edit.SetData("");
		m_Country.SetData("");
	}

	GetDlgItem(IDOK)->EnableWindow(m_OptSetEdit.GetWindowTextLength() > 0 ? TRUE :FALSE);
}

void COptSettlementDlg::OnOK() 
{
	m_SetCode = m_OptSetEdit.GetData();

	CDialog::OnOK();
}

void COptSettlementDlg::OnBnClickedOptsetClearButton()
{
	OnDblClickOptSetList(0, 0);
}

void COptSettlementDlg::OnBnClickedOptsetSaveButton()
{
	CString OptSetCode;
	CDBRec Rec;

	OptSetCode = m_OptSetEdit.GetData();
	Rec.Add(OptSetCode);
	Rec.Add(m_ZoneEdit.GetData());
	Rec.Add(m_TimeEdit.GetData());
	Rec.Add(m_Time24Edit.GetData());
	Rec.Add(m_Country.GetData());

	if(OptSetCode.IsEmpty() || strlen(Rec.GetAt(1)) == 0 || strlen(Rec.GetAt(2)) == 0  || strlen(Rec.GetAt(3)) == 0  || strlen(Rec.GetAt(0)) == 4)
	{
		MessageBox("Imcomplete Data", "OPTSET_CODE", MB_OK);
		return;
	}

	if(m_OraLoader.GetCount("SELECT COUNT(*) FROM SEMAM.NW_OPT_SETTLEMENT WHERE OPT_SET_CODE = '" + OptSetCode + "' ") > 0)
	{
		m_OraLoader.Open("SELECT OPT_SET_CODE, OPT_SET_ZONE, OPT_SET_TIME, OPT_SET_TIME_24, OPT_COUNTRY FROM SEMAM.NW_OPT_SETTLEMENT WHERE OPT_SET_CODE = '" + OptSetCode + "' ", ODYNASET_DEFAULT);
		m_OraLoader.UpdateRecord(Rec);
	}
	else
	{
		m_OraLoader.Open("SELECT OPT_SET_CODE, OPT_SET_ZONE, OPT_SET_TIME, OPT_SET_TIME_24, OPT_COUNTRY FROM SEMAM.NW_OPT_SETTLEMENT ", ODYNASET_DEFAULT);
		m_OraLoader.UpdateRecord(Rec, TRUE);
	}

	LoadData();
}
