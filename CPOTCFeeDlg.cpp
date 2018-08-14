// CPFutureCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "CPOTCFeeDlg.h"
#include "afxdialogex.h"
#include "qdata.h"

// CCPFutureCommDlg dialog

IMPLEMENT_DYNAMIC(CCPOTCFeeDlg, CDialog)

CCPOTCFeeDlg::CCPOTCFeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPOTCFeeDlg::IDD, pParent)
{
}

void CCPOTCFeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OTC_LIST, m_SS);
}

void CCPOTCFeeDlg::EnableCtrls()
{
	GetDlgItem(IDC_OTC_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_OTC_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BOOL CCPOTCFeeDlg::IsOK(int nAction)
{
	CString Text;

	if(nAction != 1)
	{
		if(m_SS.GetSheetCurRow() < 0 || m_SS.GetSheetCurRow() > m_SS.GetSheetRows())
			Text = "No Record selected";
	}

	if(m_CP.GetCurSel() < 0)
		Text = "No counterparty selected";
	if(m_TransType.GetCurSel() < 0)
		Text = "No trans type selected";
	if(m_FeeType.GetCurSel() < 0)
		Text = "No fee type selected";
	if(m_Currency.GetCurSel() < 0)
		Text = "No currency selected";
	if(m_FromDate.GetData().IsEmpty())
		Text = "No From Date entered";

	if(nAction != 3) // Delete
	{
		if(m_Fee.GetWindowTextLength() == 0)
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

void CCPOTCFeeDlg::RowToControls()
{
	if(m_SS.GetSheetCurRow() > 0 && m_SS.GetSheetCurRow() <= m_SS.GetSheetRows())
	{
		int Row;

		Row = m_SS.GetSheetCurRow();
		m_CP.SetData(m_SS.GetSheetText(1, Row));
		m_FeeType.SetData(m_SS.GetSheetText(2, Row));
		m_TransType.SetData(m_SS.GetSheetText(3, Row));
		m_Currency.SetData(m_SS.GetSheetText(4, Row));
		m_LowLimit.SetData(m_SS.GetSheetText(5, Row));
		m_HighLimit.SetData(m_SS.GetSheetText(6, Row));
		m_Fee.SetData(m_SS.GetSheetText(7, Row));
		m_FromDate.SetData(m_SS.GetSheetText(8, Row));
		m_ToDate.SetData(m_SS.GetSheetText(9, Row));
		m_RowID = m_SS.GetSheetText(10, Row);
	}
}

BEGIN_MESSAGE_MAP(CCPOTCFeeDlg, CDialog)
	ON_BN_CLICKED(IDC_OTC_LOAD_BUTTON, &CCPOTCFeeDlg::OnBnClickedOtcLoadButton)
	ON_BN_CLICKED(IDC_OTC_ADD_BUTTON, &CCPOTCFeeDlg::OnBnClickedOtcAddButton)
	ON_BN_CLICKED(IDC_OTC_UPDATE_BUTTON, &CCPOTCFeeDlg::OnBnClickedOtcUpdateButton)
	ON_BN_CLICKED(IDC_OTC_DELETE_BUTTON, &CCPOTCFeeDlg::OnBnClickedOtcDeleteButton)
END_MESSAGE_MAP()


// CCPOTCFeeDlg message handlers
BEGIN_EVENTSINK_MAP(CCPOTCFeeDlg, CDialog)
	ON_EVENT(CCPOTCFeeDlg, IDC_OTC_LIST, 2, CCPOTCFeeDlg::BlockSelectedOtcList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CCPOTCFeeDlg, IDC_OTC_LIST, 11, CCPOTCFeeDlg::DblClickOtcList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CCPOTCFeeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

	m_CP.Setup(this, IDC_OTC_CP_COMBO);
	m_FeeType.Setup(this, IDC_OTC_FEETYPE_COMBO);
	m_TransType.Setup(this, IDC_OTC_TRANSTYPE_COMBO, TRUE);
	m_Currency.Setup(this, IDC_OTC_CURRENCY_COMBO, TRUE);
	m_LowLimit.Setup(this, IDC_OTC_LOW_EDIT);
	m_HighLimit.Setup(this, IDC_OTC_HIGH_EDIT);
	m_Fee.Setup(this, IDC_OTC_FEE_EDIT);
	m_FromDate.Setup(this, IDC_OTC_FROMDATE_EDIT);
	m_ToDate.Setup(this, IDC_OTC_TODATE_EDIT);

	BeginWaitCursor();

	m_OraLoader = m_pData->GetOraLoader();
	m_pData->GetContactList().CopyKeyToComboBox(m_CP);
	m_pData->GetCurrencyArr().CopyToComboBox(m_Currency);
	m_FeeType.AddString("BOOK FEE");
	m_FeeType.AddString("DAILY FEE");
	m_FeeType.AddString("TRADE FEE");
	m_TransType.AddString(NONE);
	m_TransType.AddString("CDS");
	m_TransType.AddString("CDX");
	m_TransType.AddString("INT. SWAP");

	this->SetWindowText("Counterparty OTC Fee Schedule");
	OnBnClickedOtcLoadButton();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPOTCFeeDlg::BlockSelectedOtcList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	DblClickOtcList(BlockCol, BlockRow);
}


void CCPOTCFeeDlg::DblClickOtcList(long Col, long Row)
{
	m_SS.SetSheetCurRow(Row);
	RowToControls();
	EnableCtrls();
}


void CCPOTCFeeDlg::OnBnClickedOtcLoadButton()
{
	m_SS.ClearSheet();

	m_OraLoader.Open("SELECT COUNTERPARTY, FEE_TYPE, TRANS_TYPE, CURRENCY, LOW_LIMIT, "
					"HIGH_LIMIT, FEE, FROM_DATE, TO_DATE, ROWIDTOCHAR(ROWID) \"ROWID\" "
						"FROM SEMAM.NW_OTC_FEE_SCHEDULE ORDER BY 1, 2, 3, 4, 5 ");
	
	m_OraLoader.LoadDBSheet(m_SS, TRUE);

	if(m_SS.GetSheetRows() > 0)
		DblClickOtcList(1, 1);
}


void CCPOTCFeeDlg::OnBnClickedOtcAddButton()
{
	if(!IsOK(1))
		return;

	CDBRec Rec;

	m_OraLoader.Open("SELECT COUNTERPARTY, FEE_TYPE, TRANS_TYPE, CURRENCY, LOW_LIMIT, "
						"HIGH_LIMIT, FEE, FROM_DATE, TO_DATE "
						"FROM SEMAM.NW_OTC_FEE_SCHEDULE ", ODYNASET_DEFAULT);
	
	Rec.Add(m_CP.GetData());
	Rec.Add(m_FeeType.GetData());
	Rec.Add(m_TransType.GetData());
	Rec.Add(m_Currency.GetData());
	Rec.Add(m_LowLimit.GetData());
	Rec.Add(m_HighLimit.GetData());
	Rec.Add(m_Fee.GetData());
	Rec.Add(m_FromDate.GetData());
	Rec.Add(m_ToDate.GetData());

	m_OraLoader.UpdateRecord(Rec, TRUE);
	m_SS.AddSheetRow(Rec);

	OnBnClickedOtcLoadButton();
}


void CCPOTCFeeDlg::OnBnClickedOtcUpdateButton()
{
	if(!IsOK(2))
		return;

	CDBRec Rec;

	m_OraLoader.Open("SELECT COUNTERPARTY, FEE_TYPE, TRANS_TYPE, CURRENCY, LOW_LIMIT, "
						"HIGH_LIMIT, FEE, FROM_DATE, TO_DATE "
						"FROM SEMAM.NW_OTC_FEE_SCHEDULE WHERE ROWIDTOCHAR(ROWID) = '" + m_RowID + "' ", ODYNASET_DEFAULT);
	Rec.Add(m_CP.GetData());
	Rec.Add(m_FeeType.GetData());
	Rec.Add(m_TransType.GetData());
	Rec.Add(m_Currency.GetData());
	Rec.Add(m_LowLimit.GetData());
	Rec.Add(m_HighLimit.GetData());
	Rec.Add(m_Fee.GetData());
	Rec.Add(m_FromDate.GetData());
	Rec.Add(m_ToDate.GetData());

	m_OraLoader.UpdateRecord(Rec);
	m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), Rec);
	m_SS.SetSheetCurRow(-1);
}


void CCPOTCFeeDlg::OnBnClickedOtcDeleteButton()
{
	if(!IsOK(3))
		return;

	m_OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_OTC_FEE_SCHEDULE WHERE ROWIDTOCHAR(ROWID) = '" + m_RowID + "' ");
	m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
	m_SS.SetSheetCurRow(-1);
}
