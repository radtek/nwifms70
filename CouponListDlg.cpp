// CouponListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "CouponListDlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCouponListDlg dialog


CCouponListDlg::CCouponListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCouponListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCouponListDlg)
	m_Counterparty = "";
	m_bCash = TRUE;
	//}}AFX_DATA_INIT
}

void CCouponListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCouponListDlg)
	DDX_Control(pDX, IDC_COUPONLIST_SEL_LIST, m_SelSS);
	DDX_Control(pDX, IDC_COUPONLIST_LIST, m_SS);
	DDX_Control(pDX, IDC_COUPONLIST_INFO_LIST, m_sSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCouponListDlg, CDialog)
	//{{AFX_MSG_MAP(CCouponListDlg)
	ON_BN_CLICKED(IDC_COUPONLIST_UPDATE_BUTTON, OnCouponListUpdateButton)
	ON_BN_CLICKED(IDC_COUPONLIST_REMOVE_BUTTON, OnCouponListRemoveButton)
	ON_EN_KILLFOCUS(IDC_COUPONLIST_AMOUNT_EDIT, OnKillfocusCouponListAmountEdit)
	ON_EN_KILLFOCUS(IDC_COUPONLIST_FXRATE_EDIT, OnKillfocusCouponListFxrateEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCouponListDlg::UpdateFields()
{
	int i = 0;
	double Amount, Remaining;
	CQData QData;

	Amount = 0;
	Remaining = 0;
	for (i = 1; i <= m_SelSS.GetSheetRows(); i++)
	{
		Amount += atof(QData.RemoveComma(m_SelSS.GetSheetText(4, i)));
		Remaining += atof(QData.RemoveComma(m_SelSS.GetSheetText(8, i)));
	}

	m_TotalEdit.SetData(QData.WriteNumber(Amount, TRUE, 2));
	m_Amount = m_TotalEdit.GetData();
	m_TotalRemainingEdit.SetData(QData.WriteNumber(Remaining, TRUE, 2));
	m_Remaining = m_TotalRemainingEdit.GetData();

	GetDlgItem(IDOK)->EnableWindow(m_SelSS.GetSheetRows() > 0 ? TRUE : FALSE);
}

void CCouponListDlg::UpdateNumber(BOOL bFxChange)
{
	CString Text;
	double Fxrate, Amt, LAmt;
	CQData QData;

	if(!m_bLoaded)
		return;

	LAmt = atof(QData.RemoveComma(m_LAmount));

	Text = m_FxrateEdit.GetData();

	if(Text.IsEmpty() || atof(QData.RemoveComma(Text)) == 1.0)
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
/////////////////////////////////////////////////////////////////////////////
// CCouponListDlg message handlers

BEGIN_EVENTSINK_MAP(CCouponListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCouponListDlg)
	ON_EVENT(CCouponListDlg, IDC_COUPONLIST_LIST, 11 /* DblClick */, OnDblClickCouponList, VTS_I4 VTS_I4)
	ON_EVENT(CCouponListDlg, IDC_COUPONLIST_SEL_LIST, 11 /* DblClick */, OnDblClickCouponSelList, VTS_I4 VTS_I4)
	ON_EVENT(CCouponListDlg, IDC_COUPONLIST_INFO_LIST, 11 /* DblClick */, OnDblClickCouponInfoList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCouponListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	CString Port, CP, Curr, SqlPhrase = "AND A.INSTRUCTED IS NULL ";

	m_SelSS.SetVisibleRows(7);
	m_SelSS.SetVisibleCols(6);

	m_SS.SetVisibleRows(10);

	m_sSS.SetVisibleRows(10);
	m_sSS.SetVisibleCols(8);
    	
	m_AssetEdit.Setup(this, IDC_COUPONLIST_ASSET_EDIT);
	m_DateEdit.Setup(this, IDC_COUPONLIST_TODATE_EDIT);
	m_FxrateEdit.Setup(this, IDC_COUPONLIST_FXRATE_EDIT);
	m_AmountEdit.Setup(this, IDC_COUPONLIST_AMOUNT_EDIT, NULL, 2);
	m_RemainingEdit.Setup(this, IDC_COUPONLIST_REMAINING_EDIT, NULL, 2);
	m_CouponPaid.Setup(this, IDC_COUPONLIST_PAID_CHECK);
	m_TotalEdit.Setup(this, IDC_COUPONLIST_TOTALAMOUNT_EDIT, NULL, 2);
	m_TotalRemainingEdit.Setup(this, IDC_COUPONLIST_TOTALREMAINING_EDIT, NULL, 2);

	Port = QData.GetQueryText(m_Portfolio);
	Port += " ";
	CP = QData.GetQueryText(m_Counterparty);
	CP += " ";
	Curr = QData.GetQueryText(m_Currency);
	Curr += " ";

	switch(m_CouponType)
	{
		case 1: // CDS only
			m_SS.SetVisibleCols(5);
			if(m_Currency == "USD")
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", B.CP_TRADE_ID, A.TRANS_NUM, "
										"A.ACCRUAL \"AMOUNT\", 'Y' \"COUPON_PAID\" "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
										"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
										"AND A.PMNT_TYPE IN ('CDS') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND A.PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				if(!m_Counterparty.IsEmpty())
					m_OraLoader.GetSql() += "AND B.COUNTERPARTY = " + CP;
			}
			else
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, A.CURRENCY, B.CP_TRADE_ID, A.TRANS_NUM, "
										"A.L_ACCRUAL \"AMOUNT\", 'Y' \"COUPON_PAID\" "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
										"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
										"AND A.PMNT_TYPE IN ('CDS') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND A.PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				if(!m_Currency.IsEmpty())
					m_OraLoader.GetSql() += "AND A.CURRENCY = " + Curr;

				if(!m_Counterparty.IsEmpty())
					m_OraLoader.GetSql() += "AND B.COUNTERPARTY = " + CP; 
			}
			
			m_OraLoader.GetSql() += "ORDER BY 1, 2, 4 ";
			break;
		
		case 2: // IRS
			if(m_Currency == "USD")
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", B.CP_TRADE_ID, A.TRANS_NUM, "
										"A.ACCRUAL \"AMOUNT\", 'Y' \"COUPON_PAID\" "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
										"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
										"AND A.PMNT_TYPE IN ('INT. SWAP') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND A.PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				if(!m_Counterparty.IsEmpty())
					m_OraLoader.GetSql() += "AND A.COUNTERPARTY = " + CP;
			}
			else
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, A.CURRENCY, B.CP_TRADE_ID, A.TRANS_NUM, "
										"A.L_ACCRUAL \"AMOUNT\", 'Y' \"COUPON_PAID\"  "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A, SEMAM.NW_TR_TICKETS B "
										"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
										"AND A.PMNT_TYPE IN ('INT. SWAP') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND A.PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				if(!m_Currency.IsEmpty())
					m_OraLoader.GetSql() += "AND A.CURRENCY = " + Curr;

				if(!m_Counterparty.IsEmpty())
					m_OraLoader.GetSql() += "AND A.COUNTERPARTY = " + CP;
			}
			m_OraLoader.GetSql() += "ORDER BY 1, 2, 4 ";
			break;

		case 3: // None CDS
			m_SS.SetVisibleCols(3);
			if(m_Currency == "USD")
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", SUM(A.ACCRUAL) \"AMOUNT\", 'Y' \"COUPON_PAID\" "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A "
										"WHERE A.PMNT_TYPE IN ('SECURITIES') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND A.PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				m_OraLoader.GetSql() += "GROUP BY A.ASSET_CODE HAVING ABS(SUM(A.L_ACCRUAL)) > 10 ";
			}
			else
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, A.CURRENCY, SUM(A.L_ACCRUAL) \"AMOUNT\", 'Y' \"COUPON_PAID\" "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A "
										"WHERE A.PMNT_TYPE IN ('SECURITIES') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND A.PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				if(!m_Currency.IsEmpty())
					m_OraLoader.GetSql() += "AND A.CURRENCY = " + Curr;

				m_OraLoader.GetSql() += "GROUP BY A.ASSET_CODE, A.CURRENCY "
										"HAVING ABS(SUM(A.L_ACCRUAL)) > 10 ";
			}
			
			m_OraLoader.GetSql() += "ORDER BY 1, 2 ";
			break;
	
		case 4: // All
			m_SS.SetVisibleCols(3);
			if(m_Currency == "USD")
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, 'USD' \"CURRENCY\", SUM(A.ACCRUAL) \"AMOUNT\", "
										"'Y' \"COUPON_PAID\" "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A "
										"WHERE A.PMNT_TYPE IN ('CDS', 'SECURITIES') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				m_OraLoader.GetSql() += "GROUP BY A.ASSET_CODE HAVING ABS(SUM(A.L_ACCRUAL)) > 10 ";
			}
			else
			{
				m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, A.CURRENCY, SUM(A.L_ACCRUAL) \"AMOUNT\", "
										"'Y' \"COUPON_PAID\" "
										"FROM SEMAM.NW_ASSET_INTEREST_FULL_V A "
										"WHERE A.PMNT_TYPE IN ('CDS', 'SECURITIES') "
										"AND A.COUPON_PAID IS NULL "
										"AND ABS(A.RECEIVABLE - A.RECEIVED) > 10 "
										"AND A.PORTFOLIO = " + Port;
				if(!m_bCash)
					m_OraLoader.GetSql() += SqlPhrase;

				if(!m_Currency.IsEmpty())
					m_OraLoader.GetSql() += "AND A.CURRENCY = " + Curr;
				
				m_OraLoader.GetSql() += "GROUP BY A.ASSET_CODE, A.CURRENCY "
										"HAVING ABS(SUM(A.L_ACCRUAL)) > 10 ";
			}

			m_OraLoader.GetSql() += "ORDER BY 1, 2 ";
			break;
		default:
			return FALSE;
	}

	if(m_OraLoader.Open())
	{
		switch(m_CouponType)
		{
			case 1: // CDS
			case 2:
			{
				m_OraLoader.GetFieldArray().GetAt(3)->SetPrecision(0);
				m_OraLoader.GetFieldArray().GetAt(3)->SetWithComma(FALSE);
				m_OraLoader.GetFieldArray().GetAt(4)->SetPrecision(2);
				m_OraLoader.GetFieldArray().GetAt(4)->SetWithComma(TRUE);
			}
			break;
			default:
			{
				m_OraLoader.GetFieldArray().GetAt(2)->SetPrecision(2);
				m_OraLoader.GetFieldArray().GetAt(2)->SetWithComma(TRUE);
			}
		}
		
		m_OraLoader.LoadDBSheet(m_SS, TRUE);
		
		m_OraLoader.Open("SELECT A.ASSET_CODE, ASS_TO, TRANS_TYPE, ACCRUAL, A.TRANS_NUM, "
						"INV_NUM, 0 ""FXRATE"", 0 ""REMAINING"", 'Y' \"COUPON_PAID\", "
						"0 ""L_ACCRUAL"", CURRENCY "
						"FROM SEMAM.NW_INTEREST_RECEIVABLE A "
						"WHERE 1 = 2 ");

		m_OraLoader.LoadDBSheet(m_SelSS, TRUE);
	}

	for (int i = 0; i < m_CouponArray.GetSize(); i ++)
		m_SS.AddSheetRow(*m_CouponArray.GetAt(i));
	UpdateFields();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCouponListDlg::OnDblClickCouponList(long Col, long Row) 
{
	BOOL bOK;
	
	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		BeginWaitCursor();
		m_Asset = m_SS.GetSheetText(1, Row);
		m_Currency = m_SS.GetSheetText(2, Row);
		if(m_CouponType == 1 || m_CouponType == 2)
			m_Amount = m_SS.GetSheetText(5, Row);
		else
			m_Amount = m_SS.GetSheetText(3, Row);
		bOK = TRUE;

		CString Port, Asset;
		CString Sql, SqlPhrase = "AND B.INSTRUCTED IS NULL ";
		CQData QData;
		
		Port = QData.GetQueryText(m_Portfolio);
		Port += " ";
		Asset = QData.GetQueryText(m_Asset);
		Asset += " ";

		if(m_Currency != "USD")
		{
			Sql = "SELECT A.CURRENCY, A.ASS_TO, A.TRANS_TYPE, SUM(A.L_ACCRUAL) \"L_ACCRUAL\", TO_NUMBER(NULL) \"CASH\", "
					"A.TRANS_NUM, A.INV_NUM, 1 \"FXRATE\", 0 \"REMAINING\", SUM(A.L_ACCRUAL) \"L_ACCRUAL\", 'Y' \"COUPON_PAID\" "
					"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
					"WHERE B.PORTFOLIO = A.PORTFOLIO "
					"AND B.ASSET_CODE = A.ASSET_CODE "
					"AND B.ASSET_TO = A.ASS_TO "
					"AND DECODE(B.PMNT_TYPE, 'CDS', A.TRANS_NUM, 1) = DECODE(B.PMNT_TYPE, 'CDS', B.TRANS_NUM, 1) "
					"AND B.COUPON_PAID IS NULL "
					"AND ABS(B.RECEIVABLE - B.RECEIVED) > 10 ";

			if(!m_bCash)
				Sql += SqlPhrase;

			Sql += "AND A.PORTFOLIO = " + Port + 
				   "AND A.ASSET_CODE = " + Asset + 
				   "AND A.CURRENCY = '" + m_Currency + "' "
				   "GROUP BY A.CURRENCY, A.ASS_TO, A.TRANS_TYPE, A.TRANS_NUM, A.INV_NUM "
				   "ORDER BY 1, 2, 3 DESC ";
		}
		else
		{
			Sql = "SELECT 'USD' \"CURRENCY\", A.ASS_TO, A.TRANS_TYPE, SUM(A.ACCRUAL) \"ACCRUAL\", TO_NUMBER(NULL) \"CASH\", A.TRANS_NUM, "
					"A.INV_NUM, SUM(A.L_ACCRUAL)/SUM(A.ACCRUAL) \"FXRATE\", 0 \"REMAINING\", SUM(A.L_ACCRUAL) \"L_ACCRUAL\", 'Y' \"COUPON_PAID\"  "
					"FROM SEMAM.NW_INTEREST_RECEIVABLE A, SEMAM.NW_ASSET_INTEREST_FULL_V B "
					"WHERE B.PORTFOLIO = A.PORTFOLIO "
					"AND B.ASSET_CODE = A.ASSET_CODE "
					"AND B.ASSET_TO = A.ASS_TO "
					"AND DECODE(B.PMNT_TYPE, 'CDS', A.TRANS_NUM, 1) = DECODE(B.PMNT_TYPE, 'CDS', B.TRANS_NUM, 1) "
					"AND B.COUPON_PAID IS NULL "
					"AND ABS(B.RECEIVABLE - B.RECEIVED) > 10 ";

			if(!m_bCash)
				Sql += SqlPhrase;

			Sql += "AND A.PORTFOLIO = " + Port + 
					"AND A.ASSET_CODE = " + Asset + 
					"GROUP BY A.ASS_TO, A.TRANS_TYPE, A.TRANS_NUM, A.INV_NUM "
					"ORDER BY 1, 2, 3 DESC ";
		}

		if(m_OraLoader.Open(Sql))
		{
			m_OraLoader.GetFieldArray().GetAt(3)->SetWithComma(TRUE);
			m_OraLoader.GetFieldArray().GetAt(3)->SetPrecision(2);
			m_OraLoader.GetFieldArray().GetAt(4)->SetWithComma(TRUE);
			m_OraLoader.GetFieldArray().GetAt(4)->SetPrecision(2);
			m_OraLoader.GetFieldArray().GetAt(5)->SetWithComma(FALSE);
			m_OraLoader.GetFieldArray().GetAt(5)->SetPrecision(0);
			m_OraLoader.GetFieldArray().GetAt(6)->SetWithComma(FALSE);
			m_OraLoader.GetFieldArray().GetAt(6)->SetPrecision(0);
			m_OraLoader.LoadDBSheet(m_sSS, TRUE);
		}

		EndWaitCursor();
	}
	else
	{
		m_Currency.Empty();
		m_Amount.Empty();
		m_sSS.ClearSheet();
		bOK = FALSE;
	}

	GetDlgItem(IDC_COUPONLIST_CURRENCY_EDIT)->SetWindowText(m_Currency);
}

