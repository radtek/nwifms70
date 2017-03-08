// ClaimCP.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "ClaimCP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClaimCP dialog


CClaimCP::CClaimCP(CWnd* pParent /*=NULL*/)
	: CDialog(CClaimCP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClaimCP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CClaimCP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClaimCP)
	DDX_Control(pDX, IDC_CLAIM_LIST, m_SS);
	//}}AFX_DATA_MAP
}

void CClaimCP::EnableCtrls()
{
	GetDlgItem(IDC_CLAIM_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_CLAIM_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BEGIN_MESSAGE_MAP(CClaimCP, CDialog)
	//{{AFX_MSG_MAP(CClaimCP)
	ON_BN_CLICKED(IDC_CLAIM_ADD_BUTTON, OnClaimAddButton)
	ON_BN_CLICKED(IDC_CLAIM_CLEAR_BUTTON, OnClaimClearButton)
	ON_BN_CLICKED(IDC_CLAIM_DELETE_BUTTON, OnClaimDeleteButton)
	ON_BN_CLICKED(IDC_CLAIM_FIND_BUTTON, OnClaimFindButton)
	ON_BN_CLICKED(IDC_CLAIM_UPDATE_BUTTON, OnClaimUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClaimCP message handlers

BEGIN_EVENTSINK_MAP(CClaimCP, CDialog)
    //{{AFX_EVENTSINK_MAP(CClaimCP)
	ON_EVENT(CClaimCP, IDC_CLAIM_LIST, 2 /* BlockSelected */, OnBlockSelectedClaimList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CClaimCP, IDC_CLAIM_LIST, 11 /* DblClick */, OnDblClickClaimList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CClaimCP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(15);
	m_SS.SetVisibleCols(6);

	m_CP = (COptComboBox *) new COptComboBox(this, IDC_CLAIM_CPCODE_COMBO, TRUE);

	m_ClaimCode.Setup(this, IDC_CLAIM_CONTACT_EDIT);
	m_ClaimCode.LimitText(2);
	m_Name.Setup(this, IDC_CLAIM_NAME_EDIT, EMPTYSTRING, NOCASE);
	m_Name.LimitText(40);
	m_Phone.Setup(this, IDC_CLAIM_PHONE_EDIT);
	m_Phone.LimitText(16);
	m_Fax.Setup(this, IDC_CLAIM_FAX_EDIT);
	m_Fax.LimitText(16);
	m_EMail.Setup(this, IDC_CLAIM_EMAIL_EDIT);
	m_EMail.LimitText(60);

	m_Data.Add(m_CP, &m_Data.GetRec().GetCP());
	m_Data.Add(&m_ClaimCode, &m_Data.GetRec().GetClaimCode());
	m_Data.Add(&m_Name, &m_Data.GetRec().GetName());
	m_Data.Add(&m_Phone, &m_Data.GetRec().GetPhone());
	m_Data.Add(&m_Fax, &m_Data.GetRec().GetFax());
	m_Data.Add(&m_EMail, &m_Data.GetRec().GetEMail());

	m_Data.Setup(&m_SS);
	m_Data.SetOraLoader(m_pData->GetOraLoader());

	BeginWaitCursor();

	m_pData->GetContactList().CopyKeyToComboBox(*m_CP);

	this->SetWindowText("Claim Counterparty");

	OnClaimFindButton();

	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClaimCP::OnClaimAddButton() 
{
	if(m_Data.AddRow())
		m_Data.Refresh();
}

void CClaimCP::OnClaimClearButton() 
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_CLAIM_ADD_BUTTON)->EnableWindow(FALSE);
}

void CClaimCP::OnClaimDeleteButton() 
{
	m_Data.DeleteRow();
}

void CClaimCP::OnClaimFindButton() 
{
	m_Data.LoadDBData();
}

void CClaimCP::OnBlockSelectedClaimList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickClaimList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CClaimCP::OnDblClickClaimList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);

	EnableCtrls();
}

void CClaimCP::OnClaimUpdateButton() 
{
	m_Data.UpdateRow();
}

void CClaimCP::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
