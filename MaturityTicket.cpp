// FmsTicket.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "MaturityTicket.h"
#include "assetdlg.h"
#include "Finddlg.h"
#include "invert.h"
#include "qdata.h"
#include "repoprice.h"
#include "optexpdlg.h"
#include "xcurrencydlg.h"
#include "optsettlementdlg.h"
#include "setconventiondlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFmsTicket

IMPLEMENT_DYNCREATE(CMaturityTicket, CFormView)

CMaturityTicket::CMaturityTicket()
	: CFormView(CMaturityTicket::IDD)
{
	//{{AFX_DATA_INIT(CMaturityTicket)
	m_TRS = NULL;
	m_DocRq = NULL;
	m_DocRecvd = NULL;
	m_Confirm = NULL;

    m_Portfolio = NULL;
	m_TransType = NULL;
    m_Currency = NULL;
    m_CP = NULL;
    m_RateBasis = NULL;
	m_Formula = NULL;
	m_Custodian = NULL;

	m_InvTrDesc = NULL;
	m_InvAccount = NULL;
	m_InvCurrency = NULL;
	m_InvAssignCP = NULL;

	m_SetCurrency = NULL;
	//}}AFX_DATA_INIT
}

CMaturityTicket::~CMaturityTicket()
{
	if(m_TRS)
		delete m_TRS;
	if(m_DocRq)
		delete m_DocRq;
	if(m_DocRecvd)
		delete m_DocRecvd;
	if(m_Confirm)
		delete m_Confirm;

    if(m_Portfolio)
		delete m_Portfolio;
	if(m_TransType)
		delete m_TransType;
    if(m_Currency)
		delete m_Currency;
	if(m_CP)
		delete m_CP;
    if(m_RateBasis)
		delete m_RateBasis;
	if(m_Formula)
		delete m_Formula;
	if(m_Custodian)
		delete m_Custodian;

	if(m_InvTrDesc)
		delete m_InvTrDesc;
	if(m_InvAccount)
		delete m_InvAccount;
	if(m_InvCurrency)
		delete m_InvCurrency;
	if(m_InvAssignCP)
		delete m_InvAssignCP;

	if(m_SetCurrency)
		delete m_SetCurrency;
}

void CMaturityTicket::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaturityTicket)
	DDX_Control(pDX, IDC_MATURITY_TICKET_LIST, m_SS);
	DDX_Control(pDX, IDC_MATURITY_INV_LIST, m_InvSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaturityTicket, CFormView)
	//{{AFX_MSG_MAP(CMaturityTicket)
	ON_BN_CLICKED(IDC_MATURITY_LOAD_BUTTON, &CMaturityTicket::OnBnClickedMaturityLoadButton)
	ON_BN_CLICKED(IDC_MATURITY_PROCESS_BUTTON, &CMaturityTicket::OnBnClickedMaturityProcessButton)
	ON_CBN_SELCHANGE(IDC_MATURITY_SETCURRENCY_COMBO, &CMaturityTicket::OnCbnSelchangeMaturitySetCurrencyCombo)
	ON_EN_CHANGE(IDC_MATURITY_SETCASH_EDIT, &CMaturityTicket::OnEnChangeMaturitySetcashEdit)
	ON_COMMAND(ID_MATURITY_LOAD_TRADE, &CMaturityTicket::OnMaturityLoadTrade)
	ON_COMMAND(ID_MATURITY_PROCESS_TRADE, &CMaturityTicket::OnMaturityProcessTrade)
	ON_UPDATE_COMMAND_UI(ID_MATURITY_PROCESS_TRADE, &CMaturityTicket::OnUpdateMaturityProcessTrade)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_MATURITY_SETFX_EDIT, &CMaturityTicket::OnEnChangeMaturitySetfxEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaturityTicket diagnostics

#ifdef _DEBUG
void CMaturityTicket::AssertValid() const
{
	CFormView::AssertValid();
}

void CMaturityTicket::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CMaturityTicket::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNWIFMS70Doc)));
	return (CNWIFMS70Doc*)m_pDocument;
}
#endif //_DEBUG


