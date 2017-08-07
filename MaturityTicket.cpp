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
	//}}AFX_DATA_INIT
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
	ON_EN_CHANGE(IDC_MATURITY_SETMATURITY_EDIT, &CMaturityTicket::OnEnChangeMaturitySetmaturityEdit)
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

void CMaturityTicket::ComputeValue()
{
	double Fxrate;
	CQData QData;

	Fxrate = m_Val.GetFxRate();
	if(Fxrate <= 0)
		Fxrate = 1;

	if(m_Val.GetType() == INTSWAP)
	{		
		m_RecCurrency.SetData(m_Val.GetFxRate() > 0 ? USD : m_Currency.GetData());
		m_RecCash.SetData(atof(QData.RemoveComma(m_InvAmount.GetData()))/Fxrate);
		m_RecInterest.SetData(0);
		m_RecAccretion.SetData(0);
		m_RecTotal.SetData(m_RecCash.GetData());
	}
	else
		if(m_Val.GetType() == REPO || m_Val.GetType() == LEVERAGE)
		{
			m_Val.ComputeLevAmount(TRUE);
			if(strcmp(m_Val.GetType(), LEVERAGE) == 0)
				m_RecCash.SetData(m_Val.GetNomAmount()/Fxrate);
			else
				m_RecCash.SetData(m_Val.GetLevAmount()/Fxrate);

			m_RecAccretion.SetData(0);
			if(m_Maturity.GetData().IsEmpty() && m_InvValueDate.GetData().IsEmpty() && m_SetMaturity.GetData().IsEmpty())
				m_RecInterest.SetData(0);
			else
				m_RecInterest.SetData(m_Val.GetLevInterest()/Fxrate);

			if(m_Val.GetType() == LEVERAGE)
				m_RecTotal.SetData((m_Val.GetNomAmount() + m_Val.GetLevInterest())/Fxrate);
			else
				m_RecTotal.SetData((m_Val.GetLevAmount() + m_Val.GetLevInterest())/Fxrate);
		}
		else
		{
			m_RecCurrency.SetData(Fxrate > 0 ? USD : m_Currency.GetData());
			m_RecCash.SetData(m_Val.GetValue()/Fxrate);
			m_RecInterest.SetData(m_Val.GetPrePaidInt()/Fxrate);
			m_RecAccretion.SetData(m_Val.GetPrePaidAccretionValue()/Fxrate);
			m_RecTotal.SetData((m_Val.GetValue() + m_Val.GetPrePaid() + m_Val.GetPrePaidAccretionValue())/Fxrate);
		}
}

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
	
	m_TRS.Setup(this, IDC_MATURITY_TRS_CHECK, TRS);
	m_DocRq.Setup(this, IDC_MATURITY_DOCREQ_CHECK);
	m_DocRecvd.Setup(this, IDC_MATURITY_DOCRCVD_CHECK);
	m_Confirm.Setup(this, IDC_MATURITY_CONFIRM_CHECK, Y);

    m_Portfolio.Setup(this, IDC_MATURITY_PORTFOLIO_COMBO);
	m_TransType.Setup(this, IDC_MATURITY_TRANSTYPE_COMBO);
	m_Currency.Setup(this, IDC_MATURITY_CURRENCY_COMBO);
    m_CP.Setup(this, IDC_MATURITY_CP_COMBO);
    m_RateBasis.Setup(this, IDC_MATURITY_RATEBASIS_COMBO, TRUE);
	m_Formula.Setup(this, IDC_MATURITY_FORMULA_COMBO, TRUE, TRUE);
    m_Custodian.Setup(this, IDC_MATURITY_CUSTODIAN_COMBO, TRUE);
 
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
	m_InvTrDesc.Setup(this, IDC_MATURITY_INV_TRDESC_COMBO);
	m_InvAccount.Setup(this, IDC_MATURITY_INV_ACCOUNT_COMBO, TRUE);
	m_InvCurrency.Setup(this, IDC_MATURITY_INV_CURRENCY_COMBO, TRUE);
	m_InvAssignCP.Setup(this, IDC_MATURITY_INV_ASSIGNCP_COMBO, TRUE);

	m_RecCurrency.Setup(this, IDC_MATURITY_INV_REC_CURRENCY_EDIT);
	m_RecInterest.Setup(this, IDC_MATURITY_INV_REC_INT_EDIT, NULL, 2);
	m_RecAccretion.Setup(this, IDC_MATURITY_INV_REC_ACCRETION_EDIT, NULL, 2);
	m_RecCash.Setup(this, IDC_MATURITY_INV_REC_CASH_EDIT, NULL, 2);
	m_RecTotal.Setup(this, IDC_MATURITY_INV_REC_TOTAL_EDIT, NULL, 2);

	m_SetCurrency.Setup(this, IDC_MATURITY_SETCURRENCY_COMBO);
	m_SetFx.Setup(this, IDC_MATURITY_SETFX_EDIT);
	m_SetAmount.Setup(this, IDC_MATURITY_SETCASH_EDIT, NULL, TRUE);
	m_SetMaturity.Setup(this, IDC_MATURITY_SETMATURITY_EDIT);
}

