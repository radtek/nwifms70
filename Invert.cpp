// Invert.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "Invert.h"
#include "qdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvert dialog


CInvert::CInvert(CWnd* pParent /*=NULL*/)
	: CDialog(CInvert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInvert)
	m_New = _T("");
	m_Origin = _T("");
	//}}AFX_DATA_INIT
}


void CInvert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvert)
	DDX_Text(pDX, IDC_INVERT_NEW_EDIT, m_New);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInvert, CDialog)
	//{{AFX_MSG_MAP(CInvert)
	ON_EN_CHANGE(IDC_INVERT_ORIGIN_EDIT, OnChangeInvertOriginEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvert message handlers
BOOL CInvert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_OriginEdit.Setup(this, IDC_INVERT_ORIGIN_EDIT);
	m_OriginEdit.SetWindowText(m_Origin);
	m_OriginEdit.SetModify(TRUE);
	OnChangeInvertOriginEdit();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInvert::OnChangeInvertOriginEdit() 
{
	if(m_OriginEdit.GetModify())
	{
		CString Text;
		CQData QData;
		double Origin;
		
		m_OriginEdit.GetWindowText(Text);
		Origin = atof(QData.RemoveComma(Text));
		if(Origin != 0)
			Text = QData.WriteNumber(1/Origin);
		else
			Text.Empty();
		
		GetDlgItem(IDC_INVERT_NEW_EDIT)->SetWindowText(Text);
		m_OriginEdit.SetModify(FALSE);
	}
}