void CMaturityTicket::InitControls()
{
	m_SS.SetVisibleCols(9);
	m_SS.SetVisibleRows(10);
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);

	m_InvSS.SetVisibleCols(9);
	m_InvSS.SetVisibleRows(3);
	m_InvSS.LockSheet();
	m_InvSS.SetSheetRows(0);
	
	m_TRS = (CCheckBox*) new CCheckBox(this, IDC_MATURITY_TRS_CHECK, TRS);
	m_DocRq = (CCheckBox*) new CCheckBox(this, IDC_MATURITY_DOCREQ_CHECK);
	m_DocRecvd = (CCheckBox*) new CCheckBox(this, IDC_MATURITY_DOCRCVD_CHECK);
	m_Confirm = (CCheckBox*) new CCheckBox(this, IDC_MATURITY_CONFIRM_CHECK, Y);

    m_Portfolio = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_PORTFOLIO_COMBO);
	m_TransType = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_TRANSTYPE_COMBO);
	m_Currency = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_CURRENCY_COMBO);
    m_CP = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_CP_COMBO);
    m_RateBasis = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_RATEBASIS_COMBO, TRUE);
	m_Formula = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_FORMULA_COMBO, TRUE, TRUE);
    m_Custodian = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_CUSTODIAN_COMBO, TRUE);
 
	m_Dir.Setup(this, IDC_MATURITY_DIR_COMBO);
	m_Trader.Setup(this, IDC_MATURITY_TRADER_COMBO);

	m_TransNum.Setup(this, IDC_MATURITY_TRANSNUM_EDIT);
	m_Ticket.Setup(this, IDC_MATURITY_TICKET_EDIT);
	m_TradeDate.Setup(this, IDC_MATURITY_TRADEDATE_EDIT);
	m_ValueDate.Setup(this, IDC_MATURITY_VALUEDATE_EDIT);
	m_Amount.Setup(this, IDC_MATURITY_NOMINAL_EDIT);
	m_Fxrate.Setup(this, IDC_MATURITY_FXRATE_EDIT);
	m_BrFee.Setup(this, IDC_MATURITY_COMMISSION_EDIT, NULL, 2);
	m_SoftDAmount.Setup(this, IDC_MATURITY_SOFTDOLLAR_EDIT, NULL, 2);
	m_OtherFee.Setup(this, IDC_MATURITY_OTHERFEE_EDIT, NULL, 2);
	m_Price.Setup(this, IDC_MATURITY_PRICE_EDIT);
	m_CT.Setup(this, IDC_MATURITY_CT_EDIT, EMPTYSTRING, NOCASE);
	m_CT.LimitText(3);
	m_Note.Setup(this, IDC_MATURITY_NOTE_EDIT, EMPTYSTRING, NOCASE);
	m_Note.LimitText(30);
	m_Note2.Setup(this, IDC_MATURITY_NOTE2_EDIT, EMPTYSTRING, NOCASE);
	m_Note2.LimitText(30);
	m_Rate.Setup(this, IDC_MATURITY_RATE_EDIT);

	m_Maturity.Setup(this, IDC_MATURITY_MATURITY_EDIT);
	m_OriMaturity.Setup(this, IDC_MATURITY_SWMATURITY_EDIT);

	m_Asset.Setup(this, IDC_MATURITY_ASSET_EDIT, EMPTYSTRING, UPPERCASE);
	m_Asset.SetLimitText(10);
	m_AssetDesc.Setup(this, IDC_MATURITY_ASSETDESC_EDIT);
	m_AssetCurr.Setup(this, IDC_MATURITY_ASSETCURRENCY_EDIT);
	m_OriBooking.Setup(this, IDC_MATURITY_SWBOOKING_EDIT);
	m_CPTradeID.Setup(this, IDC_MATURITY_CP_ID_EDIT);
	m_CPTradeID.LimitText(20);

	m_InvAsset.Setup(this, IDC_MATURITY_INV_ASSET_EDIT);
	m_InvAssignCT.Setup(this, IDC_MATURITY_INV_ASSIGNCT_EDIT);
	m_InvValueDate.Setup(this, IDC_MATURITY_INV_VALUEDATE_EDIT);
	m_InvCloseDate.Setup(this, IDC_MATURITY_INV_CLOSE_DATE_EDIT);
	m_InvPostDate.Setup(this, IDC_MATURITY_INV_POSTDATE_EDIT);
	m_InvForexRef.Setup(this, IDC_MATURITY_INV_FOREX_REF_EDIT);
	m_InvCrossRate.Setup(this, IDC_MATURITY_INV_CROSS_RATE_EDIT);
	m_InvAmount.Setup(this, IDC_MATURITY_INV_NOMINAL_EDIT);
	m_InvCash.Setup(this, IDC_MATURITY_INV_CASH_EDIT, NULL, 2);
	m_InvInterest.Setup(this, IDC_MATURITY_INV_INTEREST_EDIT, NULL, 2);
	m_InvTrDesc = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_INV_TRDESC_COMBO);
	m_InvAccount = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_INV_ACCOUNT_COMBO, TRUE);
	m_InvCurrency = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_INV_CURRENCY_COMBO, TRUE);
	m_InvAssignCP = (COptComboBox*) new COptComboBox(this, IDC_MATURITY_INV_ASSIGNCP_COMBO, TRUE);

	m_RecCurrency.Setup(this, IDC_MATURITY_INV_REC_CURRENCY_EDIT);
	m_RecInterest.Setup(this, IDC_MATURITY_INV_REC_INT_EDIT, NULL, 2);
	m_RecAccretion.Setup(this, IDC_MATURITY_INV_REC_ACCRETION_EDIT, NULL, 2);
	m_RecCash.Setup(this, IDC_MATURITY_INV_REC_CASH_EDIT, NULL, 2);
	m_RecTotal.Setup(this, IDC_MATURITY_INV_REC_TOTAL_EDIT, NULL, 2);

	m_SetCurrency = new COptComboBox(this, IDC_MATURITY_SETCURRENCY_COMBO);
	m_SetFx.Setup(this, IDC_MATURITY_SETFX_EDIT);
	m_SetAmount.Setup(this, IDC_MATURITY_SETCASH_EDIT, NULL, TRUE);
	m_SetMaturity.Setup(this, IDC_MATURITY_SETMATURITY_EDIT);
}

