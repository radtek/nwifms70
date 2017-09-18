// FXCategoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "FXCategoryDlg.h"
#include "afxdialogex.h"
#include "qdata.h"


// CFXCategoryDlg dialog

IMPLEMENT_DYNAMIC(CFXCategoryDlg, CDialog)

CFXCategoryDlg::CFXCategoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFXCategoryDlg::IDD, pParent)
{
}

void CFXCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FXCAT_LIST, m_SS);
}

void CFXCategoryDlg::EnableCtrls()
{
	GetDlgItem(IDC_FXCAT_LOAD_BUTTON)->EnableWindow(m_TradeDate.GetWindowTextLength() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_FXCAT_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 && m_Category.GetCurSel() >= 0 ? TRUE : FALSE);
}

BOOL CFXCategoryDlg::IsOK()
{
	CString Text;

	if(m_Currency.GetCurSel() < 0)
		Text = "No currency selected";
	if(m_Trader.GetCurSel() < 0)
		Text = "No trader selected";
	if(m_Category.GetCurSel() < 0)
		Text = "No category selected";

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text, "FXCategory");
	return FALSE;
}

void CFXCategoryDlg::RowToControls()
{
	if(m_SS.GetSheetCurRow() > 0 && m_SS.GetSheetCurRow() <= m_SS.GetSheetRows())
	{
		int Row;

		Row = m_SS.GetSheetCurRow();
		m_TradeDate.SetData(m_SS.GetSheetText(1, Row));
		m_Currency.SetData(m_SS.GetSheetText(2, Row));
		m_Ticket.SetData(m_SS.GetSheetText(3, Row));
		m_Fxrate.SetData(m_SS.GetSheetText(4, Row));
		m_Nominal.SetData(m_SS.GetSheetText(5, Row));
		m_Trader.SetData(m_SS.GetSheetText(6, Row));
		m_Category.SetData(m_SS.GetSheetText(7, Row));
		m_RowID = m_SS.GetSheetText(8, Row);
	}
}

BEGIN_MESSAGE_MAP(CFXCategoryDlg, CDialog)
	ON_EN_CHANGE(IDC_FXCAT_TRADEDATE_EDIT, &CFXCategoryDlg::OnEnChangeFxcatTradedateEdit)
	ON_CBN_SELCHANGE(IDC_FXCAT_CATEGORY_COMBO, &CFXCategoryDlg::OnCbnSelchangeFxcatCategoryCombo)
	ON_BN_CLICKED(IDC_FXCAT_LOAD_BUTTON, &CFXCategoryDlg::OnBnClickedFxcatLoadButton)
	ON_BN_CLICKED(IDC_FXCAT_UPDATE_BUTTON, &CFXCategoryDlg::OnBnClickedFxcatUpdateButton)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFXCategoryDlg, CDialog)
	ON_EVENT(CFXCategoryDlg, IDC_FXCAT_LIST, 11, CFXCategoryDlg::DblClickFxcatList, VTS_I4 VTS_I4)
	ON_EVENT(CFXCategoryDlg, IDC_FXCAT_LIST, 2, CFXCategoryDlg::BlockSelectedFxcatList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

// CFXCategoryDlg message handlers
BOOL CFXCategoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Date;
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

	m_Currency.Setup(this, IDC_FXCAT_CURRENCY_COMBO);
	m_Trader.Setup(this, IDC_FXCAT_TRADER_COMBO);
	m_Category.Setup(this, IDC_FXCAT_CATEGORY_COMBO);
	
	m_TradeDate.Setup(this, IDC_FXCAT_TRADEDATE_EDIT);
	m_Ticket.Setup(this, IDC_FXCAT_TICKET_EDIT);
	m_Fxrate.Setup(this, IDC_FXCAT_FXRATE_EDIT);
	m_Nominal.Setup(this, IDC_FXCAT_NOMAMOUNT_EDIT);

	BeginWaitCursor();

	m_OraLoader = m_pData->GetOraLoader();
	m_OraLoader.Today(Date);
	m_TradeDate.SetData(Date);
	m_pData->GetCurrencyArr().CopyToComboBox(m_Currency);
	m_pData->GetTraderArr().CopyToComboBox(m_Trader);
	m_OraLoader.Open("SELECT STRATEGY, NVL(TRADER_INI, 'HH') FROM SEMAM.NW_INV_STRATEGIES WHERE TRANS_BUCKET IS NULL ORDER BY 1 ");
	m_OraLoader.LoadMCComboBox(m_Category);

	this->SetWindowText("Forex Category");

	OnBnClickedFxcatLoadButton();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFXCategoryDlg::DblClickFxcatList(long Col, long Row)
{
	m_SS.SetSheetCurRow(Row);
	RowToControls();
	EnableCtrls();
}


void CFXCategoryDlg::BlockSelectedFxcatList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	DblClickFxcatList(BlockCol, BlockRow);
}

