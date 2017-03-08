// ErrorEntryLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "ErrorEntryLinkDlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorEntryLinkDlg dialog


CErrorEntryLinkDlg::CErrorEntryLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorEntryLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CErrorEntryLinkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CErrorEntryLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorEntryLinkDlg)
	DDX_Control(pDX, IDC_ERROR_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorEntryLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CErrorEntryLinkDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorEntryLinkDlg message handlers

BEGIN_EVENTSINK_MAP(CErrorEntryLinkDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CErrorEntryLinkDlg)
	ON_EVENT(CErrorEntryLinkDlg, IDC_ERROR_LIST, 11 /* DblClick */, OnDblClickErrorList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CErrorEntryLinkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CQData qData;
	CString Text;

	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

    m_SS.SetCol(5);
    m_SS.SetCol2(5);
	m_SS.SetBlockMode(TRUE);
 	m_SS.SetTypeCurrencySeparator(",");
	m_SS.SetTypeCurrencyShowSep(TRUE);
    m_SS.SetTypeCurrencySymbol("$");
    m_SS.SetTypeCurrencyDecimal(",");
    m_SS.SetTypeCurrencyDecPlaces(2);
	m_SS.SetBlockMode(FALSE);

	BeginWaitCursor();
	m_OraLoader.GetSql().Format("SELECT A.ACC_CODE, A.CURRENCY, A.CLOSE_DATE, A.CASH_NUM, "
				"A.AMOUNT + SUM(NVL(B.AMOUNT, 0)) \"AMOUNT\" "
				"FROM SEMAM.NW_CASH_PMNTS A "
				"LEFT OUTER JOIN SEMAM.NW_CASH_PMNTS B ON (A.CASH_NUM = B.LINK "
						"AND A.PORTFOLIO = B.PORTFOLIO AND A.ASS_CODE =  B.ASS_CODE) "
				"WHERE A.LINK IS NULL "
				"AND A.AMOUNT != 0 "
				"AND A.ASS_CODE IN ('ERRORENTRY', 'RECON') "
				"AND A.PORTFOLIO = %s ", qData.GetQueryText(m_Portfolio));

	if(!m_Account.IsEmpty())
	{
		Text.Format("AND A.ACC_CODE = %s ", qData.GetQueryText(m_Account));
		m_OraLoader.GetSql() += Text;
	}

	if(!m_Currency.IsEmpty())
	{
		Text.Format("AND A.CURRENCY = %s ", qData.GetQueryText(m_Currency));
		m_OraLoader.GetSql() += Text;
	}

	m_OraLoader.GetSql() += "GROUP BY A.ACC_CODE, A.CURRENCY, A.CLOSE_DATE, A.CASH_NUM, A.AMOUNT "
							"HAVING ABS(A.AMOUNT + SUM(NVL(B.AMOUNT, 0))) > 0.005 "
							"ORDER BY 1, 2, 3, 4 ";

	if(m_OraLoader.Open())
		m_OraLoader.LoadDBSheet(m_SS, TRUE);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CErrorEntryLinkDlg::OnDblClickErrorList(long Col, long Row) 
{
	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		CQData QData;

		GetDlgItem(IDC_ERROR_ACCOUNT_EDIT)->SetWindowText(m_SS.GetSheetText(1, Row));
		GetDlgItem(IDC_ERROR_CURRENCY_EDIT)->SetWindowText(m_SS.GetSheetText(2, Row));
		GetDlgItem(IDC_ERROR_CLOSE_DATE_EDIT)->SetWindowText(m_SS.GetSheetText(3, Row));
		m_CashNum = m_SS.GetSheetText(4, Row);
		m_CashNum = QData.RemoveComma(m_CashNum);
		GetDlgItem(IDC_ERROR_CASH_NUM_EDIT)->SetWindowText(m_CashNum);
		GetDlgItem(IDC_ERROR_AMOUNT_EDIT)->SetWindowText(m_SS.GetSheetText(5, Row));

		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDOK)->EnableWindow(FALSE);
}