BOOL CMaturityTicket::UpdateData(BOOL bSaveandValid)
{
	BeginWaitCursor();
	if(bSaveandValid)
	{
	}
	else
	{
		CQData QData;

		GetData().LoadSupportData();
		QData.CopyDBRecArrayToComboBox(GetData().GetPortfolioArr(), *m_Portfolio);
		QData.CopyDBRecArrayToComboBox(GetData().GetTransTypeArr(), *m_TransType);
		QData.CopyDBRecArrayToComboBox(GetData().GetRateBasisArr(), *m_RateBasis);
		QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_Currency, 0, FALSE);
		QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_InvCurrency);
		QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_SetCurrency);
		QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_CP, FALSE);
		QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_Custodian, FALSE);
		QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_InvAssignCP, TRUE);
		QData.CopyDBRecArrayToComboBox(GetData().GetAccountArr(), *m_InvAccount);
		GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);
		GetData().GetTraderArr().CopyToMCComboBox(m_Trader);
	}

	EndWaitCursor();

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CFmsTicket message handlers
void CMaturityTicket::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();
	InitControls();
	
	m_InvTrDesc->AddString(BOOKING);
	m_InvTrDesc->AddString(FXBKNG);
	m_InvTrDesc->AddString(MATURITY);

	m_Formula->AddString(NONE);
	m_Formula->AddString(DAILYREPO);
	m_Formula->AddString(DAILYREPOA);

	UpdateData(FALSE);
	OnMaturityLoadTrade();
	EndWaitCursor();
}

