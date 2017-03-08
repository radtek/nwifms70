// ThreeField.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "ThreeField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThreeField dialog


CThreeField::CThreeField(CWnd* pParent /*=NULL*/)
	: CDialog(CThreeField::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThreeField)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CThreeField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThreeField)
	DDX_Control(pDX, IDC_THREE_DATA_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThreeField, CDialog)
	//{{AFX_MSG_MAP(CThreeField)
	ON_EN_CHANGE(IDC_THREE_KEY_EDIT, OnChangeThreeKeyEdit)
	ON_EN_CHANGE(IDC_THREE_DATA_EDIT, OnChangeThreeDataEdit)
	ON_BN_CLICKED(IDC_THREE_FIND_BUTTON, OnThreeFindButton)
	ON_BN_CLICKED(IDC_THREE_CLEAR_BUTTON, OnThreeClearButton)
	ON_BN_CLICKED(IDC_THREE_ADD_BUTTON, OnThreeAddButton)
	ON_BN_CLICKED(IDC_THREE_DELETE_BUTTON, OnThreeDeleteButton)
	ON_BN_CLICKED(IDC_THREE_UPDATE_BUTTON, OnThreeUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThreeField message handlers

BEGIN_EVENTSINK_MAP(CThreeField, CDialog)
    //{{AFX_EVENTSINK_MAP(CThreeField)
	ON_EVENT(CThreeField, IDC_THREE_DATA_LIST, 11 /* DblClick */, OnDblClickThreeDataList, VTS_I4 VTS_I4)
	ON_EVENT(CThreeField, IDC_THREE_DATA_LIST, 2 /* BlockSelected */, OnBlockSelectedThreeDataList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CThreeField::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(3);

	m_KeyEdit.Setup(this, IDC_THREE_KEY_EDIT, EMPTYSTRING, UPPERCASE);
	m_DataEdit.Setup(this, IDC_THREE_DATA_EDIT, EMPTYSTRING, UPPERCASE);
	m_Data2Edit.Setup(this, IDC_THREE_DATA2_EDIT, EMPTYSTRING, UPPERCASE);
	m_Data.Add(&m_KeyEdit, &m_Data.GetKey());
	if(strlen(m_Data.GetKeyName2()) > 0)
		m_Data.Add(&m_DataEdit, &m_Data.GetKey2());
	else
		m_Data.Add(&m_DataEdit);
	m_Data.Add(&m_Data2Edit);
	m_Data.SetSS(&m_SS);
	m_Data.SetSheetHeaders();

	m_Data.SetLimitText(m_KeyEdit, 0);
	m_Data.SetLimitText(m_DataEdit, 1);
	m_Data.SetLimitText(m_Data2Edit, 2);

	GetDlgItem(IDC_THREE_KEY_STATIC)->SetWindowText(m_Data.GetKeyName());
	if(m_SS.GetFieldArray().GetSize() >= 3)
	{
		GetDlgItem(IDC_THREE_DATA_STATIC)->SetWindowText(m_SS.GetFieldArray().GetAt(1)->GetName());
		GetDlgItem(IDC_THREE_DATA2_STATIC)->SetWindowText(m_SS.GetFieldArray().GetAt(2)->GetName());
	}

	GetDlgItem(IDC_THREE_FIND_BUTTON)->EnableWindow(FALSE);

	if(m_Data.GetPreLoad())
	{
		m_KeyEdit.SetWindowText(PERCENT);
		m_Data.LoadDBData();
	}

	OnThreeClearButton();
	m_bModified = FALSE;
	this->SetWindowText(m_Data.GetTable());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThreeField::OnChangeThreeDataEdit() 
{
	OnChangeThreeKeyEdit();
}

void CThreeField::OnChangeThreeData2Edit()
{
	OnChangeThreeKeyEdit();
}

void CThreeField::OnChangeThreeKeyEdit() 
{
	int KeyLen, DataLen, Data2Len;

	KeyLen = m_KeyEdit.GetWindowTextLength();
	DataLen = m_DataEdit.GetWindowTextLength();
	Data2Len = m_Data2Edit.GetWindowTextLength();

	GetDlgItem(IDC_THREE_DELETE_BUTTON)->EnableWindow(KeyLen > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_THREE_FIND_BUTTON)->EnableWindow(KeyLen > 0 || DataLen > 0 || Data2Len > 0 ? TRUE : FALSE);
	if(KeyLen > 0)
	{
		GetDlgItem(IDC_THREE_ADD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_THREE_UPDATE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_THREE_ADD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_THREE_UPDATE_BUTTON)->EnableWindow(FALSE);
	}
}

void CThreeField::OnThreeFindButton() 
{
	m_Data.LoadDBData();
}

void CThreeField::OnThreeClearButton() 
{
	m_Data.Refresh();
	GetDlgItem(IDC_THREE_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_THREE_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_THREE_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void CThreeField::OnThreeAddButton() 
{
	if(m_Data.AddRow())
	{
		m_Data.Refresh();
		m_bModified = TRUE;
	}
}

void CThreeField::OnThreeDeleteButton() 
{
	m_Data.DeleteRow();
	m_bModified = TRUE;
}

void CThreeField::OnThreeUpdateButton() 
{
	m_Data.UpdateRow();
	m_bModified = TRUE;
}

void CThreeField::OnDblClickThreeDataList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	
	GetDlgItem(IDC_THREE_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_THREE_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

void CThreeField::OnBlockSelectedThreeDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickThreeDataList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}