void CCouponListDlg::OnDblClickCouponInfoList(long Col, long Row)
{
	CDBRec Rec;
	CString Text;
	CQData QData;

	if(Row > 0 && Row <= m_sSS.GetSheetRows())
	{
		Text = m_sSS.GetSheetText(3, Row);
		if(Text == "COUPON PAY" || Text == "SWAP INT")
			return;
		Rec.RemoveAll();

		Rec.Add(m_Asset);  // Asset
		Rec.Add(m_sSS.GetSheetText(2, Row)); // ToDate
		Rec.Add(m_sSS.GetSheetText(3, Row)); // TransType
		Rec.Add(m_sSS.GetSheetText(4, Row)); // Accrual
		Rec.Add(m_sSS.GetSheetText(6, Row)); // TransNum
		Rec.Add(m_sSS.GetSheetText(7, Row)); // InvNum
		m_Curr = m_sSS.GetSheetText(1, Row); // Currency
		if(m_Curr == "USD")                  // Fxrate
			Rec.Add(m_sSS.GetSheetText(8, Row));
		else
			Rec.Add("");
		
		Rec.Add(m_sSS.GetSheetText(9, Row));  // Remaining
		Rec.Add(m_sSS.GetSheetText(11, Row));  // Coupon Paid
		Rec.Add(m_sSS.GetSheetText(10, Row));  // Local Amount
		Rec.Add(m_Curr);

		m_SelSS.AddSheetRow(Rec);

		OnDblClickCouponSelList(1, m_SelSS.GetSheetRows());
		UpdateFields();
	}
}