void CMaturityTicket::SetupAssetInfo()
{
	double Fxrate, LevRate, Amount, Price;
	CString Date;
	CQData QData;
	COraLoader OraLoader;
	
	OraLoader.SetDB(&theDB);
	Amount = atof(QData.RemoveComma(m_InvAmount.GetData()));
	Price = atof(QData.RemoveComma(m_Price.GetData()));
	
	if(m_Fxrate.GetData().IsEmpty() && m_SetFx.GetData().IsEmpty())
		Fxrate = 1;
	else
		if(m_SetFx.GetData().IsEmpty())
			Fxrate = atof(QData.RemoveComma(m_Fxrate.GetData()));
		else
			Fxrate = atof(QData.RemoveComma(m_SetFx.GetData()));
	
	LevRate = atof(QData.RemoveComma(m_Rate.GetData()));
	
	Date = m_SetMaturity.GetData();
	if(Date.IsEmpty())
		Date = m_Maturity.GetData().IsEmpty() ? m_InvValueDate.GetData() : m_Maturity.GetData();

	if(!Date.IsEmpty())
		m_Val.Setup(OraLoader, m_TransType.GetData(), m_Dir.GetData(), m_Asset.GetData(), m_ValueDate.GetData(), 
					Date, Amount, Price, Fxrate, m_RateBasis.GetData(), LevRate, m_Formula.GetData());
}