BEGIN_EVENTSINK_MAP(CMaturityTicket, CFormView)
    //{{AFX_EVENTSINK_MAP(CMaturityTicket)
	ON_EVENT(CMaturityTicket, IDC_MATURITY_TICKET_LIST, 11 /* DblClick */, OnDblClickMaturityTicketList, VTS_I4 VTS_I4)
	ON_EVENT(CMaturityTicket, IDC_MATURITY_INV_LIST, 11 /* DblClick */, OnDblClickMaturityInvList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMaturityTicket::OnDblClickMaturityTicketList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);

		m_Processed = m_SS.GetSheetText(1, Row);
		m_TransNum.SetData(m_SS.GetSheetText(2, Row));
		m_Portfolio->SelectString(0, m_SS.GetSheetText(3, Row));
		m_Ticket.SetData(m_SS.GetSheetText(4, Row));
		m_CP->SelectString(0, m_SS.GetSheetText(5, Row));
		m_Asset.SetWindowText(m_SS.GetSheetText(6, Row));
		m_Trader.SelectString(0, m_SS.GetSheetText(7, Row));
		m_TradeDate.SetData(m_SS.GetSheetText(8, Row));
		m_ValueDate.SetData(m_SS.GetSheetText(9, Row));
		m_TransType->SelectString(0, m_SS.GetSheetText(10, Row));
		m_Dir.SelectString(0, m_SS.GetSheetText(11, Row));
		m_Amount.SetWindowText(m_SS.GetSheetText(12, Row));
		m_Price.SetWindowTextA(m_SS.GetSheetText(13, Row));
		m_Currency->SelectString(0, m_SS.GetSheetText(14, Row));
		m_SetCurrency->SelectString(0, m_SS.GetSheetText(14, Row));
		m_Fxrate.SetWindowText(m_SS.GetSheetText(15, Row));
		m_CT.SetWindowText(m_SS.GetSheetText(16, Row));
		m_TRS->SetCheck(m_SS.GetSheetText(17, Row));
		m_BrFee.SetWindowText(m_SS.GetSheetText(18, Row));
		m_SoftDAmount.SetWindowText(m_SS.GetSheetText(19, Row));
		m_OtherFee.SetWindowText(m_SS.GetSheetText(20, Row));
		m_Rate.SetWindowText(m_SS.GetSheetText(21, Row));
		m_RateBasis->SelectString(0, m_SS.GetSheetText(22, Row));
		m_Maturity.SetData(m_SS.GetSheetText(23, Row));
		m_Formula->SelectString(0, m_SS.GetSheetText(24, Row));
		m_OriBooking.SetData(m_SS.GetSheetText(25, Row));
		m_OriMaturity.SetData(m_SS.GetSheetText(26, Row));
		m_Custodian->SelectString(0, m_SS.GetSheetText(27, Row));
		m_Note.SetWindowText(m_SS.GetSheetText(28, Row));
		m_Note2.SetWindowText(m_SS.GetSheetText(29, Row));
		m_CPTradeID.SetWindowText(m_SS.GetSheetText(30, Row));
		m_DocRq->SetCheck(m_SS.GetSheetText(31, Row));
		m_DocRecvd->SetCheck(m_SS.GetSheetText(32, Row));
		m_Confirm->SetCheck(m_SS.GetSheetText(33, Row));
		m_AssetDesc.SetWindowText(m_SS.GetSheetText(34, Row));
		m_AssetCurr.SetWindowText(m_SS.GetSheetText(35, Row));
		m_SetMaturity.SetData(m_SS.GetSheetText(36, Row));
		m_SetAmount.SetWindowText(m_SS.GetSheetText(37, Row));
		m_InvNum = m_SS.GetSheetText(38, Row);
		
		COraLoader OraLoader;

		OraLoader = GetData().GetOraLoader();
		OraLoader.GetSql().Format("SELECT ASSET_CODE, TR_DESC, NOM_AMOUNT, ACTUAL_VDATE, "
									"ACTUAL_CLOSING, POST_DATE, CURRENCY, FOREX_REF_NUM, "
									"CROSS_RATE, CASH_AMOUNT, INT_AMOUNT, ACC_CODE, "
									"ASSIGN_CP, ASSIGN_CT FROM SEMAM.NW_TR_INV "
									"WHERE INV_NUM = %s ", (LPCTSTR) m_InvNum);

		m_InvSS.ClearSheet();
		OraLoader.Open();
		OraLoader.LoadDBSheet(m_InvSS);
		OnDblClickMaturityInvList(1, m_InvSS.GetSheetRows() > 0 ? 1 : -1);
	}
	else
	{
		m_SS.SetSheetCurRow(-1);
		m_Processed.Empty();
		m_TransNum.SetData(EMPTYSTRING);
		m_Portfolio->SetCurSel(-1);
		m_Ticket.SetData(EMPTYSTRING);
		m_CP->SetCurSel(-1);
		m_Asset.SetWindowText(EMPTYSTRING);
		m_Trader.SetCurSel(-1);
		m_TradeDate.SetData(EMPTYSTRING);
		m_ValueDate.SetData(EMPTYSTRING);
		m_TransType->SetCurSel(-1);
		m_Dir.SetCurSel(-1);
		m_Amount.SetWindowText(EMPTYSTRING);
		m_Price.SetWindowText(EMPTYSTRING);
		m_Currency->SetCurSel(-1);
		m_Fxrate.SetWindowText(EMPTYSTRING);
		m_CT.SetWindowText(EMPTYSTRING);
		m_TRS->SetCheck(0);
		m_BrFee.SetWindowText(EMPTYSTRING);
		m_SoftDAmount.SetWindowText(EMPTYSTRING);
		m_OtherFee.SetWindowText(EMPTYSTRING);
		m_Rate.SetWindowText(EMPTYSTRING);
		m_RateBasis->SetCurSel(-1);
		m_Maturity.SetData(EMPTYSTRING);
		m_Formula->SetCurSel(-1);
		m_OriBooking.SetData(EMPTYSTRING);
		m_OriMaturity.SetData(EMPTYSTRING);
		m_Custodian->SetCurSel(-1);
		m_Note.SetWindowText(EMPTYSTRING);
		m_Note2.SetWindowText(EMPTYSTRING);
		m_CPTradeID.SetWindowText(EMPTYSTRING);
		m_DocRq->SetCheck(0);
		m_DocRecvd->SetCheck(0);
		m_Confirm->SetCheck(0);
		m_AssetDesc.SetWindowText(EMPTYSTRING);
		m_AssetCurr.SetWindowText(EMPTYSTRING);
		m_SetMaturity.SetData(EMPTYSTRING);
		m_SetAmount.SetWindowText(EMPTYSTRING);
		m_InvNum.Empty();
	}

	CString TransType;

	m_TransType->GetSelString(TransType);
	if(TransType == INTSWAP)
		m_SetAmount.EnableWindow(TRUE);
	else
	{
		m_SetAmount.EnableWindow(FALSE);
		m_SetAmount.SetWindowText(EMPTYSTRING);
	}

	GetDlgItem(IDC_MATURITY_PROCESS_BUTTON)->EnableWindow(m_InvSS.GetSheetCurRow() > 0);
}