void CCouponListDlg::OnDblClickCouponSelList(long Col, long Row)
{
	if(Row > 0 && Row <= m_SelSS.GetSheetRows())
	{
		CString Text;
		m_bLoaded = FALSE;
		CQData QData;
		
		m_SelSS.SetSheetCurRow(Row);
		m_AssetEdit.SetData(m_Asset);
		m_DateEdit.SetData(m_SelSS.GetSheetText(2, Row));
		
		m_Curr = m_SelSS.GetSheetText(m_SelSS.GetSheetCols(), Row); // GetCurrency
		if(m_Curr == "USD")
			m_FxrateEdit.SetData(m_SelSS.GetSheetText(7, Row));  // Update Fxrate
		else
			m_FxrateEdit.SetData(EMPTYSTRING);  // if none USD, do not lock fxrate
	
		Text = m_FxrateEdit.GetData();
		m_FxrateEdit.EnableWindow(Text.IsEmpty() || atof(QData.RemoveComma(Text)) == 1 ? FALSE : TRUE);
		m_AmountEdit.SetData(m_SelSS.GetSheetText(4, Row));  // Update Amount
		m_RemainingEdit.SetData(m_SelSS.GetSheetText(8, Row)); // Update Remaining
		m_CouponPaid.SetData(m_SelSS.GetSheetText(9, Row));
		m_LAmount = m_SelSS.GetSheetText(10, Row); // Update Local Amount
		m_SelSS.SetSheetCurRow(Row);
		m_bLoaded = TRUE;
	}
	else
		m_SelSS.SetSheetCurRow(-1);
}