BOOL CMaturityTicket::UpdateData(BOOL bSaveandValid)
{
	BeginWaitCursor();
	if(bSaveandValid)
	{
	}
	else
	{
		GetData().LoadSupportData();
		GetData().GetPortfolioArr().CopyToComboBox(m_Portfolio);
		GetData().GetTransTypeArr().CopyToComboBox(m_TransType);
		GetData().GetRateBasisArr().CopyToComboBox(m_RateBasis);
		GetData().GetCurrencyArr().CopyToComboBox(m_Currency);
		GetData().GetCurrencyArr().CopyToComboBox(m_InvCurrency);
		GetData().GetCurrencyArr().CopyToComboBox(m_SetCurrency);
		GetData().GetContactList().CopyKeyToComboBox(m_CP);
		GetData().GetContactList().CopyKeyToComboBox(m_Custodian);
		GetData().GetContactList().CopyKeyToComboBox(m_InvAssignCP);
		GetData().GetAccountArr().CopyToComboBox(m_InvAccount);
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
	
	m_InvTrDesc.AddString(BOOKING);
	m_InvTrDesc.AddString(FXBKNG);
	m_InvTrDesc.AddString(MATURITY);

	m_Formula.AddString(NONE);
	m_Formula.AddString(DAILYREPO);
	m_Formula.AddString(DAILYREPOA);

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
		m_Portfolio.SetData(m_SS.GetSheetText(3, Row));
		m_Ticket.SetData(m_SS.GetSheetText(4, Row));
		m_CP.SetData(m_SS.GetSheetText(5, Row));
		m_Asset.SetData(m_SS.GetSheetText(6, Row));
		m_Trader.SetData(m_SS.GetSheetText(7, Row));
		m_TradeDate.SetData(m_SS.GetSheetText(8, Row));
		m_ValueDate.SetData(m_SS.GetSheetText(9, Row));
		m_TransType.SetData(m_SS.GetSheetText(10, Row));
		m_Dir.SetData(m_SS.GetSheetText(11, Row));
		m_Amount.SetData(m_SS.GetSheetText(12, Row));
		m_Price.SetData(m_SS.GetSheetText(13, Row));
		m_Currency.SetData(m_SS.GetSheetText(14, Row));
		m_SetCurrency.SetData(m_SS.GetSheetText(14, Row));
		m_Fxrate.SetData(m_SS.GetSheetText(15, Row));
		m_CT.SetData(m_SS.GetSheetText(16, Row));
		m_TRS.SetData(m_SS.GetSheetText(17, Row));
		m_BrFee.SetData(m_SS.GetSheetText(18, Row));
		m_SoftDAmount.SetData(m_SS.GetSheetText(19, Row));
		m_OtherFee.SetData(m_SS.GetSheetText(20, Row));
		m_Rate.SetData(m_SS.GetSheetText(21, Row));
		m_RateBasis.SetData(m_SS.GetSheetText(22, Row));
		m_Maturity.SetData(m_SS.GetSheetText(23, Row));
		m_Formula.SetData(m_SS.GetSheetText(24, Row));
		m_OriBooking.SetData(m_SS.GetSheetText(25, Row));
		m_OriMaturity.SetData(m_SS.GetSheetText(26, Row));
		m_Custodian.SetData(m_SS.GetSheetText(27, Row));
		m_Note.SetData(m_SS.GetSheetText(28, Row));
		m_Note2.SetData(m_SS.GetSheetText(29, Row));
		m_CPTradeID.SetData(m_SS.GetSheetText(30, Row));
		m_DocRq.SetData(m_SS.GetSheetText(31, Row));
		m_DocRecvd.SetData(m_SS.GetSheetText(32, Row));
		m_Confirm.SetData(m_SS.GetSheetText(33, Row));
		m_AssetDesc.SetData(m_SS.GetSheetText(34, Row));
		m_AssetCurr.SetData(m_SS.GetSheetText(35, Row));
		m_SetMaturity.SetData(m_SS.GetSheetText(36, Row));
		m_SetAmount.SetData(m_SS.GetSheetText(37, Row));
		m_InvNum = m_SS.GetSheetText(38, Row);
		
		COraLoader OraLoader;

		OraLoader = GetData().GetOraLoader();

		m_InvSS.ClearSheet();
		OraLoader.Open("SELECT ASSET_CODE, TR_DESC, NOM_AMOUNT, ACTUAL_VDATE, ACTUAL_CLOSING, POST_DATE, "
							"CURRENCY, FOREX_REF_NUM, CROSS_RATE, CASH_AMOUNT, INT_AMOUNT, ACC_CODE, "
							"ASSIGN_CP, ASSIGN_CT FROM SEMAM.NW_TR_INV "
							"WHERE INV_NUM = " + m_InvNum);
		OraLoader.LoadDBSheet(m_InvSS);
		OnDblClickMaturityInvList(1, m_InvSS.GetSheetRows() > 0 ? 1 : -1);
	}
	else
	{
		m_SS.SetSheetCurRow(-1);
		m_Processed.Empty();
		m_TransNum.SetData(EMPTYSTRING);
		m_Portfolio.SetCurSel(-1);
		m_Ticket.SetData(EMPTYSTRING);
		m_CP.SetCurSel(-1);
		m_Asset.SetData(EMPTYSTRING);
		m_Trader.SetCurSel(-1);
		m_TradeDate.SetData(EMPTYSTRING);
		m_ValueDate.SetData(EMPTYSTRING);
		m_TransType.SetCurSel(-1);
		m_Dir.SetCurSel(-1);
		m_Amount.SetData(EMPTYSTRING);
		m_Price.SetData(EMPTYSTRING);
		m_Currency.SetCurSel(-1);
		m_Fxrate.SetData(EMPTYSTRING);
		m_CT.SetData(EMPTYSTRING);
		m_TRS.SetCheck(0);
		m_BrFee.SetData(EMPTYSTRING);
		m_SoftDAmount.SetData(EMPTYSTRING);
		m_OtherFee.SetData(EMPTYSTRING);
		m_Rate.SetData(EMPTYSTRING);
		m_RateBasis.SetCurSel(-1);
		m_Maturity.SetData(EMPTYSTRING);
		m_Formula.SetCurSel(-1);
		m_OriBooking.SetData(EMPTYSTRING);
		m_OriMaturity.SetData(EMPTYSTRING);
		m_Custodian.SetCurSel(-1);
		m_Note.SetData(EMPTYSTRING);
		m_Note2.SetData(EMPTYSTRING);
		m_CPTradeID.SetData(EMPTYSTRING);
		m_DocRq.SetCheck(0);
		m_DocRecvd.SetCheck(0);
		m_Confirm.SetCheck(0);
		m_AssetDesc.SetData(EMPTYSTRING);
		m_AssetCurr.SetData(EMPTYSTRING);
		m_SetMaturity.SetData(EMPTYSTRING);
		m_SetAmount.SetData(EMPTYSTRING);
		m_InvNum.Empty();
	}

	CString TransType;

	TransType = m_TransType.GetData();
	if(TransType == INTSWAP)
		m_SetAmount.EnableWindow(TRUE);
	else
	{
		m_SetAmount.EnableWindow(FALSE);
		m_SetAmount.SetData(EMPTYSTRING);
	}

	GetDlgItem(IDC_MATURITY_PROCESS_BUTTON)->EnableWindow(m_InvSS.GetSheetCurRow() > 0);
}

