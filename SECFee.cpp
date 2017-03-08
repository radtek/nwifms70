// SECFee.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "SECFee.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSECFee dialog


CSECFee::CSECFee(CWnd* pParent /*=NULL*/)
	: CDialog(CSECFee::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSECFee)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSECFee::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSECFee)
	DDX_Control(pDX, IDC_SECFEE_LIST, m_SS);
	//}}AFX_DATA_MAP
}

void CSECFee::LoadData()
{
	m_SS.ClearSheet();

	if(m_bSecFee)
		m_OraLoader.Open("SELECT FROM_DATE, TO_DATE, SEC_FEE FROM SEMAM.NW_SEC_FEE ORDER BY 1 ");
	else
		m_OraLoader.Open("SELECT FROM_DATE, TO_DATE, OR_FEE FROM SEMAM.NW_OR_FEE ORDER BY 1 ");

	m_OraLoader.LoadDBSheet(m_SS, TRUE);
}

void CSECFee::UpdateData(BOOL bAdd)
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

	Rec.Add(Text);
	Rec.Add(m_ToDate.GetData());

	m_Fee.GetWindowText(Text);
	if(!Text.IsEmpty())
	{
		MessageBox("Invalid Fee");
		return;
	}
	Rec.Add(Text);

	if(bAdd)
	{
		if(m_bSecFee)
			m_OraLoader.Open("SELECT FROM_DATE, TO_DATE, SEC_FEE FROM SEMAM.NW_SEC_FEE ");
		else
			m_OraLoader.Open("SELECT FROM_DATE, TO_DATE, OR_FEE FROM SEMAM.NW_OR_FEE ");
		m_OraLoader.UpdateRecord(Rec, bAdd);
	}
	else
	{
		if(m_bSecFee)
			m_OraLoader.GetSql() = "SELECT FROM_DATE, TO_DATE, SEC_FEE FROM SEMAM.NW_SEC_FEE " 
									"WHERE FROM_DATE = ";
		else
			m_OraLoader.GetSql() = "SELECT FROM_DATE, TO_DATE, OR_FEE FROM SEMAM.NW_OR_FEE " 
									"WHERE FROM_DATE = ";
		m_OraLoader.GetSql() += QData.GetQueryDate(Rec.GetAt(0));
		
		if(m_OraLoader.Open(m_OraLoader.GetSql(), ODYNASET_DEFAULT))
			m_OraLoader.UpdateRecord(Rec);
	}

	LoadData();
}

BEGIN_MESSAGE_MAP(CSECFee, CDialog)
	//{{AFX_MSG_MAP(CSECFee)
	ON_EN_CHANGE(IDC_SECFEE_FROM_EDIT, &CSECFee::OnEnChangeSecfeeFromEdit)
	ON_EN_CHANGE(IDC_SECFEE_TO_EDIT, &CSECFee::OnEnChangeSecfeeToEdit)
	ON_EN_CHANGE(IDC_SECFEE_FEE_EDIT, OnChangeSecFeeEdit)
	ON_BN_CLICKED(IDC_SECFEE_ADD_BUTTON, &CSECFee::OnBnClickedSecfeeAddButton)
	ON_BN_CLICKED(IDC_SECFEE_UPDATE_BUTTON, &CSECFee::OnBnClickedSecfeeUpdateButton)
	ON_BN_CLICKED(IDC_SECFEE_DELETE_BUTTON, &CSECFee::OnBnClickedSecfeeDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSECFee message handlers

BOOL CSECFee::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	
	m_FromDate.Setup(this, IDC_SECFEE_FROM_EDIT);
	m_ToDate.Setup(this, IDC_SECFEE_TO_EDIT);
	m_Fee.Setup(this, IDC_SECFEE_FEE_EDIT);

	GetDlgItem(IDC_SECFEE_STATIC)->SetWindowText(m_bSecFee ? "SEC FEE" : "OR FEE");

	LoadData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSECFee::OnEnChangeSecfeeFromEdit()
{
	GetDlgItem(IDC_SECFEE_ADD_BUTTON)->EnableWindow(!m_FromDate.GetData().IsEmpty() && !m_ToDate.GetData().IsEmpty() && m_Fee.GetWindowTextLength() > 0);
	GetDlgItem(IDC_SECFEE_UPDATE_BUTTON)->EnableWindow(!m_FromDate.GetData().IsEmpty() && !m_ToDate.GetData().IsEmpty() && m_Fee.GetWindowTextLength() > 0);
}


void CSECFee::OnEnChangeSecfeeToEdit()
{
	OnEnChangeSecfeeFromEdit();
}

void CSECFee::OnChangeSecFeeEdit() 
{
	OnEnChangeSecfeeFromEdit();
}

void CSECFee::OnBnClickedSecfeeAddButton()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_SECFEE_ADD_BUTTON)->EnableWindow(FALSE);
}


void CSECFee::OnBnClickedSecfeeUpdateButton()
{
	UpdateData(FALSE);
	GetDlgItem(IDC_SECFEE_UPDATE_BUTTON)->EnableWindow(FALSE);
}


void CSECFee::OnBnClickedSecfeeDeleteButton()
{
	CString Text;
	CQData QData;

	Text = m_FromDate.GetData();
	if(Text.IsEmpty())
		return;

	if(m_bSecFee)
		m_OraLoader.GetSql() = "DELETE FROM SEMAM.NW_SEC_FEE WHERE FROM_DATE = ";
	else
		m_OraLoader.GetSql() = "DELETE FROM SEMAM.NW_OR_FEE WHERE FROM_DATE = ";
	m_OraLoader.GetSql() += QData.GetQueryDate(Text);
	m_OraLoader.ExecuteSql();
	LoadData();

	GetDlgItem(IDC_SECFEE_DELETE_BUTTON)->EnableWindow(FALSE);
}


BEGIN_EVENTSINK_MAP(CSECFee, CDialog)
	ON_EVENT(CSECFee, IDC_SECFEE_LIST, 11, CSECFee::OnDblClickSecfeeList, VTS_I4 VTS_I4)
	ON_EVENT(CSECFee, IDC_SECFEE_LIST, 2, CSECFee::OnBlockSelectedSecfeeList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CSECFee::OnDblClickSecfeeList(long Col, long Row)
{
	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		CString Text;
		for(int i = 1; i <= m_SS.GetSheetCols(); i++)
		{
			Text = m_SS.GetSheetText(i, Row);
			switch(i)
			{
				case 1:
					m_FromDate.SetData(Text);
					break;
				case 2:
					m_ToDate.SetData(Text);
					break;
				case 3:
					m_Fee.SetWindowText(Text);
					break;
				default:
					break;
			}
		}
	}
	else
	{
		m_FromDate.SetData(EMPTYSTRING);
		m_ToDate.SetData(EMPTYSTRING);
		m_Fee.SetWindowText(EMPTYSTRING);
	}
}


void CSECFee::OnBlockSelectedSecfeeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickSecfeeList(BlockCol, BlockRow);
	GetDlgItem(IDC_SECFEE_DELETE_BUTTON)->EnableWindow(TRUE);
}


