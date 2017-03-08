// ShortCash.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "ShortCash.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortCash dialog


CShortCash::CShortCash(CWnd* pParent /*=NULL*/)
	: CDialog(CShortCash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShortCash)
	m_PortfolioCombo = NULL;
	m_AccountCombo = NULL;
	m_CurrencyCombo = NULL;
	//}}AFX_DATA_INIT
}


void CShortCash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShortCash)
	DDX_Control(pDX, IDC_CASH_SHORT_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShortCash, CDialog)
	//{{AFX_MSG_MAP(CShortCash)
	ON_CBN_SELCHANGE(IDC_CASH_SHORT_ACCOUNT_COMBO, OnSelchangeCashShortAccountCombo)
	ON_BN_CLICKED(IDC_CASH_SHORT_ADD, OnCashShortAdd)
	ON_EN_KILLFOCUS(IDC_CASH_SHORT_AMOUNT_EDIT, OnKillfocusCashShortAmountEdit)
	ON_CBN_SELCHANGE(IDC_CASH_SHORT_CURRENCY_COMBO, OnSelchangeCashShortCurrencyCombo)
	ON_EN_KILLFOCUS(IDC_CASH_SHORT_DATE_EDIT, OnKillfocusCashShortDateEdit)
	ON_BN_CLICKED(IDC_CASH_SHORT_DELETE, OnCashShortDelete)
	ON_BN_CLICKED(IDC_CASH_SHORT_FIND, OnCashShortFind)
	ON_CBN_SELCHANGE(IDC_CASH_SHORT_PORTFOLIO_COMBO, OnSelchangeCashShortPortfolioCombo)
	ON_BN_CLICKED(IDC_CASH_SHORT_UPDATE, OnCashShortUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CShortCash::EnableCtrls()
{
	int i;
	BOOL bEnable = TRUE;
	BOOL bFind = FALSE;
	CStringArray StrArr;

	m_Data.ToStringArray(StrArr);
	for(i = 0; i < StrArr.GetSize(); i++)
	{
		if(StrArr.GetAt(i).IsEmpty())
			bEnable = FALSE;
		else
			bFind = TRUE;
	}

	GetDlgItem(IDC_CASH_SHORT_FIND)->EnableWindow(bFind);
	GetDlgItem(IDC_CASH_SHORT_ADD)->EnableWindow(bEnable);
	GetDlgItem(IDC_CASH_SHORT_UPDATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CASH_SHORT_DELETE)->EnableWindow(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// CShortCash message handlers

BEGIN_EVENTSINK_MAP(CShortCash, CDialog)
    //{{AFX_EVENTSINK_MAP(CShortCash)
	ON_EVENT(CShortCash, IDC_CASH_SHORT_LIST, 2 /* BlockSelected */, OnBlockSelectedCashShortList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CShortCash, IDC_CASH_SHORT_LIST, 11 /* DblClick */, OnDblClickCashShortList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CShortCash::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CQData QData;
	
	m_SS.SetVisibleCols(5);
	m_SS.SetVisibleRows(4);

	m_PortfolioCombo = (COptComboBox*) new COptComboBox(this, IDC_CASH_SHORT_PORTFOLIO_COMBO);
	m_AccountCombo = (COptComboBox*) new COptComboBox(this, IDC_CASH_SHORT_ACCOUNT_COMBO);
	m_CurrencyCombo = (COptComboBox*) new COptComboBox(this, IDC_CASH_SHORT_CURRENCY_COMBO);

	m_DateEdit.Setup(this, IDC_CASH_SHORT_DATE_EDIT);
	m_DateEdit.LimitText(10);
	m_AmountEdit.Setup(this, IDC_CASH_SHORT_AMOUNT_EDIT);

	m_Data.Setup(m_pData->GetOraLoader(), &m_SS);
	m_Data.Add(&m_DateEdit, &m_Data.GetDate());
	m_Data.Add(m_PortfolioCombo, &m_Data.GetPortfolio());
	m_Data.Add(m_AccountCombo, &m_Data.GetAccount());
	m_Data.Add(m_CurrencyCombo, &m_Data.GetCurrency());
	m_Data.Add(&m_AmountEdit, &m_Data.GetAmount());

	m_Data.GetOraLoader().Today(m_Data.GetDate());
	m_DateEdit.SetData(m_Data.GetDate());		

	QData.CopyDBRecArrayToComboBox(m_pData->GetPortfolioArr(), *m_PortfolioCombo, 0, FALSE);
	QData.CopyDBRecArrayToComboBox(m_pData->GetAccountArr(), *m_AccountCombo, 0, FALSE);
	QData.CopyDBRecArrayToComboBox(m_pData->GetCurrencyArr(), *m_CurrencyCombo, 0, FALSE);
 
	m_Data.GetOraLoader().GetFieldArray().RemoveAll();
	m_Data.LoadDBData();
	
	EnableCtrls();	
	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShortCash::OnSelchangeCashShortAccountCombo() 
{
	EnableCtrls();
}

void CShortCash::OnCashShortAdd() 
{
	m_Data.SaveData(TRUE);
}

void CShortCash::OnKillfocusCashShortAmountEdit() 
{
	EnableCtrls();
}

void CShortCash::OnSelchangeCashShortCurrencyCombo() 
{
	EnableCtrls();
}

void CShortCash::OnKillfocusCashShortDateEdit() 
{
	EnableCtrls();
}

void CShortCash::OnCashShortDelete() 
{
	if(MessageBox("Are you sure you want to delete this record ?", "Delete Record",
				MB_YESNO) == IDYES)
		m_Data.DeleteData();
	
	GetDlgItem(IDC_CASH_SHORT_DELETE)->EnableWindow(FALSE);
}

void CShortCash::OnCashShortFind() 
{
	m_Data.LoadDBData();
}

void CShortCash::OnBlockSelectedCashShortList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	BOOL bEnable = FALSE;
	if(BlockRow >= 1 && BlockRow <= m_Data.GetSS()->GetSheetRows())
	{
		m_Data.SetCurrentRow(BlockRow);
		if(strlen(m_Data.GetID()) > 0)
			bEnable = TRUE;
	}
	
	GetDlgItem(IDC_CASH_SHORT_DELETE)->EnableWindow(bEnable);
}

void CShortCash::OnDblClickCashShortList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_Data.GetSS()->GetSheetRows())
		m_Data.SetCurrentRow(Row);
	EnableCtrls();
}

void CShortCash::OnSelchangeCashShortPortfolioCombo() 
{
	EnableCtrls();
}

void CShortCash::OnCashShortUpdate() 
{
	m_Data.SaveData();
}
