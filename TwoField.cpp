// TwoField.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "TwoField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTwoField dialog


CTwoField::CTwoField(CWnd* pParent /*=NULL*/)
	: CDialog(CTwoField::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTwoField)
	//}}AFX_DATA_INIT
}


void CTwoField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTwoField)
	DDX_Control(pDX, IDC_TWO_DATA_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTwoField, CDialog)
	//{{AFX_MSG_MAP(CTwoField)
	ON_EN_CHANGE(IDC_TWO_KEY_EDIT, OnChangeTwoKeyEdit)
	ON_EN_CHANGE(IDC_TWO_DATA_EDIT, OnChangeTwoDataEdit)
	ON_BN_CLICKED(IDC_TWO_FIND_BUTTON, OnTwoFindButton)
	ON_BN_CLICKED(IDC_TWO_ADD_BUTTON, OnTwoAddButton)
	ON_BN_CLICKED(IDC_TWO_DELETE_BUTTON, OnTwoDeleteButton)
	ON_BN_CLICKED(IDC_TWO_UPDATE_BUTTON, OnTwoUpdateButton)
	ON_BN_CLICKED(IDC_TWO_CLEAR_BUTTON, OnTwoClearButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTwoField message handlers

BEGIN_EVENTSINK_MAP(CTwoField, CDialog)
    //{{AFX_EVENTSINK_MAP(CTwoField)
	ON_EVENT(CTwoField, IDC_TWO_DATA_LIST, 11 /* DblClick */, OnDblClickTwoDataList, VTS_I4 VTS_I4)
	ON_EVENT(CTwoField, IDC_TWO_DATA_LIST, 2 /* BlockSelected */, OnBlockSelectedTwoDataList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CTwoField::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(2);

	m_KeyEdit.Setup(this, IDC_TWO_KEY_EDIT, EMPTYSTRING, UPPERCASE);
	m_DataEdit.Setup(this, IDC_TWO_DATA_EDIT, EMPTYSTRING, UPPERCASE);
	m_Data.Add(&m_KeyEdit, &m_Data.GetKey());
	if(strlen(m_Data.GetKeyName2()) > 0)
		m_Data.Add(&m_DataEdit, &m_Data.GetKey2());
	else
		m_Data.Add(&m_DataEdit);
	m_Data.SetSS(&m_SS);
	m_Data.SetSheetHeaders();

	m_Data.SetLimitText(m_KeyEdit, 0);
	m_Data.SetLimitText(m_DataEdit, 1);

	GetDlgItem(IDC_TWO_KEY_STATIC)->SetWindowText(m_Data.GetKeyName());
	if(m_SS.GetFieldArray().GetSize() >= 2)
		GetDlgItem(IDC_TWO_DATA_STATIC)->SetWindowText(m_SS.GetFieldArray().GetAt(1)->GetName());
	GetDlgItem(IDC_TWO_FIND_BUTTON)->EnableWindow(FALSE);

	if(m_Data.GetPreLoad())
	{
		m_KeyEdit.SetWindowText(PERCENT);
		m_Data.LoadDBData();
	}

	OnTwoClearButton();
	m_bModified = FALSE;
	this->SetWindowText(m_Data.GetTable());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTwoField::OnChangeTwoDataEdit() 
{
	OnChangeTwoKeyEdit();
}

void CTwoField::OnChangeTwoKeyEdit() 
{
	int KeyLen, DataLen;

	KeyLen = m_KeyEdit.GetWindowTextLength();
	DataLen = m_DataEdit.GetWindowTextLength();

	GetDlgItem(IDC_TWO_DELETE_BUTTON)->EnableWindow(KeyLen > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_TWO_FIND_BUTTON)->EnableWindow(KeyLen > 0 || DataLen > 0 ? TRUE : FALSE);
	if(KeyLen > 0)
	{
		GetDlgItem(IDC_TWO_ADD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_TWO_UPDATE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_TWO_ADD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_TWO_UPDATE_BUTTON)->EnableWindow(FALSE);
	}
}

void CTwoField::OnTwoFindButton() 
{
	m_Data.LoadDBData();
}

void CTwoField::OnTwoClearButton() 
{
	m_Data.Refresh();

	GetDlgItem(IDC_TWO_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_TWO_UPDATE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_TWO_DELETE_BUTTON)->EnableWindow(FALSE);
}

void CTwoField::OnTwoAddButton() 
{
	m_Data.AddRow();
	m_bModified = TRUE;
}

void CTwoField::OnTwoDeleteButton() 
{
	m_Data.DeleteRow();
	m_bModified = TRUE;
}

void CTwoField::OnTwoUpdateButton() 
{
	m_Data.UpdateRow();
	m_bModified = TRUE;
}

void CTwoField::OnDblClickTwoDataList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	
	GetDlgItem(IDC_TWO_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_TWO_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

void CTwoField::OnBlockSelectedTwoDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickTwoDataList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}
