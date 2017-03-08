// disperr.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "disperr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayErrorFile dialog


CDisplayErrorFile::CDisplayErrorFile(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayErrorFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplayErrorFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDisplayErrorFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayErrorFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDisplayErrorFile, CDialog)
	//{{AFX_MSG_MAP(CDisplayErrorFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDisplayErrorFile message handlers
BOOL CDisplayErrorFile::OnInitDialog()
{
	CDialog::OnInitDialog();
	char Buf[1024];
	char* p;
	CListBox* List;
	    
    List = ((CListBox*) GetDlgItem(IDC_ERROR_LIST));
    while (m_ErrF.ReadString(Buf, sizeof(Buf)))
    {
		p = strchr(Buf, '\n');
		if(p)
			p[0] = 0;    	
    	List->AddString(Buf);	
    }
	
	m_ErrF.Close();
	SetWindowText(m_Title);
	return TRUE;
}
