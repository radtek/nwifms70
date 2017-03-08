// Password.cpp : implementation file
//

#include "stdafx.h"
#include "NWIfms70.h"
#include "Password.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassword dialog


CPassword::CPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassword)
	m_NewPassword2 = _T("");
	m_NewPassword = _T("");
	m_Password = _T("");
	//}}AFX_DATA_INIT
}


void CPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassword)
	DDX_Text(pDX, IDC_NEW_PASSWORD_2_EDIT, m_NewPassword2);
	DDX_Text(pDX, IDC_NEW_PASSWORD_EDIT, m_NewPassword);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_Password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassword, CDialog)
	//{{AFX_MSG_MAP(CPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassword message handlers
BOOL CPassword::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetWindowText(_T("Change Password"));
	return TRUE;
}

void CPassword::OnOK() 
{
	if(!CDialog::UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination\n");
		return;
	}

	if(m_NewPassword != m_NewPassword2)
	{
		MessageBox(_T("New Password mismatch"));
		return;
	}

	if(m_Password == m_NewPassword)
	{
		MessageBox(_T("No change"));
		return;
	}
	
	m_OraLoader.GetSql() = "ALTER USER ";
	m_OraLoader.GetSql() += m_OraLoader.GetDB()->GetConnectString();
	m_OraLoader.GetSql() +=  " IDENTIFIED BY \"" + m_NewPassword + "\" REPLACE \"" + m_Password + "\"";

	if(!m_OraLoader.ExecuteSql())
	{
		MessageBox(_T("Fail to change password. Please try again"));
		return;
	}

	MessageBox(_T("Password has been changed"));
	CDialog::OnOK();
}
