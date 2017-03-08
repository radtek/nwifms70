// FindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "FindDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindDlg dialog


CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDlg)
	DDX_Control(pDX, IDC_FIND_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	//{{AFX_MSG_MAP(CFindDlg)
	ON_EN_CHANGE(IDC_FIND_DATA_EDIT, OnChangeFindDataEdit)
	ON_EN_CHANGE(IDC_FIND_KEY_EDIT, OnChangeFindKeyEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFindDlg::Find()
{
	CString Buf;
	int i;

	m_KeyEdit.GetWindowText(Buf);
	if(Buf.IsEmpty())
		return FALSE;

	for(i = 1; i <= m_SS.GetSheetRows(); i++)
	{
		if(Buf == m_SS.GetSheetText(1, i))
		{
			m_SS.SetSheetActiveCell(1, i);
			return TRUE;
		}
	}

	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// CFindDlg message handlers

BEGIN_EVENTSINK_MAP(CFindDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFindDlg)
	ON_EVENT(CFindDlg, IDC_FIND_LIST, 11 /* DblClick */, OnDblClickFindList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CFindDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_OraLoader.GetSql().IsEmpty())
		return FALSE;

	BeginWaitCursor();
	m_SS.SetVisibleRows(15);
	m_KeyEdit.SubclassDlgItem(IDC_FIND_KEY_EDIT, this);
	m_DataEdit.SubclassDlgItem(IDC_FIND_DATA_EDIT, this);

	m_OraLoader.Open();
	m_OraLoader.LoadDBSheet(m_SS);
	m_SS.SetVisibleCols(m_SS.GetSheetCols());
	
	if(!m_Key.IsEmpty())
	{
		m_KeyEdit.SetWindowText(m_Key);
		if(Find())
			OnDblClickFindList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
	}
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindDlg::OnDblClickFindList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		int i;
		CString Text;

		m_KeyEdit.SetWindowText(m_SS.GetSheetText(1, Row));
		
		Text.Empty();
		for(i = 2; i <= m_SS.GetSheetCols(); i ++)
		{
			if(!Text.IsEmpty())
				Text += " - ";
			Text += m_SS.GetSheetText(i, Row);
		}
		
		m_DataEdit.SetWindowText(Text);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
}

void CFindDlg::OnChangeFindDataEdit() 
{
	if(m_DataEdit.GetModify())
	{
		Find();
		m_DataEdit.SetModify(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
}

void CFindDlg::OnChangeFindKeyEdit() 
{
	if(m_KeyEdit.GetModify())
	{
		Find();
		m_KeyEdit.SetModify(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
}

void CFindDlg::OnOK() 
{
	m_KeyEdit.GetWindowText(m_Key);
	CDialog::OnOK();
}
