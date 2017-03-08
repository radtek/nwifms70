// Portfolio.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "Portfolio.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortfolio dialog


CPortfolio::CPortfolio(CWnd* pParent /*=NULL*/)
	: CDialog(CPortfolio::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortfolio)
	m_TableSpace = NULL;
	m_DefCustodian = NULL;
	m_DefAccount = NULL;
	m_Currency = NULL;
	m_Status = NULL;
	//}}AFX_DATA_INIT
}


CPortfolio::~CPortfolio()
{
	if(m_TableSpace)
		delete m_TableSpace;
	if(m_DefCustodian)
		delete m_DefCustodian;
	if(m_DefAccount)
		delete m_DefAccount;
	if(m_Currency)
		delete m_Currency;
	if(m_Status)
		delete m_Status;
}


void CPortfolio::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortfolio)
	DDX_Control(pDX, IDC_PORTFOLIO_LIST, m_SS);
	//}}AFX_DATA_MAP
}


void CPortfolio::EnableCtrls()
{
	GetDlgItem(IDC_PORTFOLIO_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_PORTFOLIO_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BEGIN_MESSAGE_MAP(CPortfolio, CDialog)
	//{{AFX_MSG_MAP(CPortfolio)
	ON_BN_CLICKED(IDC_PORTFOLIO_ADD_BUTTON, OnPortfolioAddButton)
	ON_BN_CLICKED(IDC_PORTFOLIO_CLEAR_BUTTON, OnPortfolioClearButton)
	ON_BN_CLICKED(IDC_PORTFOLIO_DELETE_BUTTON, OnPortfolioDeleteButton)
	ON_BN_CLICKED(IDC_PORTFOLIO_FIND_BUTTON, OnPortfolioFindButton)
	ON_BN_CLICKED(IDC_PORTFOLIO_UPDATE_BUTTON, OnPortfolioUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortfolio message handlers

BEGIN_EVENTSINK_MAP(CPortfolio, CDialog)
    //{{AFX_EVENTSINK_MAP(CPortfolio)
	ON_EVENT(CPortfolio, IDC_PORTFOLIO_LIST, 11 /* DblClick */, OnDblClickPortfolioList, VTS_I4 VTS_I4)
	ON_EVENT(CPortfolio, IDC_PORTFOLIO_LIST, 2 /* BlockSelected */, OnBlockSelectedPortfolioList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CPortfolio::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(9);

	m_Portfolio.Setup(this, IDC_PORTFOLIO_PORTFOLIO_EDIT);
	m_Portfolio.LimitText(10);
	m_Name.Setup(this, IDC_PORTFOLIO_FULLNAME_EDIT);
	m_Name.LimitText(50);
	m_Accountant.Setup(this, IDC_PORTFOLIO_ACCOUNTANT_EDIT);
	m_Accountant.LimitText(10);
	m_Amount.Setup(this, IDC_PORTFOLIO_AMOUNT_EDIT);
	m_CtrlFactor.Setup(this, IDC_PORTFOLIO_EQCTRLFACTOR_EDIT);
	m_WtFactor.Setup(this, IDC_PORTFOLIO_WTFACTOR_EDIT);
	m_Status =  (CCheckBox*) new CCheckBox(this, IDC_PORTFOLIO_STATUS_CHECK, "C");
	m_TableSpace = (COptComboBox *) new COptComboBox(this, IDC_PORTFOLIO_TABLESPACE_COMBO);
	m_DefCustodian = (COptComboBox *) new COptComboBox(this, IDC_PORTFOLIO_DEFCUSTODIAN_COMBO);
	m_DefAccount = (COptComboBox *) new COptComboBox(this, IDC_PORTFOLIO_DEFACCOUNT_COMBO);
	m_Currency = (COptComboBox *) new COptComboBox(this, IDC_PORTFOLIO_CURRENCY_COMBO);

	m_Data.Add(&m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.Add(&m_Name, &m_Data.GetRec().GetFullName());
	m_Data.Add(m_TableSpace, &m_Data.GetRec().GetTableSpace());
	m_Data.Add(m_DefCustodian, &m_Data.GetRec().GetCustodian());
	m_Data.Add(m_DefAccount, &m_Data.GetRec().GetAccount());
	m_Data.Add(m_Currency, &m_Data.GetRec().GetCurrency());
	m_Data.Add(&m_Accountant, &m_Data.GetRec().GetAccountant());
	m_Data.Add(&m_Amount, &m_Data.GetRec().GetAmount());
	m_Data.Add(&m_CtrlFactor, &m_Data.GetRec().GetCtrlFactor());
	m_Data.Add(&m_WtFactor, &m_Data.GetRec().GetWtFactor());
	m_Data.Add(m_Status, &m_Data.GetRec().GetStatus());

	m_Data.Setup(m_pData->GetOraLoader(), &m_SS);

	QData.CopyKeyDBListKeyToComboBox(m_pData->GetContactList(), *m_DefCustodian, FALSE);
	QData.CopyDBRecArrayToComboBox(m_pData->GetAccountArr(), *m_DefAccount);
	QData.CopyDBRecArrayToComboBox(m_pData->GetCurrencyArr(), *m_Currency);
	m_TableSpace->AddString(SEMAM);
	m_TableSpace->AddString(MNGACC);
	OnPortfolioFindButton();
	
	this->SetWindowText("PORTFOLIOS");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortfolio::OnPortfolioAddButton() 
{
	if(m_Data.AddRow())
	{
		m_Data.Refresh();
		m_pData->GetPortfolioArr().Modified();
	}
}

void CPortfolio::OnPortfolioClearButton() 
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_PORTFOLIO_ADD_BUTTON)->EnableWindow(FALSE);
}

void CPortfolio::OnPortfolioDeleteButton() 
{
	m_Data.DeleteRow();
	m_pData->GetPortfolioArr().Modified();
}

void CPortfolio::OnPortfolioFindButton() 
{
	m_Data.LoadDBData();
}

void CPortfolio::OnPortfolioUpdateButton() 
{
	m_Data.UpdateRow();
	m_pData->GetPortfolioArr().Modified();
}

void CPortfolio::OnDblClickPortfolioList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	EnableCtrls();
}

void CPortfolio::OnBlockSelectedPortfolioList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickPortfolioList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}
