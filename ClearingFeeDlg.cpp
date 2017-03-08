// ClearingFeeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "ClearingFeeDlg.h"


// CClearingFeeDlg dialog

IMPLEMENT_DYNAMIC(CClearingFeeDlg, CDialog)

CClearingFeeDlg::CClearingFeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClearingFeeDlg::IDD, pParent)
{
	m_Account = NULL;
}

CClearingFeeDlg::~CClearingFeeDlg()
{
	if(m_Account)
		delete m_Account;
}

void CClearingFeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CL_LIST, m_SS);
}

void CClearingFeeDlg::EnableCtrls()
{
	GetDlgItem(IDC_CL_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_CL_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BEGIN_MESSAGE_MAP(CClearingFeeDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CL_ACCOUNT_COMBO, &CClearingFeeDlg::OnCbnSelchangeClAccountCombo)
	ON_BN_CLICKED(IDC_CL_FIND_BUTTON, &CClearingFeeDlg::OnBnClickedClFindButton)
	ON_BN_CLICKED(IDC_CL_CLEAR_BUTTON, &CClearingFeeDlg::OnBnClickedClClearButton)
	ON_BN_CLICKED(IDC_CL_ADD_BUTTON, &CClearingFeeDlg::OnBnClickedClAddButton)
	ON_BN_CLICKED(IDC_CL_UPDATE_BUTTON, &CClearingFeeDlg::OnBnClickedClUpdateButton)
	ON_BN_CLICKED(IDC_CL_DELETE_BUTTON, &CClearingFeeDlg::OnBnClickedClDeleteButton)
END_MESSAGE_MAP()

BOOL CClearingFeeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(15);
	m_SS.SetVisibleCols(4);

	m_Account = (COptComboBox *) new COptComboBox(this, IDC_CL_ACCOUNT_COMBO);
	m_FromDate.Setup(this, IDC_CL_FROMDATE_EDIT);
	m_ToDate.Setup(this, IDC_CL_TODATE_EDIT);
	m_Fee.Setup(this, IDC_CL_FEE_EDIT);

	m_Data.Add(m_Account, &m_Data.GetRec().GetAccount());
	m_Data.Add(&m_FromDate, &m_Data.GetRec().GetFromDate());
	m_Data.Add(&m_ToDate, &m_Data.GetRec().GetToDate());
	m_Data.Add(&m_Fee, &m_Data.GetRec().GetClearingFee());

	m_Data.Setup(&m_SS);

	m_Data.LoadAccounts(m_Account);
	m_Data.Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// CClearingFeeDlg message handlers

void CClearingFeeDlg::OnCbnSelchangeClAccountCombo()
{
	GetDlgItem(IDC_CL_FIND_BUTTON)->EnableWindow(m_Account->GetCurSel() >= 0);
}

BEGIN_EVENTSINK_MAP(CClearingFeeDlg, CDialog)
	ON_EVENT(CClearingFeeDlg, IDC_CL_LIST, 2, CClearingFeeDlg::OnBlockSelectedClList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CClearingFeeDlg, IDC_CL_LIST, 11, CClearingFeeDlg::OnDblClickClList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CClearingFeeDlg::OnBlockSelectedClList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickClList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CClearingFeeDlg::OnDblClickClList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);

	EnableCtrls();
}

void CClearingFeeDlg::OnBnClickedClFindButton()
{
	m_Data.LoadDBData();
}

void CClearingFeeDlg::OnBnClickedClClearButton()
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_CL_ADD_BUTTON)->EnableWindow(FALSE);
}

void CClearingFeeDlg::OnBnClickedClAddButton()
{
	if(m_Data.AddRow())
		m_Data.Refresh();
	m_Data.LoadDBData();
}

void CClearingFeeDlg::OnBnClickedClUpdateButton()
{
	m_Data.UpdateRow();
}

void CClearingFeeDlg::OnBnClickedClDeleteButton()
{
	m_Data.DeleteRow();
}
