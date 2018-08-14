// FutureFeesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "FutureFeesDlg.h"
#include "afxdialogex.h"


// CFutureFeesDlg dialog

IMPLEMENT_DYNAMIC(CFutureFeesDlg, CDialog)

CFutureFeesDlg::CFutureFeesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFutureFeesDlg::IDD, pParent)
{
}

void CFutureFeesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FFEE_LIST, m_SS);
}

void CFutureFeesDlg::EnableCtrls()
{
	GetDlgItem(IDC_FFEE_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_FFEE_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BOOL CFutureFeesDlg::IsOK(int nAction)
{
	CString Text;

	if(nAction != 1)
	{
		if(m_SS.GetSheetCurRow() < 0 || m_SS.GetSheetCurRow() > m_SS.GetSheetRows())
			Text = "No Record selected";
	}

	if(m_Custodian.GetCurSel() < 0)
		Text = "No counterparty selected";
	if(m_Account.GetCurSel() < 0)
		Text = "No account selected";
	
	if(m_TransType.GetCurSel() < 0)
		Text = "No trans_type selected";
	
	if(m_Currency.GetCurSel() < 0)
		Text = "No currency selected";

	if(m_FutureType.GetCurSel() < 0)
		Text = "No Future Type selected";

	if(m_FromDate.GetData().IsEmpty())
		Text = "No From Date entered";

	if(nAction != 3) // Delete
	{
		if(m_Fees.GetWindowTextLength() == 0)
			Text = "No Fees entered";
	}

	if(Text.IsEmpty())
		return TRUE;
	else
	{
		MessageBox(Text, "Future Fees");
		return FALSE;
	}
}

void CFutureFeesDlg::RowToControls()
{
	if(m_SS.GetSheetCurRow() > 0 && m_SS.GetSheetCurRow() <= m_SS.GetSheetRows())
	{
		int Row;

		Row = m_SS.GetSheetCurRow();
		m_Custodian.SetData(m_SS.GetSheetText(1, Row));
		m_Account.SetData(m_SS.GetSheetText(2, Row));
		m_TransType.SetData(m_SS.GetSheetText(3, Row));
		m_FutureType.SetData(m_SS.GetSheetText(4, Row));
		m_Currency.SetData(m_SS.GetSheetText(5, Row));
		m_FromDate.SetData(m_SS.GetSheetText(6, Row));
		m_ToDate.SetData(m_SS.GetSheetText(7, Row));
		m_Fees.SetData(m_SS.GetSheetText(8, Row));
		m_RowID = m_SS.GetSheetText(9, Row);
	}
}

BEGIN_MESSAGE_MAP(CFutureFeesDlg, CDialog)
	ON_BN_CLICKED(IDC_FFEE_LOAD_BUTTON, &CFutureFeesDlg::OnBnClickedFfeeLoadButton)
	ON_BN_CLICKED(IDC_FFEE_ADD_BUTTON, &CFutureFeesDlg::OnBnClickedFfeeAddButton)
	ON_BN_CLICKED(IDC_FFEE_UPDATE_BUTTON, &CFutureFeesDlg::OnBnClickedFfeeUpdateButton)
	ON_BN_CLICKED(IDC_FFEE_DELETE_BUTTON, &CFutureFeesDlg::OnBnClickedFfeeDeleteButton)
END_MESSAGE_MAP()


// CFutureFeesDlg message handlers
BOOL CFutureFeesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

	m_Custodian.Setup(this, IDC_FFEE_CUSTODIAN_COMBO);
	m_Account.Setup(this, IDC_FFEE_ACCOUNT_COMBO);
	m_TransType.Setup(this, IDC_FFEE_TRANSTYPE_COMBO);
	m_FutureType.Setup(this, IDC_FFEE_FUTURETYPE_COMBO);
	m_Currency.Setup(this, IDC_FFEE_CURRENCY_COMBO);
	m_FromDate.Setup(this, IDC_FFEE_FROMDATE_EDIT);
	m_ToDate.Setup(this, IDC_FFEE_TODATE_EDIT);
	m_Fees.Setup(this, IDC_FFEE_FEE_EDIT);

	BeginWaitCursor();

	m_OraLoader = m_pData->GetOraLoader();
	m_pData->GetContactList().CopyKeyToComboBox(m_Custodian);
	m_pData->GetAccountArr().CopyToComboBox(m_Account);
	m_pData->GetFutureTypeArr().CopyToComboBox(m_FutureType);
	m_pData->GetCurrencyArr().CopyToComboBox(m_Currency);
	m_TransType.AddString("OPTION");
	m_TransType.AddString("CDS");
	m_TransType.AddString("CDX");
	m_TransType.AddString("INT. SWAP");
	m_TransType.AddString(SECURITIES);

	this->SetWindowText("Future Fees");
	OnBnClickedFfeeLoadButton();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_EVENTSINK_MAP(CFutureFeesDlg, CDialog)
	ON_EVENT(CFutureFeesDlg, IDC_FFEE_LIST, 11, CFutureFeesDlg::DblClickFfeeList, VTS_I4 VTS_I4)
	ON_EVENT(CFutureFeesDlg, IDC_FFEE_LIST, 2, CFutureFeesDlg::BlockSelectedFfeeList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CFutureFeesDlg::DblClickFfeeList(long Col, long Row)
{
	m_SS.SetSheetCurRow(Row);
	RowToControls();
	EnableCtrls();
}


void CFutureFeesDlg::BlockSelectedFfeeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	DblClickFfeeList(BlockCol, BlockRow);
}


void CFutureFeesDlg::OnBnClickedFfeeLoadButton()
{
	m_SS.ClearSheet();

	m_OraLoader.Open("SELECT CUSTODIAN, ACC_CODE, TRANS_TYPE, FUTURE_TYPE, CURRENCY, FROM_DATE, "
					"TO_DATE, FEES, ROWIDTOCHAR(ROWID) \"ROWID\" "
					"FROM SEMAM.NW_FUTURE_FEES ORDER BY 1, 2, 3, 4, 5, 6 ");
	
	m_OraLoader.LoadDBSheet(m_SS, TRUE);

	if(m_SS.GetSheetRows() > 0)
		DblClickFfeeList(1, 1);
}


void CFutureFeesDlg::OnBnClickedFfeeAddButton()
{
	if(!IsOK(1))
		return;

	CDBRec Rec;

	m_OraLoader.Open("SELECT CUSTODIAN, ACC_CODE, TRANS_TYPE, FUTURE_TYPE, CURRENCY, FROM_DATE, "
					"TO_DATE, FEES FROM SEMAM.NW_FUTURE_FEES ", ODYNASET_DEFAULT);
	
	Rec.Add(m_Custodian.GetData());
	Rec.Add(m_Account.GetData());
	Rec.Add(m_TransType.GetData());
	Rec.Add(m_FutureType.GetData());
	Rec.Add(m_Currency.GetData());
	Rec.Add(m_FromDate.GetData());
	Rec.Add(m_ToDate.GetData());
	Rec.Add(m_Fees.GetData());

	m_OraLoader.UpdateRecord(Rec, TRUE);
	m_SS.AddSheetRow(Rec);

	OnBnClickedFfeeLoadButton();
}


void CFutureFeesDlg::OnBnClickedFfeeUpdateButton()
{
	if(!IsOK(2))
		return;

	CDBRec Rec;

	m_OraLoader.Open("SELECT CUSTODIAN, ACC_CODE, TRANS_TYPE, FUTURE_TYPE, CURRENCY, FROM_DATE, "
					"TO_DATE, FEES FROM SEMAM.NW_FUTURE_FEES WHERE ROWIDTOCHAR(ROWID) = '" + m_RowID + "' ", ODYNASET_DEFAULT);
	Rec.Add(m_Custodian.GetData());
	Rec.Add(m_Account.GetData());
	Rec.Add(m_TransType.GetData());
	Rec.Add(m_FutureType.GetData());
	Rec.Add(m_Currency.GetData());
	Rec.Add(m_FromDate.GetData());
	Rec.Add(m_ToDate.GetData());
	Rec.Add(m_Fees.GetData());

	m_OraLoader.UpdateRecord(Rec);
	m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), Rec);
	m_SS.SetSheetCurRow(-1);
}


void CFutureFeesDlg::OnBnClickedFfeeDeleteButton()
{
	if(!IsOK(3))
		return;

	m_OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_FUTURE_FEES WHERE ROWIDTOCHAR(ROWID) = '" + m_RowID + "' ");
	m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
	m_SS.SetSheetCurRow(-1);
}
