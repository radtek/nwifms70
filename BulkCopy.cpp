// BulkCopy.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "BulkCopy.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBulkCopy dialog


CBulkCopy::CBulkCopy(CWnd* pParent /*=NULL*/)
	: CDialog(CBulkCopy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBulkCopy)
	m_Owner = NULL;
	m_Table = NULL;
	m_AvailCol = NULL;
	m_SelCol = NULL;
	//}}AFX_DATA_INIT
}


CBulkCopy::~CBulkCopy()
{
	if(m_Owner)
		delete m_Owner;
	if(m_Table)
		delete m_Table;
	if(m_AvailCol)
		delete m_AvailCol;
	if(m_SelCol)
		delete m_SelCol;
}


void CBulkCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBulkCopy)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBulkCopy, CDialog)
	//{{AFX_MSG_MAP(CBulkCopy)
	ON_BN_CLICKED(IDC_BC_ADD_ALL_BUTTON, OnBcAddAllButton)
	ON_BN_CLICKED(IDC_BC_ADD_BUTTON, OnBcAddButton)
	ON_BN_CLICKED(IDC_BC_DEL_BUTTON, OnBcDelButton)
	ON_BN_CLICKED(IDC_BC_DELALL_BUTTON, OnBcDelallButton)
	ON_LBN_SELCHANGE(IDC_BC_AVAIL_LIST, OnSelchangeBcAvailList)
	ON_LBN_DBLCLK(IDC_BC_AVAIL_LIST, OnDblclkBcAvailList)
	ON_CBN_SELCHANGE(IDC_BC_OWNER_COMBO, OnSelchangeBcOwnerCombo)
	ON_LBN_SELCHANGE(IDC_BC_SEL_LIST, OnSelchangeBcSelList)
	ON_LBN_DBLCLK(IDC_BC_SEL_LIST, OnDblclkBcSelList)
	ON_BN_CLICKED(IDC_BC_RUN_BUTTON, OnBcRunButton)
	ON_CBN_SELCHANGE(IDC_BC_TABLE_COMBO, OnSelchangeBcTableCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBulkCopy message handlers

BOOL CBulkCopy::OnInitDialog() 
{
	CString Path;
	LPCTSTR Template = "Template";
	LPCTSTR TXT = "TXT";
	CQData QData;	
			
	CDialog::OnInitDialog();

	m_Source = "Template Files (*.TXT) || ";
	// Get the user name and host from the private profile	
	if(!QData.GetFileName(AfxGetApp(), m_Source, Template, "Source Data File", TXT))
		return FALSE;

	GetDlgItem(IDC_BC_SOURCE_EDIT)->SetWindowText(m_Source);

	m_Owner = new COptComboBox(this, IDC_BC_OWNER_COMBO);
	m_Table = new COptComboBox(this, IDC_BC_TABLE_COMBO);
	m_AvailCol = new CHLBox(this, IDC_BC_AVAIL_LIST);
	m_SelCol = new CHLBox(this, IDC_BC_SEL_LIST);

	((CButton *) GetDlgItem(IDC_BC_APPEND_RADIO))->SetCheck(1);
	m_pOraLoader->Open("SELECT USERNAME FROM SYS.ALL_USERS "
						"WHERE USERNAME IN ('SEMAM', USER) ");
	m_pOraLoader->LoadComboBox(*m_Owner);

	EnableButtons();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBulkCopy::OnBcAddButton() 
{
	AddSelItem(m_AvailCol, m_SelCol);
}

void CBulkCopy::OnBcAddAllButton() 
{
	for(int i = 0; i < m_AvailCol->GetCount(); i ++)
	{
		m_AvailCol->SetCurSel(i);
		AddSelItem(m_AvailCol, m_SelCol);
	}
}

void CBulkCopy::OnBcDelButton() 
{
	DelSelItem(m_SelCol);
}

void CBulkCopy::OnBcDelallButton() 
{
	m_SelCol->ResetContent();
}

void CBulkCopy::OnSelchangeBcAvailList() 
{
	EnableButtons();
}

void CBulkCopy::OnDblclkBcAvailList() 
{
	OnBcAddButton();
}

void CBulkCopy::OnSelchangeBcOwnerCombo() 
{
	CString Owner;

	m_Owner->GetSelString(Owner);
	if(Owner.IsEmpty())
		return;
	
	BeginWaitCursor();
	m_pOraLoader->GetSql().Format("SELECT DISTINCT TABLE_NAME FROM SYS.ALL_TABLES WHERE OWNER = '%s' "
								"AND TABLE_NAME LIKE 'NW_%%' ", (LPCTSTR ) Owner);
	m_pOraLoader->Open();
	m_Table->ResetContent();
	m_pOraLoader->LoadComboBox(*m_Table);
	if(m_Table->GetCount() == 1)
		m_Table->SetCurSel(0);
	EndWaitCursor();
}

void CBulkCopy::OnSelchangeBcTableCombo() 
{
	CString Owner, Table;

	m_Owner->GetSelString(Owner);
	m_Table->GetSelString(Table);
	if(Owner.IsEmpty() || Table.IsEmpty())
		return;

	m_pOraLoader->GetSql().Format("SELECT COLUMN_NAME FROM SYS.ALL_TAB_COLUMNS "
								"WHERE OWNER = '%s' AND TABLE_NAME = '%s' ORDER BY COLUMN_ID ", 
								(LPCTSTR ) Owner, (LPCTSTR ) Table);

	m_pOraLoader->Open();
	m_AvailCol->ResetContent();
	m_pOraLoader->LoadListBox(*m_AvailCol);
	if(m_AvailCol->GetCount() == 1)
		m_AvailCol->SetCurSel(0);
	
	m_SelCol->ResetContent();
	EnableButtons();
}

void CBulkCopy::OnSelchangeBcSelList() 
{
	EnableButtons();
}

void CBulkCopy::OnDblclkBcSelList() 
{
	DelSelItem(m_SelCol);
}

void CBulkCopy::OnBcRunButton() 
{
	CString DelSql, Msg, Sql, Owner, Table;

	m_Owner->GetSelString(Owner);
	m_Table->GetSelString(Table);

	if(Owner.IsEmpty() || Table.IsEmpty())
		return;
	
	if(GetCheckedRadioButton(IDC_BC_APPEND_RADIO, IDC_BC_OVERWRITE_RADIO) == IDC_BC_APPEND_RADIO)
	{
		DelSql.Empty();
		Msg = "Are you sure you want to append data from "+m_Source+" file to " + Table + "?";
	}
	else
	{
		DelSql = "DELETE FROM " + Owner + "." + Table;
		Msg = "Are you sure you want to replace data from "+Table+" with the data from "+m_Source+" file?";
	}
	
	if(m_SelCol->GetCount() == 0)
		GetSqlSelColumns(Sql, m_AvailCol);
	else
		GetSqlSelColumns(Sql, m_SelCol);
	Sql += " FROM " + Owner + "." + Table;
	
	if(MessageBox(Msg, "Bulk Copy", MB_YESNO) != IDYES)
		return;
	
	BeginWaitCursor();
	ProcessData(DelSql, Sql);
	EndWaitCursor();
}

void CBulkCopy::GetSqlSelColumns(CString &Sql, CHLBox *SelCols)
{
	int i;
	CString Text;

	Sql.Empty();
	for(i = 0; i < SelCols->GetCount(); i ++)
	{
		SelCols->GetText(i, Text);
		if(Sql.IsEmpty())
			Sql += "SELECT " + Text;
		else
			Sql += ", " + Text;
	}
}

void CBulkCopy::ProcessData(CString& DelSql, CString& Sql)
{
	CStdioFile InFile;
	CFileException e;
	CString Buffer;
	CDBRec Rec;
	char Text[2048], *p, *q;

	if(!DelSql.IsEmpty())
		m_pOraLoader->ExecuteSql(DelSql);
	
	m_pOraLoader->Open(Sql, ODYNASET_DEFAULT);

	if(!InFile.Open(m_Source, CFile::modeRead, &e))
	{
		e.GetErrorMessage(Text, sizeof(Text) - 1);
		MessageBox(Text, "File could not be opened ", MB_OK);
		return;
	}

	InFile.ReadString(Text, sizeof(Text));
	InFile.ReadString(Text, sizeof(Text));
	while(InFile.ReadString(Text, sizeof(Text)))
	{
		Rec.RemoveAll();
		p = Text;
		while (p && Rec.GetSize() < m_pOraLoader->GetFieldCount())
		{
			q = strstr(p, TAB);
			if(q)
				q[0] = 0;
			Buffer = p;
			Buffer = Buffer.SpanExcluding("\"");
			Buffer.TrimLeft();
			Buffer.TrimRight();
			Rec.Add(Buffer);
			if(q)
				p = q+1;
			else
				p = 0;
		}
	
		if(Rec.GetSize() >= m_pOraLoader->GetFieldCount())	
			m_pOraLoader->UpdateRecord(Rec, TRUE);
	}
}

void CBulkCopy::AddSelItem(CHLBox *Avail, CHLBox *Sel)
{
	CString Text;
	
	Avail->GetSelString(Text);
	if(Text.IsEmpty())
		return;
	if(Sel->FindStringExact(-1, Text) < 0)
		Sel->AddString(Text);
}

void CBulkCopy::DelSelItem(CHLBox *Sel)
{
	int Index;

	Index = Sel->GetCurSel();
	if(Index >= 0)
		Sel->DeleteString(Index);
}

void CBulkCopy::EnableButtons()
{
	GetDlgItem(IDC_BC_RUN_BUTTON)->EnableWindow(m_Owner->GetCurSel() >= 0 || 
												m_Table->GetCurSel() >= 0 ? TRUE : FALSE);
	GetDlgItem(IDC_BC_ADD_BUTTON)->EnableWindow(m_AvailCol->GetCurSel() >= 0 ? TRUE : FALSE);
	GetDlgItem(IDC_BC_DEL_BUTTON)->EnableWindow(m_SelCol->GetCurSel() >= 0 ? TRUE : FALSE);
	GetDlgItem(IDC_BC_ADD_ALL_BUTTON)->EnableWindow(m_AvailCol->GetCount() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_BC_DELALL_BUTTON)->EnableWindow(m_SelCol->GetCount() > 0 ? TRUE : FALSE);
}