void CCouponListDlg::OnKillfocusCouponListAmountEdit() 
{
	UpdateNumber(FALSE);
}

void CCouponListDlg::OnKillfocusCouponListFxrateEdit() 
{
	UpdateNumber(TRUE);
}

void CCouponListDlg::OnCouponListRemoveButton()
{
	long Row;

	Row = m_SelSS.GetSheetCurRow();
	if(Row > 0 && Row <= m_SelSS.GetSheetRows())
	{
		m_SelSS.DeleteSheetRow(Row);
		UpdateFields();
		m_SelSS.SetSheetCurRow(-1);
	}
}

void CCouponListDlg::OnCouponListUpdateButton()
{
	CString Text;

	if(m_SelSS.GetSheetCurRow() > 0)
	{
		Text = m_AmountEdit.GetData();
		m_SelSS.SetSheetText(4, m_SelSS.GetSheetCurRow(), Text);
		Text = m_FxrateEdit.GetData();
		m_SelSS.SetSheetText(7, m_SelSS.GetSheetCurRow(), Text);
		Text = m_RemainingEdit.GetData();
		m_SelSS.SetSheetText(8, m_SelSS.GetSheetCurRow(), Text);
		m_SelSS.SetSheetText(9, m_SelSS.GetSheetCurRow(), m_CouponPaid.GetCheckString());
		UpdateFields();
	}
}

void CCouponListDlg::OnOK()
{
	int i, j;
	CDBRec Rec;
	CQData QData;

	m_CouponArray.RemoveAll();
	for (i = 1; i <= m_SelSS.GetSheetRows(); i ++)
	{
		Rec.RemoveAll();
		for (j = 1; j < m_SelSS.GetSheetCols(); j++)
			if(j != 3)
				Rec.Add(m_SelSS.GetSheetText(j, i));
		m_CouponArray.Add(Rec);
	}

	m_Amount = m_TotalEdit.GetData();
	if(atof(QData.RemoveComma(m_Amount)) > 0)
		m_Amount = "-" + m_Amount;
	else
		m_Amount = (LPCTSTR) m_Amount + 1;
	CDialog::OnOK();
}
