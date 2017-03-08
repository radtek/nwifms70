// OptExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "OptExpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptExpDlg dialog


COptExpDlg::COptExpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptExpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptExpDlg)
	m_Date.Empty();
	//}}AFX_DATA_INIT
}


void COptExpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptExpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptExpDlg, CDialog)
	//{{AFX_MSG_MAP(COptExpDlg)
	ON_EN_KILLFOCUS(IDC_OPTEXP_SRCDATE_EDIT, OnKillfocusOptexpSrcdateEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptExpDlg message handlers

BOOL COptExpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDOK)->EnableWindow(!m_Date.IsEmpty());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptExpDlg::OnKillfocusOptexpSrcdateEdit() 
{
	CString Date;

	GetDlgItem(IDC_OPTEXP_SRCDATE_EDIT)->GetWindowText(Date);
	if(!Date.IsEmpty())
	{
		int Month, Year;
		Year = Date.Find("/");
		if(Year < 0)
			return;

		Month = atoi(Date);
		Year = atoi(((LPCTSTR ) Date) + Year + 1);
		Date.Format("%02d/01/%04d", Month, Year);
		GetOraLoader().OptExpDate(m_Date, Date);
		GetDlgItem(IDC_OPTEXP_DESTDATE_EDIT)->SetWindowText(m_Date);
	}
	GetDlgItem(IDOK)->EnableWindow(!m_Date.IsEmpty());
}
