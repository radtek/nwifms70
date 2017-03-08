// OtherMaint.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "OtherMaint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherMaint dialog


COtherMaint::COtherMaint(CWnd* pParent /*=NULL*/)
	: CDialog(COtherMaint::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherMaint)
	m_Table = _T("");
	m_TableSpace = _T("");
	//}}AFX_DATA_INIT
}


void COtherMaint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherMaint)
	DDX_Control(pDX, IDC_MAINT_OTHER_TABLE_LIST, m_SS);
	DDX_Text(pDX, IDC_MAINT_OTHER_TABLE_EDIT, m_Table);
	DDX_Text(pDX, IDC_MAINT_OTHER_TABLESPACE_EDIT, m_TableSpace);
	//}}AFX_DATA_MAP
}

CString &COtherMaint::GetFields()
{
	UpdateData();

	m_OraLoader.GetSql().Format("SELECT * FROM %s.%s", (LPCTSTR) m_TableSpace, (LPCTSTR) m_Table);
	m_OraLoader.Open();
	m_Fields.Format("%d", m_OraLoader.GetFieldArray().GetSize());

	return m_Fields;
}

void COtherMaint::GetKeys(LPCTSTR Keys)
{
	char Key[2048], *p, *q;
	int i = 0;
	
	strcpy_s(Key, Keys);
	p = strtok_s(Key, "|", &q);
	while(p)
	{
		switch(i)
		{
			case 0:
				m_Key = p;
				break;
			case 1:
				m_Key2 = p;
				break;
			default:
				break;
		}

		i++;
		p = strtok_s(NULL, "|", &q);
	}
}

BEGIN_MESSAGE_MAP(COtherMaint, CDialog)
	//{{AFX_MSG_MAP(COtherMaint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherMaint message handlers

BEGIN_EVENTSINK_MAP(COtherMaint, CDialog)
    //{{AFX_EVENTSINK_MAP(COtherMaint)
	ON_EVENT(COtherMaint, IDC_MAINT_OTHER_TABLE_LIST, 11 /* DblClick */, OnDblClickMaintOtherTableList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL COtherMaint::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SS.SetVisibleRows(16);
	m_SS.SetVisibleCols(4);

	if(m_OraLoader.Open("SELECT TABLE_SPACE, TABLE_NAME, KEY, PRELOAD FROM SEMAM.NW_MAINTENANCE "
						"WHERE KEY IS NOT NULL ORDER BY TABLE_NAME "))
		m_OraLoader.LoadDBSheet(m_SS);
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COtherMaint::OnDblClickMaintOtherTableList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		CString Key;
		GetDlgItem(IDC_MAINT_OTHER_TABLESPACE_EDIT)->SetWindowText(m_SS.GetSheetText(1, Row));
		GetDlgItem(IDC_MAINT_OTHER_TABLE_EDIT)->SetWindowText(m_SS.GetSheetText(2, Row));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		Key = m_SS.GetSheetText(3, Row);
		m_PreLoad = m_SS.GetSheetText(4, Row);
		GetKeys(Key);
		GetFields();
	}
}