void CMaturityTicket::OnDblClickMaturityInvList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_InvSS.GetSheetRows())
	{
		m_InvSS.SetSheetCurRow(Row);

		m_InvAsset.SetWindowText(m_InvSS.GetSheetText(1, Row));
		m_InvTrDesc->SelectString(0, m_InvSS.GetSheetText(2, Row));
		m_InvAmount.SetWindowText(m_InvSS.GetSheetText(3, Row));
		m_InvValueDate.SetData(m_InvSS.GetSheetText(4, Row));
		m_InvCloseDate.SetData(m_InvSS.GetSheetText(5, Row));
		m_InvPostDate.SetData(m_InvSS.GetSheetText(6, Row));
		m_InvCurrency->SelectString(0, m_InvSS.GetSheetText(7, Row));
		m_InvForexRef.SetData(m_InvSS.GetSheetText(8, Row));
		m_InvCrossRate.SetWindowText(m_InvSS.GetSheetText(9, Row));
		m_InvCash.SetWindowText(m_InvSS.GetSheetText(10, Row));
		m_InvInterest.SetWindowText(m_InvSS.GetSheetText(11, Row));
		m_InvAccount->SelectString(0, m_InvSS.GetSheetText(12, Row));
		m_InvAssignCP->SelectString(0, m_InvSS.GetSheetText(13, Row));
		m_InvAssignCT.SetWindowText(m_InvSS.GetSheetText(14, Row));
	}
	else
	{
		m_InvSS.SetSheetCurRow(-1);
		m_InvAsset.SetWindowText(EMPTYSTRING);
		m_InvTrDesc->SetCurSel(-1);
		m_InvAmount.SetWindowText(EMPTYSTRING);
		m_InvValueDate.SetData(EMPTYSTRING);
		m_InvCloseDate.SetData(EMPTYSTRING);
		m_InvPostDate.SetData(EMPTYSTRING);
		m_InvCurrency->SetCurSel(-1);
		m_InvForexRef.SetData(EMPTYSTRING);
		m_InvCrossRate.SetWindowText(EMPTYSTRING);
		m_InvCash.SetWindowText(EMPTYSTRING);
		m_InvInterest.SetWindowText(EMPTYSTRING);
		m_InvAccount->SetCurSel(-1);
		m_InvAssignCP->SetCurSel(-1);
		m_InvAssignCT.SetWindowText(EMPTYSTRING);
	}
}

