// CouponDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "CouponDlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCouponDlg dialog


CCouponDlg::CCouponDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCouponDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCouponDlg)
	m_bCash = TRUE;
	//}}AFX_DATA_INIT
}


void CCouponDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCouponDlg)
	DDX_Control(pDX, IDC_COUPON_LIST, m_SS);
	DDX_Control(pDX, IDC_COUPON_INFO_LIST, m_sSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCouponDlg, CDialog)
	//{{AFX_MSG_MAP(CCouponDlg)
	ON_EN_KILLFOCUS(IDC_COUPON_AMOUNT_EDIT, OnKillfocusCouponAmountEdit)
	ON_EN_KILLFOCUS(IDC_COUPON_FXRATE_EDIT, OnKillfocusCouponFxrateEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCouponDlg message handlers

BEGIN_EVENTSINK_MAP(CCouponDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCouponDlg)
	ON_EVENT(CCouponDlg, IDC_COUPON_LIST, 11 /* DblClick */, OnDblClickCouponList, VTS_I4 VTS_I4)
	ON_EVENT(CCouponDlg, IDC_COUPON_INFO_LIST, 11 /* DblClick */, OnDblClickCouponInfoList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCouponDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString Sql, SqlPhrase = "AND A.INSTRUCTED IS NULL ";

	BeginWaitCursor();
	m_SS.SetVisibleRows(10);
	m_SS.SetVisibleCols(3);

	m_sSS.SetVisibleRows(10);
	if(m_CouponType == 1)
		m_sSS.SetVisibleCols(6);
	else
		m_sSS.SetVisibleCols(5);
    
    m_SS.SetCol(3);
    m_SS.SetCol2(3);
	m_SS.SetBlockMode(TRUE);
 	m_SS.SetTypeCurrencySeparator(",");
	m_SS.SetTypeCurrencyShowSep(TRUE);
    m_SS.SetTypeCurrencySymbol("$");
    m_SS.SetTypeCurrencyDecimal(",");
    m_SS.SetTypeCurrencyDecPlaces(2);
	m_SS.SetBlockMode(FALSE);
	
	m_FxrateEdit.Setup(this, IDC_COUPON_FXRATE_EDIT, NULL, -1, FALSE);
	m_AmountEdit.Setup(this, IDC_COUPON_AMOUNT_EDIT, NULL, 2);
	m_RemainingEdit.Setup(this, IDC_COUPON_REMAINING_EDIT, NULL, 2);
	m_DateEdit.Setup(this, IDC_COUPON_DATE_EDIT);

	switch(m_CouponType)
	{
		case 1: // CDS only
			if(m_Currency == "USD")
			{
				Sql ="SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", B.CP_TRADE_ID, A.TRANS_NUM, SUM(A.ACCRUAL) \"AMOUNT\" "
					 "FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
					 "WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
					 "AND A.PMNT_TYPE IN ('CDS') "
					 "AND A.PORTFOLIO = '" + m_Portfolio + "' "
					 "AND A.COUPON_PAID IS NULL "
					 "AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 ";
			    
				if(!m_bCash)
					 Sql += SqlPhrase;
				
				Sql += "GROUP BY A.ASSET_CODE, B.CP_TRADE_ID, A.TRANS_NUM "
					   "ORDER BY 1, 2, 4 ";
			}
			else
			{
				Sql = "SELECT A.ASSET_CODE, A.CURRENCY, B.CP_TRADE_ID, A.TRANS_NUM, SUM(A.L_ACCRUAL) \"AMOUNT\" "
					   "FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
					   "WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
					   "AND A.PMNT_TYPE IN ('CDS') "
					   "AND A.PORTFOLIO = '" + m_Portfolio + "' "
					   "AND A.CURRENCY = '" + m_Currency + "' "
					   "AND A.COUPON_PAID IS NULL "
					   "AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 ";
			   
				if(!m_bCash)
					 Sql += SqlPhrase;
				
				Sql += "GROUP BY A.ASSET_CODE, A.CURRENCY, B.CP_TRADE_ID, A.TRANS_NUM "
						"ORDER BY 1, 2, 4 ";
			}

			break;
		
		case 2: // IRS Only
			if(m_Currency == "USD")
			{
				Sql = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", B.CP_TRADE_ID, A.TRANS_NUM, SUM(A.ACCRUAL) \"AMOUNT\" "
						"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
						"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
						"AND A.PMNT_TYPE IN ('INT. SWAP') "
						"AND A.PORTFOLIO = '" + m_Portfolio + "' "
						"AND A.COUPON_PAID IS NULL "
						"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 ";
				
				if(!m_bCash)
					Sql += SqlPhrase;
				
				Sql += "GROUP BY A.ASSET_CODE, B.CP_TRADE_ID, A.TRANS_NUM "
						"ORDER BY 1, 2, 4 ";
			}
			else
			{
				Sql = "SELECT A.ASSET_CODE, A.CURRENCY, B.CP_TRADE_ID, A.TRANS_NUM, SUM(A.L_ACCRUAL) \"AMOUNT\" "
						"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
						"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
						"AND A.PMNT_TYPE IN ('INT. SWAP') "
						"AND A.PORTFOLIO = '" + m_Portfolio + "' "
						"AND A.CURRENCY = '" + m_Currency + "' "
						"AND A.COUPON_PAID IS NULL "
						"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 ";
				if(!m_bCash)
					Sql += SqlPhrase;

				Sql += "GROUP BY A.ASSET_CODE, A.CURRENCY, B.CP_TRADE_ID, A.TRANS_NUM "
						"ORDER BY 1, 2, 4 ";
			}
			break;
		case 3: // All others
			if(m_Currency == "USD")
			{
				Sql = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", SUM(ACCRUAL) \"AMOUNT\" "
						"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A "
						"WHERE PORTFOLIO = '" + m_Portfolio + "' "
						"AND A.PMNT_TYPE = 'SECURITIES' "
						"AND A.COUPON_PAID IS NULL "
						"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 ";
				
				if(!m_bCash)
					Sql += SqlPhrase;
				
				Sql += "GROUP BY ASSET_CODE "
						"ORDER BY 1, 2 ";
			}
			else
			{
				Sql = "SELECT A.ASSET_CODE, CURRENCY, SUM(L_ACCRUAL) \"AMOUNT\" "
						"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A "
						"WHERE PORTFOLIO = '" + m_Portfolio + "' "
						"AND A.PMNT_TYPE = 'SECURITIES' "
						"AND CURRENCY = '" + m_Currency + "' "
						"AND A.COUPON_PAID IS NULL "
						"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 ";
				
				if(!m_bCash)
					Sql += SqlPhrase;
				
				Sql += "GROUP BY ASSET_CODE, CURRENCY "
						"ORDER BY 1, 2 ";
			}
			break;
		case 4: // Dividend
			if(m_Currency == "USD")
				Sql = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", SUM(AMOUNT) \"AMOUNT\" "
					  "FROM SEMAM.NW_DIV_RECEIVABLE A "
					  "WHERE PORTFOLIO = '" + m_Portfolio + "' "
					  "AND A.PMNT_TYPE = 'DIVIDENT P' "
					  "AND PAID = 'N' "
					  "GROUP BY ASSET_CODE "
					  "ORDER BY 1, 2 ";
			else
				Sql = "SELECT A.ASSET_CODE, CURRENCY, SUM(L_AMOUNT) \"AMOUNT\" "
					  "FROM SEMAM.NW_DIV_RECEIVABLE A "
					  "WHERE PORTFOLIO = '" + m_Portfolio + "' "
					  "AND A.CURRENCY = '" + m_Currency + "' "
					  "AND A.PMNT_TYPE = 'DIVIDENT P' "
					  "AND PAID = 'N' "
					  "GROUP BY ASSET_CODE, CURRENCY "
					  "ORDER BY 1, 2 ";
			break;
		case 5: // Tax
			if(m_Currency == "USD")
			{
				Sql = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", SUM(AMOUNT) \"AMOUNT\" "
					  "FROM SEMAM.NW_DIV_RECEIVABLE A "
					  "WHERE PORTFOLIO = '" + m_Portfolio + "' "
					  "AND A.PMNT_TYPE = 'TAX' "
					  "AND PAID = 'N' "
					  "GROUP BY ASSET_CODE "
					  "ORDER BY 1, 2 ";
			}
			else
			{
				Sql = "SELECT A.ASSET_CODE, CURRENCY, SUM(L_AMOUNT) \"AMOUNT\" "
					 "FROM SEMAM.NW_DIV_RECEIVABLE A "
					 "WHERE PORTFOLIO = '" + m_Portfolio + "' "
					 "AND CURRENCY = '" + m_Currency + "' "
					 "AND A.PMNT_TYPE = 'TAX' "
					 "AND PAID = 'N' "
					 "GROUP BY ASSET_CODE, CURRENCY "
					 "ORDER BY 1, 2 ";
			}
			break;
		default:
			break;
	}

	if(m_OraLoader.Open(Sql))
	{
		switch(m_CouponType)
		{
			case 1: // CDS
			case 2: // IRS
				m_OraLoader.GetFieldArray().GetAt(3)->SetPrecision(0);
				m_OraLoader.GetFieldArray().GetAt(3)->SetWithComma(FALSE);
				m_OraLoader.GetFieldArray().GetAt(4)->SetPrecision(2);
				m_OraLoader.GetFieldArray().GetAt(4)->SetWithComma(TRUE);
				break;
			case 3:
			case 4:
			case 5:
				m_OraLoader.GetFieldArray().GetAt(2)->SetPrecision(2);
				m_OraLoader.GetFieldArray().GetAt(2)->SetWithComma(TRUE);
				break;
		}

		m_OraLoader.LoadDBSheet(m_SS, TRUE);
	}

	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCouponDlg::OnDblClickCouponList(long Col, long Row) 
{
	BOOL bOK;

	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		m_Asset = m_SS.GetSheetText(1, Row);
		bOK = TRUE;

		CQData QData;
		CString Asset, Sql, SqlPhrase = "AND B.INSTRUCTED IS NULL ";

		Asset = QData.GetQueryText(m_Asset);
		Asset += " ";

		switch(m_CouponType)
		{
			case 1: // CDS
				if(m_Currency == "USD")
				{
					Sql = "SELECT A.ASS_TO, 'USD' \"CURRENCY\", SUM(A.ACCRUAL) \"CASH\", SUM(A.L_ACCRUAL)/SUM(A.ACCRUAL) \"FXRATE\", "
							"SUM(A.L_ACCRUAL) \"L_ACCRUAL\", A.TRANS_NUM "
							"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
							"WHERE B.PORTFOLIO = A.PORTFOLIO "
							"AND B.ASSET_CODE = A.ASSET_CODE "
							"AND B.ASSET_TO = A.ASS_TO "
							"AND A.TRANS_NUM = B.TRANS_NUM "
							"AND B.COUPON_PAID IS NULL "
							"AND ABS(B.RECEIVABLE - B.RECEIVED) > 0 ";
					
					if(!m_bCash)
						Sql += SqlPhrase;
					
					Sql += "AND A.PORTFOLIO = '" + m_Portfolio + "' "
						   "AND A.ASSET_CODE = " + Asset + 
						   "GROUP BY A.ASS_TO, A.TRANS_NUM "  
						   "ORDER BY 1 DESC ";
				}
				else
				{
					Sql = "SELECT A.ASS_TO, A.CURRENCY, SUM(A.L_ACCRUAL) \"CASH\", TO_NUMBER(NULL) \"FXRATE\" , "
							"SUM(A.L_ACCRUAL) \"L_ACCRUAL\", A.TRANS_NUM "
							"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
							"WHERE B.PORTFOLIO = A.PORTFOLIO "
							"AND B.ASSET_CODE = A.ASSET_CODE "
							"AND B.ASSET_TO = A.ASS_TO "
							"AND B.TRANS_NUM = A.TRANS_NUM "
							"AND B.COUPON_PAID IS NULL "
							"AND ABS(B.RECEIVABLE - B.RECEIVED) > 0 ";
					
					if(!m_bCash)
						Sql += SqlPhrase;
					
					Sql += "AND A.PORTFOLIO = '" + m_Portfolio + "' "
							"AND A.ASSET_CODE = " + Asset + 
							"AND A.CURRENCY = '" + m_Currency + "' "
							"GROUP BY A.ASS_TO, A.CURRENCY, A.TRANS_NUM "
							"ORDER BY 1 DESC ";
				}
				break;
			case 2: // IRS
				if(m_Currency == "USD")
				{
					Sql = "SELECT A.ASS_TO, 'USD' \"CURRENCY\", SUM(A.ACCRUAL) \"CASH\", SUM(A.L_ACCRUAL)/SUM(A.ACCRUAL) \"FXRATE\", "
							"SUM(A.L_ACCRUAL) \"L_ACCRUAL\", A.TRANS_NUM "
							"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
							"WHERE B.PORTFOLIO = A.PORTFOLIO "
							"AND B.ASSET_CODE = A.ASSET_CODE "
							"AND B.ASSET_TO = A.ASS_TO "
							"AND B.TRANS_NUM = A.TRANS_NUM "
							"AND B.COUPON_PAID IS NULL "
							"AND ABS(B.RECEIVABLE - B.RECEIVED) > 0 ";
					
					if(!m_bCash)
						Sql += SqlPhrase;
					
					Sql += "AND A.PORTFOLIO = '" + m_Portfolio + "' "
							"AND A.ASSET_CODE = " + Asset + 
							"GROUP BY A.ASS_TO, A.TRANS_NUM "  
							"ORDER BY 1 DESC ";
				}
				else
				{
					Sql = "SELECT A.ASS_TO, A.CURRENCY, SUM(A.L_ACCRUAL) \"CASH\", TO_NUMBER(NULL) \"FXRATE\" , "
							"SUM(A.L_ACCRUAL) \"L_ACCRUAL\", A.TRANS_NUM "
							"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
							"WHERE B.PORTFOLIO = A.PORTFOLIO "
							"AND B.ASSET_CODE = A.ASSET_CODE "
							"AND B.ASSET_TO = A.ASS_TO "
							"AND B.TRANS_NUM = A.TRANS_NUM "
							"AND B.COUPON_PAID IS NULL "
							"AND ABS(B.RECEIVABLE - B.RECEIVED) > 0 ";
					
					if(!m_bCash)
						Sql += SqlPhrase;
					
					Sql += "AND A.PORTFOLIO = '" + m_Portfolio + "' "
							"AND A.ASSET_CODE = " + Asset + 
							" AND A.CURRENCY = '" + m_Currency + "' "
							"GROUP BY A.ASS_TO, A.CURRENCY, A.TRANS_NUM "
							"ORDER BY 1 DESC ";
				}
				break;

			case 3: // Others
				if(m_Currency == "USD")
				{
					Sql = "SELECT A.ASS_TO, 'USD' \"CURRENCY\", SUM(A.ACCRUAL) \"CASH\", SUM(A.L_ACCRUAL)/SUM(A.ACCRUAL) \"FXRATE\", "
							"SUM(A.L_ACCRUAL) \"L_ACCRUAL\" "
							"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
							"WHERE B.PORTFOLIO = A.PORTFOLIO "
							"AND B.ASSET_CODE = A.ASSET_CODE "
							"AND B.ASSET_TO = A.ASS_TO "
							"AND B.COUPON_PAID IS NULL "
							"AND ABS(B.RECEIVABLE - B.RECEIVED) > 0 ";
					
					if(!m_bCash)
						Sql += SqlPhrase;

					Sql += "AND A.TRANS_TYPE IN ('SECURITIES', 'REPO') "
							"AND A.PORTFOLIO = '" + m_Portfolio + "' "
							"AND A.ASSET_CODE = " + Asset + 
							"GROUP BY A.ASS_TO "
							"ORDER BY 1 DESC ";
				}
				else
				{
					Sql = "SELECT A.ASS_TO, A.CURRENCY, SUM(A.L_ACCRUAL) \"CASH\", TO_NUMBER(NULL) \"FXRATE\", "
							"SUM(A.L_ACCRUAL) \"L_ACCRUAL\" "
							"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
							"WHERE B.PORTFOLIO = A.PORTFOLIO "
							"AND B.ASSET_CODE = A.ASSET_CODE "
							"AND B.ASSET_TO = A.ASS_TO "
							"AND B.COUPON_PAID IS NULL "
							"AND ABS(B.RECEIVABLE - B.RECEIVED) > 0 ";
					
					if(!m_bCash)
						Sql += SqlPhrase;

					Sql += "AND A.TRANS_TYPE IN ('SECURITIES', 'REPO') "
							"AND A.PORTFOLIO = '" + m_Portfolio + "' "
							"AND A.ASSET_CODE = " + Asset + 
							"AND A.CURRENCY = '" + m_Currency + "' "
							"GROUP BY A.ASS_TO, A.CURRENCY "
							"ORDER BY 1 DESC ";
				}
				break;
			case 4: // Dividend
				if(m_Currency == "USD") 
					Sql = "SELECT A.EXD_DATE, 'USD' \"CURRENCY\", SUM(AMOUNT) \"CASH\", SUM(L_AMOUNT)/SUM(AMOUNT) \"FXRATE\", "
							"SUM(L_AMOUNT) \"L_AMOUNT\" "
							"FROM SEMAM.NW_DIV_RECEIVABLE A "
							"WHERE PORTFOLIO = '" + m_Portfolio + "' "
							"AND ASSET_CODE = " + Asset + 
							"AND A.PMNT_TYPE = 'DIVIDENT P' "
							"AND PAID = 'N' "
							"GROUP BY EXD_DATE "
							"ORDER BY 1, 2 ";
				else
					Sql = "SELECT A.EXD_DATE, CURRENCY, SUM(L_AMOUNT) \"CASH\", TO_NUMBER(NULL) \"FXRATE\", SUM(L_AMOUNT) \"L_AMOUNT\" "
							"FROM SEMAM.NW_DIV_RECEIVABLE A "
							"WHERE PORTFOLIO = '" + m_Portfolio + "' "
							"AND ASSET_CODE = " + Asset + 
							"AND CURRENCY = '" + m_Currency + "' "
							"AND A.PMNT_TYPE = 'DIVIDENT P' "
							"AND PAID = 'N' "
							"GROUP BY EXD_DATE, CURRENCY "
							"ORDER BY 1, 2 ";
				break;
			case 5: // Tax
				if(m_Currency == "USD")
					Sql = "SELECT A.EXD_DATE, 'USD' \"CURRENCY\", SUM(AMOUNT) \"CASH\", SUM(L_AMOUNT)/SUM(AMOUNT) \"FXRATE\", "
							"SUM(L_AMOUNT) \"L_AMOUNT\" "
							"FROM SEMAM.NW_DIV_RECEIVABLE A "
							"WHERE PORTFOLIO = '" + m_Portfolio + "' "
							"AND ASSET_CODE = " + Asset + 
							" AND A.PMNT_TYPE = 'TAX' "
							"AND PAID = 'N' "
							"GROUP BY EXD_DATE "
							"ORDER BY 1, 2 ";
				else
					Sql = "SELECT A.EXD_DATE, CURRENCY, SUM(L_AMOUNT) \"CASH\", TO_NUMBER(NULL) \"FXRATE\", SUM(L_AMOUNT) \"L_AMOUNT\" "
							"FROM SEMAM.NW_DIV_RECEIVABLE A "
							"WHERE PORTFOLIO = '" + m_Portfolio + "' "
							"AND ASSET_CODE = " + Asset + 
							"AND CURRENCY = '" + m_Currency + "' "
							"AND A.PMNT_TYPE = 'TAX' "
							"AND PAID = 'N' "
							"GROUP BY A.EXD_DATE, CURRENCY "
							"ORDER BY 1, 2 ";
				break;
			default:
				break;
		}

		if(m_OraLoader.Open(Sql))
		{
			m_OraLoader.GetFieldArray().GetAt(2)->SetPrecision(2);
			m_OraLoader.GetFieldArray().GetAt(2)->SetWithComma(TRUE);
			m_OraLoader.GetFieldArray().GetAt(3)->SetPrecision(-1);
			m_OraLoader.GetFieldArray().GetAt(3)->SetWithComma(FALSE);
			if(m_CouponType == 1) // CDS 
			{
				m_OraLoader.GetFieldArray().GetAt(4)->SetPrecision(0);
				m_OraLoader.GetFieldArray().GetAt(4)->SetWithComma(FALSE);
			}
			m_OraLoader.LoadDBSheet(m_sSS, TRUE);
		}
	}
	else
	{
		m_Asset.Empty();
		m_Currency.Empty();
		m_sSS.ClearSheet();
		bOK = FALSE;
	}

	GetDlgItem(IDC_COUPON_ASSET_EDIT)->SetWindowText(m_Asset);
	GetDlgItem(IDC_COUPON_CURRENCY_EDIT)->SetWindowText(m_Currency);
	GetDlgItem(IDOK)->EnableWindow(bOK);
}

void CCouponDlg::OnDblClickCouponInfoList(long Col, long Row)
{
	CString Text;

	m_Date.Empty();
	m_Fxrate.Empty();
	m_Amount.Empty();
	m_TransNum.Empty();

	if(Row > 0 && Row <= m_sSS.GetSheetRows())
	{
		m_Date = m_sSS.GetSheetText(1, Row);
		m_Amount = m_sSS.GetSheetText(3, Row);
		m_Fxrate = m_sSS.GetSheetText(4, Row);
		m_LAmount = m_sSS.GetSheetText(5, Row);
		if(m_CouponType == 1 || m_CouponType == 2)
			m_TransNum = m_sSS.GetSheetText(m_sSS.GetSheetCols(), Row);
	}
	
	m_bLoaded = FALSE;
	m_DateEdit.SetData(m_Date);
	if(m_Currency == "USD")
		m_FxrateEdit.SetData(m_Fxrate);
	else
		m_FxrateEdit.SetData("");
	m_FxrateEdit.EnableWindow(m_Fxrate.IsEmpty() || atof(m_Fxrate) == 1.0 ? FALSE : TRUE);
	
	m_AmountEdit.SetData(m_Amount);
	m_Remaining = "0.00";
	m_RemainingEdit.SetData(m_Remaining);

	m_bLoaded = TRUE;
}

void CCouponDlg::UpdateNumber(BOOL bFxChange)
{
	CString Text;
	double Fxrate, Amt, LAmt;
	CQData QData;

//	if(!m_bLoaded || m_Currency != "USD")
//		return;
	if(!m_bLoaded)
		return;

	LAmt = atof(QData.RemoveComma(m_LAmount));

	Text = m_FxrateEdit.GetData();

	if(Text.IsEmpty() || atoi(Text) == 1)
	{
		Amt = atof(QData.RemoveComma(m_AmountEdit.GetData()));
		m_Remaining = QData.WriteNumber(Amt - LAmt, TRUE, 2);
	}
	else
	{
		if(bFxChange)
		{
			Fxrate = atof(QData.RemoveComma(m_FxrateEdit.GetData()));
			if(Fxrate > 0)
			{
				Amt = LAmt/Fxrate;
				m_AmountEdit.SetData(QData.WriteNumber(Amt, TRUE, 2));
			}
		}
		else
		{
			Amt = atof(QData.RemoveComma(m_AmountEdit.GetData()));
			if(Amt != 0)
			{
				Fxrate = LAmt/Amt;
				m_FxrateEdit.SetData(QData.WriteNumber(Fxrate, FALSE, -1));
			}
		}
		m_Remaining = "0.00";
	}

	m_RemainingEdit.SetData(m_Remaining);
}

void CCouponDlg::OnKillfocusCouponAmountEdit() 
{
	UpdateNumber(FALSE);
}

void CCouponDlg::OnKillfocusCouponFxrateEdit() 
{
	UpdateNumber(TRUE);
}

void CCouponDlg::OnOK() 
{
	m_Amount = m_AmountEdit.GetData();
	m_Fxrate = m_FxrateEdit.GetData();
	m_Remaining = m_RemainingEdit.GetData();
	
	CDialog::OnOK();
}
