// PaymentTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "qdata.h"
#include "PaymentTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaymentTemplateDlg dialog


CPaymentTemplateDlg::CPaymentTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaymentTemplateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaymentTemplateDlg)
	m_CP = NULL;
	m_Currency = NULL;
	m_Bank = NULL;
	m_Portfolio = NULL;
	m_Active = NULL;
	//}}AFX_DATA_INIT
}

CPaymentTemplateDlg::~CPaymentTemplateDlg()
{
	if(m_CP)
		delete m_CP;
	if(m_Currency)
		delete m_Currency;
	if(m_Bank)
		delete m_Bank;
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_Active)
		delete m_Active;
}

void CPaymentTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaymentTemplateDlg)
	DDX_Control(pDX, IDC_PAY_TEMPLATE_INSTRUCTION_FUND_LIST, m_FundSS);
	DDX_Control(pDX, IDC_PAY_TEMPLATE_INSTRUCTION_LIST, m_SS);
	//}}AFX_DATA_MAP
}

BOOL CPaymentTemplateDlg::AddFundList()
{
	CString ID;
	CStringArray Funds;

	m_ID.GetWindowText(ID);
	
	Funds.RemoveAll();
	for(int i = 1; i <= m_FundSS.GetSheetRows(); i ++)
		Funds.Add(m_FundSS.GetSheetText(1, i));

	return m_Data.AddSRows(ID, Funds);
}

void CPaymentTemplateDlg::LoadFundSheet()
{
	CString RefID;
	CQData QData;

	m_ID.GetWindowText(RefID);

	RefID = QData.GetQueryText(RefID);
	m_Data.GetOraLoader().Open("SELECT PORTFOLIO FROM SEMAM.NW_PAY_TEMPLATE_FUND WHERE REF_ID = " + RefID + " ORDER BY 1 ");

	m_FundSS.ClearSheet();
	m_Data.GetOraLoader().LoadDBSheet(m_FundSS);

	if(m_FundSS.GetSheetRows() > 0)
		m_FundSS.SetSheetCurRow(1);
}