void CMaturityTicket::OnBnClickedMaturityLoadButton()
{
	OnMaturityLoadTrade();
}

void CMaturityTicket::OnBnClickedMaturityProcessButton()
{
	OnMaturityProcessTrade();
}

void CMaturityTicket::OnCbnSelchangeMaturitySetCurrencyCombo()
{
	OnEnChangeMaturitySetcashEdit();
}

void CMaturityTicket::OnEnChangeMaturitySetcashEdit()
{
	GetDlgItem(IDC_MATURITY_PROCESS_BUTTON)->EnableWindow(m_SetCurrency->GetCurSel() >= 0 && m_SetAmount.GetWindowTextLength() > 0);
}

void CMaturityTicket::OnMaturityLoadTrade()
{
	COraLoader OraLoader;

	OraLoader = GetData().GetOraLoader();

	OraLoader.Open("SELECT A.PROCESSED, B.TRANS_NUM, B.PORTFOLIO, B.TICKET_NUM, B.COUNTERPARTY, "
					"B.ASSET_CODE, B.TRADER_INI, B.TRADE_DATE, B.VALUE_DATE, B.TRANS_TYPE, "
					"B.TRANS_DIRECTION, B.NOM_AMOUNT, B.PRICE, B.CURRENCY, B.FXRATE, B.CON_CODE, "
					"B.DEAL_TYPE, B.BR_FEES, B.SOFTDOLLAR, B.LEV_RATE, B.TR_RATE, B.RATE_BASIS, "
					"B.MATURITY_DATE, B.FLOAT_RATE_FORMULA, B.ORIG_SW_BOOKING, B.ORIG_SW_MATURITY, "
					"B.CUSTODIAN, B.HAN_DETAIL1, B.HAN_DETAIL2, B.CP_TRADE_ID, B.DOC_REQ, "
					"B.DOC_RECVD, B.CONFIRM, ASS_DESC, ASS_CURRENCY, A.MATURITY, "
					"DECODE(B.TRANS_TYPE, 'INT. SWAP', 0, TO_NUMBER(NULL)) \"SET_AMOUNT\", "
					"A.INV_NUM "
					"FROM SEMAM.NW_MATURITY_TRADE A "
					"JOIN SEMAM.NW_TR_TICKETS B ON (A.TRANS_NUM = B.TRANS_NUM) "
					"JOIN SEMAM.NW_ASSETS C ON (B.ASSET_CODE = C.ASS_CODE) "
					"WHERE A.PROCESSED IS NULL AND A.SIGN IS NOT NULL "
					"ORDER BY 1, 2, 3 ");
	
	OraLoader.GetFieldArray().GetAt(1)->SetWithComma(FALSE);
	OraLoader.GetFieldArray().GetAt(3)->SetWithComma(FALSE);
	OraLoader.GetFieldArray().GetAt(OraLoader.GetFieldArray().GetSize() - 1)->SetWithComma(FALSE);
	m_SS.ClearSheet();
	OraLoader.LoadDBSheet(m_SS);
	if(m_SS.GetSheetRows() > 0)
		OnDblClickMaturityTicketList(1, 1);
}

