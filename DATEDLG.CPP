// DateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "DateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateDlg dialog


CDateDlg::CDateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDateDlg)
	//}}AFX_DATA_INIT
	m_nInputType = 1;  // Default Date Type
	m_DlgHeader = "Input Date";
	m_Prompt = "Please enter Date:";
}

void CDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDateDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDateDlg, CDialog)
	//{{AFX_MSG_MAP(CDateDlg)
	ON_EN_CHANGE(IDC_INPUT, OnChangeDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateDlg message handlers
BOOL CDateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString Data;

	Data = m_Data;
	switch(m_nInputType)
	{
		case 1: // Date
			m_DateEdit.Setup(this, IDC_INPUT, FALSE);
			m_DateEdit.SetData(m_Data);
			break;
		case 2: // ID
			m_IDEdit.Setup(this, IDC_INPUT);
			m_IDEdit.SetData(m_Data);
			break;
		default:
			break;
	}

	GetDlgItem(IDC_PROMPT)->SetWindowText(m_Prompt);
	SetWindowText(m_DlgHeader);

	return TRUE;
}

void CDateDlg::EnableOk()
{
	switch(m_nInputType)
	{
		case 1: // Date
			if(m_DateEdit.ValidData())
			{
				m_Data = m_DateEdit.GetData();
				GetDlgItem(IDOK)->EnableWindow(TRUE);
			}
			else
				GetDlgItem(IDOK)->EnableWindow(FALSE);
			break;
		case 2: // ID
			if(m_IDEdit.ValidData())
			{
				m_Data = m_IDEdit.GetData();
				GetDlgItem(IDOK)->EnableWindow(TRUE);
			}
			else
				GetDlgItem(IDOK)->EnableWindow(FALSE);
			break;
		default:
			break;
	}
}   

void CDateDlg::OnChangeDate() 
{
	switch(m_nInputType)
	{
		case 1: // Date
			m_Data = m_DateEdit.GetData();
			break;
		case 2: // ID
			m_Data = m_IDEdit.GetData();
			break;
		default:
			break;
	}

	EnableOk();	
}