BEGIN_MESSAGE_MAP(CPaymentTemplateDlg, CDialog)
	//{{AFX_MSG_MAP(CPaymentTemplateDlg)
	ON_BN_CLICKED(IDC_PAY_TEMPLATE_ADD_BUTTON, OnPayTemplateAddButton)
	ON_BN_CLICKED(IDC_PAY_TEMPLATE_CLEAR_BUTTON, OnPayTemplateClearButton)
	ON_BN_CLICKED(IDC_PAY_TEMPLATE_DELETE_BUTTON, OnPayTemplateDeleteButton)
	ON_BN_CLICKED(IDC_PAY_TEMPLATE_FIND_BUTTON, OnPayTemplateFindButton)
	ON_BN_CLICKED(IDC_PAY_TEMPLATE_UPDATE_BUTTON, OnPayTemplateUpdateButton)
	ON_BN_CLICKED(IDC_PAY_TEMPLATE_ADDFUND_BUTTON, OnBnClickedPayTemplateAddfundButton)
	ON_BN_CLICKED(IDC_PAY_TEMPLATE_DELETEFUND_BUTTON, OnBnClickedPayTemplateDeletefundButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaymentTemplateDlg message handlers

BOOL CPaymentTemplateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CQData QData;
	
	m_FundSS.SetVisibleCols(1);
	m_FundSS.SetVisibleRows(4);
	m_FundSS.SetSheetCols(1);
	m_SS.SetVisibleCols(5);
	m_SS.SetVisibleRows(12);

	m_CP = (COptComboBox *) new COptComboBox(this, IDC_PAY_TEMPLATE_CP_COMBO);
	m_Currency = (COptComboBox *) new COptComboBox(this, IDC_PAY_TEMPLATE_CURRENCY_COMBO);
	m_Bank = (COptComboBox *) new COptComboBox(this, IDC_PAY_TEMPLATE_BANK_COMBO);
	m_Portfolio = (COptComboBox *) new COptComboBox(this, IDC_PAY_TEMPLATE_FUND_COMBO, TRUE);
	m_ID.Setup(this, IDC_PAY_TEMPLATE_ID_EDIT);
	m_ID.LimitText(10);
	m_Account.Setup(this, IDC_PAY_TEMPLATE_ACCOUNT_EDIT);
	m_Account.LimitText(30);
	m_CPRef.Setup(this, IDC_PAY_TEMPLATE_NOTE_EDIT);
	m_Beneficiary.Setup(this, IDC_PAY_TEMPLATE_BENEFICIARY_EDIT);
	m_Beneficiary.LimitText(30);
	m_BeneficiaryID.Setup(this, IDC_PAY_TEMPLATE_BENEFICIARY_ID_EDIT);
	m_BeneficiaryID.LimitText(11);
	m_BeneficiaryBIC.Setup(this, IDC_PAY_TEMPLATE_BENEFICIARY_BIC_EDIT);
	m_BeneficiaryBIC.LimitText(11);
	m_BeneficiaryAccount.Setup(this, IDC_PAY_TEMPLATE_BENEFICIARY_ACCOUNT_EDIT);
	m_BeneficiaryAccount.LimitText(30);
	m_FFC.Setup(this, IDC_PAY_TEMPLATE_FFC_EDIT);
	m_FFC.LimitText(30);
	m_FFCID.Setup(this, IDC_PAY_TEMPLATE_FFC_ID_EDIT);
	m_FFCID.LimitText(11);
	m_FFCBIC.Setup(this, IDC_PAY_TEMPLATE_FFC_BIC_EDIT);
	m_FFCBIC.LimitText(11);
	m_FFCAccount.Setup(this, IDC_PAY_TEMPLATE_FFCACCOUNT_EDIT);
	m_FFCAccount.LimitText(30);
	m_Active = (CCheckBox *) new CCheckBox(this, IDC_PAY_TEMPLATE_ACTIVE_CHECK);
	
	m_Data.Add(&m_ID, &m_Data.GetRec().GetRefID());
	m_Data.Add(m_CP, &m_Data.GetRec().GetCP());
	m_Data.Add(m_Currency, &m_Data.GetRec().GetCurrency());
	m_Data.Add(m_Bank, &m_Data.GetRec().GetBank());
	m_Data.Add(&m_Account, &m_Data.GetRec().GetAccount());
	m_Data.Add(&m_CPRef, &m_Data.GetRec().GetCPRef());
	m_Data.Add(&m_Beneficiary, &m_Data.GetRec().GetBeneficiary());
	m_Data.Add(&m_BeneficiaryID, &m_Data.GetRec().GetBeneficiaryID());
	m_Data.Add(&m_BeneficiaryBIC, &m_Data.GetRec().GetBeneficiaryBIC());
	m_Data.Add(&m_BeneficiaryAccount, &m_Data.GetRec().GetBeneficiaryAccount());
	m_Data.Add(&m_FFC, &m_Data.GetRec().GetFFC());
	m_Data.Add(&m_FFCID, &m_Data.GetRec().GetFFCID());
	m_Data.Add(&m_FFCBIC, &m_Data.GetRec().GetFFCBIC());
	m_Data.Add(&m_FFCAccount, &m_Data.GetRec().GetFFCAccount());
	m_Data.Add(m_Active, &m_Data.GetRec().GetActive());
	m_Data.Setup(m_pData->GetOraLoader(), &m_SS);

	GetDlgItem(IDC_PAY_TEMPLATE_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAY_TEMPLATE_UPDATE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAY_TEMPLATE_DELETEFUND_BUTTON)->EnableWindow(FALSE);

	QData.CopyKeyDBListKeyToComboBox(m_pData->GetContactList(), *m_CP, FALSE);
	QData.CopyDBRecArrayToComboBox(m_pData->GetCurrencyArr(), *m_Currency, 0, FALSE);
	QData.CopyDBRecArrayToComboBox(m_pData->GetPortfolioArr(), *m_Portfolio, 0, FALSE);
	m_Data.LoadBank(*m_Bank);

	this->SetWindowText("PAYMENT TEMPLATE");
	m_Data.LoadDBData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CPaymentTemplateDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPaymentTemplateDlg)
	ON_EVENT(CPaymentTemplateDlg, IDC_PAY_TEMPLATE_INSTRUCTION_LIST, 2 /* BlockSelected */, OnBlockSelectedPayTemplateInstructionList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPaymentTemplateDlg, IDC_PAY_TEMPLATE_INSTRUCTION_LIST, 11 /* DblClick */, OnDblClickPayTemplateInstructionList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CPaymentTemplateDlg, IDC_PAY_TEMPLATE_INSTRUCTION_FUND_LIST, 5, CPaymentTemplateDlg::OnClickPayTemplateInstructionFundList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CPaymentTemplateDlg::OnBlockSelectedPayTemplateInstructionList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickPayTemplateInstructionList(m_SS.GetActiveCol(), m_SS.GetActiveRow());	
}

