// CPFutureCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "CPFutureCommDlg.h"
#include "afxdialogex.h"
#include "qdata.h"

// CCPFutureCommDlg dialog

IMPLEMENT_DYNAMIC(CCPFutureCommDlg, CDialogEx)

CCPFutureCommDlg::CCPFutureCommDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCPFutureCommDlg::IDD, pParent)
{
	m_CP = NULL;
	m_TransType = NULL;
	m_Currency = NULL;
	m_FutureType = NULL;
	m_ETrade = NULL;
}

CCPFutureCommDlg::~CCPFutureCommDlg()
{
	if(m_CP)
		delete m_CP;
	if(m_TransType)
		delete m_TransType;
	if(m_Currency)
		delete m_Currency;
	if(m_FutureType)
		delete m_FutureType;
	if(m_ETrade)
		delete m_ETrade;
}

void CCPFutureCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FC_LIST, m_SS);
}

void CCPFutureCommDlg::EnableCtrls()
{
	GetDlgItem(IDC_FC_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_FC_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BOOL CCPFutureCommDlg::IsOK(int nAction)
{
	CString Text;

	if(nAction != 1)
	{
		if(m_SS.GetSheetCurRow() < 0 || m_SS.GetSheetCurRow() > m_SS.GetSheetRows())
			Text = "No Record selected";
	}

	if(m_CP->GetCurSel() < 0)
		Text = "No counterparty selected";
	if(m_TransType->GetCurSel() < 0)
		Text = "No trans_type selected";
	if(m_Currency->GetCurSel() < 0)
		Text = "No currency selected";
	if(m_FromDate.GetData().IsEmpty())
		Text = "No From Date entered";

	if(nAction != 3) // Delete
	{
		if(m_Commission.GetWindowTextLength() == 0)
			Text = "No commission entered";
	}

	if(Text.IsEmpty())
		return TRUE;
	else
	{
		MessageBox(Text, "Counterparty Future Commission Schedule");
		return FALSE;
	}
}

void CCPFutureCommDlg::RowToControls()
{
	if(m_SS.GetSheetCurRow() > 0 && m_SS.GetSheetCurRow() <= m_SS.GetSheetRows())
	{
		int Row;

		Row = m_SS.GetSheetCurRow();
		m_CP->SelectString(0, m_SS.GetSheetText(1, Row));
		m_TransType->SelectString(0, m_SS.GetSheetText(2, Row));
		m_Currency->SelectString(0, m_SS.GetSheetText(3, Row));
		m_FutureType->SelectString(0, m_SS.GetSheetText(4, Row));
		m_FromDate.SetData(m_SS.GetSheetText(5, Row));
		m_ToDate.SetData(m_SS.GetSheetText(6, Row));
		m_ETrade->SetCheck(m_SS.GetSheetText(7, Row));
		m_Commission.SetWindowText(m_SS.GetSheetText(8, Row));
		m_RowID = m_SS.GetSheetText(9, Row);
	}
}

BEGIN_MESSAGE_MAP(CCPFutureCommDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FC_LOAD_BUTTON, &CCPFutureCommDlg::OnBnClickedFcLoadButton)
	ON_BN_CLICKED(IDC_FC_ADD_BUTTON, &CCPFutureCommDlg::OnBnClickedFcAddButton)
	ON_BN_CLICKED(IDC_FC_UPDATE_BUTTON, &CCPFutureCommDlg::OnBnClickedFcUpdateButton)
	ON_BN_CLICKED(IDC_FC_DELETE_BUTTON, &CCPFutureCommDlg::OnBnClickedFcDeleteButton)
END_MESSAGE_MAP()


// CCPFutureCommDlg message handlers
BEGIN_EVENTSINK_MAP(CCPFutureCommDlg, CDialogEx)
	ON_EVENT(CCPFutureCommDlg, IDC_FC_LIST, 2, CCPFutureCommDlg::BlockSelectedFcList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CCPFutureCommDlg, IDC_FC_LIST, 11, CCPFutureCommDlg::DblClickFcList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CCPFutureCommDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

	m_CP = (COptComboBox*) new COptComboBox(this, IDC_FC_CP_COMBO);
	m_TransType = (COptComboBox*) new COptComboBox(this, IDC_FC_TRANSTYPE_COMBO);
	m_Currency = (COptComboBox*) new COptComboBox(this, IDC_FC_CURRENCY_COMBO);
	m_FutureType = (COptComboBox*) new COptComboBox(this, IDC_FC_FUTURETYPE_COMBO, TRUE);
	m_FromDate.Setup(this, IDC_FC_FROMDATE_EDIT);
	m_ToDate.Setup(this, IDC_FC_TODATE_EDIT);
	m_Commission.Setup(this, IDC_FC_COMMISSION_EDIT);
	m_ETrade = (CCheckBox*) new CCheckBox(this, IDC_FC_ETRADE_CHECK, Y, N);

	BeginWaitCursor();

	m_OraLoader = m_pData->GetOraLoader();
	m_pData->GetContactList().CopyKeyToComboBox(*m_CP);
	m_pData->GetCurrencyArr().CopyToComboBox(*m_Currency);
	m_pData->GetFutureTypeArr().CopyToComboBox(*m_FutureType);
	m_TransType->AddString("OPTION");
	m_TransType->AddString(SECURITIES);

	this->SetWindowText("Counterparty Future Commission Schedule");
	OnBnClickedFcLoadButton();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPFutureCommDlg::BlockSelectedFcList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	m_SS.SetSheetCurRow(BlockRow);
	RowToControls();
	EnableCtrls();
}


void CCPFutureCommDlg::DblClickFcList(long Col, long Row)
{
	m_SS.SetSheetCurRow(Row);
	RowToControls();
	EnableCtrls();
}


void CCPFutureCommDlg::OnBnClickedFcLoadButton()
{
	m_SS.ClearSheet();

	m_OraLoader.Open("SELECT COUNTERPARTY, TRANS_TYPE, CURRENCY, FUTURE_TYPE, FROM_DATE, "
					"TO_DATE, ETRADE, COMMISSION, ROWIDTOCHAR(ROWID) \"ROWID\" "
						"FROM SEMAM.NW_FUTURE_COMM_SCHEDULE ORDER BY 1, 2, 3, 4, 5 ");
	
	m_OraLoader.LoadDBSheet(m_SS, TRUE);

	if(m_SS.GetSheetRows() > 0)
		DblClickFcList(1, 1);
}


void CCPFutureCommDlg::OnBnClickedFcAddButton()
{
	if(!IsOK(1))
		return;

	CDBRec Rec;
	CString Text;

	m_OraLoader.Open("SELECT COUNTERPARTY, TRANS_TYPE, CURRENCY, FUTURE_TYPE, FROM_DATE, "
						"TO_DATE, ETRADE, COMMISSION "
						"FROM SEMAM.NW_FUTURE_COMM_SCHEDULE ", ODYNASET_DEFAULT);
	
	Rec.Add(m_CP->GetSelString(Text));
	Rec.Add(m_TransType->GetSelString(Text));
	Rec.Add(m_Currency->GetSelString(Text));
	Rec.Add(m_FutureType->GetSelString(Text));
	Rec.Add(m_FromDate.GetData());
	Rec.Add(m_ToDate.GetData());
	Rec.Add(m_ETrade->GetCheckString());
	m_Commission.GetWindowText(Text);
	Rec.Add(Text);

	m_OraLoader.UpdateRecord(Rec, TRUE);
	m_SS.AddSheetRow(Rec);

	OnBnClickedFcLoadButton();
}


void CCPFutureCommDlg::OnBnClickedFcUpdateButton()
{
	if(!IsOK(2))
		return;

	CDBRec Rec;
	CString Text;

	m_OraLoader.Open("SELECT COUNTERPARTY, TRANS_TYPE, CURRENCY, FUTURE_TYPE, FROM_DATE, "
						"TO_DATE, ETRADE, COMMISSION "
						"FROM SEMAM.NW_FUTURE_COMM_SCHEDULE WHERE ROWIDTOCHAR(ROWID) = '" + m_RowID + "' ", ODYNASET_DEFAULT);
	Rec.Add(m_CP->GetSelString(Text));
	Rec.Add(m_TransType->GetSelString(Text));
	Rec.Add(m_Currency->GetSelString(Text));
	Rec.Add(m_FutureType->GetSelString(Text));
	Rec.Add(m_FromDate.GetData());
	Rec.Add(m_ToDate.GetData());
	Rec.Add(m_ETrade->GetCheckString());
	m_Commission.GetWindowText(Text);
	Rec.Add(Text);

	m_OraLoader.UpdateRecord(Rec);
	m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), Rec);
	m_SS.SetSheetCurRow(-1);
}


void CCPFutureCommDlg::OnBnClickedFcDeleteButton()
{
	if(!IsOK(3))
		return;

	m_OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_FUTURE_COMM_SCHEDULE WHERE ROWIDTOCHAR(ROWID) = '" + m_RowID + "' ");
	m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
	m_SS.SetSheetCurRow(-1);
}
