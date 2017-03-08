// NavChange.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "NavChange.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNavChange dialog


CNavChange::CNavChange(CWnd* pParent /*=NULL*/)
	: CDialog(CNavChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNavChange)
	m_Portfolio = NULL;
	//}}AFX_DATA_INIT
}

CNavChange::~CNavChange()
{
	if(m_Portfolio)
		delete m_Portfolio;
}

void CNavChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNavChange)
	DDX_Control(pDX, IDC_NAV_SHEET, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNavChange, CDialog)
	//{{AFX_MSG_MAP(CNavChange)
	ON_BN_CLICKED(IDC_NAV_SAVE_BUTTON, OnNavSaveButton)
	ON_EN_CHANGE(IDC_NAV_DATE_EDIT, OnChangeNavDateEdit)
	ON_CBN_SELCHANGE(IDC_NAV_PORTFOLIO_COMBO, OnSelchangeNavPortfolioCombo)
	ON_BN_CLICKED(IDC_NAV_RUN_BUTTON, OnNavRunButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CNavChange::UpdateSum()
{	
	double Estimate = 0, Actual = 0, Difference = 0;
	CString Text;
	CQData QData;

	for (int i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		Estimate += atof(QData.RemoveComma(m_SS.GetSheetText(2, i)));
		Actual += atof(QData.RemoveComma(m_SS.GetSheetText(3, i)));
	}

	GetDlgItem(IDC_NAV_ESTIMATE_EDIT)->SetWindowText(QData.WriteNumber(Estimate, TRUE, 2));
	GetDlgItem(IDC_NAV_ACTUAL_EDIT)->SetWindowText(QData.WriteNumber(Actual, TRUE, 2));
	GetDlgItem(IDC_NAV_DIFFERENCE_EDIT)->SetWindowText(QData.WriteNumber(Estimate - Actual, TRUE, 2));
}
/////////////////////////////////////////////////////////////////////////////
// CNavChange message handlers

BOOL CNavChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString Date, Count;

	m_Date.Setup(this, IDC_NAV_DATE_EDIT);
	m_Portfolio = (COptComboBox *) new COptComboBox(this, IDC_NAV_PORTFOLIO_COMBO);
		
	if(!m_OraLoader.Open("SELECT PORTFOLIO FROM SEMAM.NW_PORTFOLIOS WHERE STATUS IS NULL "))
		return FALSE;
	
	BeginWaitCursor();
	m_OraLoader.LoadComboBox(*m_Portfolio);
	m_Date.SetData(m_OraLoader.Today(Date));

	m_SS.SetVisibleCols(4);
	m_SS.SetVisibleRows(18);
	m_SS.SetSheetToFit(m_SS.GetVisibleCols(), m_SS.GetVisibleRows());
	m_SS.SetCol(2);
	m_SS.SetCol2(4);
	m_SS.SetBlockMode(TRUE);
 //   m_SS.SetCellType(SS_CELL_TYPE_FLOAT);
 //   m_SS.SetTypeHAlign(SS_CELL_H_ALIGN_RIGHT);
    m_SS.SetTypeNumberDecPlaces(2);
    m_SS.SetTypeNumberMin(-999999999.0);
    m_SS.SetTypeNumberMax((double) 999999999);
 	m_SS.SetTypeCurrencySeparator(",");
    m_SS.SetTypeCurrencySymbol("$");
 	m_SS.SetTypeCurrencySeparator(",");
	m_SS.SetTypeCurrencyShowSep(TRUE);

    m_SS.SetBlockMode(FALSE);

	GetDlgItem(IDC_NAV_ESTIMATE_EDIT)->SetWindowText("0");
	GetDlgItem(IDC_NAV_ACTUAL_EDIT)->SetWindowText("0");
	GetDlgItem(IDC_NAV_DIFFERENCE_EDIT)->SetWindowText("0");
	EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNavChange::OnChangeNavDateEdit() 
{
	GetDlgItem(IDC_NAV_RUN_BUTTON)->EnableWindow(!m_Date.GetData().IsEmpty() && m_Portfolio->GetCurSel() >= 0 ? TRUE : FALSE);
}

void CNavChange::OnSelchangeNavPortfolioCombo() 
{
	GetDlgItem(IDC_NAV_RUN_BUTTON)->EnableWindow(!m_Date.GetData().IsEmpty() && m_Portfolio->GetCurSel() >= 0 ? TRUE : FALSE);
}

void CNavChange::OnNavRunButton() 
{
	CQData QData;
	CString Date, Portfolio, Count;

	Date = m_Date.GetData();
	
	Date = QData.GetQueryDate(Date);
	m_Portfolio->GetSelString(Portfolio);
	Portfolio = QData.GetQueryText(Portfolio);
	m_OraLoader.GetSql().Format("SELECT CATEGORY, ESTIMATE, ACTUAL, DAILY_CHANGE "
							"FROM SEMAM.NW_DAILY_CATEGORY_NAV A "
							"JOIN SEMAM.NW_INV_STRATEGIES B ON (A.CATEGORY = B.STRATEGY) "
							"WHERE INDATE = %s AND PORTFOLIO = %s ORDER BY CAT_INDEX ", 
							(LPCTSTR) Date, (LPCTSTR) Portfolio);
	if(m_OraLoader.Open())
	{
		m_SS.ClearSheet();
		m_OraLoader.LoadDBSheet(m_SS);
		UpdateSum();
	}
}

void CNavChange::OnNavSaveButton() 
{
	CQData QData;
	CString Date, Portfolio, Category;
	CDBRec Rec;
	int i, j;

	Date = m_Date.GetData();
	Date = QData.GetQueryDate(Date);
	m_Portfolio->GetSelString(Portfolio);
	Portfolio = QData.GetQueryText(Portfolio);
	for(i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		Category = m_SS.GetSheetText(1, i);
		Category = QData.GetQueryText(Category);
		Rec.RemoveAll();
		for(j = 2; j <= 4; j ++)
			Rec.Add(QData.RemoveComma(m_SS.GetSheetText(j, i)));
		m_OraLoader.GetSql().Format("SELECT ESTIMATE, ACTUAL, DAILY_CHANGE "
						"FROM SEMAM.NW_DAILY_CATEGORY_NAV "
						"WHERE INDATE = %s AND PORTFOLIO = %s AND CATEGORY = %s ",
						(LPCTSTR) Date, (LPCTSTR) Portfolio, (LPCTSTR) Category);
		if(m_OraLoader.Open(m_OraLoader.GetSql(), ODYNASET_DEFAULT))
			m_OraLoader.UpdateRecord(Rec);
	}
}

BEGIN_EVENTSINK_MAP(CNavChange, CDialog)
    //{{AFX_EVENTSINK_MAP(CNavChange)
	ON_EVENT(CNavChange, IDC_NAV_SHEET, 4 /* Change */, OnChangeNavSheet, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CNavChange::OnChangeNavSheet(long Col, long Row) 
{
	UpdateSum();
}
