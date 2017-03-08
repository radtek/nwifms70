// XCurrencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "XCurrencyDlg.h"
#include "qdata.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXCurrencyDlg dialog


CXCurrencyDlg::CXCurrencyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXCurrencyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXCurrencyDlg)
	m_CategoryCombo = NULL;
	m_CounterpartyCombo = NULL;
	m_CurrencyCombo = NULL;
	m_ValueDateCombo = NULL;
	m_PortfolioCombo = NULL;
	//}}AFX_DATA_INIT
}

CXCurrencyDlg::~CXCurrencyDlg()
{
	if(m_CategoryCombo)
		delete m_CategoryCombo;
	if(m_CounterpartyCombo)
		delete m_CounterpartyCombo;
	if(m_CurrencyCombo)
		delete m_CurrencyCombo;
	if(m_ValueDateCombo)
		delete m_ValueDateCombo;
	if(m_PortfolioCombo)
		delete m_PortfolioCombo;
}

void CXCurrencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXCurrencyDlg)
	DDX_Control(pDX, IDC_XCURRENCY_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXCurrencyDlg, CDialog)
	//{{AFX_MSG_MAP(CXCurrencyDlg)
	ON_CBN_SELCHANGE(IDC_XCURRENCY_CP_COMBO, OnSelchangeXcurrencyCpCombo)
	ON_CBN_SELCHANGE(IDC_XCURRENCY_CURRENCY_COMBO, OnSelchangeXcurrencyCurrencyCombo)
	ON_CBN_SELCHANGE(IDC_XCURRENCY_VDATE_COMBO, OnChangeXcurrencyVdateCombo)
	ON_EN_CHANGE(IDC_XCURRENCY_PRICE_EDIT, OnChangeXcurrencyPriceEdit)
	ON_EN_CHANGE(IDC_XCURRENCY_UNWIND_AMOUNT_EDIT, OnChangeXcurrencyUnwindAmountEdit)
	ON_BN_CLICKED(IDC_XCURRENCY_LOAD_BUTTON, OnXcurrencyLoadButton)
	ON_CBN_SELCHANGE(IDC_XCURRENCY_CATEGORY_COMBO, OnSelchangeXcurrencyCategoryCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXCurrencyDlg::ComputeData()
{
	CString Text;
	CQData QData;

	double uw_amount, nom_amount, nom_amount2, price;

	m_UnWindNominalEdit.GetWindowText(Text);
	uw_amount = atof(QData.RemoveComma(Text));
	m_PriceEdit.GetWindowText(Text);
	price = atof(QData.RemoveComma(Text));

	if(uw_amount != 0 && price != 0)
	{
		m_NominalEdit.GetWindowText(Text);
		nom_amount = atof(QData.RemoveComma(Text));
		m_Nominal2Edit.GetWindowText(Text);
		nom_amount2 = atof(QData.RemoveComma(Text));
		if(nom_amount >= 0)
		{
			m_UnWindNominal2Edit.SetWindowText(QData.WriteNumber(uw_amount*price, TRUE, 2));
			m_NetEdit.SetWindowText(QData.WriteNumber(fabs(nom_amount) - fabs(uw_amount), TRUE, 2));
			m_Net2Edit.SetWindowText(QData.WriteNumber(fabs(nom_amount*price) - fabs(nom_amount2), TRUE, 2));
		}
		else
		{
			m_UnWindNominal2Edit.SetWindowText(QData.WriteNumber(-1*uw_amount*price, TRUE, 2));
			m_NetEdit.SetWindowText(QData.WriteNumber(fabs(uw_amount) - fabs(nom_amount), TRUE, 2));
			m_Net2Edit.SetWindowText(QData.WriteNumber(fabs(nom_amount2) - fabs(nom_amount*price), TRUE, 2));
		}
	}
	else
	{
		m_UnWindNominal2Edit.SetWindowText("");
		m_NetEdit.SetWindowText("");
		m_Net2Edit.SetWindowText("");
	}
}

void CXCurrencyDlg::ComputeNominal(int Row, int Row2)
{
	int Start, End;

	if(m_SS.GetSheetRows() <= 0)
	{
		m_NominalEdit.SetWindowText("");
		m_Nominal2Edit.SetWindowText("");
	}

	if(Row <= Row2)
	{
		Start = Row;
		End = Row2;
	}
	else
	{
		Start = Row2;
		End = Row;
	}

	CQData QData;
	double nom_amount = 0, nom_amount2 = 0;

	for (int i = Start; i <= End;  i ++)
	{
		nom_amount += atof(QData.RemoveComma(m_SS.GetSheetText(6, i)));
		nom_amount2 += atof(QData.RemoveComma(m_SS.GetSheetText(8, i)));
	}
	
	m_NominalEdit.SetWindowText(QData.WriteNumber(nom_amount, TRUE, 2));
	m_UnWindNominalEdit.SetWindowText(QData.WriteNumber(fabs(nom_amount), TRUE, 2));
	m_Nominal2Edit.SetWindowText(QData.WriteNumber(nom_amount2, TRUE, 2));
}

void CXCurrencyDlg::EnableButton()
{
	if(m_CounterpartyCombo->GetCurSel() >= 0 && m_CurrencyCombo->GetCurSel() >= 0 && m_ValueDateCombo->GetCurSel() >= 0)
		GetDlgItem(IDC_XCURRENCY_LOAD_BUTTON)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_XCURRENCY_LOAD_BUTTON)->EnableWindow(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// CXCurrencyDlg message handlers

BEGIN_EVENTSINK_MAP(CXCurrencyDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CXCurrencyDlg)
	ON_EVENT(CXCurrencyDlg, IDC_XCURRENCY_LIST, 2 /* BlockSelected */, OnBlockSelectedXcurrencyList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CXCurrencyDlg, IDC_XCURRENCY_LIST, 24 /* SelChange */, OnSelChangeXcurrencyList, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CXCurrencyDlg, IDC_XCURRENCY_LIST, 5 /* Click */, OnClickXcurrencyList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CXCurrencyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();
	m_SS.SetVisibleCols(10);
	m_SS.SetVisibleRows(10);
	
	m_CategoryCombo = (COptComboBox *) new COptComboBox(this, IDC_XCURRENCY_CATEGORY_COMBO);
	m_CounterpartyCombo = (COptComboBox *) new COptComboBox(this, IDC_XCURRENCY_CP_COMBO);
	m_CurrencyCombo = (COptComboBox *) new COptComboBox(this, IDC_XCURRENCY_CURRENCY_COMBO);
	m_ValueDateCombo = (COptComboBox *) new COptComboBox(this, IDC_XCURRENCY_VDATE_COMBO);
	m_PortfolioCombo = (COptComboBox *) new COptComboBox(this, IDC_XCURRENCY_PORTFOLIO_COMBO, TRUE);

	m_NominalEdit.Setup(this, IDC_XCURRENCY_AMOUNT_EDIT);
	m_Nominal2Edit.Setup(this, IDC_XCURRENCY_AMOUNT2_EDIT);
	m_UnWindNominalEdit.Setup(this, IDC_XCURRENCY_UNWIND_AMOUNT_EDIT, "Invalid Number", 2);
	m_UnWindNominal2Edit.Setup(this, IDC_XCURRENCY_UNWIND_AMOUNT2_EDIT, "Invalid Number", 2);
	m_PriceEdit.Setup(this, IDC_XCURRENCY_PRICE_EDIT);
	m_NetEdit.Setup(this, IDC_XCURRENCY_NET_EDIT);
	m_Net2Edit.Setup(this, IDC_XCURRENCY_NET2_EDIT);

	m_OraLoader = GetData().GetOraLoader();

	if(m_UserClass == 1) /* Trader */
	{
		m_OraLoader.GetSql().Format("SELECT DISTINCT ASS_CATEGORY "
								"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
								"WHERE A.ASSET_CODE = B.ASS_CODE "
								"AND B.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') "
								"AND A.CURRENCY != 'USD' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) ");
		m_OraLoader.Open();
		m_OraLoader.LoadComboBox(*m_CategoryCombo);
	}
	else
	{
		m_CategoryCombo->ShowWindow(FALSE);
		GetDlgItem(IDC_XCURRENCY_CATEGORY_STATIC)->ShowWindow(FALSE);

		m_OraLoader.GetSql().Format("SELECT DISTINCT NVL(ASSIGN_CP, COUNTERPARTY) "
								"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND A.CURRENCY != 'USD' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') ");
		m_OraLoader.Open();
		m_OraLoader.LoadComboBox(*m_CounterpartyCombo);
	}
	
	if(m_UserClass != 3) /* Operation Transaction process */
	{	
		GetDlgItem(IDC_XCURRENCY_PORTFOLIO_STATIC)->ShowWindow(FALSE);
		m_PortfolioCombo->ShowWindow(FALSE);
	}

	GetDlgItem(IDC_XCURRENCY_LOAD_BUTTON)->EnableWindow(FALSE);
	EndWaitCursor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXCurrencyDlg::OnSelchangeXcurrencyCategoryCombo() 
{
	m_CounterpartyCombo->ResetContent();

	if(m_CategoryCombo->GetCurSel() >= 0)
	{
		BeginWaitCursor();
		
		CString Category;

		m_CategoryCombo->GetSelString(Category);
		m_OraLoader.GetSql().Format("SELECT DISTINCT NVL(ASSIGN_CP, COUNTERPARTY) "
								"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND C.ASS_CATEGORY = '%s' "
								"AND A.CURRENCY != 'USD' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') ", (const char *) Category);
		m_OraLoader.Open();
		m_OraLoader.LoadComboBox(*m_CounterpartyCombo);

		EndWaitCursor();
	}
	else
	{		
		m_CurrencyCombo->ResetContent();
		m_ValueDateCombo->ResetContent();
	}
}

void CXCurrencyDlg::OnSelchangeXcurrencyCpCombo() 
{
	m_CurrencyCombo->ResetContent();

	if(m_CounterpartyCombo->GetCurSel() >= 0)
	{
		BeginWaitCursor();

		CString Category, CP;

		m_CounterpartyCombo->GetSelString(CP);
		if(m_UserClass == 1)
		{
			m_CategoryCombo->GetSelString(Category);
			m_OraLoader.GetSql().Format("SELECT DISTINCT A.CURRENCY "
								"FROM SEMAM.NW_CF_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND C.ASS_CATEGORY = '%s' "
								"AND A.CURRENCY != 'USD' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') ", (const char *) Category, (const char *) CP);
		}
		else
			m_OraLoader.GetSql().Format("SELECT DISTINCT A.CURRENCY "
								"FROM SEMAM.NW_CF_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND A.CURRENCY != 'USD' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') ", (const char *) CP);

		m_OraLoader.Open();
		m_OraLoader.LoadComboBox(*m_CurrencyCombo);

		EndWaitCursor();
	}
	else
		m_ValueDateCombo->ResetContent();

	EnableButton();
}

void CXCurrencyDlg::OnSelchangeXcurrencyCurrencyCombo() 
{
	m_ValueDateCombo->ResetContent();

	if(m_CounterpartyCombo->GetCurSel() >= 0 && m_CurrencyCombo->GetCurSel() >= 0)
	{
		BeginWaitCursor();

		CString Category, CP, Currency;

		m_CategoryCombo->GetSelString(Category);
		m_CounterpartyCombo->GetSelString(CP);
		m_CurrencyCombo->GetSelString(Currency);

		if(m_UserClass == 1) /* Trader */
		{
			m_CategoryCombo->GetSelString(Category);
			m_OraLoader.GetSql().Format("SELECT DISTINCT A.VALUE_DATE "
								"FROM SEMAM.NW_CF_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND C.ASS_CATEGORY = '%s' "
								"AND A.CURRENCY = '%s' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') ", (const char *) Category, 
								(const char *) Currency, (const char *) CP);
		}
		else
			m_OraLoader.GetSql().Format("SELECT DISTINCT A.VALUE_DATE "
								"FROM SEMAM.NW_CF_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND A.CURRENCY = '%s' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') ", (const char *) Currency, (const char *) CP);
		m_OraLoader.Open();
		m_OraLoader.LoadComboBox(*m_ValueDateCombo);
		EndWaitCursor();
	}

	EnableButton();
}

void CXCurrencyDlg::OnChangeXcurrencyVdateCombo() 
{
	if(m_UserClass == 3 && m_CounterpartyCombo->GetCurSel() >= 0 && 
		m_CurrencyCombo->GetCurSel() >= 0 && m_ValueDateCombo->GetCurSel() >= 0)
	{
		BeginWaitCursor();

		CQData QData;
		CString Category, CP, Currency, ValueDate;

		m_CounterpartyCombo->GetSelString(CP);
		m_CurrencyCombo->GetSelString(Currency);
		m_ValueDateCombo->GetSelString(ValueDate);
		
		if(m_UserClass == 1)
		{
			m_CategoryCombo->GetSelString(Category);
			m_OraLoader.GetSql().Format("SELECT DISTINCT A.PORTFOLIO "
								"FROM SEMAM.NW_CF_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND C.ASS_CATEGORY = '%s' "
								"AND A.CURRENCY = '%s' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') "
								"AND A.VALUE_DATE = %s ", (const char *) Category, (const char *) Currency, 
								(const char *) CP, QData.GetQueryDate(ValueDate));
		}
		else
			m_OraLoader.GetSql().Format("SELECT DISTINCT A.PORTFOLIO "
								"FROM SEMAM.NW_CF_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
								"WHERE A.TRANS_NUM = B.TRANS_NUM "
								"AND A.ASSET_CODE = C.ASS_CODE "
								"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
								"AND A.CURRENCY = '%s' "
								"AND A.VALUE_DATE >= TRUNC(SYSDATE) "
								"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
								"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') "
								"AND A.VALUE_DATE = %s ", (const char *) Currency, (const char *) CP, 
								QData.GetQueryDate(ValueDate));
		
		m_OraLoader.Open();
		m_OraLoader.LoadComboBox(*m_PortfolioCombo);
		EndWaitCursor();
	}
	else
		m_PortfolioCombo->ResetContent();
	
	EnableButton();
}

void CXCurrencyDlg::OnChangeXcurrencyPriceEdit() 
{
	ComputeData();
}

void CXCurrencyDlg::OnChangeXcurrencyUnwindAmountEdit() 
{
	ComputeData();
}

void CXCurrencyDlg::OnXcurrencyLoadButton() 
{
	CString Category, CP, Currency, ValueDate, Portfolio;
	const char *p;
	CQData QData;
	COraLoader OraLoader(GetData().GetOraLoader());

	BeginWaitCursor();
	m_UnWindNominalEdit.SetWindowText("");
	m_UnWindNominal2Edit.SetWindowText("");
	m_PriceEdit.SetWindowText("");

	m_CounterpartyCombo->GetSelString(CP);
	m_CurrencyCombo->GetSelString(Currency);
	m_ValueDateCombo->GetSelString(ValueDate);
	p = QData.GetQueryDate(ValueDate);

	switch(m_UserClass)
	{
		case 1:
			m_CategoryCombo->GetSelString(Category);
			OraLoader.GetSql().Format("SELECT PORTFOLIO, A.ASSET_CODE, A.TRANS_TYPE, TRADE_DATE, A.TRANS_NUM, "
				"DECODE(TRANS_TYPE, 'FOREX', 0, DECODE(TRANS_DIRECTION, 'P', 1, 'S', -1)*B.NOM_AMOUNT) \"NOM_AMOUNT\", "
				"DECODE(TRANS_TYPE, 'FOREX', 1/FXRATE, PRICE) \"PRICE\", "
				"DECODE(TRANS_TYPE, 'FOREX', -1, PRICE)*DECODE(TRANS_DIRECTION, 'P', -1, 'S', 1)*B.NOM_AMOUNT \"NOM_AMOUNT2\", "
				"NVL(ASSIGN_CP, COUNTERPARTY) \"COUNTERPARTY\", A.CURRENCY, VALUE_DATE "
				"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_INV B, SEMAM.NW_ASSETS C "
				"WHERE A.TRANS_NUM = B.TRANS_NUM "
				"AND B.TR_DESC = 'BOOKING' "
				"AND A.ASSET_CODE = C.ASS_CODE "
				"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') "
				"AND C.ASS_CATEGORY = '%s' "
				"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
				"AND A.CURRENCY = '%s' "
				"AND A.VALUE_DATE = %s "
				"ORDER BY 1, 2, 3, 4, 5", (const char *) Category, (const char *) CP, (const char *) Currency, p);
			break;
		case 2:
			OraLoader.GetSql().Format("SELECT PORTFOLIO, A.ASSET_CODE, A.TRANS_TYPE, TRADE_DATE, A.TRANS_NUM, "
				"DECODE(TRANS_TYPE, 'FOREX', 0, DECODE(TRANS_DIRECTION, 'P', 1, 'S', -1)*B.NOM_AMOUNT) \"NOM_AMOUNT\", "
				"DECODE(TRANS_TYPE, 'FOREX', 1/FXRATE, PRICE) \"PRICE\", "
				"DECODE(TRANS_TYPE, 'FOREX', -1, PRICE)*DECODE(TRANS_DIRECTION, 'P', -1, 'S', 1)*B.NOM_AMOUNT \"NOM_AMOUNT2\", "
				"NVL(ASSIGN_CP, COUNTERPARTY) \"COUNTERPARTY\", A.CURRENCY, VALUE_DATE "
				"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_INV B "
				"WHERE A.TRANS_NUM = B.TRANS_NUM "
				"AND B.TR_DESC = 'BOOKING' "
				"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') "
				"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
				"AND A.CURRENCY = '%s' "
				"AND A.VALUE_DATE = %s " 
				"ORDER BY 1, 2, 3, 4  ", (const char *) CP, (const char *) Currency, p);
			break;
		case 3:
			m_PortfolioCombo->GetSelString(Portfolio);
			if(Portfolio.IsEmpty())
				OraLoader.GetSql().Format("SELECT PORTFOLIO, A.ASSET_CODE, A.TRANS_TYPE, TRADE_DATE, A.TRANS_NUM, "
					"DECODE(TRANS_TYPE, 'FOREX', 0, DECODE(TRANS_DIRECTION, 'P', 1, 'S', -1)*B.NOM_AMOUNT) \"NOM_AMOUNT\", "
					"DECODE(TRANS_TYPE, 'FOREX', 1/FXRATE, PRICE) \"PRICE\", "
					"DECODE(TRANS_TYPE, 'FOREX', -1, PRICE)*DECODE(TRANS_DIRECTION, 'P', -1, 'S', 1)*B.NOM_AMOUNT \"NOM_AMOUNT2\", "
					"NVL(ASSIGN_CP, COUNTERPARTY) \"COUNTERPARTY\", A.CURRENCY, VALUE_DATE "
					"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_INV B "
					"WHERE A.TRANS_NUM = B.TRANS_NUM "
					"AND B.TR_DESC = 'BOOKING' "
					"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') "
					"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
					"AND A.CURRENCY = '%s' "
					"AND A.VALUE_DATE = %s "
					"ORDER BY 1, 2, 3, 4, 5  ", (const char *) CP, (const char *) Currency, p);
			else
				OraLoader.GetSql().Format("SELECT PORTFOLIO, A.ASSET_CODE, A.TRANS_TYPE, TRADE_DATE, A.TRANS_NUM, "
					"DECODE(TRANS_TYPE, 'FOREX', 0, DECODE(TRANS_DIRECTION, 'P', 1, 'S', -1)*B.NOM_AMOUNT) \"NOM_AMOUNT\", "
					"DECODE(TRANS_TYPE, 'FOREX', 1/FXRATE, PRICE) \"PRICE\", "
					"DECODE(TRANS_TYPE, 'FOREX', -1, PRICE)*DECODE(TRANS_DIRECTION, 'P', -1, 'S', 1)*B.NOM_AMOUNT \"NOM_AMOUNT2\", "
					"NVL(ASSIGN_CP, COUNTERPARTY) \"COUNTERPARTY\", A.CURRENCY, VALUE_DATE "
					"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_INV B "
					"WHERE A.TRANS_NUM = B.TRANS_NUM "
					"AND B.TR_DESC = 'BOOKING' "
					"AND A.TRANS_TYPE IN ('FOREX', 'SECURITIES') "
					"AND NVL(ASSIGN_CP, A.COUNTERPARTY) = '%s' "
					"AND A.CURRENCY = '%s' "
					"AND A.VALUE_DATE = %s "
					"AND A.PORTFOLIO = '%s' "
					"ORDER BY 1, 2, 3, 4  ", (const char *) CP, (const char *) Currency, p, (const char *) Portfolio);
			break;
		default:
			break;
	}

	OraLoader.Open();
	OraLoader.LoadDBSheet(m_SS);
	ComputeNominal(1, m_SS.GetSheetRows());
	EndWaitCursor();
}

void CXCurrencyDlg::OnBlockSelectedXcurrencyList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	if(BlockRow >= 1 && BlockRow <= m_SS.GetSheetRows() && BlockRow2 >= 1 && BlockRow2 <= m_SS.GetSheetRows())
		ComputeNominal(BlockRow, BlockRow2);
	else
		ComputeNominal(1, m_SS.GetSheetRows());
}

void CXCurrencyDlg::OnSelChangeXcurrencyList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2, long CurCol, long CurRow) 
{
	OnBlockSelectedXcurrencyList(BlockCol, BlockRow, BlockCol2, BlockRow2);
}

void CXCurrencyDlg::OnClickXcurrencyList(long Col, long Row) 
{
	ComputeNominal(1, m_SS.GetSheetRows());
}
