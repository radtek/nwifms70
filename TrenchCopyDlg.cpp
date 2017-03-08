// TrenchCopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "TrenchCopyDlg.h"
#include "afxdialogex.h"


// CTrenchCopyDlg dialog

IMPLEMENT_DYNAMIC(CTrenchCopyDlg, CDialog)

CTrenchCopyDlg::CTrenchCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrenchCopyDlg::IDD, pParent)
{
	m_TrenchCode = NULL;
}

CTrenchCopyDlg::~CTrenchCopyDlg()
{
	if(m_TrenchCode)
		delete m_TrenchCode;
}

void CTrenchCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRENCH_LIST, m_SS);
}


BEGIN_MESSAGE_MAP(CTrenchCopyDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_TRENCH_CODE_COMBO, &CTrenchCopyDlg::OnCbnSelchangeTrenchCodeCombo)
	ON_BN_CLICKED(IDOK, &CTrenchCopyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CTrenchCopyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SS.SetVisibleCols(1);
	m_SS.SetVisibleRows(14);
	m_SS.LockSheet();

	m_Ticket.Setup(this, IDC_TRENCH_TICKET_EDIT);
	m_Ticket.LimitText(15);
	m_TrenchCode = new COptComboBox(this, IDC_TRENCH_CODE_COMBO);

	m_OraLoader.SetDB(&theDB);
	m_OraLoader.Open("SELECT DISTINCT TRENCH_CODE FROM SEMAM.NW_TRENCH_ASSETS ORDER BY 1 ");
	m_OraLoader.LoadComboBox(*m_TrenchCode);

	m_Ticket.SetWindowText(m_sTicket);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	return TRUE;
}
// CTrenchCopyDlg message handlers


void CTrenchCopyDlg::OnCbnSelchangeTrenchCodeCombo()
{
	if(m_TrenchCode->GetCurSel() < 0)
		return;

	m_TrenchCode->GetSelString(m_sTrenchCode);
	
	if(!m_sTrenchCode.IsEmpty())
	{
		m_OraLoader.Open("SELECT ASSET_CODE FROM SEMAM.NW_TRENCH_ASSETS WHERE TRENCH_CODE = '" + m_sTrenchCode + "' ORDER BY 1");
		m_OraLoader.LoadSheet(m_SS);
	}

	GetDlgItem(IDOK)->EnableWindow(!m_sTrenchCode.IsEmpty());
}


void CTrenchCopyDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_TrenchCode->GetSelString(m_sTrenchCode);
	
	CDialog::OnOK();
}
