// Connect.cpp : implementation file
//

#include "stdafx.h"
#include "Connect.h"
#include "oraloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnect dialog


CConnect::CConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CConnect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnect)
	m_nCount = 0;
	m_nTrials = 1;		// Default try once
	m_pTheApp = (CNWIFMS70App *) AfxGetApp();
	//}}AFX_DATA_INIT
}


void CConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnect)
	DDX_Text(pDX, IDC_HOST, m_pTheApp->m_Host);
	DDV_MaxChars(pDX, m_pTheApp->m_Host, 30);
	DDX_Text(pDX, IDC_PASSWORD, m_pTheApp->m_Password);
	DDV_MaxChars(pDX, m_pTheApp->m_Password, 20);
	DDX_Text(pDX, IDC_USER, m_pTheApp->m_User);
	DDV_MaxChars(pDX, m_pTheApp->m_User, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnect, CDialog)
	//{{AFX_MSG_MAP(CConnect)
	ON_EN_CHANGE(IDC_HOST, OnChangeHost)
	ON_EN_CHANGE(IDC_PASSWORD, OnChangePassword)
	ON_EN_CHANGE(IDC_USER, OnChangeUser)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnect message handlers

void CConnect::OnChangeHost() 
{
	EnableOk();
}

void CConnect::OnChangePassword() 
{
	EnableOk();
}

void CConnect::OnChangeUser() 
{
	EnableOk();
}

BOOL CConnect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(strlen((LPCTSTR )m_pTheApp->GetHost()) == 0 || 
		strlen((LPCTSTR )m_pTheApp->GetUser()) == 0 || 
		strlen((LPCTSTR )m_pTheApp->GetPassword()) == 0)
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	SetWindowText(_T("Log on"));
	return TRUE; 	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnect::EnableOk()
{
	CEdit* H;
	CEdit* U;
	CEdit* P;
   
	H = (CEdit*) GetDlgItem(IDC_HOST);
	U = (CEdit*) GetDlgItem(IDC_USER);
	P = (CEdit*) GetDlgItem(IDC_PASSWORD);

	GetDlgItem(IDOK)->EnableWindow(H->LineLength() && U->LineLength() && P->LineLength());
}

int CConnect::PasswordExpiryWarning()
{  // return Days
	COraLoader OraLoader;
	int Days = -1;
	CString Text;

	OraLoader.SetDB(&theDB);

	if(!OraLoader.Open("SELECT EXPIRY_DATE - TRUNC(SYSDATE), 'Your database password is about to expire. You must change it by ' ||TO_CHAR(Expiry_Date)||'. ' "
					"FROM USER_USERS "
					"where ACCOUNT_STATUS = 'OPEN' "
					"AND EXPIRY_DATE IS NOT NULL "
					"AND EXPIRY_DATE - TRUNC(SYSDATE) < 7 "
					"UNION "
					"SELECT EXPIRY_DATE - TRUNC(SYSDATE), 'Your database password expired on '||TO_CHAR(Expiry_Date)||'.  Please talk to system administrator. ' "
					"FROM USER_USERS "
					"WHERE ACCOUNT_STATUS = 'EXPIRED' "))
		return Days;

	while(!OraLoader.IsEOF())
	{
		OraLoader.GetFieldValue(0, &Days);
		Text = OraLoader.GetDBString(1);
		OraLoader.MoveNext();
	}

	if(!Text.IsEmpty())
		MessageBox(Text, "Connection", MB_OK);

	return Days;
}

void CConnect::OnOK()
{	
	if(!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

    BeginWaitCursor();
	if(m_nCount < m_nTrials)
	{
		if(theDB.Open((LPCTSTR) theApp.GetHost(), (LPCTSTR) theApp.GetUser(), (LPCTSTR) theApp.GetPassword()) != OSUCCESS)
		{
			MessageBox(theDB.GetErrorText(), theDB.LookupErrorText(theDB.ErrorNumber()));
			MessageBox(_T("Fail to Login"));
			m_nCount ++;			
			if(m_nCount == m_nTrials)  // Allow three trial only
				CDialog::OnCancel();
		}
		else
		{
			PasswordExpiryWarning();
			CDialog::OnOK();
		}
	}

	EndWaitCursor();
}

void CConnect::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(m_pTheApp->GetUser().GetLength() == 0)
		GotoDlgCtrl(GetDlgItem(IDC_USER));
	else
		GotoDlgCtrl(GetDlgItem(IDC_PASSWORD));
}

