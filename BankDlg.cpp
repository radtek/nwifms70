// BankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "BankDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBankDlg dialog


CBankDlg::CBankDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBankDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBankDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBankDlg)
	DDX_Control(pDX, IDC_BANK_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBankDlg, CDialog)
	//{{AFX_MSG_MAP(CBankDlg)
	ON_BN_CLICKED(IDC_BANK_CLEAR_BUTTON, OnBankClearButton)
	ON_BN_CLICKED(IDC_BANK_DELETE_BUTTON, OnBankDeleteButton)
	ON_BN_CLICKED(IDC_BANK_FIND_BUTTON, OnBankFindButton)
	ON_BN_CLICKED(IDC_BANK_UPDATE_BUTTON, OnBankUpdateButton)
	ON_BN_CLICKED(IDC_BANK_ADD_BUTTON, OnBankAddButton)
	ON_EN_CHANGE(IDC_BANK_ABA_EDIT, OnChangeBankAbaEdit)
	ON_EN_CHANGE(IDC_BANK_BIC_EDIT, OnChangeBankBicEdit)
	ON_EN_CHANGE(IDC_BANK_ID_EDIT, OnChangeBankIdEdit)
	ON_EN_CHANGE(IDC_BANK_NAME_EDIT, OnChangeBankNameEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBankDlg message handlers

BEGIN_EVENTSINK_MAP(CBankDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBankDlg)
	ON_EVENT(CBankDlg, IDC_BANK_LIST, 2 /* BlockSelected */, OnBlockSelectedBankList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CBankDlg, IDC_BANK_LIST, 11 /* DblClick */, OnDblClickBankList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CBankDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(4);

	m_BankEdit.Setup(this, IDC_BANK_NAME_EDIT, EMPTYSTRING, UPPERCASE);
	m_IDEdit.Setup(this, IDC_BANK_ID_EDIT, EMPTYSTRING, UPPERCASE);
	m_ABAEdit.Setup(this, IDC_BANK_ABA_EDIT, "0123456789", UPPERCASE);
	m_BICEdit.Setup(this, IDC_BANK_BIC_EDIT, EMPTYSTRING, UPPERCASE);
	m_Data.Add(&m_BankEdit, &m_Data.GetKey());
	m_Data.Add(&m_IDEdit);
	m_Data.Add(&m_ABAEdit);
	m_Data.Add(&m_BICEdit);
	m_Data.SetSS(&m_SS);
	m_Data.SetSheetHeaders();

	m_Data.SetLimitText(m_BankEdit, 0);
	m_Data.SetLimitText(m_IDEdit, 1);
	m_Data.SetLimitText(m_ABAEdit, 2);
	m_Data.SetLimitText(m_BICEdit, 3);

	GetDlgItem(IDC_BANK_FIND_BUTTON)->EnableWindow(FALSE);
	OnBankClearButton();
	m_bModified = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBankDlg::OnBlockSelectedBankList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickBankList(m_SS.GetActiveCol(), m_SS.GetActiveRow());	
}

void CBankDlg::OnDblClickBankList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	
	GetDlgItem(IDC_BANK_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_BANK_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

void CBankDlg::OnBankClearButton() 
{
	m_Data.Refresh();

	GetDlgItem(IDC_BANK_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BANK_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_BANK_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void CBankDlg::OnBankDeleteButton() 
{
	m_Data.DeleteRow();
	m_bModified = TRUE;
}

void CBankDlg::OnBankFindButton() 
{
	m_Data.LoadDBData();
}

void CBankDlg::OnBankUpdateButton() 
{
	m_Data.UpdateRow();
	m_bModified = TRUE;	
}

void CBankDlg::OnBankAddButton() 
{
	if(m_Data.AddRow())
	{
		m_Data.Refresh();
		m_bModified = TRUE;
	}
}

void CBankDlg::OnChangeBankAbaEdit() 
{
	OnChangeBankNameEdit();
}

void CBankDlg::OnChangeBankBicEdit() 
{
	OnChangeBankNameEdit();	
}

void CBankDlg::OnChangeBankIdEdit() 
{
	OnChangeBankNameEdit();	
}

void CBankDlg::OnChangeBankNameEdit() 
{
	int KeyLen, DataLen, Data2Len, Data3Len;

	KeyLen = m_BankEdit.GetWindowTextLength();
	DataLen = m_IDEdit.GetWindowTextLength();
	Data2Len = m_ABAEdit.GetWindowTextLength();
	Data3Len = m_BICEdit.GetWindowTextLength();

	GetDlgItem(IDC_BANK_DELETE_BUTTON)->EnableWindow(KeyLen > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_BANK_FIND_BUTTON)->EnableWindow(KeyLen > 0 || DataLen > 0 || Data2Len > 0 || Data3Len > 0 ? TRUE : FALSE);
	if(KeyLen > 0)
	{
		GetDlgItem(IDC_BANK_ADD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_BANK_UPDATE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BANK_ADD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BANK_UPDATE_BUTTON)->EnableWindow(FALSE);
	}
}
