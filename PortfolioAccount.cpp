// PortfoliAccount.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "PortfolioAccount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortfolioInfo dialog


CPortfolioAccount::CPortfolioAccount(CWnd* pParent /*=NULL*/)
	: CDialog(CPortfolioAccount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortfolioInfo)
	m_Portfolio = NULL;
	m_CustodianCode = NULL;
	m_Margin = NULL;
	//}}AFX_DATA_INIT
}


CPortfolioAccount::~CPortfolioAccount()
{
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_CustodianCode)
		delete m_CustodianCode;
	if(m_Margin)
		delete m_Margin;
}

void CPortfolioAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortfolioAccount)
	DDX_Control(pDX, IDC_PORTFOLIOACCT_LIST, m_SS);
	//}}AFX_DATA_MAP
}


void CPortfolioAccount::EnableCtrls()
{
	GetDlgItem(IDC_PORTFOLIOACCT_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_PORTFOLIOACCT_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}


BEGIN_MESSAGE_MAP(CPortfolioAccount, CDialog)
	//{{AFX_MSG_MAP(CPortfolioAccount)
	ON_BN_CLICKED(IDC_PORTFOLIOACCT_ADD_BUTTON, OnPortfolioAccountAddButton)
	ON_BN_CLICKED(IDC_PORTFOLIOACCT_DELETE_BUTTON, OnPortfolioAccountDeleteButton)
	ON_BN_CLICKED(IDC_PORTFOLIOACCT_UPDATE_BUTTON, OnPortfolioAccountUpdateButton)
	ON_BN_CLICKED(IDC_PORTFOLIOACCT_FIND_BUTTON, OnPortfolioAccountFindButton)
	ON_BN_CLICKED(IDC_PORTFOLIOACCT_CLEAR_BUTTON, OnPortfolioAccountClearButton)
	ON_EN_CHANGE(IDC_PORTFOLIOACCT_ACCOUNT2_EDIT, OnEnChangePortfolioAccountAccount2Edit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortfolioAccount message handlers

BEGIN_EVENTSINK_MAP(CPortfolioAccount, CDialog)
    //{{AFX_EVENTSINK_MAP(CPortfolioAccount)
	ON_EVENT(CPortfolioAccount, IDC_PORTFOLIOACCT_LIST, 11 /* DblClick */, OnDblClickPortfolioAccountList, VTS_I4 VTS_I4)
	ON_EVENT(CPortfolioAccount, IDC_PORTFOLIOACCT_LIST, 2 /* BlockSelected */, OnBlockSelectedPortfolioAccountList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CPortfolioAccount::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(9);

	m_Portfolio = (COptComboBox *) new COptComboBox(this, IDC_PORTFOLIOACCT_PORTFOLIO_COMBO);
	m_CustodianCode = (COptComboBox *) new COptComboBox(this, IDC_PORTFOLIOACCT_CUSTODIANCODE_COMBO);
	m_Margin = (CCheckBox *) new CCheckBox(this, IDC_PORTFOLIOACCT_MARGIN_CHECK, Y);
	m_Custodian.Setup(this, IDC_PORTFOLIOACCT_CUSTODIAN_EDIT);
	m_CustodianAccount.Setup(this, IDC_PORTFOLIOACCT_CUSTODIANACCOUNT_EDIT);
	m_AccountNum.Setup(this, IDC_PORTFOLIOACCT_ACCNUM_EDIT);
	m_Account.Setup(this, IDC_PORTFOLIOACCT_ACCOUNT_EDIT);
	m_Account2.Setup(this, IDC_PORTFOLIOACCT_ACCOUNT2_EDIT);
	m_Bank.Setup(this, IDC_PORTFOLIOACCT_BANK_EDIT);
	m_EuroClear.Setup(this, IDC_PORTFOLIOACCT_EUROCLEAR_EDIT, "Invalid Number", 0, FALSE);
	m_Contact.Setup(this, IDC_PORTFOLIOACCT_CONTACT_EDIT);
	m_Phone.Setup(this, IDC_PORTFOLIOACCT_PHONE_EDIT);
	m_Fax.Setup(this, IDC_PORTFOLIOACCT_FAX_EDIT);

	m_Data.Add(m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.Add(&m_Custodian, &m_Data.GetRec().GetCustodian());
	m_Data.Add(&m_CustodianAccount, &m_Data.GetRec().GetCustodianAccount());
	m_Data.Add(&m_AccountNum, &m_Data.GetRec().GetAccountNum());
	m_Data.Add(&m_Account, &m_Data.GetRec().GetAccount());
	m_Data.Add(&m_Account2, &m_Data.GetRec().GetAccount2());
	m_Data.Add(&m_Bank, &m_Data.GetRec().GetBank());
	m_Data.Add(&m_EuroClear, &m_Data.GetRec().GetEuroClear());
	m_Data.Add(m_CustodianCode, &m_Data.GetRec().GetCustodianCode());
	m_Data.Add(m_Margin, &m_Data.GetRec().GetMargin());
	m_Data.Add(&m_Contact, &m_Data.GetRec().GetContact());
	m_Data.Add(&m_Phone, &m_Data.GetRec().GetPhone());
	m_Data.Add(&m_Fax, &m_Data.GetRec().GetFax());

	m_Data.Setup(m_OraLoader, &m_SS);

	m_OraLoader.Open("SELECT PORTFOLIO FROM SEMAM.NW_PORTFOLIOS WHERE STATUS IS NULL ");
	m_OraLoader.LoadComboBox(*m_Portfolio);

	m_OraLoader.Open("SELECT CP_CODE FROM SEMAM.NW_COUNTER_PARTY ORDER BY 1 ");
	m_OraLoader.LoadComboBox(*m_CustodianCode);

	this->SetWindowText("PORTFOLIO ACCOUNTS");
	OnPortfolioAccountFindButton();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortfolioAccount::OnPortfolioAccountAddButton() 
{
	if(m_Data.AddRow())
		m_Data.Refresh();
}

void CPortfolioAccount::OnPortfolioAccountDeleteButton() 
{
	m_Data.DeleteRow();
}

void CPortfolioAccount::OnPortfolioAccountUpdateButton() 
{
	m_Data.UpdateRow();
}

void CPortfolioAccount::OnPortfolioAccountFindButton() 
{
	m_Data.LoadDBData();
}

void CPortfolioAccount::OnPortfolioAccountClearButton() 
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_PORTFOLIOACCT_ADD_BUTTON)->EnableWindow(FALSE);
}

void CPortfolioAccount::OnDblClickPortfolioAccountList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	EnableCtrls();
}

void CPortfolioAccount::OnBlockSelectedPortfolioAccountList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickPortfolioAccountList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CPortfolioAccount::OnEnChangePortfolioAccountAccount2Edit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
