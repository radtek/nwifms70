// PastDueTrades.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "PastDueTrades.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPastDueTrades dialog


CPastDueTrades::CPastDueTrades(CWnd* pParent /*=NULL*/)
	: CDialog(CPastDueTrades::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPastDueTrades)
	//}}AFX_DATA_INIT
}


void CPastDueTrades::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPastDueTrades)
	DDX_Control(pDX, IDC_PASTDUE_TRADE_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPastDueTrades, CDialog)
	//{{AFX_MSG_MAP(CPastDueTrades)
	ON_BN_CLICKED(IDC_PASTDUE_COPY_BUTTON, OnPastdueCopyButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPastDueTrades message handlers

BOOL CPastDueTrades::OnInitDialog() 
{
	CDialog::OnInitDialog();

	COraLoader OraLoader;
	CQData QData;
	CString Sql, Date, T_Minus3, TraderSql;

	BeginWaitCursor();
	m_SS.SetVisibleRows(24);
	m_SS.SetVisibleCols(12);

	OraLoader = m_pData->GetOraLoader();
	OraLoader.Today(Date);
	T_Minus3 = QData.GetQueryDate(OraLoader.TMinus3(T_Minus3, Date));

//	if(!(m_pData->IsSupervisor() || m_pData->IsOperation()))
	if(m_pData->IsTrader())
		TraderSql.Format("AND C.TRADER_INI = '%s' ", (LPCTSTR) m_pData->GetTraderIni());

	Sql.Format("SELECT A.PORTFOLIO, A.VALUE_DATE, A.COUNTERPARTY, "
		"DECODE(DIR, 'P', A.TRANS_TYPE, 'S', A.TRANS_TYPE, 'SEC-'||TR_DESC) \"TRANS_TYPE\", "
		"C.ASSET_CODE, A.TRANS_NUM, A.TRADE_DATE, DIR, A.NOM_AMOUNT, A.CURRENCY, EXRATE, "
		"A.PRICE, OPT_EXP, STRIKE, "
		"DECODE(DIR, 'P', -1, 'S', 1)*AMOUNT/DECODE(A.PAY_CUR, 'USD', 1, EXRATE) \"AMOUNT\", "
		"DECODE(DIR, 'P', -1, 'S', 1)*INTEREST/DECODE(A.PAY_CUR, 'USD', 1, EXRATE) \"INTEREST\", "
		"DECODE(DIR, 'P', -1, 'S', 1)*DECODE(A.PAY_CUR, 'USD', TO_NUMBER(NULL), NVL(AMOUNT, 0)+NVL(INTEREST, 0)) \"TOTAL\", "
		"ASS_DESC "
		"FROM SEMAM.NW_OPEN_TRADE_2 A "
		"LEFT OUTER JOIN SEMAM.NW_ASSETS B ON (A.ASS_CODE = B.ASS_CODE) "
		"JOIN SEMAM.NW_TR_TICKETS C ON (A.TRANS_NUM = C.TRANS_NUM) "
		"JOIN SEMAM.NW_PORTFOLIOS D ON (A.PORTFOLIO = D.PORTFOLIO) "
		"WHERE A.VALUE_DATE < %s "
		"AND A.TRANS_TYPE IN ('SECURITIES','CALL','PUT')  "
		"AND DIR IN ('P', 'S') "
		"AND B.ASS_CONV_TYPE IS NULL "
		"AND D.STATUS IS NULL ", (LPCTSTR) T_Minus3);
	
	OraLoader.GetSql() = Sql;
	if(m_pData->IsTrader())
		OraLoader.GetSql() += TraderSql;

	Sql.Format("UNION ALL "
		"SELECT A.PORTFOLIO, A.VALUE_DATE, A.COUNTERPARTY, "
		"SUBSTR(A.TRANS_TYPE,1,4)||'-'||SUBSTR(A.TR_DESC,1,1) \"TRANS_TYPE\", "
		"C.ASSET_CODE, A.TRANS_NUM, A.TRADE_DATE, A.DIR, "
		"DECODE(A.TRANS_TYPE, 'LEVERAGE', A.PRICE*1000000, A.NOM_AMOUNT) \"NOM_AMOUNT\", "
		"A.CURRENCY, EXRATE, A.PRICE, A.MATURITY_DATE, TO_NUMBER(NULL), "
		"DECODE(A.TRANS_TYPE, 'REPO', 1, 'LEVERAGE', -1, 1)*"
			"DECODE(A.TR_DESC, 'BOOKING', 1, 'MATURIY', -1, 1)*"
			"DECODE(A.DIR, 'P', -1, 'S', 1)*A.AMOUNT/DECODE(A.PAY_CUR, 'USD', 1, EXRATE) \"AMOUNT\", "
		"DECODE(A.TRANS_TYPE, 'REPO', 1, 'LEVERAGE', -1, 1)*"
			"DECODE(A.TR_DESC, 'BOOKING', 1, 'MATURIY', -1, 1)*"
			"DECODE(A.DIR, 'P', -1, 'S', 1)*SIGN(NVL(TR_RATE, 0.01))*"
			"INTEREST/DECODE(A.PAY_CUR, 'USD', 1, EXRATE) \"INTEREST\", "
		"DECODE(A.TRANS_TYPE, 'REPO', 1, 'LEVERAGE', -1, 1)*"
			"DECODE(A.TR_DESC, 'BOOKING', 1, 'MATURIY', -1, 1)*"
			"DECODE(A.DIR, 'P', -1, 'S', 1)*DECODE(A.CURRENCY, 'USD', TO_NUMBER(NULL), "
			"NVL(AMOUNT, 0) + SIGN(NVL(TR_RATE, .01))*NVL(INTEREST, 0)) \"F_AMOUNT\", ASS_DESC "
		"FROM SEMAM.NW_OPEN_TRADE_2 A "
		"LEFT OUTER JOIN SEMAM.NW_ASSETS B ON (A.ASS_CODE = B.ASS_CODE) "
		"JOIN SEMAM.NW_TR_TICKETS C ON (A.TRANS_NUM = C.TRANS_NUM) "
		"JOIN SEMAM.NW_PORTFOLIOS D ON (A.PORTFOLIO = D.PORTFOLIO) "
		"WHERE A.VALUE_DATE < %s "
		"AND (A.TRANS_TYPE NOT IN ('SECURITIES', 'CALL', 'PUT', 'FOREX', 'INT. SWAP') OR "
			"A.TRANS_TYPE IN ('SECURITIES','CALL','PUT') AND A.DIR NOT IN ('P', 'S')) "
			"AND D.STATUS IS NULL ", (LPCTSTR) T_Minus3);

	OraLoader.GetSql() += Sql;
	if(m_pData->IsTrader())
		OraLoader.GetSql() += TraderSql;
	
	Sql.Format("UNION ALL "
		"SELECT A.PORTFOLIO, A.VALUE_DATE, A.COUNTERPARTY, "
		"SUBSTR(A.TRANS_TYPE,1,4)||'-'||SUBSTR(A.TR_DESC,1,1) \"TRANS_TYPE\", "
		"C.ASSET_CODE, A.TRANS_NUM, A.TRADE_DATE, A.DIR, "
		"DECODE(A.TRANS_TYPE, 'LEVERAGE', A.PRICE*1000000, A.NOM_AMOUNT) \"NOM_AMOUNT\", "
		"A.CURRENCY, EXRATE, A.PRICE, A.MATURITY_DATE, TO_NUMBER(NULL), "
		"DECODE(A.DIR, 'P', -1, 'S', 1)*DECODE(A.TRANS_TYPE, 'FOREX', "
			"DECODE(A.TR_DESC, 'FX-BKNG', 1, 'BOOKING', -1)*"
			"A.AMOUNT/DECODE(A.PAY_CUR, 'USD', 1, EXRATE), 0) \"AMOUNT\", "
		"DECODE(A.DIR, 'P', -1, 'S', 1)*"
			"DECODE(A.TRANS_TYPE, 'FOREX', 0, A.INTEREST/DECODE(A.PAY_CUR, 'USD', 1, EXRATE)) \"INTEREST\", "
		"DECODE(A.DIR, 'P', -1, 'S', 1)*"
			"DECODE(A.TRANS_TYPE, 'FOREX', DECODE(A.TR_DESC, 'BOOKING', A.NOM_AMOUNT), "
            "'INT. SWAP', DECODE(A.PAY_CUR, 'USD', TO_NUMBER(NULL), A.NOM_AMOUNT)) \"F_AMOUNT\", ASS_DESC "
		"FROM SEMAM.NW_OPEN_TRADE_2 A "
		"LEFT OUTER JOIN SEMAM.NW_ASSETS B ON (A.ASS_CODE = B.ASS_CODE) "
		"JOIN SEMAM.NW_TR_TICKETS C ON (A.TRANS_NUM = C.TRANS_NUM) "
		"JOIN SEMAM.NW_PORTFOLIOS D ON (A.PORTFOLIO = D.PORTFOLIO) "
		"WHERE A.VALUE_DATE < %s "
		"AND A.TRANS_TYPE IN ('FOREX', 'INT. SWAP') "
		"AND D.STATUS IS NULL ", (LPCTSTR) T_Minus3);
	
	OraLoader.GetSql() += Sql;
	if(m_pData->IsTrader())
		OraLoader.GetSql() += TraderSql;

	OraLoader.GetSql() += "ORDER BY 1, 2, 3, 4, 5, 6 ";

	if(!OraLoader.Open())
		return FALSE;

	if(!OraLoader.LoadDBSheet(m_SS))
		return FALSE;
	
	GetDlgItem(IDC_PASTDUE_COPY_BUTTON)->EnableWindow(FALSE);
	SetWindowText("Past Due Trades");
	EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CPastDueTrades, CDialog)
    //{{AFX_EVENTSINK_MAP(CPastDueTrades)
	ON_EVENT(CPastDueTrades, IDC_PASTDUE_TRADE_LIST, 2 /* BlockSelected */, OnBlockSelectedPastdueTradeList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPastDueTrades::OnPastdueCopyButton() 
{
	m_SS.SetBlockMode(TRUE);
	m_SS.SetCol(m_SS.GetSelBlockCol());
	m_SS.SetCol2(m_SS.GetSelBlockCol2());
	m_SS.SetRow(m_SS.GetSelBlockRow());
	m_SS.SetRow2(m_SS.GetSelBlockRow2());
	m_SS.SetBlockMode(FALSE);
	m_SS.ClipboardCopy();
}

void CPastDueTrades::OnBlockSelectedPastdueTradeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	GetDlgItem(IDC_PASTDUE_COPY_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}