void CPaymentTemplateDlg::OnDblClickPayTemplateInstructionList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	
	GetDlgItem(IDC_PAY_TEMPLATE_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_PAY_TEMPLATE_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
	LoadFundSheet();
}

void CPaymentTemplateDlg::OnPayTemplateAddButton() 
{
	if(m_Data.AddRow())
	{
		AddFundList();
		m_Data.Refresh();
	}
}

void CPaymentTemplateDlg::OnPayTemplateClearButton() 
{
	m_Data.Refresh();
	GetDlgItem(IDC_PAY_TEMPLATE_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAY_TEMPLATE_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAY_TEMPLATE_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void CPaymentTemplateDlg::OnPayTemplateDeleteButton() 
{
	CString ID;

	m_ID.GetWindowText(ID);

	if(m_Data.DeleteSRows(ID))
		m_Data.DeleteRow();

	GetDlgItem(IDC_PAY_TEMPLATE_DELETE_BUTTON)->EnableWindow(FALSE);
}

void CPaymentTemplateDlg::OnPayTemplateFindButton() 
{
	m_Data.LoadDBData();	
}

void CPaymentTemplateDlg::OnPayTemplateUpdateButton() 
{
	if(m_Data.UpdateRow())
		AddFundList();
	GetDlgItem(IDC_PAY_TEMPLATE_UPDATE_BUTTON)->EnableWindow(FALSE);	
}


void CPaymentTemplateDlg::OnBnClickedPayTemplateAddfundButton()
{
	BOOL bFound = FALSE;
	CString Portfolio;
	CStringArray Data;

	m_Portfolio->GetSelString(Portfolio);

	for (int i = 1; i <= m_FundSS.GetSheetRows(); i ++)
	{
		if(Portfolio == m_FundSS.GetSheetText(1, i))
			bFound = TRUE;
	}

	if(!bFound)
	{
		Data.RemoveAll();
		Data.Add(Portfolio);
		m_FundSS.AddSheetRow(Data);
	}
}


void CPaymentTemplateDlg::OnBnClickedPayTemplateDeletefundButton()
{
	if(m_FundSS.GetSheetCurRow() <= 0)
		return;

	m_FundSS.DeleteSheetRow(m_FundSS.GetSheetCurRow());
	GetDlgItem(IDC_PAY_TEMPLATE_DELETEFUND_BUTTON)->EnableWindow(FALSE);
}


void CPaymentTemplateDlg::OnClickPayTemplateInstructionFundList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_FundSS.GetSheetRows())
		m_FundSS.SetSheetCurRow(Row);

	GetDlgItem(IDC_PAY_TEMPLATE_DELETEFUND_BUTTON)->EnableWindow(m_FundSS.GetSheetCurRow());
}