void CMaturityTicket::OnDblClickMaturityInvList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_InvSS.GetSheetRows())
	{
		m_InvSS.SetSheetCurRow(Row);

		m_InvAsset.SetData(m_InvSS.GetSheetText(1, Row));
		m_InvTrDesc.SetData(m_InvSS.GetSheetText(2, Row));
		m_InvAmount.SetData(m_InvSS.GetSheetText(3, Row));
		m_InvValueDate.SetData(m_InvSS.GetSheetText(4, Row));
		m_InvCloseDate.SetData(m_InvSS.GetSheetText(5, Row));
		m_InvPostDate.SetData(m_InvSS.GetSheetText(6, Row));
		m_InvCurrency.SetData(m_InvSS.GetSheetText(7, Row));
		m_InvForexRef.SetData(m_InvSS.GetSheetText(8, Row));
		m_InvCrossRate.SetData(m_InvSS.GetSheetText(9, Row));
		m_InvCash.SetData(m_InvSS.GetSheetText(10, Row));
		m_InvInterest.SetData(m_InvSS.GetSheetText(11, Row));
		m_InvAccount.SetData(m_InvSS.GetSheetText(12, Row));
		m_InvAssignCP.SetData(m_InvSS.GetSheetText(13, Row));
		m_InvAssignCT.SetData(m_InvSS.GetSheetText(14, Row));
		
		SetupAssetInfo();
		ComputeValue();
	}
	else
	{
		m_InvSS.SetSheetCurRow(-1);
		m_InvAsset.SetData(EMPTYSTRING);
		m_InvTrDesc.SetCurSel(-1);
		m_InvAmount.SetData(EMPTYSTRING);
		m_InvValueDate.SetData(EMPTYSTRING);
		m_InvCloseDate.SetData(EMPTYSTRING);
		m_InvPostDate.SetData(EMPTYSTRING);
		m_InvCurrency.SetCurSel(-1);
		m_InvForexRef.SetData(EMPTYSTRING);
		m_InvCrossRate.SetData(EMPTYSTRING);
		m_InvCash.SetData(EMPTYSTRING);
		m_InvInterest.SetData(EMPTYSTRING);
		m_InvAccount.SetCurSel(-1);
		m_InvAssignCP.SetCurSel(-1);
		m_InvAssignCT.SetData(EMPTYSTRING);
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
	GetDlgItem(IDC_MATURITY_PROCESS_BUTTON)->EnableWindow(m_SetCurrency.GetCurSel() >= 0 && m_SetAmount.GetWindowTextLength() > 0);
}

