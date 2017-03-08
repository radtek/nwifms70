// CashIRSIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "CashIRSIDDlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCashIRSIDDlg dialog


CCashIRSIDDlg::CCashIRSIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCashIRSIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCashIRSIDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCashIRSIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCashIRSIDDlg)
	DDX_Control(pDX, IDC_CASHIRS_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCashIRSIDDlg, CDialog)
	//{{AFX_MSG_MAP(CCashIRSIDDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCashIRSIDDlg message handlers

BEGIN_EVENTSINK_MAP(CCashIRSIDDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCashIRSIDDlg)
	ON_EVENT(CCashIRSIDDlg, IDC_CASHIRS_LIST, 11 /* DblClick */, OnDblClickCashirsList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCashIRSIDDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CQData QData;
	CString Text;

	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(9);

    m_SS.SetCol(8);
    m_SS.SetCol2(8);
	m_SS.SetBlockMode(TRUE);
 	m_SS.SetTypeCurrencySeparator(",");
	m_SS.SetTypeCurrencyShowSep(TRUE);
    m_SS.SetTypeCurrencySymbol("$");
    m_SS.SetTypeCurrencyDecimal(",");
    m_SS.SetTypeCurrencyDecPlaces(2);
	m_SS.SetBlockMode(FALSE);

	BeginWaitCursor();
	if(m_PayType == "SWAP INT")
	{
		m_OraLoader.GetSql().Format("SELECT TRANS_TYPE \"TYPE\", DIR, ASSET_CODE, ASS_INT_TYPE, CURRENCY, "
				"TICKET_NUM, TRANS_NUM, NOM_AMOUNT, MATURITY_DATE "
				"FROM SEMAM.ALL_TICKET_INV_V A "
				"LEFT OUTER JOIN SEMAM.NW_ASSETS B ON (A.ASSET_CODE = B.ASS_CODE) "
				"WHERE A.TRANS_TYPE IN ('INT. SWAP') "
				"AND A.TR_DESC = 'MATURITY' "
				"AND NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, SYSDATE)) > TRUNC(SYSDATE) "
				"AND A.PORTFOLIO = %s ", QData.GetQueryText(m_Portfolio));

		if(!m_Currency.IsEmpty())
		{
			Text.Format("AND A.CURRENCY = %s ", QData.GetQueryText(m_Currency));
			m_OraLoader.GetSql() += Text;
		}
	}
	else
	{
		m_OraLoader.GetSql().Format("SELECT ASS_INDUSTRY \"TYPE\", TRANS_DIRECTION \"DIR\", ASSET_CODE, NULL, "
				"CURRENCY, TICKET_NUM, TRANS_NUM, A.NOM_AMOUNT, A.VALUE_DATE " 
				"FROM SEMAM.NW_TR_TICKETS A "
				"JOIN SEMAM.NW_ASSETS B ON (A.ASSET_CODE = B.ASS_CODE) "
				"WHERE B.ASS_INDUSTRY = 'CURRENCY FWDS' "
				"AND A.TRANS_TYPE = 'SECURITIES' "
				"AND A.VALUE_DATE > TRUNC(SYSDATE) "
				"AND A.PORTFOLIO = %s ", QData.GetQueryText(m_Portfolio));
	}
	
	m_OraLoader.GetSql() += "ORDER BY 1, 2, 3 ";

	if(m_OraLoader.Open())
		m_OraLoader.LoadDBSheet(m_SS, TRUE);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	EndWaitCursor();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCashIRSIDDlg::OnDblClickCashirsList(long Col, long Row) 
{
	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		CQData QData;

		GetDlgItem(IDC_CASHIRS_TYPE_EDIT)->SetWindowText(m_SS.GetSheetText(1, Row));
		GetDlgItem(IDC_CASHIRS_DIR_EDIT)->SetWindowText(m_SS.GetSheetText(2, Row));
		GetDlgItem(IDC_CASHIRS_ASSET_EDIT)->SetWindowText(m_SS.GetSheetText(3, Row));
		GetDlgItem(IDC_CASHIRS_RATE_TYPE_EDIT)->SetWindowText(m_SS.GetSheetText(4, Row));
		GetDlgItem(IDC_CASHIRS_CURRENCY_EDIT)->SetWindowText(m_SS.GetSheetText(5, Row));
		GetDlgItem(IDC_CASHIRS_TICKET_EDIT)->SetWindowText(m_SS.GetSheetText(6, Row));
		m_TransNum = m_SS.GetSheetText(7, Row);
		m_TransNum = QData.RemoveComma(m_TransNum);
		GetDlgItem(IDC_CASHIRS_TRANS_NUM_EDIT)->SetWindowText(m_TransNum);
		GetDlgItem(IDC_CASHIRS_NOMAMOUNT_EDIT)->SetWindowText(m_SS.GetSheetText(8, Row));
		GetDlgItem(IDC_CASHIRS_MATURITY_EDIT)->SetWindowText(m_SS.GetSheetText(9, Row));

		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDOK)->EnableWindow(FALSE);
}
