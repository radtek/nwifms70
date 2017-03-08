// AccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "AccountDlg.h"
#include "finddlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog


CAccountDlg::CAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountDlg)
	m_CP = NULL;
	m_Collateral = NULL;
	m_Future = NULL;
	m_Active = NULL;
	//}}AFX_DATA_INIT
}

CAccountDlg::~CAccountDlg()
{
	if(m_CP)
		delete m_CP;
	if(m_Collateral)
		delete m_Collateral;
	if(m_Future)
		delete m_Future;
	if(m_Active)
		delete m_Active;
}

void CAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountDlg)
	DDX_Control(pDX, IDC_ACCOUNT_LIST, m_SS);
	//}}AFX_DATA_MAP
}

void CAccountDlg::EnableCtrls()
{
	GetDlgItem(IDC_ACCOUNT_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_ACCOUNT_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
	GetDlgItem(IDC_ACCOUNT_GROUP_BUTTON)->EnableWindow(m_CP->GetCurSel() >= 0 ? TRUE : FALSE);
}

BEGIN_MESSAGE_MAP(CAccountDlg, CDialog)
	//{{AFX_MSG_MAP(CAccountDlg)
	ON_BN_CLICKED(IDC_ACCOUNT_CLEAR_BUTTON, OnAccountClearButton)
	ON_BN_CLICKED(IDC_ACCOUNT_FIND_BUTTON, OnAccountFindButton)
	ON_BN_CLICKED(IDC_ACCOUNT_ADD_BUTTON, OnAccountAddButton)
	ON_BN_CLICKED(IDC_ACCOUNT_UPDATE_BUTTON, OnAccountUpdateButton)
	ON_CBN_SELCHANGE(IDC_ACCOUNT_COMBO, OnSelchangeAccountCombo)
	ON_BN_CLICKED(IDC_ACCOUNT_GROUP_BUTTON, OnAccountGroupButton)
	ON_BN_CLICKED(IDC_ACCOUNT_DELETE_BUTTON, OnAccountDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg message handlers

BOOL CAccountDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(15);
	m_SS.SetVisibleCols(5);

	m_CP = (COptComboBox *) new COptComboBox(this, IDC_ACCOUNT_COMBO, TRUE);
	m_Collateral = (CCheckBox*) new CCheckBox(this, IDC_ACCOUNT_COLLATERAL_CHECK);
	m_Future = (CCheckBox*) new CCheckBox(this, IDC_ACCOUNT_FUTURE_CHECK);
	m_Clearing = (CCheckBox*) new CCheckBox(this, IDC_ACCOUNT_CLEARING_CHECK);
	m_Active = (CCheckBox*) new CCheckBox(this, IDC_ACCOUNT_ACTIVE_CHECK);

	m_Account.Setup(this, IDC_ACCOUNT_EDIT);
	m_Account.LimitText(10);
	m_Desc.Setup(this, IDC_ACCOUNT_DESC_EDIT);
	m_Desc.LimitText(30);
	m_Group.Setup(this, IDC_ACCOUNT_GROUP_EDIT);
	m_Group.LimitText(3);

	m_Data.Add(&m_Account, &m_Data.GetRec().GetAccount());
	m_Data.Add(&m_Desc, &m_Data.GetRec().GetDesc());
	m_Data.Add(m_CP, &m_Data.GetRec().GetCP());
	m_Data.Add(&m_Group, &m_Data.GetRec().GetGroup());
	m_Data.Add(m_Collateral, &m_Data.GetRec().GetCollateral());
	m_Data.Add(m_Future, &m_Data.GetRec().GetFuture());
	m_Data.Add(m_Clearing, &m_Data.GetRec().GetClearing());
	m_Data.Add(m_Active, &m_Data.GetRec().GetActive());

	m_Data.Setup(&m_SS);
	m_Data.SetOraLoader(m_pData->GetOraLoader());

	m_pData->GetContactList().CopyKeyToComboBox(*m_CP);
	OnAccountFindButton();
	GetDlgItem(IDC_ACCOUNT_GROUP_BUTTON)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAccountDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAccountDlg)
	ON_EVENT(CAccountDlg, IDC_ACCOUNT_LIST, 2 /* BlockSelected */, OnBlockSelectedAccountList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CAccountDlg, IDC_ACCOUNT_LIST, 11 /* DblClick */, OnDblClickAccountList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAccountDlg::OnAccountClearButton() 
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_ACCOUNT_ADD_BUTTON)->EnableWindow(FALSE);
}

void CAccountDlg::OnAccountFindButton() 
{
	m_Data.LoadDBData();
}

void CAccountDlg::OnAccountAddButton() 
{
	if(m_Data.AddRow())
	{
		m_Data.Refresh();
		m_pData->GetAccountArr().Modified();
	}
}

void CAccountDlg::OnAccountUpdateButton() 
{
	m_Data.UpdateRow();
	m_pData->GetAccountArr().Modified();
}

void CAccountDlg::OnBlockSelectedAccountList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickAccountList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CAccountDlg::OnDblClickAccountList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);

	EnableCtrls();
}

void CAccountDlg::OnSelchangeAccountCombo() 
{
	EnableCtrls();
}

void CAccountDlg::OnAccountGroupButton() 
{
	CFindDlg FindDlg;
	CString CP;
	CQData QData;

	m_CP->GetSelString(CP);
	FindDlg.m_OraLoader = m_Data.GetOraLoader();
	FindDlg.m_OraLoader.GetSql().Format("SELECT CT_GROUP, CT_CODE, CT_NAME, CT_ADDRESS1 \"CP_CT\" , TO_NUMBER(CT_CODE) \"IDX\" "
										"FROM SEMAM.NW_COUNTER_CONTACTS WHERE ISVALID = 'Y' AND CP_CODE = %s ORDER BY 4 ", 
										QData.GetQueryText(CP));
	if(FindDlg.DoModal() == IDOK)
		m_Group.SetWindowText(FindDlg.m_Key);
}

void CAccountDlg::OnAccountDeleteButton() 
{
	m_Data.DeleteRow();
	m_pData->GetAccountArr().Modified();
}