void CMaturityTicket::OnMaturityProcessTrade()
{
	int Row;
	COraLoader OraLoader;
	CString TransType, Currency, Amount, AssignCP, AssignCT;

	OraLoader.SetDB(&theDB);

	m_TransType->GetSelString(TransType);
	m_SetCurrency->GetSelString(Currency);

	Row = m_SS.GetSheetCurRow();
	if(TransType == INTSWAP)
	{
		CQData QData;
		m_InvAssignCP->GetSelString(AssignCP);
		AssignCP = QData.GetQueryText(AssignCP);
		m_InvAssignCT.GetWindowText(AssignCT);
		AssignCT = QData.GetQueryText(AssignCT);
		m_InvCash.GetWindowText(Amount);
		if(Amount.IsEmpty() || atof(Amount) == 0)
			m_SetAmount.GetWindowText(Amount);

		Amount = QData.GetQueryNumber(Amount);
		OraLoader.GetSql().Format("UPDATE SEMAM.NW_TR_INV SET ACTUAL_VDATE = '%s', "
									"CURRENCY = '%s', CASH_AMOUNT = %s, INT_AMOUNT = 0, "
									"ASSIGN_CP = %s, ASSIGN_CT = %s WHERE INV_NUM = %s ",
									(LPCTSTR) m_SetMaturity.GetData(), (LPCTSTR) Currency,
									(LPCTSTR) (Amount.IsEmpty() ? "0" : (LPCTSTR) Amount), 
									(LPCTSTR) AssignCP, (LPCTSTR) AssignCT, (LPCTSTR) m_InvNum);
	}
	else
		OraLoader.GetSql().Format("UPDATE SEMAM.NW_TR_INV SET ACTUAL_VDATE = '%s', CURRENCY = '%s' "
									"WHERE INV_NUM = %s ", (LPCTSTR) m_SetMaturity.GetData(),
									(LPCTSTR) Currency, (LPCTSTR) m_InvNum);

	if(OraLoader.ExecuteSql())
	{
		OraLoader.GetSql().Format("UPDATE SEMAM.NW_TR_TICKETS A "
									"SET MATURITY_DATE = '%s', ORIG_SW_MATURITY = '%s' "
									"WHERE A.TRANS_NUM = (SELECT TRANS_NUM FROM SEMAM.NW_TR_INV B "
															"WHERE B.INV_NUM = %s) ",
									(LPCTSTR) m_SetMaturity.GetData(), (LPCTSTR) m_SetMaturity.GetData(), 
									(LPCTSTR) m_InvNum);

		if(OraLoader.ExecuteSql())
		{
			if(TransType == INTSWAP)
				m_InvCash.SetWindowText(Amount);
			else
				m_InvCash.SetWindowText(EMPTYSTRING);

			m_InvCurrency->SelectString(0, Currency);
			m_InvValueDate.SetData(m_SetMaturity.GetData());
			OraLoader.GetSql().Format("UPDATE SEMAM.NW_MATURITY_TRADE SET PROCESSED = 'Y' "
										"WHERE INV_NUM = %s ", (LPCTSTR) m_InvNum);
			OraLoader.ExecuteSql();
		}
		
		OnDblClickMaturityTicketList(1, Row);
	}
}

void CMaturityTicket::OnUpdateMaturityProcessTrade(CCmdUI *pCmdUI)
{
	CString TransType;

	if(m_SetMaturity.GetData().IsEmpty())
		pCmdUI->Enable(FALSE);
	else
	{
		m_TransType->GetSelString(TransType);
		if(TransType == INTSWAP)
			pCmdUI->Enable(m_SetCurrency->GetCurSel() >= 0 && m_SetAmount.GetWindowTextLength() > 0);
		else
			pCmdUI->Enable(TRUE);
	}
}

void CMaturityTicket::OnEnChangeMaturitySetfxEdit()
{
	CString Currency;
	
	if(m_SetFx.GetWindowTextLength() > 0)
		Currency = USD;
	else
		m_Currency->GetSelString(Currency);

	m_SetCurrency->SelectString(0, Currency);
}
