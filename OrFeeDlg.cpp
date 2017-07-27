// OrFeeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "OrFeeDlg.h"
#include "afxdialogex.h"
#include "QData.h"


// COrFeeDlg dialog

COrFeeDlg::COrFeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrFeeDlg::IDD, pParent)
{

}

COrFeeDlg::~COrFeeDlg()
{
}

void COrFeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORFEE_LIST, m_SS);
}

void COrFeeDlg::LoadData()
{
	m_SS.ClearSheet();

	m_OraLoader.Open("SELECT PB_NAME, CP, LOW_LIMIT, HIGH_LIMIT, FROM_DATE, TO_DATE, OR_FEE, OCC_FEE, ROWIDTOCHAR(ROWID) \"ID\" FROM LIN.NW_OR_FEE ORDER BY 1, 5 DESC ");

	m_OraLoader.LoadDBSheet(m_SS, TRUE);	
}

void COrFeeDlg::UpdateData(BOOL bAdd)
{
	CString Text;
	CDBRec Rec;
	CQData QData;

	Text = m_FromDate.GetData();
	if(Text.IsEmpty())
	{
		MessageBox("Invalid From Date");
		return;
	}

	if(!m_OrFee.GetData().IsEmpty())
	{
		MessageBox("Invalid Fee");
		return;
	}

	Rec.Add(m_PB.GetData());
	Rec.Add(m_CP.GetData());
	Rec.Add(m_LowLimit.GetData());
	Rec.Add(m_HighLimit.GetData());
	Rec.Add(Text);
	Rec.Add(m_ToDate.GetData());
	Rec.Add(m_OrFee.GetData());
	Rec.Add(m_OCCFee.GetData());

	if(bAdd)
	{
		m_OraLoader.Open("SELECT PB_NAME, CP, LOW_LIMIT, HIGH_LIMIT, FROM_DATE, TO_DATE, OR_FEE, OCC_FEE FROM LIN.NW_OR_FEE ", ODYNASET_DEFAULT);
		m_OraLoader.UpdateRecord(Rec, bAdd);
	}
	else
	{
		Text = QData.GetQueryText(m_ID);
		if(m_OraLoader.Open("SELECT PB_NAME, CP, LOW_LIMIT, HIGH_LIMIT, FROM_DATE, TO_DATE, OR_FEE, OCC_FEE FROM LIN.NW_OR_FEE " 
							"WHERE ROWIDTOCHAR(ROWID) = " + Text, ODYNASET_DEFAULT))
			m_OraLoader.UpdateRecord(Rec);
	}

	LoadData();
}

BEGIN_MESSAGE_MAP(COrFeeDlg, CDialog)
	//{{AFX_MSG_MAP(CORFee)
	ON_EN_CHANGE(IDC_ORFEE_FROM_EDIT, OnEnChangeOrfeeFromEdit)
	ON_EN_CHANGE(IDC_ORFEE_TO_EDIT, OnEnChangeOrfeeToEdit)
	ON_EN_CHANGE(IDC_ORFEE_ORFEE_EDIT, OnChangeOrFeeEdit)
	ON_BN_CLICKED(IDC_ORFEE_ADD_BUTTON, OnBnClickedOrfeeAddButton)
	ON_BN_CLICKED(IDC_ORFEE_UPDATE_BUTTON, OnBnClickedOrfeeUpdateButton)
	ON_BN_CLICKED(IDC_ORFEE_DELETE_BUTTON, OnBnClickedOrfeeDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// COrFeeDlg message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL COrFeeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_PB.Setup(this, IDC_ORFEE_PB_COMBO, TRUE);
	m_CP.Setup(this, IDC_ORFEE_CP_COMBO, TRUE);
	m_LowLimit.Setup(this, IDC_ORFEE_LOWLIMIT_EDIT);
	m_HighLimit.Setup(this, IDC_ORFEE_HIGHLIMIT_EDIT);
	m_FromDate.Setup(this, IDC_ORFEE_FROM_EDIT);
	m_ToDate.Setup(this, IDC_ORFEE_TO_EDIT);
	m_OrFee.Setup(this, IDC_ORFEE_ORFEE_EDIT);
	m_OCCFee.Setup(this, IDC_ORFEE_OCCFEE_EDIT);

	m_Data->LoadSupportData(FALSE);
	m_Data->GetContactList().CopyKeyToComboBox(m_PB);
	m_Data->GetContactList().CopyKeyToComboBox(m_CP);

	SetWindowText("OR FEE");

	LoadData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrFeeDlg::OnEnChangeOrfeeFromEdit()
{
	GetDlgItem(IDC_ORFEE_ADD_BUTTON)->EnableWindow(!m_FromDate.GetData().IsEmpty() && !m_OrFee.GetData().IsEmpty());
	GetDlgItem(IDC_ORFEE_UPDATE_BUTTON)->EnableWindow(!m_FromDate.GetData().IsEmpty() && !m_OrFee.GetData().IsEmpty());
}

void COrFeeDlg::OnEnChangeOrfeeToEdit()
{
	OnEnChangeOrfeeFromEdit();
}

void COrFeeDlg::OnChangeOrFeeEdit() 
{
	OnEnChangeOrfeeFromEdit();
}

void COrFeeDlg::OnBnClickedOrfeeAddButton()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ORFEE_ADD_BUTTON)->EnableWindow(FALSE);
}

void COrFeeDlg::OnBnClickedOrfeeUpdateButton()
{
	UpdateData(FALSE);
	GetDlgItem(IDC_ORFEE_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void COrFeeDlg::OnBnClickedOrfeeDeleteButton()
{
	if(m_ID.IsEmpty())
		return;

	CQData QData;
	CString Text;

	Text = QData.GetQueryText(m_ID);
	GetOraLoader().ExecuteSql("DELETE FROM LIN.NW_OR_FEE WHERE ROWIDTOCHAR(ROWID) = " + Text);
	LoadData();

	GetDlgItem(IDC_ORFEE_DELETE_BUTTON)->EnableWindow(FALSE);
}

BEGIN_EVENTSINK_MAP(COrFeeDlg, CDialog)
	ON_EVENT(COrFeeDlg, IDC_ORFEE_LIST, 11, OnDblClickOrfeeList, VTS_I4 VTS_I4)
	ON_EVENT(COrFeeDlg, IDC_ORFEE_LIST, 2, OnBlockSelectedOrfeeList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void COrFeeDlg::OnDblClickOrfeeList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);
		m_PB.SetData(m_SS.GetSheetText(1, Row));
		m_CP.SetData(m_SS.GetSheetText(2, Row));
		m_LowLimit.SetData(m_SS.GetSheetText(3, Row));
		m_HighLimit.SetData(m_SS.GetSheetText(4, Row));
		m_FromDate.SetData(m_SS.GetSheetText(5, Row));
		m_ToDate.SetData(m_SS.GetSheetText(6, Row));
		m_OrFee.SetData(m_SS.GetSheetText(7, Row));
		m_OCCFee.SetData(m_SS.GetSheetText(8, Row));
		m_ID = m_SS.GetSheetText(9, Row);
	}
}

void COrFeeDlg::OnBlockSelectedOrfeeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickOrfeeList(BlockCol, BlockRow);
	GetDlgItem(IDC_ORFEE_DELETE_BUTTON)->EnableWindow(TRUE);
}
