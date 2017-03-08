// FourField.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "FourField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFourField dialog


CFourField::CFourField(CWnd* pParent /*=NULL*/)
	: CDialog(CFourField::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFourField)
	//}}AFX_DATA_INIT
}


void CFourField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFourField)
	DDX_Control(pDX, IDC_FOUR_DATA_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFourField, CDialog)
	//{{AFX_MSG_MAP(CFourField)
	ON_BN_CLICKED(IDC_FOUR_ADD_BUTTON, OnFourAddButton)
	ON_BN_CLICKED(IDC_FOUR_CLEAR_BUTTON, OnFourClearButton)
	ON_BN_CLICKED(IDC_FOUR_DELETE_BUTTON, OnFourDeleteButton)
	ON_BN_CLICKED(IDC_FOUR_FIND_BUTTON, OnFourFindButton)
	ON_BN_CLICKED(IDC_FOUR_UPDATE_BUTTON, OnFourUpdateButton)
	ON_EN_CHANGE(IDC_FOUR_DATA_EDIT, OnChangeFourDataEdit)
	ON_EN_CHANGE(IDC_FOUR_DATA2_EDIT, OnChangeFourData2Edit)
	ON_EN_CHANGE(IDC_FOUR_DATA3_EDIT, OnChangeFourData3Edit)
	ON_EN_CHANGE(IDC_FOUR_KEY_EDIT, OnChangeFourKeyEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFourField message handlers

BEGIN_EVENTSINK_MAP(CFourField, CDialog)
    //{{AFX_EVENTSINK_MAP(CFourField)
	ON_EVENT(CFourField, IDC_FOUR_DATA_LIST, 2 /* BlockSelected */, OnBlockSelectedFourDataList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CFourField, IDC_FOUR_DATA_LIST, 11 /* DblClick */, OnDblClickFourDataList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CFourField::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(4);

	m_KeyEdit.Setup(this, IDC_FOUR_KEY_EDIT, EMPTYSTRING, UPPERCASE);
	m_DataEdit.Setup(this, IDC_FOUR_DATA_EDIT, EMPTYSTRING, UPPERCASE);
	m_Data2Edit.Setup(this, IDC_FOUR_DATA2_EDIT, EMPTYSTRING, UPPERCASE);
	m_Data3Edit.Setup(this, IDC_FOUR_DATA3_EDIT, EMPTYSTRING, UPPERCASE);
	m_Data.Add(&m_KeyEdit, &m_Data.GetKey());
	if(strlen(m_Data.GetKeyName2()) > 0)
		m_Data.Add(&m_DataEdit, &m_Data.GetKey2());
	else
		m_Data.Add(&m_DataEdit);
	m_Data.Add(&m_Data2Edit);
	m_Data.Add(&m_Data3Edit);
	m_Data.SetSS(&m_SS);
	m_Data.SetSheetHeaders();

	m_Data.SetLimitText(m_KeyEdit, 0);
	m_Data.SetLimitText(m_DataEdit, 1);
	m_Data.SetLimitText(m_Data2Edit, 2);
	m_Data.SetLimitText(m_Data3Edit, 3);

	GetDlgItem(IDC_FOUR_KEY_STATIC)->SetWindowText(m_Data.GetKeyName());
	if(m_SS.GetFieldArray().GetSize() >= 4)
	{
		GetDlgItem(IDC_FOUR_DATA_STATIC)->SetWindowText(m_SS.GetFieldArray().GetAt(1)->GetName());
		GetDlgItem(IDC_FOUR_DATA2_STATIC)->SetWindowText(m_SS.GetFieldArray().GetAt(2)->GetName());
		GetDlgItem(IDC_FOUR_DATA3_STATIC)->SetWindowText(m_SS.GetFieldArray().GetAt(3)->GetName());
	}

	GetDlgItem(IDC_FOUR_FIND_BUTTON)->EnableWindow(FALSE);
	
	if(m_Data.GetPreLoad())
	{
		m_KeyEdit.SetWindowText(PERCENT);
		m_Data.LoadDBData();
	}

	OnFourClearButton();
	m_bModified = FALSE;
	this->SetWindowText(m_Data.GetTable());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFourField::OnFourAddButton() 
{
	if(m_Data.AddRow())
	{
		m_Data.Refresh();
		m_bModified = TRUE;
	}
}

void CFourField::OnFourClearButton() 
{
	m_Data.Refresh();

	GetDlgItem(IDC_FOUR_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_FOUR_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_FOUR_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void CFourField::OnFourDeleteButton() 
{
	m_Data.DeleteRow();
	m_bModified = TRUE;
}

void CFourField::OnFourFindButton() 
{
	m_Data.LoadDBData();
}

void CFourField::OnFourUpdateButton() 
{
	m_Data.UpdateRow();
	m_bModified = TRUE;
}

void CFourField::OnBlockSelectedFourDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickFourDataList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CFourField::OnDblClickFourDataList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	
	GetDlgItem(IDC_FOUR_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_FOUR_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

void CFourField::OnChangeFourDataEdit() 
{
	OnChangeFourKeyEdit();
}

void CFourField::OnChangeFourData2Edit() 
{
	OnChangeFourKeyEdit();	
}

void CFourField::OnChangeFourData3Edit() 
{
	OnChangeFourKeyEdit();
}

void CFourField::OnChangeFourKeyEdit() 
{
	int KeyLen, DataLen, Data2Len, Data3Len;

	KeyLen = m_KeyEdit.GetWindowTextLength();
	DataLen = m_DataEdit.GetWindowTextLength();
	Data2Len = m_Data2Edit.GetWindowTextLength();
	Data3Len = m_Data3Edit.GetWindowTextLength();

	GetDlgItem(IDC_FOUR_DELETE_BUTTON)->EnableWindow(KeyLen > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_FOUR_FIND_BUTTON)->EnableWindow(KeyLen > 0 || DataLen > 0 || Data2Len > 0 || Data3Len > 0 ? TRUE : FALSE);
	if(KeyLen > 0)
	{
		GetDlgItem(IDC_FOUR_ADD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_FOUR_UPDATE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_FOUR_ADD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_FOUR_UPDATE_BUTTON)->EnableWindow(FALSE);
	}
}