void CFXCategoryDlg::OnEnChangeFxcatTradedateEdit()
{
	EnableCtrls();
}


void CFXCategoryDlg::OnCbnSelchangeFxcatCategoryCombo()
{
	EnableCtrls();
}


void CFXCategoryDlg::OnBnClickedFxcatLoadButton()
{
	CQData QData;
	CString Sql, Text;

	m_SS.ClearSheet();

	if(!m_sTicket.IsEmpty())
	{
		Text = QData.GetQueryNumber(m_sTicket);
		Sql = "SELECT A.TRADE_DATE, A.CURRENCY, TO_CHAR(A.TICKET_NUM) \"TICKET_NUM\", A.FXRATE, "
				"A.NOM_AMOUNT, A.TRADER_INI, NVL(B.CATEGORY, C.CATEGORY) \"CATEGORY\", "
				"ROWIDTOCHAR(B.ROWID) \"ROWID\" "
				"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_FX_TICKET_CATEGORY B, SEMAM.NW_FX_CATEGORY C "
				"WHERE B.TICKET_NUM(+) = A.TICKET_NUM "
				"AND C.TRADER_INI(+) = A.TRADER_INI "
				"AND A.TRANS_TYPE IN ('FOREX') "
				"AND (A.ASSET_CODE IS NULL OR A.ASSET_CODE = 'NEW ASSET') "
				"AND A.TICKET_NUM = " + Text;
	}
	else
	{
		Text = QData.GetQueryDate(m_TradeDate.GetData());
		Sql = "SELECT A.TRADE_DATE, A.CURRENCY, A.TICKET_NUM, A.FXRATE, A.NOM_AMOUNT, A.TRADER_INI, "
				"NVL(B.CATEGORY, C.CATEGORY) \"CATEGORY\", ROWIDTOCHAR(B.ROWID) \"ROWID\" "
				"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_FX_TICKET_CATEGORY B, SEMAM.NW_FX_CATEGORY C "
				"WHERE B.TICKET_NUM(+) = A.TICKET_NUM "
				"AND C.TRADER_INI(+) = A.TRADER_INI "
				"AND A.TRANS_TYPE IN ('FOREX') "
				"AND (A.ASSET_CODE IS NULL OR A.ASSET_CODE = 'NEW ASSET') "
				"AND A.TRADE_DATE = " + Text;
	}

	m_OraLoader.Open(Sql + " ORDER BY 1, 2, 3 ");	
	m_OraLoader.LoadDBSheet(m_SS, TRUE);

	if(m_SS.GetSheetRows() > 0)
		DblClickFxcatList(1, 1);
}


void CFXCategoryDlg::OnBnClickedFxcatUpdateButton()
{
	if(!IsOK())
		return;

	CDBRec Rec;

	Rec.RemoveAll();

	Rec.Add(m_Ticket.GetData());
	Rec.Add(m_Category.GetData());
	if(m_RowID.IsEmpty())
	{
		m_OraLoader.Open("SELECT TICKET_NUM, CATEGORY FROM SEMAM.NW_FX_TICKET_CATEGORY ", ODYNASET_DEFAULT);
		m_OraLoader.UpdateRecord(Rec, TRUE);
	}
	else
	{
		m_OraLoader.Open("SELECT TICKET_NUM, CATEGORY FROM SEMAM.NW_FX_TICKET_CATEGORY WHERE ROWIDTOCHAR(ROWID) = '" + m_RowID + "' ", ODYNASET_DEFAULT);
		m_OraLoader.UpdateRecord(Rec);
	}
	
	m_SS.SetSheetText(7, m_SS.GetSheetCurRow(), m_Category.GetData());
	OnBnClickedFxcatLoadButton();
}

