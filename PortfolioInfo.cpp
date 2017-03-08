// PortfoliInfo.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "PortfolioInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortfolioInfo dialog


CPortfolioInfo::CPortfolioInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CPortfolioInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortfolioInfo)
	m_Portfolio = NULL;
	m_Margin = NULL;
	//}}AFX_DATA_INIT
}


CPortfolioInfo::~CPortfolioInfo()
{
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_Margin)
		delete m_Margin;
}

void CPortfolioInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortfolioInfo)
	DDX_Control(pDX, IDC_PORTFOLIOINFO_LIST, m_SS);
	//}}AFX_DATA_MAP
}


void CPortfolioInfo::EnableCtrls()
{
	GetDlgItem(IDC_PORTFOLIOINFO_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_PORTFOLIOINFO_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}


BEGIN_MESSAGE_MAP(CPortfolioInfo, CDialog)
	//{{AFX_MSG_MAP(CPortfolioInfo)
	ON_BN_CLICKED(IDC_PORTFOLIOINFO_ADD_BUTTON, OnPortfolioinfoAddButton)
	ON_BN_CLICKED(IDC_PORTFOLIOINFO_DELETE_BUTTON, OnPortfolioinfoDeleteButton)
	ON_BN_CLICKED(IDC_PORTFOLIOINFO_UPDATE_BUTTON, OnPortfolioinfoUpdateButton)
	ON_BN_CLICKED(IDC_PORTFOLIOINFO_FIND_BUTTON, OnPortfolioinfoFindButton)
	ON_BN_CLICKED(IDC_PORTFOLIOINFO_CLEAR_BUTTON, OnPortfolioinfoClearButton)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_PORTFOLIOINFO_ACCOUNT2_EDIT, &CPortfolioInfo::OnEnChangePortfolioinfoAccount2Edit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortfolio message handlers

BEGIN_EVENTSINK_MAP(CPortfolioInfo, CDialog)
    //{{AFX_EVENTSINK_MAP(CPortfolioInfo)
	ON_EVENT(CPortfolioInfo, IDC_PORTFOLIOINFO_LIST, 11 /* DblClick */, OnDblClickPortfolioInfoList, VTS_I4 VTS_I4)
	ON_EVENT(CPortfolioInfo, IDC_PORTFOLIOINFO_LIST, 2 /* BlockSelected */, OnBlockSelectedPortfolioInfoList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CPortfolioInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(9);

	m_Portfolio = (COptComboBox *) new COptComboBox(this, IDC_PORTFOLIOINFO_PORTFOLIO_COMBO);
	m_Margin = (CCheckBox *) new CCheckBox(this, IDC_PORTFOLIOINFO_MARGIN_CHECK, Y);
	m_Custodian.Setup(this, IDC_PORTFOLIOINFO_CUSTODIAN_EDIT);
	m_CustodianAccount.Setup(this, IDC_PORTFOLIOINFO_CUSTODIANACCOUNT_EDIT);
	m_Account.Setup(this, IDC_PORTFOLIOINFO_ACCOUNT_EDIT);
	m_Account2.Setup(this, IDC_PORTFOLIOINFO_ACCOUNT2_EDIT);
	m_Bank.Setup(this, IDC_PORTFOLIOINFO_BANK_EDIT);
	m_EuroClear.Setup(this, IDC_PORTFOLIOINFO_EUROCLEAR_EDIT, "Invalid Number", 0, FALSE);
	m_Contact.Setup(this, IDC_PORTFOLIOINFO_CONTACT_EDIT);
	m_Phone.Setup(this, IDC_PORTFOLIOINFO_PHONE_EDIT);
	m_Fax.Setup(this, IDC_PORTFOLIOINFO_FAX_EDIT);

	m_Data.Add(m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.Add(&m_Custodian, &m_Data.GetRec().GetCustodian());
	m_Data.Add(&m_CustodianAccount, &m_Data.GetRec().GetCustodianAccount());
	m_Data.Add(&m_Account, &m_Data.GetRec().GetAccount());
	m_Data.Add(&m_Account2, &m_Data.GetRec().GetAccount2());
	m_Data.Add(&m_Bank, &m_Data.GetRec().GetBank());
	m_Data.Add(&m_EuroClear, &m_Data.GetRec().GetEuroClear());
	m_Data.Add(m_Margin, &m_Data.GetRec().GetMargin());
	m_Data.Add(&m_Contact, &m_Data.GetRec().GetContact());
	m_Data.Add(&m_Phone, &m_Data.GetRec().GetPhone());
	m_Data.Add(&m_Fax, &m_Data.GetRec().GetFax());

	m_Data.Setup(m_OraLoader, &m_SS);

	m_OraLoader.Open("SELECT PORTFOLIO FROM SEMAM.NW_PORTFOLIOS WHERE STATUS IS NULL ");
	m_OraLoader.LoadComboBox(*m_Portfolio);

	this->SetWindowText("PORTFOLIO INFO");
	OnPortfolioinfoFindButton();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortfolioInfo::OnPortfolioinfoAddButton() 
{
	if(m_Data.AddRow())
		m_Data.Refresh();
}

void CPortfolioInfo::OnPortfolioinfoDeleteButton() 
{
	m_Data.DeleteRow();
}

void CPortfolioInfo::OnPortfolioinfoUpdateButton() 
{
	m_Data.UpdateRow();
}

void CPortfolioInfo::OnPortfolioinfoFindButton() 
{
	m_Data.LoadDBData();
}

void CPortfolioInfo::OnPortfolioinfoClearButton() 
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_PORTFOLIOINFO_ADD_BUTTON)->EnableWindow(FALSE);
}

void CPortfolioInfo::OnDblClickPortfolioInfoList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	EnableCtrls();
}

void CPortfolioInfo::OnBlockSelectedPortfolioInfoList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickPortfolioInfoList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CPortfolioInfo::OnEnChangePortfolioinfoAccount2Edit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
