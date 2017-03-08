// OneField.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "OneField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COneField dialog


COneField::COneField(CWnd* pParent /*=NULL*/)
	: CDialog(COneField::IDD, pParent)
{
	//{{AFX_DATA_INIT(COneField)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COneField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COneField)
	DDX_Control(pDX, IDC_ONE_DATA_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COneField, CDialog)
	//{{AFX_MSG_MAP(COneField)
	ON_EN_CHANGE(IDC_ONE_KEY_EDIT, OnChangeOneKeyEdit)
	ON_BN_CLICKED(IDC_ONE_FIND_BUTTON, OnOneFindButton)
	ON_BN_CLICKED(IDC_ONE_CLEAR_BUTTON, OnOneClearButton)
	ON_BN_CLICKED(IDC_ONE_ADD_BUTTON, OnOneAddButton)
	ON_BN_CLICKED(IDC_ONE_DELETE_BUTTON, OnOneDeleteButton)
	ON_BN_CLICKED(IDC_ONE_UPDATE_BUTTON, OnOneUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COneField message handlers

BEGIN_EVENTSINK_MAP(COneField, CDialog)
    //{{AFX_EVENTSINK_MAP(COneField)
	ON_EVENT(COneField, IDC_ONE_DATA_LIST, 11 /* DblClick */, OnDblClickOneDataList, VTS_I4 VTS_I4)
	ON_EVENT(COneField, IDC_ONE_DATA_LIST, 2 /* BlockSelected */, OnBlockSelectedOneDataList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL COneField::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(1);

	m_KeyEdit.Setup(this, IDC_ONE_KEY_EDIT, EMPTYSTRING, UPPERCASE);

	m_Data.Add(&m_KeyEdit, &m_Data.GetKey());
	m_Data.SetSS(&m_SS);
	m_Data.SetSheetHeaders();

	m_Data.SetLimitText(m_KeyEdit, 0);

	GetDlgItem(IDC_ONE_KEY_STATIC)->SetWindowText(m_Data.GetKeyName());
	
	if(m_Data.GetPreLoad())
	{
		m_KeyEdit.SetWindowText(PERCENT);
		m_Data.LoadDBData();
	}

	OnOneClearButton();
	m_bModified = FALSE;
	this->SetWindowText(m_Data.GetTable());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COneField::OnChangeOneKeyEdit() 
{
	if(m_KeyEdit.GetWindowTextLength() > 0)
	{
		GetDlgItem(IDC_ONE_FIND_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_ONE_ADD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_ONE_DELETE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_ONE_UPDATE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ONE_FIND_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_ONE_ADD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_ONE_DELETE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_ONE_UPDATE_BUTTON)->EnableWindow(FALSE);
	}
}

void COneField::OnOneFindButton() 
{
	m_Data.LoadDBData();
}

void COneField::OnOneClearButton() 
{
	m_Data.Refresh();

	GetDlgItem(IDC_ONE_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ONE_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ONE_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void COneField::OnOneAddButton() 
{
	m_Data.AddRow();
	m_bModified = TRUE;
}

void COneField::OnOneDeleteButton() 
{
	m_Data.DeleteRow();
	m_bModified = TRUE;
}

void COneField::OnOneUpdateButton() 
{
	m_Data.UpdateRow();
	m_bModified = TRUE;
}

void COneField::OnDblClickOneDataList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	GetDlgItem(IDC_ONE_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_ONE_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

void COneField::OnBlockSelectedOneDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickOneDataList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}