void CMaturityTicket::OnMaturityLoadTrade()
{
	COraLoader OraLoader;

	OraLoader = GetData().GetOraLoader();

	OraLoader.Open("SELECT A.PROCESSED, B.TRANS_NUM, B.PORTFOLIO, B.TICKET_NUM, B.COUNTERPARTY, B.ASSET_CODE, "
					"B.TRADER_INI, B.TRADE_DATE, B.VALUE_DATE, B.TRANS_TYPE, B.TRANS_DIRECTION, B.NOM_AMOUNT, "
					"B.PRICE, B.CURRENCY, B.FXRATE, B.CON_CODE, B.DEAL_TYPE, B.BR_FEES, B.SOFTDOLLAR, B.LEV_RATE, "
					"B.TR_RATE, B.RATE_BASIS, B.MATURITY_DATE, B.FLOAT_RATE_FORMULA, B.ORIG_SW_BOOKING, "
					"B.ORIG_SW_MATURITY, B.CUSTODIAN, B.HAN_DETAIL1, B.HAN_DETAIL2, B.CP_TRADE_ID, B.DOC_REQ, "
					"B.DOC_RECVD, B.CONFIRM, ASS_DESC, ASS_CURRENCY, A.MATURITY, "
					"DECODE(B.TRANS_TYPE, 'INT. SWAP', 0, TO_NUMBER(NULL)) \"SET_AMOUNT\", A.INV_NUM "
					"FROM SEMAM.NW_MATURITY_TRADE A, SEMAM.NW_TR_TICKETS B, SEMAM.NW_ASSETS C "
					"WHERE B.TRANS_NUM = A.TRANS_NUM "
					"AND C.ASS_CODE = B.ASSET_CODE "
					"AND A.PROCESSED IS NULL "
					"AND A.SIGN IS NOT NULL "
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
	CString TransType, Currency, Amount, AssignCP, AssignCT, Maturity;

	OraLoader.SetDB(&theDB);

	TransType = m_TransType.GetData();
	Currency = m_SetCurrency.GetData();
	Maturity = m_SetMaturity.GetData();

	Row = m_SS.GetSheetCurRow();
	if(TransType == INTSWAP)
	{
		CQData QData;
		AssignCP = QData.GetQueryText(m_InvAssignCP.GetData());
		AssignCT = QData.GetQueryText(m_InvAssignCT.GetData());
		Amount = m_InvCash.GetData();
		if(Amount.IsEmpty() || atof(Amount) == 0)
			Amount = m_SetAmount.GetData();

		Amount = QData.GetQueryNumber(Amount);
		if(Amount.IsEmpty())
			Amount = "0";

		OraLoader.GetSql() =  "UPDATE SEMAM.NW_TR_INV SET ACTUAL_VDATE = '" + Maturity + "', "
								"CURRENCY = '" + Currency + "', "
								"CASH_AMOUNT = " + Amount + ", "
								"INT_AMOUNT = 0, "
								"ASSIGN_CP = " + AssignCP + ", "
								"ASSIGN_CT = " + AssignCT + 
								" WHERE INV_NUM = " + m_InvNum;
	}
	else
		OraLoader.GetSql() = "UPDATE SEMAM.NW_TR_INV SET ACTUAL_VDATE = '" + Maturity + "', "
								"CURRENCY = '" + Currency + "' "
								"WHERE INV_NUM = " + m_InvNum;

	if(OraLoader.ExecuteSql())
	{
		OraLoader.GetSql() = "UPDATE SEMAM.NW_TR_TICKETS A "
								"SET MATURITY_DATE = '" + Maturity + "', "
								"ORIG_SW_MATURITY = '" + Maturity + "' "
								"WHERE A.TRANS_NUM = (SELECT TRANS_NUM FROM SEMAM.NW_TR_INV B "
														"WHERE B.INV_NUM = " + m_InvNum + ") ";

		if(OraLoader.ExecuteSql())
		{
			if(TransType == INTSWAP)
				m_InvCash.SetData(Amount);
			else
				m_InvCash.SetData(EMPTYSTRING);

			m_InvCurrency.SetData(Currency);
			m_InvValueDate.SetData(m_SetMaturity.GetData());
			OraLoader.GetSql() = "UPDATE SEMAM.NW_MATURITY_TRADE SET PROCESSED = 'Y' "
									"WHERE INV_NUM = " + m_InvNum;
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
		TransType = m_TransType.GetData();
		if(TransType == INTSWAP)
			pCmdUI->Enable(m_SetCurrency.GetCurSel() >= 0 && m_SetAmount.GetWindowTextLength() > 0);
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
		Currency = m_Currency.GetData();

	m_SetCurrency.SetData(Currency);
	SetupAssetInfo();
	ComputeValue();
}


void CMaturityTicket::OnEnChangeMaturitySetmaturityEdit()
{
	SetupAssetInfo();
	ComputeValue();
}
