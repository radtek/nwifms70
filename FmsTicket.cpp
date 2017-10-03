// FmsTicket.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "FmsTicket.h"
#include "assetdlg.h"
#include "Finddlg.h"
#include "invert.h"
#include "qdata.h"
#include "repoprice.h"
#include "optexpdlg.h"
#include "xcurrencydlg.h"
#include "setconventiondlg.h"
#include "otherfeedlg.h"
#include "FXCategoryDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int FMS_NORMAL = 1;
const int FMS_SEARCH = 0;

/////////////////////////////////////////////////////////////////////////////
// CFmsTicket

IMPLEMENT_DYNCREATE(CFmsTicket, CFormView)

CFmsTicket::CFmsTicket()
	: CFormView(CFmsTicket::IDD)
{
	//{{AFX_DATA_INIT(CFmsTicket)
	m_bSameDate = FALSE;
	//}}AFX_DATA_INIT
}

void CFmsTicket::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFmsTicket)
	DDX_Control(pDX, IDC_TICKET_TICKET_LIST, m_SS);
	DDX_Control(pDX, IDC_TICKET_INV_LIST, m_InvSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFmsTicket, CFormView)
	//{{AFX_MSG_MAP(CFmsTicket)
	ON_CBN_SELCHANGE(IDC_TICKET_PORTFOLIO_COMBO, OnSelchangeTicketPortfolioCombo)
	ON_CBN_SELCHANGE(IDC_TICKET_TYPE_COMBO, OnSelchangeTicketTypeCombo)
	ON_CBN_SELCHANGE(IDC_TICKET_DIR_COMBO, OnSelchangeTicketDirCombo)
	ON_EN_SETFOCUS(IDC_TICKET_CT_EDIT, OnSetfocusTicketCtEdit)
	ON_BN_CLICKED(IDC_TICKET_LISTED_CHECK, OnTicketListedCheck)
	ON_COMMAND(ID_TRANS_ADD, OnTransAdd)
	ON_UPDATE_COMMAND_UI(ID_TRANS_ADD, OnUpdateTransAdd)
	ON_UPDATE_COMMAND_UI(ID_TRANS_CLEAR_SCREEN, OnUpdateTransClearscreen)
	ON_COMMAND(ID_TRANS_CLEAR_SCREEN, OnTransClearscreen)
	ON_COMMAND(ID_TRANS_COPY, OnTransCopy)
	ON_UPDATE_COMMAND_UI(ID_TRANS_COPY, OnUpdateTransCopy)
	ON_COMMAND(ID_TRANS_DELETE, OnTransDelete)
	ON_UPDATE_COMMAND_UI(ID_TRANS_DELETE, OnUpdateTransDelete)
	ON_COMMAND(ID_TRANS_FIND_ASSET, OnTransFindasset)
	ON_UPDATE_COMMAND_UI(ID_TRANS_FIND_ASSET, OnUpdateTransFindasset)
	ON_COMMAND(ID_TRANS_FIND_TICKET, OnTransFindticket)
	ON_UPDATE_COMMAND_UI(ID_TRANS_FIND_TICKET, OnUpdateTransFindticket)
	ON_COMMAND(ID_TRANS_REFRESH, OnTransRefresh)
	ON_UPDATE_COMMAND_UI(ID_TRANS_REFRESH, OnUpdateTransRefresh)
	ON_COMMAND(ID_TRANS_UPDATE, OnTransUpdate)
	ON_UPDATE_COMMAND_UI(ID_TRANS_UPDATE, OnUpdateTransUpdate)
	ON_COMMAND(ID_TRANS_FIND_ITEM, OnTransFindItem)
	ON_UPDATE_COMMAND_UI(ID_TRANS_FIND_ITEM, OnUpdateTransFindItem)
	ON_EN_SETFOCUS(IDC_TICKET_DEAL_EDIT, OnSetfocusTicketDealEdit)
	ON_EN_CHANGE(IDC_TICKET_TRANSNUM_EDIT, OnChangeTicketTransNum)
	ON_UPDATE_COMMAND_UI(ID_TRANS_INVERT_PRICE, OnUpdateTransInvertPrice)
	ON_COMMAND(ID_TRANS_INVERT_PRICE, OnTransInvertPrice)
	ON_COMMAND(ID_TRANS_REPOPRICE, OnTransRepoPrice)
	ON_UPDATE_COMMAND_UI(ID_TRANS_REPOPRICE, OnUpdateTransRepoPrice)
	ON_EN_SETFOCUS(IDC_TICKET_INV_ASSET_EDIT, OnSetfocusTicketInvAssetEdit)
	ON_EN_SETFOCUS(IDC_TICKET_ASSET_EDIT, OnSetfocusTicketAssetEdit)
	ON_UPDATE_COMMAND_UI(ID_TRANS_OPTION_EXP, OnUpdateTransOptionExp)
	ON_COMMAND(ID_TRANS_OPTION_EXP, OnTransOptionExp)
	ON_BN_CLICKED(IDC_TICKET_TRS_CHECK, OnTicketTrsCheck)
	ON_EN_SETFOCUS(IDC_TICKET_INV_ASSIGNCT_EDIT, OnSetfocusTicketInvAssignctEdit)
	ON_COMMAND(ID_TRANS_XCURRENCY, OnTransXcurrency)
	ON_COMMAND(ID_TRANS_FIND_EXACT_TICKET, OnTransFindExactTicket)
	ON_UPDATE_COMMAND_UI(ID_TRANS_FIND_EXACT_TICKET, OnUpdateTransFindExactTicket)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_TICKET_INV_FOREX_REF_EDIT, &CFmsTicket::OnEnSetfocusTicketInvForexRefEdit)
	ON_EN_SETFOCUS(IDC_TICKET_SETCONVENTION_EDIT, &CFmsTicket::OnEnSetfocusTicketSetconventionEdit)
	ON_UPDATE_COMMAND_UI(ID_TRANS_OTHERFEE, &CFmsTicket::OnUpdateTransOtherfee)
	ON_COMMAND(ID_TRANS_OTHERFEE, &CFmsTicket::OnTransOtherfee)
	ON_EN_SETFOCUS(IDC_TICKET_BINARY_EDIT, &CFmsTicket::OnEnSetfocusTicketBinaryEdit)
	ON_COMMAND(ID_TRANS_CC, &CFmsTicket::OnTransCc)
	ON_UPDATE_COMMAND_UI(ID_TRANS_CC, &CFmsTicket::OnUpdateTransCc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmsTicket diagnostics

#ifdef _DEBUG
void CFmsTicket::AssertValid() const
{
	CFormView::AssertValid();
}

void CFmsTicket::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CFmsTicket::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNWIFMS70Doc)));
	return (CNWIFMS70Doc*)m_pDocument;
}
#endif //_DEBUG

void CFmsTicket::ChangeShortLabel()
{
	CString Dir;

	Dir = m_Dir.GetData();

	if(Dir == P)
		m_ShortSale.SetWindowText("CShort");
	else
		if(Dir == S)
			m_ShortSale.SetWindowText("SShort");
}

void CFmsTicket::EnableCtrls()
{
	CString TransType, Dir, TradeDate;
	BOOL bReadOnly = TRUE, bSearch;
	int nCmdShow = SW_HIDE;

	if(GetData().IsPowerUser() || m_Mode == FMS_SEARCH)
		bReadOnly = FALSE;
	else
		if(m_Portfolio.GetCurSel() >= 0)
		{
			if(m_bInCurrMonth && GetData().IsOperation() || GetData().IsGuest())
				bReadOnly = FALSE;
		}

	bSearch = m_Mode == FMS_SEARCH ? TRUE : FALSE;

	m_TransNum.SetReadOnly(!bSearch);
	m_Ticket.SetReadOnly(!bSearch);
	m_TradeDate.SetReadOnly(bReadOnly);
	m_ValueDate.SetReadOnly(bReadOnly);
	m_Trader.EnableWindow(!bReadOnly);
	m_TransType.SetReadOnly(bReadOnly);
	m_Dir.EnableWindow(!bReadOnly);
	m_Amount.SetReadOnly(bReadOnly);
	m_Currency.SetReadOnly(bReadOnly);
	m_Fxrate.SetReadOnly(bReadOnly);
	m_PFU.SetReadOnly(bReadOnly);
	m_CT.SetReadOnly(bReadOnly);
	m_Custodian.SetReadOnly(bReadOnly);
	m_BestExecution.SetReadOnly(bReadOnly);
	TransType = m_TransType.GetData();
	Dir = m_Dir.GetData();
	TransType = GetData().DefineTransType(TransType, Dir);

	m_Price.SetReadOnly(bReadOnly);
	m_Strike.SetReadOnly(bReadOnly);
	m_OptExp.SetReadOnly(bReadOnly);
	m_FXDate.SetReadOnly(bReadOnly);
	m_OptTicker.SetReadOnly(bReadOnly);
	m_OptID.SetReadOnly(bReadOnly);
	m_DeliveryDate.SetReadOnly(bReadOnly);
	m_BrFee.SetReadOnly(bReadOnly);
	m_SoftDollar.SetReadOnly(bReadOnly);
	m_OtherFee.SetReadOnly(bReadOnly);
	m_VAR.SetReadOnly(bReadOnly);
	m_Seq.SetReadOnly(bReadOnly);
	m_Asset.SetReadOnly(bReadOnly);
	m_Price.SetReadOnly(bReadOnly);
	m_SetConvention.SetReadOnly(bReadOnly);
	m_Rate.SetReadOnly(bReadOnly);
	m_RateBasis.SetReadOnly(bReadOnly);
	m_Formula.SetReadOnly(bReadOnly);
	m_Maturity.SetReadOnly(bReadOnly);
	m_OriBooking.SetReadOnly(bReadOnly);
	m_USDAcct.SetReadOnly(bReadOnly);
	m_FxAcct.SetReadOnly(bReadOnly);
	m_CancelTicket.SetReadOnly(bReadOnly);
	m_CorrectTicket.SetReadOnly(bReadOnly);
	m_DV01.SetReadOnly(bReadOnly);
	m_Margin.SetReadOnly(bReadOnly);
	m_MarginCurrency.SetReadOnly(bReadOnly);
	m_MarginAmount.SetReadOnly(bReadOnly);
	m_AssetDesc.SetReadOnly(bReadOnly);
	m_AssetCurr.SetReadOnly(bReadOnly);
	m_SoftDollar.SetReadOnly(bReadOnly);

	m_EuroOpt.SetReadOnly(bReadOnly);
	m_OptAuto.SetReadOnly(bReadOnly);
	m_Binary.SetReadOnly(bReadOnly);
	m_Listed.SetReadOnly(bReadOnly);
	m_OrFee.SetReadOnly(bReadOnly);
	m_IPO.SetReadOnly(bReadOnly);
	m_TRS.SetReadOnly(bReadOnly);
	m_ShortSale.SetReadOnly(bReadOnly);
	m_CSPBShort.SetReadOnly(bReadOnly);
	m_Funded.SetReadOnly(bReadOnly);
	m_SecFee.SetReadOnly(bReadOnly);
	m_WI.SetReadOnly(bReadOnly);

	nCmdShow = (bSearch || (m_Mode == FMS_NORMAL && TransType != FOREX)) ? SW_SHOW : SW_HIDE; // Forex Type Impact
	GetDlgItem(IDC_TICKET_PRICE_STATIC)->ShowWindow(nCmdShow);
	m_Price.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_ASSETCURRENCY_STATIC)->ShowWindow(nCmdShow);
	m_AssetCurr.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_SWBOOKING_STATIC)->ShowWindow(nCmdShow);
	m_OriBooking.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_DV01_STATIC)->ShowWindow(nCmdShow);
	m_DV01.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_MARGIN_STATIC)->ShowWindow(nCmdShow);
	m_Margin.ShowWindow(nCmdShow);
	
	m_MarginCurrency.ShowWindow(nCmdShow);
	m_MarginAmount.ShowWindow(nCmdShow);
	if(nCmdShow == SW_HIDE && TransType == FOREX && m_Asset.GetWindowTextLength() > 0)
		nCmdShow = SW_SHOW;	
	GetDlgItem(IDC_TICKET_ASSET_STATIC)->ShowWindow(nCmdShow);
	m_Asset.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_ASSETDESC_STATIC)->ShowWindow(nCmdShow);
	m_AssetDesc.ShowWindow(nCmdShow);

	nCmdShow = (bSearch || (m_Mode == FMS_NORMAL && TransType == FOREX)) ? SW_SHOW : SW_HIDE; // Forex Type Impact
	GetDlgItem(IDC_TICKET_USDACCOUNT_STATIC)->ShowWindow(nCmdShow);
	m_USDAcct.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_FXACCOUNT_STATIC)->ShowWindow(nCmdShow);
	m_FxAcct.ShowWindow(nCmdShow); 
	
	nCmdShow = (bSearch || (TransType == CALL || TransType == PUT)) ? SW_SHOW : SW_HIDE;  // Option Impact
	m_EuroOpt.ShowWindow(nCmdShow);
	m_OptAuto.ShowWindow(nCmdShow);
	m_Binary.ShowWindow(nCmdShow);
	m_Listed.ShowWindow(nCmdShow);
	m_OrFee.ShowWindow(m_Listed.GetCheck() && nCmdShow == SW_SHOW ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_TICKET_STRIKE_STATIC)->ShowWindow(nCmdShow);
	m_Strike.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_EXERCISEDATE_STATIC)->ShowWindow(nCmdShow);
	m_ExerciseDate.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_EXPIRATION_STATIC)->ShowWindow(nCmdShow);
	m_OptExp.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_FXDATE_STATIC)->ShowWindow(nCmdShow);
	m_FXDate.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_OPTTICK_STATIC)->ShowWindow(nCmdShow);
	m_OptTicker.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_OPTSET_STATIC)->ShowWindow(nCmdShow);
	m_OptSet.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_OPTSET2_STATIC)->ShowWindow(nCmdShow);
	m_OptSet2.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_OPTID_STATIC)->ShowWindow(nCmdShow);
	m_OptID.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_DELIVERY_STATIC)->ShowWindow(nCmdShow);
	m_DeliveryDate.ShowWindow(nCmdShow);

	nCmdShow = (bSearch || (TransType == CALL || TransType == PUT || TransType == SECURITIES)) ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TICKET_DEAL_STATIC)->ShowWindow(nCmdShow);
	m_Link.ShowWindow(nCmdShow);

	// Leverage, Repo, IRS Impact
	nCmdShow = (bSearch || (TransType == LEVERAGE || TransType == REPO || TransType == INTSWAP)) ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TICKET_RATE_STATIC)->ShowWindow(nCmdShow);
	m_Rate.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_RATEBASIS_STATIC)->ShowWindow(nCmdShow);
	m_RateBasis.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_MATURITY_STATIC)->ShowWindow(nCmdShow);
	m_Maturity.ShowWindow(nCmdShow);
	
	// Leverage, Repo Impact
	nCmdShow = (bSearch || (TransType == LEVERAGE || TransType == REPO)) ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TICKET_FORMULA_STATIC)->ShowWindow(nCmdShow);
	m_Formula.ShowWindow(nCmdShow);
	if(TransType == LEVERAGE)
		m_TRS.ShowWindow(nCmdShow);

	// IRS Impact
	nCmdShow = (bSearch || (TransType == INTSWAP)) ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TICKET_INDEX_STATIC)->ShowWindow(nCmdShow);
	m_Index.ShowWindow(nCmdShow);
	GetDlgItem(IDC_TICKET_SWMATURITY_STATIC)->ShowWindow(nCmdShow);
	m_OriMaturity.ShowWindow(nCmdShow);

	// Currency Fwds Impact
	nCmdShow = (bSearch || (TransType == SECURITIES && m_Data.GetTicket().GetAssetClass() == "CURRENCY FWDS")) ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TICKET_SWAP_TICKET_STATIC)->ShowWindow(nCmdShow);
	m_SwapTicket.ShowWindow(nCmdShow);

	nCmdShow = (bSearch || (TransType == SECURITIES && m_Data.GetTicket().GetAssetClass() == "CURRENCY FWDS") || 
		((TransType == CALL || TransType == PUT) && m_Data.GetTicket().GetAssetClass() == "CURRENCY OPTION")) ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TICKET_SETCONVENTION_STATIC)->ShowWindow(nCmdShow);
	m_SetConvention.ShowWindow(nCmdShow);

	// Securities Impact
	nCmdShow = (bSearch || (TransType == SECURITIES)) ? SW_SHOW : SW_HIDE;
	m_IPO.ShowWindow(nCmdShow);
	m_TRS.ShowWindow(nCmdShow);

	// Call, Put, CDS, Securities Impact
	nCmdShow = (bSearch || (TransType == CALL || TransType == CDS || TransType == PUT || TransType == SECURITIES)) ? SW_SHOW : SW_HIDE;
	m_ShortSale.ShowWindow(nCmdShow);
	m_CSPBShort.ShowWindow(nCmdShow);
	m_WI.ShowWindow(nCmdShow);
	m_SecFee.ShowWindow(Dir == S && nCmdShow == SW_SHOW ? SW_SHOW : SW_HIDE);

	nCmdShow = (bSearch || (TransType == CDS || TransType == INTSWAP)) ? SW_SHOW : SW_HIDE;
	m_Funded.ShowWindow(nCmdShow);

	return;
}

void CFmsTicket::EnableInvCtrls()
{
	BOOL bReadOnly = TRUE;

	if(GetData().IsPowerUser() || m_Mode == FMS_SEARCH)
		bReadOnly = FALSE;
	else
		if(m_bInCurrMonth && GetData().IsOperation() || GetData().IsGuest())
		{
			if(!m_TransNum.GetData().IsEmpty() && m_InvCloseDate.GetData().IsEmpty())
				bReadOnly = FALSE;
		}

	m_InvAsset.SetReadOnly(bReadOnly);
	m_InvTrDesc.SetReadOnly(bReadOnly);
	m_InvAmount.SetReadOnly(bReadOnly);
	m_InvValueDate.SetReadOnly(bReadOnly);
	m_InvCloseDate.SetReadOnly(bReadOnly);
	m_InvPostDate.SetReadOnly(bReadOnly);
	m_InvForexRef.SetReadOnly(bReadOnly);
	m_InvCash.SetReadOnly(bReadOnly);
	m_InvInterest.SetReadOnly(bReadOnly);
	m_InvAccount.SetReadOnly(bReadOnly);
	m_InvAssignCT.SetReadOnly(bReadOnly);
	m_InvCurrency.SetReadOnly(bReadOnly);
	m_InvCrossRate.SetReadOnly(bReadOnly);
}

void CFmsTicket::InitControls()
{
	m_SS.SetVisibleCols(9);
	m_SS.SetVisibleRows(5);
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);

	m_InvSS.SetVisibleCols(9);
	m_InvSS.SetVisibleRows(3);
	m_InvSS.LockSheet();
	m_InvSS.SetSheetRows(0);
	
	m_Listed.Setup(this, IDC_TICKET_LISTED_CHECK);
	m_IPO.Setup(this, IDC_TICKET_IPO_CHECK);
	m_TRS.Setup(this, IDC_TICKET_TRS_CHECK, TRS);
	m_EuroOpt.Setup(this, IDC_TICKET_EUROOPT_CHECK);
	m_DocRq.Setup(this, IDC_TICKET_DOCREQ_CHECK);
	m_DocRecvd.Setup(this, IDC_TICKET_DOCRCVD_CHECK);
	m_WI.Setup(this, IDC_TICKET_WI_CHECK, Y);
	m_Confirm.Setup(this, IDC_TICKET_CONFIRM_CHECK, Y);
	m_OptAuto.Setup(this, IDC_TICKET_AUTO_CHECK, Y);
	m_Funded.Setup(this, IDC_TICKET_FUNDED_CHECK, Y);
	m_CSPBShort.Setup(this, IDC_TICKET_CSPBSHORT_CHECK, Y);
	m_ShortSale.Setup(this, IDC_TICKET_SHORT_CHECK, Y);
	m_SecFee.Setup(this, IDC_TICKET_SECFEE_CHECK, Y);
	m_OrFee.Setup(this, IDC_TICKET_ORFEE_CHECK, Y);

    m_Portfolio.Setup(this, IDC_TICKET_PORTFOLIO_COMBO);
	m_TransType.Setup(this, IDC_TICKET_TYPE_COMBO);
	m_CP.Setup(this, IDC_TICKET_CP_COMBO);
	m_PFU.Setup(this, IDC_TICKET_PFU_COMBO);
    m_Currency.Setup(this, IDC_TICKET_CURRENCY_COMBO);
    m_RateBasis.Setup(this, IDC_TICKET_RATEBASIS_COMBO, TRUE);
	m_Formula.Setup(this, IDC_TICKET_FORMULA_COMBO, TRUE, TRUE);
    m_Custodian.Setup(this, IDC_TICKET_CUSTODIAN_COMBO, TRUE);
    m_USDAcct.Setup(this, IDC_TICKET_USDACCOUNT_COMBO, TRUE);
    m_FxAcct.Setup(this, IDC_TICKET_FXACCOUNT_COMBO, TRUE);
	m_MarginCurrency.Setup(this, IDC_TICKET_MARGIN_CURRENCY_COMBO, TRUE);
	m_BestExecution.Setup(this, IDC_TICKET_BESTEXECUTION_COMBO);

	m_Ticket.Setup(this, IDC_TICKET_TICKET_EDIT);
	m_Ticket.SetReadOnly();
	m_TradeDate.Setup(this, IDC_TICKET_TRADEDATE_EDIT);
	m_ValueDate.Setup(this, IDC_TICKET_VALUEDATE_EDIT);
	m_Amount.Setup(this, IDC_TICKET_NOMINAL_EDIT);
	m_Fxrate.Setup(this, IDC_TICKET_FXRATE_EDIT);
	m_BrFee.Setup(this, IDC_TICKET_COMMISSION_EDIT, NULL, 2);
	m_SoftDollar.Setup(this, IDC_TICKET_SOFTDOLLAR_EDIT, NULL, 2);
	m_OtherFee.Setup(this, IDC_TICKET_OTHERFEE_EDIT, NULL, 2);
	m_VAR.Setup(this, IDC_TICKET_VAR_EDIT, NULL, 2);
	m_Price.Setup(this, IDC_TICKET_PRICE_EDIT);
	m_CT.Setup(this, IDC_TICKET_CT_EDIT, EMPTYSTRING, NOCASE);
	m_CT.LimitText(3);
	m_Note.Setup(this, IDC_TICKET_NOTE_EDIT, EMPTYSTRING, NOCASE);
	m_Note.LimitText(30);
	m_Note2.Setup(this, IDC_TICKET_NOTE2_EDIT, EMPTYSTRING, NOCASE);
	m_Note2.LimitText(30);
	m_Binary.Setup(this, IDC_TICKET_BINARY_EDIT);
	m_Binary.LimitText(30);
	m_Rate.Setup(this, IDC_TICKET_RATE_EDIT);

	m_Maturity.Setup(this, IDC_TICKET_MATURITY_EDIT, TRUE,  
					"Maturity Date must be greater than or equal to Trade Date", &m_TradeDate);
	m_OriMaturity.Setup(this, IDC_TICKET_SWMATURITY_EDIT, TRUE,  
						"Swap Maturity Date must be greater than or equal to Swap Booking Date", &m_OriBooking);
	m_Strike.Setup(this, IDC_TICKET_STRIKE_EDIT);
	m_ExerciseDate.Setup(this, IDC_TICKET_EXERCISEDATE_EDIT, TRUE, 
						"Exercise Date must be greater than or equal to Trade Date", &m_TradeDate);
	m_OptExp.Setup(this, IDC_TICKET_EXPIRATION_EDIT, TRUE, 
					"Option Expiration Date must be greater than or equal to Trade Date", &m_TradeDate);
	m_FXDate.Setup(this, IDC_TICKET_FXDATE_EDIT, TRUE, 
					"NDF FX Fix Date must be greater than or equal to Trade Date", &m_TradeDate);
	m_DeliveryDate.Setup(this, IDC_TICKET_DELIVERY_EDIT, TRUE, 
						"Option Delivery Date must be greater than or equal to Option Expiration Date", &m_OptExp);

	m_Dir.Setup(this, IDC_TICKET_DIR_COMBO, 2);
	m_Trader.Setup(this, IDC_TICKET_TRADER_COMBO, 2);
	m_Index.Setup(this, IDC_TICKET_INDEX_COMBO, 2, TRUE);
	m_OptSet.Setup(this, IDC_TICKET_OPTSET_COMBO, 2, TRUE);
	m_OptSet2.Setup(this, IDC_TICKET_OPTSET2_COMBO, 2, TRUE);
	m_ETrade.Setup(this, IDC_TICKET_ETRADE_COMBO, 2, TRUE);

	m_Asset.Setup(this, IDC_TICKET_ASSET_EDIT, EMPTYSTRING, UPPERCASE);
	m_Asset.SetLimitText(10);
	m_AssetDesc.Setup(this, IDC_TICKET_ASSETDESC_EDIT);
	m_AssetCurr.Setup(this, IDC_TICKET_ASSETCURRENCY_EDIT);
	m_OriBooking.Setup(this, IDC_TICKET_SWBOOKING_EDIT);
	m_OptTicker.Setup(this, IDC_TICKET_OPTTICK_EDIT);
	m_OptTicker.LimitText(25);
	m_OptID.Setup(this, IDC_TICKET_OPTID_EDIT);
	m_OptID.LimitText(25);
	m_SetConvention.Setup(this, IDC_TICKET_SETCONVENTION_EDIT);
	m_Link.Setup(this, IDC_TICKET_DEAL_EDIT);
	m_Seq.Setup(this, IDC_TICKET_SEQ_EDIT, 2);
	m_CPTradeID.Setup(this, IDC_TICKET_CP_ID_EDIT);
	m_CPTradeID.LimitText(20);
	m_Margin.Setup(this, IDC_TICKET_MARGIN_EDIT);
	m_MarginAmount.Setup(this, IDC_TICKET_MARGIN_AMOUNT_EDIT);
	m_DV01.Setup(this, IDC_TICKET_DV01_EDIT);
	m_SwapTicket.Setup(this, IDC_TICKET_SWAP_TICKET_EDIT);
	m_SwapTicket.LimitText(10);
	m_CancelTicket.Setup(this, IDC_TICKET_CANCEL_TICKET_EDIT);
	m_CorrectTicket.Setup(this, IDC_TICKET_CORRECT_TICKET_EDIT);
	m_TransNum.Setup(this, IDC_TICKET_TRANSNUM_EDIT);
	m_TransNum.SetReadOnly();

	m_InvAsset.Setup(this, IDC_TICKET_INV_ASSET_EDIT);
	m_InvAssignCT.Setup(this, IDC_TICKET_INV_ASSIGNCT_EDIT);
	m_InvValueDate.Setup(this, IDC_TICKET_INV_VALUEDATE_EDIT);
	m_InvCloseDate.Setup(this, IDC_TICKET_INV_CLOSE_DATE_EDIT);
	m_InvPostDate.Setup(this, IDC_TICKET_INV_POSTDATE_EDIT);
	m_InvForexRef.Setup(this, IDC_TICKET_INV_FOREX_REF_EDIT);
	m_InvCrossRate.Setup(this, IDC_TICKET_INV_CROSS_RATE_EDIT);
	m_InvAmount.Setup(this, IDC_TICKET_INV_NOMINAL_EDIT);
	m_InvCash.Setup(this, IDC_TICKET_INV_CASH_EDIT, NULL, 2);
	m_InvInterest.Setup(this, IDC_TICKET_INV_INTEREST_EDIT, NULL, 2);
	m_InvTrDesc.Setup(this, IDC_TICKET_INV_TRDESC_COMBO);
	m_InvAccount.Setup(this, IDC_TICKET_INV_ACCOUNT_COMBO, TRUE);
	m_InvCurrency.Setup(this, IDC_TICKET_INV_CURRENCY_COMBO, TRUE);
	m_InvAssignCP.Setup(this, IDC_TICKET_INV_ASSIGNCP_COMBO, TRUE);

	m_RecCurrency.Setup(this, IDC_TICKET_INV_REC_CURRENCY_EDIT);
	m_RecInterest.Setup(this, IDC_TICKET_INV_REC_INT_EDIT, NULL, 2);
	m_RecAccretion.Setup(this, IDC_TICKET_INV_REC_ACCRETION_EDIT, NULL, 2);
	m_RecCash.Setup(this, IDC_TICKET_INV_REC_CASH_EDIT, NULL, 2);
	m_RecTotal.Setup(this, IDC_TICKET_INV_REC_TOTAL_EDIT, NULL, 2);

	m_Data.Add(&m_TransNum, &m_Data.GetTicket().GetTransNum());
	m_Data.Add(&m_Portfolio, &m_Data.GetTicket().GetPortfolio());
	m_Data.Add(&m_Ticket, &m_Data.GetTicket().GetTicket());
	m_Data.Add(&m_CP, &m_Data.GetTicket().GetCP());
	m_Data.Add(&m_Asset, &m_Data.GetTicket().GetAsset());
	m_Data.Add(&m_Trader, &m_Data.GetTicket().GetTrader());
	m_Data.Add(&m_TradeDate, &m_Data.GetTicket().GetTradeDate());
	m_Data.Add(&m_ValueDate, &m_Data.GetTicket().GetValueDate());
	m_Data.Add(&m_TransType, &m_Data.GetTicket().GetTransType());
	m_Data.Add(&m_Dir, &m_Data.GetTicket().GetDir());
	m_Data.Add(&m_Amount, &m_Data.GetTicket().GetAmount());
	m_Data.Add(&m_Price, &m_Data.GetTicket().GetPrice());
	m_Data.Add(&m_Currency, &m_Data.GetTicket().GetCurrency());
	m_Data.Add(&m_Fxrate, &m_Data.GetTicket().GetFxRate());
	m_Data.Add(&m_CT, &m_Data.GetTicket().GetCT());
	m_Data.Add(&m_TRS, &m_Data.GetTicket().GetTRS());
	m_Data.Add(&m_Strike, &m_Data.GetTicket().GetStrike());
	m_Data.Add(&m_ExerciseDate, &m_Data.GetTicket().GetExerciseDate());
	m_Data.Add(&m_OptExp, &m_Data.GetTicket().GetOptExp());
	m_Data.Add(&m_FXDate, &m_Data.GetTicket().GetFXDate());
	m_Data.Add(&m_OptTicker, &m_Data.GetTicket().GetOptTicker());
	m_Data.Add(&m_OptSet, &m_Data.GetTicket().GetOptSet());
	m_Data.Add(&m_OptSet2, &m_Data.GetTicket().GetOptSet2());
	m_Data.Add(&m_SetConvention, &m_Data.GetTicket().GetSetConvention());
	m_Data.Add(&m_OptID, &m_Data.GetTicket().GetOptID());
	m_Data.Add(&m_EuroOpt, &m_Data.GetTicket().GetEuroOpt());
	m_Data.Add(&m_OptAuto, &m_Data.GetTicket().GetOptAuto());
	m_Data.Add(&m_BrFee, &m_Data.GetTicket().GetBrokerFee());
	m_Data.Add(&m_SoftDollar, &m_Data.GetTicket().GetSoftDollar());
	m_Data.Add(&m_OtherFee, &m_Data.GetTicket().GetOtherFee());
	m_Data.Add(&m_VAR, &m_Data.GetTicket().GetVAR());
	m_Data.Add(&m_Rate, &m_Data.GetTicket().GetRate());
	m_Data.Add(&m_RateBasis, &m_Data.GetTicket().GetRateBasis());
	m_Data.Add(&m_Maturity, &m_Data.GetTicket().GetMaturity());
	m_Data.Add(&m_Index, &m_Data.GetTicket().GetIndex());
	m_Data.Add(&m_Formula, &m_Data.GetTicket().GetFormula());
	m_Data.Add(&m_OriBooking, &m_Data.GetTicket().GetSWBooking());
	m_Data.Add(&m_OriMaturity, &m_Data.GetTicket().GetSWMaturity());
	m_Data.Add(&m_FxAcct, &m_Data.GetTicket().GetFxAcc());
	m_Data.Add(&m_USDAcct, &m_Data.GetTicket().GetUSDAcc());
	m_Data.Add(&m_PFU, &m_Data.GetTicket().GetPFU());
	m_Data.Add(&m_IPO, &m_Data.GetTicket().GetIPO());
	m_Data.Add(&m_Custodian, &m_Data.GetTicket().GetCustodian());
	m_Data.Add(&m_Link, &m_Data.GetTicket().GetLink());
	m_Data.Add(&m_Seq, &m_Data.GetTicket().GetTransSeq());
	m_Data.Add(&m_Note, &m_Data.GetTicket().GetNote());
	m_Data.Add(&m_Note2, &m_Data.GetTicket().GetNote2());
	m_Data.Add(&m_CPTradeID, &m_Data.GetTicket().GetCPID());
	m_Data.Add(&m_DocRq, &m_Data.GetTicket().GetDocReq());
	m_Data.Add(&m_DocRecvd, &m_Data.GetTicket().GetDocRecvd());
	m_Data.Add(&m_WI, &m_Data.GetTicket().GetWI());
	m_Data.Add(&m_Confirm, &m_Data.GetTicket().GetConfirm());
	m_Data.Add(&m_SwapTicket, &m_Data.GetTicket().GetSwapTicket());
	m_Data.Add(&m_DeliveryDate, &m_Data.GetTicket().GetDeliveryDate());
	m_Data.Add(&m_Margin, &m_Data.GetTicket().GetMargin());
	m_Data.Add(&m_MarginCurrency, &m_Data.GetTicket().GetMarginCurrency());
	m_Data.Add(&m_MarginAmount, &m_Data.GetTicket().GetMarginAmount());
	m_Data.Add(&m_Listed, &m_Data.GetTicket().GetListed());
	m_Data.Add(&m_Funded, &m_Data.GetTicket().GetFunded());
	m_Data.Add(&m_Binary, &m_Data.GetTicket().GetBinary());
	m_Data.Add(&m_DV01, &m_Data.GetTicket().GetDV01());
	m_Data.Add(&m_BestExecution, &m_Data.GetTicket().GetBestExecution());
	m_Data.Add(&m_ShortSale, &m_Data.GetTicket().GetShortSale());
	m_Data.Add(&m_CSPBShort, &m_Data.GetTicket().GetCSPBShort());
	m_Data.Add(&m_CancelTicket, &m_Data.GetTicket().GetCancelTicket());
	m_Data.Add(&m_CorrectTicket, &m_Data.GetTicket().GetCorrectTicket());
	m_Data.Add(&m_SecFee, &m_Data.GetTicket().GetSecFee());
	m_Data.Add(&m_OrFee, &m_Data.GetTicket().GetOrFee());
	m_Data.Add(&m_ETrade, &m_Data.GetTicket().GetETrade());
	m_Data.Add(&m_Data.GetTicket().GetBooker());
	m_Data.Add(&m_Data.GetTicket().GetBookDate());
	m_Data.Add(&m_Data.GetTicket().GetVersion());
	m_Data.Add(&m_AssetDesc, &m_Data.GetTicket().GetAssetDesc());
	m_Data.Add(&m_AssetCurr, &m_Data.GetTicket().GetAssetCurr());
	m_Data.Add(&m_Data.GetTicket().GetAssetClass()); 

	m_Data.GetSRowCtrl().Add(&m_InvAsset, &m_Data.GetInv().GetAsset());
	m_Data.GetSRowCtrl().Add(&m_InvTrDesc, &m_Data.GetInv().GetTrDesc());
	m_Data.GetSRowCtrl().Add(&m_InvAmount, &m_Data.GetInv().GetNomAmount());
	m_Data.GetSRowCtrl().Add(&m_InvValueDate, &m_Data.GetInv().GetValueDate());
	m_Data.GetSRowCtrl().Add(&m_InvCloseDate, &m_Data.GetInv().GetCloseDate());
	m_Data.GetSRowCtrl().Add(&m_InvPostDate, &m_Data.GetInv().GetPostDate());
	m_Data.GetSRowCtrl().Add(&m_InvCurrency, &m_Data.GetInv().GetCurrency());
	m_Data.GetSRowCtrl().Add(&m_InvForexRef, &m_Data.GetInv().GetForexRef());
	m_Data.GetSRowCtrl().Add(&m_InvCrossRate, &m_Data.GetInv().GetCrossRate());
	m_Data.GetSRowCtrl().Add(&m_InvCash, &m_Data.GetInv().GetCash());
	m_Data.GetSRowCtrl().Add(&m_InvInterest, &m_Data.GetInv().GetInterest());
	m_Data.GetSRowCtrl().Add(&m_InvAccount, &m_Data.GetInv().GetAccount());
	m_Data.GetSRowCtrl().Add(&m_InvAssignCP, &m_Data.GetInv().GetAssignCP());
	m_Data.GetSRowCtrl().Add(&m_InvAssignCT, &m_Data.GetInv().GetAssignCT());
}

BOOL CFmsTicket::IsOK()
{
	CString Text, TransType, Dir, Portfolio, Asset, Link, Data;
	COraLoader OraLoader;
	CQData QData;
	
	OraLoader = GetData().GetOraLoader();
	TransType = m_TransType.GetData();
	if(TransType.IsEmpty())
		Text = "Invalid Trans Type";
	
	Dir = m_Dir.GetData();
	TransType = GetData().DefineTransType(TransType, Dir);
	if(Dir.IsEmpty())
		Text = "Invalid Dir";

	if(m_TradeDate.GetData().IsEmpty())
		Text = "Invalid Trade Date";

	if((TransType == REPO || TransType == LEVERAGE || TransType == SECURITIES || 
		TransType == CDS || TransType == CALL || TransType == PUT || 
		TransType == SPRSWCLL || TransType == SPRSWPUT) && m_Asset.GetWindowTextLength() == 0)
		Text = "Invalid Asset";

	if(m_Amount.GetWindowTextLength() == 0)
		Text = "Invalid Nominal Amount";
											 										 	
	if(TransType == CALL || TransType == PUT ||	TransType == SPRSWCLL || TransType == SPRSWPUT) 
	{
		if(m_OptExp.GetData().IsEmpty())
			Text = "Invalid Option Expiration";
		
		if(m_Listed.GetCheck() && m_OptTicker.GetWindowTextLength() == 0)
			Text = "Invalid Option Ticker";
	}

	if(TransType == INTSWAP && m_DV01.GetWindowTextLength() == 0)
		Text = "Invalid DV01";

	if((TransType == REPO || TransType == LEVERAGE || TransType == CASH) && m_RateBasis.GetCurSel() < 0)
		Text = "Invalid Rate Basis";

	if(m_Ticket.GetData().IsEmpty())
		Text = "Invalid Text";

	if(m_CP.GetCurSel() < 0)
		Text = "Invalid Conterparty";

	if(m_CT.GetWindowTextLength() == 0)
		Text = "Invalid Contact";

	if(m_PFU.GetCurSel() < 0) 
		Text = "Invalid PFU";

	Asset = m_Asset.GetData();
	Portfolio = QData.GetQueryText(m_Portfolio.GetData());
	if(!m_Link.GetData().IsEmpty() && !Asset.IsEmpty() &&
		(TransType == SECURITIES || TransType == CALL || TransType == PUT))
	{
		Asset = QData.GetQueryText(Asset);
		Link = m_Link.GetData();

		OraLoader.GetSql() = "SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS A "
							"WHERE A.PORTFOLIO = " + Portfolio + 
							" AND A.ASSET_CODE = " + Asset + 
							" AND A.TRANS_NUM = " + QData.GetQueryNumber(Link) + 
							" AND A.TRANS_TYPE IN ('CALL', 'PUT') ";

		if(TransType == CALL || TransType == PUT)
		{
			OraLoader.GetSql() += " AND A.TRANS_TYPE = ";
			OraLoader.GetSql() += QData.GetQueryText(TransType);
			OraLoader.GetSql() += " AND A.TRANS_DIRECTION != ";
			OraLoader.GetSql() += QData.GetQueryText(Dir);
			
			OraLoader.GetSql() += " AND EXERCISE_PRICE = ";
			OraLoader.GetSql() += QData.GetQueryNumber(m_Strike.GetData());

			OraLoader.GetSql() += " AND OPT_EXPIRATION = ";
			OraLoader.GetSql() += QData.GetQueryDate(m_OptExp.GetData());
		
			if(m_Listed.GetCheck())
				OraLoader.GetSql() += " AND LISTED = 'Y' ";

			if(m_EuroOpt.GetCheck())
				OraLoader.GetSql() += " AND EURO_OPT = 'Y' ";
		}

		if(OraLoader.GetCount() <= 0)
			Text = "Invalid Option Link";
	}

	if(m_BestExecution.GetCurSel() < 0)
		Text = "Invalid Best Execution";

	if(!m_InvForexRef.GetData().IsEmpty())
	{
		Data = m_Currency.GetData();
		
		if(!Data.IsEmpty())
		{
			Data = QData.GetQueryText(Data);
			if(OraLoader.GetCount("SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS "
								"WHERE PORTFOLIO = " + Portfolio + 
								" AND TRANS_TYPE = 'FOREX' "
								" AND CURRENCY = " + Data + 
								" AND TRANS_NUM = " + QData.RemoveComma(m_InvForexRef.GetData())) <= 0)
				Text = "Invalid Forex trade";
		}
	}

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}

void CFmsTicket::SetInCurrMonth()
{
	COraLoader OraLoader;
	CString TradeDate;

	TradeDate = m_TradeDate.GetData();
	OraLoader = GetData().GetOraLoader();
	m_bInCurrMonth = OraLoader.InCurrentMonth(TradeDate);
}

void CFmsTicket::SetMode(int Mode)
{
	m_Mode = Mode;
	switch(m_Mode)
	{
		case FMS_NORMAL:
			GetDocument()->SetViewTitle("TRANSACTION PROCESS");
			break;
		case FMS_SEARCH:
			GetDocument()->SetViewTitle("TRANSACTION PROCESS - SEARCH");
			break;
		default:
			break;
	}
}

void CFmsTicket::SetSameDate()
{
	COraLoader OraLoader;
	CString Today;

	OraLoader = GetData().GetOraLoader();
	OraLoader.Today(Today);
	m_bSameDate = (m_Data.GetTicket().GetBookDate() == Today);
}

void CFmsTicket::UpdateCPCT(COptComboBox &CP, CCntlEdit &CT)
{
	CFindDlg FindDlg;
	CQData QData;
	CString sCP;

	FindDlg.m_OraLoader = GetData().GetOraLoader();

	sCP = CP.GetData();
	if(sCP.IsEmpty())
	{
		MessageBox("Insufficient search condition", "Error", MB_OK);
		return;
	}

	FindDlg.m_OraLoader.GetSql().Format("SELECT CT_CODE \"CT\", CT_NAME, CT_EURO_NUM \"EUROCLEAR\", "
						"CT_CEDEL_NUM \"CEDEL\", CT_ADDRESS1 \"CP_CT\" , TO_NUMBER(CT_CODE) \"IDX\" "
						"FROM SEMAM.NW_COUNTER_CONTACTS WHERE ISVALID = 'Y' AND CP_CODE = %s "
						"ORDER BY 6 ", QData.GetQueryText(sCP));
	if(FindDlg.DoModal() == IDOK)
		CT.SetData(FindDlg.m_Key);
}

BOOL CFmsTicket::UpdateData(BOOL bSaveandValid)
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
		GetData().GetCurrencyArr().CopyToComboBox(m_MarginCurrency);
		GetData().GetCurrencyArr().CopyToComboBox(m_InvCurrency);
		GetData().GetContactList().CopyKeyToComboBox(m_CP);

		GetData().GetTraderArr().CopyToMCComboBox(m_Trader);
		GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);
		GetData().GetIndexArr().CopyToMCComboBox(m_Index);
		GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet);
		GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet2);
		GetData().GetETradeTypeArr().CopyToMCComboBox(m_ETrade);
		GetData().GetContactList().CopyKeyToComboBox(m_InvAssignCP);
		GetData().GetContactList().CopyKeyToComboBox(m_Custodian);
		GetData().GetBestExecutionArr().CopyToComboBox(m_BestExecution);
		GetData().GetPFUArr().CopyToComboBox(m_PFU);
		GetData().GetAccountArr().CopyToComboBox(m_USDAcct);
		GetData().GetAccountArr().CopyToComboBox(m_FxAcct);
		GetData().GetAccountArr().CopyToComboBox(m_InvAccount);
		GetData().GetTrDescArr().CopyToComboBox(m_InvTrDesc);
	}

	EndWaitCursor();

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CFmsTicket message handlers
void CFmsTicket::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();
	InitControls();
	
	m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_InvSS, GetData().GetOraUser(), 0);
	m_Data.SetHeaders();

	GetData().LoadDates();

	m_Formula.AddString(NONE);
	m_Formula.AddString(DAILYREPO);
	m_Formula.AddString(DAILYREPOA);

	SetMode(FMS_NORMAL);
	UpdateData(FALSE);
	EndWaitCursor();

	m_Data.Refresh();
	EnableCtrls();
	EnableInvCtrls();
}

void CFmsTicket::OnSelchangeTicketPortfolioCombo() 
{
	CString Portfolio;

	Portfolio = m_Portfolio.GetData();
	if(Portfolio != m_Data.GetTicket().GetPortfolio())
	{
		m_Data.GetAt(1)->SetData(m_Data.GetTicket().GetPortfolio());
		if(m_Data.GetCurrentRow() > 0 && m_Data.GetModify())
		{
			if(MessageBox("Record has been changed. Save it first ?", NULL, MB_YESNO) == IDYES)
				return;
		}
		
		m_Data.GetAt(1)->SetData(Portfolio);
	}

	m_Port = Portfolio;
	OnTransRefresh();
	m_SS.ClearSheet();
}

void CFmsTicket::OnChangeTicketTransNum()
{
	EnableInvCtrls();
}

void CFmsTicket::OnSelchangeTicketDirCombo()
{
	CString TransType, Dir;

	TransType = m_TransType.GetData();
	Dir = m_Dir.GetData();
	TransType = GetData().DefineTransType(TransType, Dir);
	if(TransType.IsEmpty())
		return;
		
	if(TransType == REPO || TransType == LEVERAGE || TransType == BORROW || TransType == LEND)		
	{
		if(m_Mode != FMS_SEARCH && m_RateBasis.GetCurSel() < 0)
			m_RateBasis.SetData("A/360");
	}

	ChangeShortLabel();

	EnableCtrls();
}

void CFmsTicket::OnSelchangeTicketTypeCombo() 
{
	OnSelchangeTicketDirCombo();
}

BEGIN_EVENTSINK_MAP(CFmsTicket, CFormView)
    //{{AFX_EVENTSINK_MAP(CFmsTicket)
	ON_EVENT(CFmsTicket, IDC_TICKET_TICKET_LIST, 2 /* BlockSelected */, OnBlockSelectedTicketTicketList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CFmsTicket, IDC_TICKET_TICKET_LIST, 11 /* DblClick */, OnDblClickTicketTicketList, VTS_I4 VTS_I4)
	ON_EVENT(CFmsTicket, IDC_TICKET_INV_LIST, 2 /* BlockSelected */, OnBlockSelectedTicketInvList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CFmsTicket, IDC_TICKET_INV_LIST, 11 /* DblClick */, OnDblClickTicketInvList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
void CFmsTicket::OnSetfocusTicketCtEdit() 
{
	m_nCurrID = IDC_TICKET_CT_EDIT;
}

void CFmsTicket::OnSetfocusTicketDealEdit() 
{
	m_nCurrID = IDC_TICKET_DEAL_EDIT;
}

void CFmsTicket::OnTicketListedCheck() 
{
	CString Date;

	Date = m_TradeDate.GetData();

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, m_TransType.GetData(), m_Asset.GetData(), m_Listed.GetCheck()));

	if(!m_OrFee.GetReadOnly())
		m_OrFee.EnableWindow(m_Listed.GetCheck());
}

void CFmsTicket::OnBlockSelectedTicketTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickTicketTicketList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CFmsTicket::OnDblClickTicketTicketList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row);

		SetInCurrMonth();
		SetSameDate();
		SetMode(FMS_NORMAL);
		EnableCtrls();
		EnableInvCtrls();
	}
	else
	{
		m_Data.SetCurrentRow(-1);
		OnTransRefresh();
	}

	ChangeShortLabel();
}

void CFmsTicket::OnTransAdd() 
{
	m_SS.SetFocus();

   	if(!IsOK())
		return;

	if(m_TransNum.GetData().IsEmpty())
	{
		CString TransNum;
		
		m_Data.SetKey(EMPTYSTRING);
		GetData().GetOraLoader().GenerateUniqueID(TRANSNUM, TransNum);			
		m_TransNum.SetData(TransNum);
	}

	m_Data.AddRow();
}

void CFmsTicket::OnUpdateTransAdd(CCmdUI* pCmdUI) 
{
	if(GetData().IsOperation() || GetData().IsPowerUser())
		pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && 
			((!m_Ticket.GetData().IsEmpty() && m_TransNum.GetData().IsEmpty()) ||
			(!m_TransNum.GetData().IsEmpty() && m_InvAmount.GetWindowTextLength() > 0)));
	else
		pCmdUI->Enable(FALSE);
}

void CFmsTicket::OnUpdateTransClearscreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 ? TRUE : FALSE);
}

void CFmsTicket::OnTransClearscreen() 
{
	if(m_Data.GetCurrentRow() > 0 && m_Data.GetModify())
	{
		if(MessageBox("Record has been changed.  Save it first ?", NULL, MB_YESNO) == IDYES)
			return;
	}

	OnTransRefresh();
	m_TradeDate.SetData(EMPTYSTRING);
	m_ValueDate.SetData(EMPTYSTRING);
	m_bInCurrMonth = TRUE;
	SetMode(FMS_SEARCH);
	EnableCtrls();
}

void CFmsTicket::OnTransCopy() 
{
	CString Date, TransType, Dir;

	m_TransNum.SetData(EMPTYSTRING);

	Date = GetData().GetDate();
	m_TradeDate.SetData(Date);

	TransType = m_TransType.GetData();

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, TransType, m_Asset.GetData(), m_Listed.GetCheck()));

	Dir = m_Dir.GetData();
	TransType = GetData().DefineTransType(TransType, Dir);
	if(TransType == REPO || TransType == LEVERAGE || TransType == INTSWAP ||
		TransType == BORROW || TransType == LEND)
		m_Maturity.SetData(EMPTYSTRING);

	OnTransAdd();	
}

void CFmsTicket::OnUpdateTransCopy(CCmdUI* pCmdUI) 
{
//	if((GetData().IsOperation() && !GetData().IsGuest()) || GetData().IsPowerUser())
//		pCmdUI->Enable((!GetData().IsGuest() && m_Portfolio->GetCurSel() >= 0 && m_SS.GetSheetCurRow() >= 1) ? TRUE : FALSE);
//	else
		pCmdUI->Enable(FALSE);
}

void CFmsTicket::OnTransDelete() 
{
	if(MessageBox("Are you sure you want to delete this entry", NULL, MB_YESNO) == IDYES)
		m_Data.DeleteRow();
}

void CFmsTicket::OnUpdateTransDelete(CCmdUI* pCmdUI) 
{
	if(GetData().IsOperation() || GetData().IsPowerUser())
	{
		if(m_bSameDate || GetData().IsPowerUser())
			pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && 
					m_SS.GetSheetCurRow() >= 1 && 
					(m_SS.GetIsBlockSelected() || m_InvSS.GetIsBlockSelected()) && strlen(m_Data.GetKey()) > 0);
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CFmsTicket::OnTransUpdate() 
{
	if(IsOK())
		m_Data.UpdateRow();
}

void CFmsTicket::OnUpdateTransUpdate(CCmdUI* pCmdUI) 
{
	if(GetData().IsOperation() || GetData().IsPowerUser())
		pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && m_SS.GetSheetCurRow() > 0);
	else
		pCmdUI->Enable(FALSE);
}

void CFmsTicket::OnSetfocusTicketInvAssetEdit()
{
	m_nCurrID = IDC_TICKET_INV_ASSET_EDIT;
}

void CFmsTicket::OnSetfocusTicketAssetEdit()
{
	m_nCurrID = IDC_TICKET_ASSET_EDIT;
}

void CFmsTicket::OnTransFindasset() 
{
	CAssetDlg Dlg;
	BOOL bValueDateChangable = TRUE;
	CString TransType, Currency;

	Dlg.m_pData = &GetData();
	if(m_nCurrID == IDC_TICKET_INV_ASSET_EDIT)
		Dlg.m_FindData.GetRec().GetAsset() = m_InvAsset.GetData();
	else
	{
		Dlg.m_FindData.GetRec().GetAsset() =  m_Asset.GetData();
		Dlg.m_FindData.GetRec().GetDesc() = m_AssetDesc.GetData();
		Dlg.m_FindData.GetRec().GetOptTicker() = m_OptTicker.GetData();
	}

	if(Dlg.DoModal() != IDOK)
		return;

	if(m_nCurrID == IDC_TICKET_INV_ASSET_EDIT)
	{
		m_InvAsset.SetData(Dlg.m_FindData.GetRec().GetAsset());
		return;
	}
		
	m_Asset.SetData(Dlg.m_FindData.GetRec().GetAsset());
	m_AssetDesc.SetData(Dlg.m_FindData.GetRec().GetDesc());
	m_AssetCurr.SetData(Dlg.m_FindData.GetRec().GetCurrency());
	m_Trader.SetData(Dlg.m_FindData.GetTrader());
	m_Custodian.SetData(Dlg.m_FindData.GetCustodian());

	TransType = m_TransType.GetData();
	if(TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
	{
		m_OptTicker.SetData(Dlg.m_FindData.GetRec().GetOptTicker()); // Set Opt Ticker
		m_Listed.SetCheck(Dlg.m_OptListed);
	}
	else
		m_OptTicker.SetData(EMPTYSTRING);		

	Currency = m_Currency.GetData();
	if(Currency.IsEmpty() || (Currency != Dlg.m_FindData.GetRec().GetCurrency() &&
		 MessageBox("Note: Trading Currency is Different from Asset Currency! "
					"Do you want to synchonize them ?", "Warning", MB_YESNO) == IDYES))
		m_Currency.SetData(Dlg.m_FindData.GetRec().GetCurrency());

	if(m_Fxrate.GetWindowTextLength() == 0)
		m_Fxrate.SetData(Currency == USD ? "1" : EMPTYSTRING);							

	if(m_TradeDate.GetData().IsEmpty())
		m_TradeDate.SetData(GetData().GetDate());

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(m_TradeDate.GetData(), TransType, m_Asset.GetData(), m_Listed.GetCheck()));	

	EnableCtrls();
	m_TransType.SetFocus();
}

void CFmsTicket::OnUpdateTransFindasset(CCmdUI* pCmdUI) 
{
	CString TransType;

	TransType = m_TransType.GetData();
	pCmdUI->Enable((m_Portfolio.GetCurSel() < 0 || TransType.IsEmpty() || TransType == CASH || 
					TransType == SPRSWCLL || TransType == SPRSWCLL) ? FALSE : TRUE);	
}

void CFmsTicket::OnTransFindticket() 
{
	BeginWaitCursor();
	m_Data.SetExact(FALSE);
	if(m_Data.LoadTickets() > 0)
		SetMode(FMS_NORMAL);
	EndWaitCursor();
}

void CFmsTicket::OnUpdateTransFindticket(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Mode == FMS_SEARCH && m_Portfolio.GetCurSel() >= 0);
}

void CFmsTicket::OnTransRefresh() 
{	
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	m_Portfolio.SetData(m_Port);
	m_RecCurrency.SetData(EMPTYSTRING);
	m_RecInterest.SetData(EMPTYSTRING);
	m_RecAccretion.SetData(EMPTYSTRING);		
	m_RecCash.SetData(EMPTYSTRING);
	m_RecTotal.SetData(EMPTYSTRING);
	m_bInCurrMonth = TRUE;
	m_bSameDate = TRUE;

	EnableCtrls();
	EnableInvCtrls();

	m_TradeDate.SetData(GetData().GetDate());
	m_ValueDate.SetData(GetData().GetPlus3Date());
}

void CFmsTicket::OnUpdateTransRefresh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_Portfolio.GetCurSel() >= 0);
}

void CFmsTicket::OnUpdateTransInventory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && m_InvSS.GetSheetRows() > 0);
}

void CFmsTicket::OnBlockSelectedTicketInvList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickTicketInvList(m_InvSS.GetActiveCol(), m_InvSS.GetActiveRow());
}

void CFmsTicket::OnDblClickTicketInvList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_InvSS.GetSheetRows())
	{
		CQData QData;

		m_Data.GetSRowCtrl().SetCurrentRow(Row);
		m_Data.SetInvNum(m_Data.GetSRowCtrl().GetID());
		m_Data.Compute();

		m_RecCurrency.SetData(m_Data.GetRecCurrency());
		m_RecInterest.SetData(QData.WriteNumber(m_Data.GetPrePaid(), TRUE, 2));
		m_RecAccretion.SetData(QData.WriteNumber(m_Data.GetAccretion(), TRUE, 2));		
		m_RecCash.SetData(QData.WriteNumber(m_Data.GetCash(), TRUE, 2));
		m_RecTotal.SetData(QData.WriteNumber(m_Data.GetCash() + m_Data.GetAccretion() + m_Data.GetPrePaid(), TRUE, 2));
	}
	else
	{
		m_Data.GetSRowCtrl().SetCurrentRow(-1);
		m_Data.GetSRowCtrl().Refresh();
		m_RecCurrency.SetData(EMPTYSTRING);
		m_RecInterest.SetData(EMPTYSTRING);
		m_RecAccretion.SetData(EMPTYSTRING);
		m_RecCash.SetData(EMPTYSTRING);
		m_RecTotal.SetData(EMPTYSTRING);
	}

	SetMode(FMS_NORMAL);
	EnableInvCtrls();
}

void CFmsTicket::OnTransFindItem() 
{
	CFindDlg FindDlg;
	CSetConventionDlg SetDlg;
	CString CP, Portfolio, Asset, ValueDate, Strike, OptExp, TransType, Dir, Currency;
	CQData QData;
	int CurrID;

	CurrID = m_nCurrID;
	m_nCurrID = 0;

	FindDlg.m_OraLoader = GetData().GetOraLoader();
	switch(CurrID)
	{
		case IDC_TICKET_DEAL_EDIT:
			TransType = m_TransType.GetData();
			Dir = m_Dir.GetData();
			TransType = GetData().DefineTransType(TransType, Dir);
			if(TransType == SECURITIES ||TransType == CDS || TransType == CALL || 
				TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
			{
				Portfolio = QData.GetQueryText(m_Portfolio.GetData());
				Asset = QData.GetQueryText(m_Asset.GetData());
				ValueDate = m_ValueDate.GetData();

				if(TransType == SECURITIES || TransType == CDS)
				{
					if(Portfolio.IsEmpty() || Asset.IsEmpty() || ValueDate.IsEmpty())
					{
						MessageBox("Insufficient search condition", "Error", MB_OK);
						return;
					}
					
					FindDlg.m_OraLoader.GetSql() = "SELECT TRANS_NUM, ASSET_CODE, TRANS_DIRECTION \"DIR\", TRANS_TYPE, "
													"EXERCISE_PRICE \"STRIKE\", OPT_EXPIRATION \"OPT_EXP\", NOM_AMOUNT "
													"FROM SEMAM.NW_OUT_OPTIONS "
													"WHERE PORTFOLIO = " + Portfolio + 
													" AND ASSET_CODE = " + Asset + 
													" AND TRANS_TYPE IN ('PUT', 'CALL', 'SPR.SW.CLL', 'SPR.SW.PUT') "
													" AND (OPT_EXPIRATION IS NULL OR OPT_EXPIRATION >=TO_DATE(" + QData.GetQueryDate(ValueDate)+ ") - 5) "
													" AND EXERCISE_DATE IS NULL AND NOM_AMOUNT > 0 ORDER BY TRANS_NUM ";
				}
				else
				{
					Strike = QData.GetQueryNumber(m_Strike.GetData());
					OptExp = QData.GetQueryDate(m_OptExp.GetData());
					TransType = QData.GetQueryText(TransType);
					Dir = QData.GetQueryText(Dir);
					if(Portfolio.IsEmpty() || Asset.IsEmpty() || ValueDate.IsEmpty() || Strike.IsEmpty() ||
						OptExp.IsEmpty() || Dir.IsEmpty())
					{
						MessageBox("Insufficient search condition", "Error", MB_OK);
						return;
					}

					FindDlg.m_OraLoader.GetSql() = "SELECT TRANS_NUM, ASSET_CODE, TRANS_DIRECTION \"DIR\", "
													"TRANS_TYPE, EXERCISE_PRICE \"STRIKE\", OPT_EXPIRATION \"OPT_EXP\", NOM_AMOUNT "
													"FROM SEMAM.NW_OUT_OPTIONS "
													"WHERE PORTFOLIO = " + Portfolio + 
													" AND ASSET_CODE = " + Asset + 
													" AND TRANS_TYPE IN ('PUT', 'CALL', 'SPR.SW.CLL', 'SPR.SW.PUT') "
													" AND (OPT_EXPIRATION IS NULL OR OPT_EXPIRATION >=TO_DATE(" + QData.GetQueryDate(ValueDate)+ ") - 5) "
													" AND EXERCISE_DATE IS NULL "
													" AND NOM_AMOUNT > 0 "
													" AND EXERCISE_PRICE = " + Strike + 
													" AND OPT_EXPIRATION = " + OptExp + 
													" AND TRANS_TYPE = " + TransType + 
													" AND TRANS_DIRECTION != " + Dir;
				}

				if(FindDlg.DoModal() == IDOK)
					m_Link.SetData(FindDlg.m_Key);
			}
			break;
		case IDC_TICKET_CT_EDIT:
			UpdateCPCT(m_CP, m_CT);
			break;
		case IDC_TICKET_INV_ASSIGNCT_EDIT:
			UpdateCPCT(m_InvAssignCP, m_InvAssignCT);
			break;
		case IDC_TICKET_INV_FOREX_REF_EDIT:
			Portfolio = m_Portfolio.GetData();
			Currency = m_Currency.GetData();
			Dir = m_Dir.GetData();
			ValueDate = m_ValueDate.GetData();

			if(Currency.IsEmpty())
			{
				MessageBox("Currency is not selected", "Error", MB_OK);
				return;
			}

			FindDlg.m_OraLoader.GetSql() = "SELECT TRANS_NUM, TRADE_DATE, VALUE_DATE, "
											"FXRATE, TRANS_DIRECTION, NOM_AMOUNT "
											"FROM SEMAM.NW_TR_TICKETS A "
											"WHERE A.PORTFOLIO = '" + Portfolio + "' "
											"AND A.CURRENCY = '" + Currency + "' "
											"AND A.VALUE_DATE <= '" + ValueDate + "' "
											"AND A.VALUE_DATE >= TO_DATE('" + ValueDate + "') - 30 "
											"AND A.TRANS_DIRECTION = '" + Dir + "' "
											"AND A.TRANS_TYPE = 'FOREX' "
											"AND ASSET_CODE IS NULL ORDER BY 3, 1 ";
			if(FindDlg.DoModal() == IDOK)
				m_InvForexRef.SetData(QData.RemoveComma(FindDlg.m_Key));
			break;
		case IDC_TICKET_SETCONVENTION_EDIT:
			SetDlg.m_sAsset = m_Asset.GetData();
			SetDlg.m_sPortfolio = m_Portfolio.GetData();
			SetDlg.m_pData = &GetData();
			if(SetDlg.DoModal() == IDOK)
				m_SetConvention.SetData(SetDlg.m_sSetConvention);
			break;
		case IDC_TICKET_BINARY_EDIT:
			FindDlg.m_OraLoader.Open("SELECT OPT_TYPE, OPT_TYPE_DESC FROM SEMAM.NW_EXOTIC_OPTTYPE ORDER BY 1 ");
			if(FindDlg.DoModal() == IDOK)
			{
				CString Text;
				Text = m_Binary.GetData();
				Text.MakeUpper();
				Text = Text.SpanExcluding("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				Text = FindDlg.m_Key + Text;
				m_Binary.SetData(Text);
			}
			break;
		default:
			break;
	} 
}

void CFmsTicket::OnUpdateTransFindItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nCurrID && ((m_Portfolio.GetCurSel() >= 0 && m_Asset.GetWindowTextLength() > 0
				&& !m_ValueDate.GetData().IsEmpty() && m_TransType.GetCurSel() >= 0) || m_CP.GetCurSel() > 0));
}

void CFmsTicket::OnUpdateTransInvertPrice(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0);
}

void CFmsTicket::OnTransInvertPrice()
{
	CInvert Dlg;

	Dlg.m_Origin = m_Price.GetData();
	if(Dlg.DoModal() == IDOK)
		m_Price.SetData(Dlg.m_New);
}

void CFmsTicket::OnTransRepoPrice()
{
	CRepoPrice Dlg;
	CString	ValueDate, RateBasis, Formula;
	
	Dlg.m_Asset = m_Asset.GetData();
	Dlg.m_HairCut = m_Margin.GetData();
	if(Dlg.m_HairCut.IsEmpty())
	{
		if(m_TRS.GetCheck())
			Dlg.m_HairCut = "0";
		else
			Dlg.m_HairCut = "10";
	}
	
	Dlg.m_NWINetPrice = m_Price.GetData();
	ValueDate = m_ValueDate.GetData();
	RateBasis = m_RateBasis.GetData(); // Lev Rate Basis
	Formula = m_Formula.GetData();
	Dlg.m_Val.Setup(GetData().GetOraLoader(), REPO, P, Dlg.m_Asset, ValueDate, ValueDate, 
					"0", "0", "1", RateBasis, "1", Formula); 
					
	if(Dlg.DoModal() == IDOK)
	{
		m_Price.SetData(Dlg.m_NWINetPrice);
		m_Margin.SetData(Dlg.m_HairCut);
	}
}

void CFmsTicket::OnUpdateTransRepoPrice(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && m_TransType.GetData() == REPO && m_Asset.GetWindowTextLength() > 0);
}

void CFmsTicket::OnUpdateTransOptionExp(CCmdUI* pCmdUI)
{
	CString TransType;

	TransType = m_TransType.GetData();
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && (TransType == CALL || TransType == PUT) ? TRUE : FALSE);
}

void CFmsTicket::OnTransOptionExp()
{
	COptExpDlg Dlg;

	Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
	if(Dlg.DoModal() == IDOK)
		m_OptExp.SetData(Dlg.m_Date);
}

void CFmsTicket::OnTicketTrsCheck() 
{
	if(m_SS.GetSheetCurRow() > 0)
	{
		if(!m_TRS.GetReadOnly())
		{
			if(MessageBox("Do you want to make a change on TRS Field?", NULL, MB_YESNO) != IDYES)
				m_TRS.SetCheck(m_Data.GetTicket().GetTRS()); // Change back to the original one
		}
	}
}

void CFmsTicket::OnSetfocusTicketInvAssignctEdit() 
{
	m_nCurrID = IDC_TICKET_INV_ASSIGNCT_EDIT;
}

void CFmsTicket::OnTransXcurrency() 
{
	CXCurrencyDlg Dlg;

	Dlg.m_pData = &GetData();
	Dlg.m_UserClass = 3;
	Dlg.DoModal();
}

void CFmsTicket::OnTransFindExactTicket() 
{
	BeginWaitCursor();
	m_Data.SetExact(TRUE);
	if(m_Data.LoadTickets() > 0)
		SetMode(FMS_NORMAL);
	EndWaitCursor();
}

void CFmsTicket::OnUpdateTransFindExactTicket(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Mode == FMS_SEARCH && m_Portfolio.GetCurSel() >= 0);
}

void CFmsTicket::OnEnSetfocusTicketInvForexRefEdit()
{
	m_nCurrID = IDC_TICKET_INV_FOREX_REF_EDIT;
}

void CFmsTicket::OnEnSetfocusTicketSetconventionEdit()
{
	m_nCurrID = IDC_TICKET_SETCONVENTION_EDIT;
}

void CFmsTicket::OnUpdateTransOtherfee(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_TransNum.GetData().IsEmpty() && (m_SecFee.GetCheck() > 0 || m_OrFee.GetCheck() > 0));
}

void CFmsTicket::OnTransOtherfee()
{
	COtherFeeDlg Dlg;
	CQData QData;

	Dlg.m_TransType = m_TransType.GetData();
	Dlg.m_Dir = m_Dir.GetData();
	Dlg.m_Asset = QData.GetQueryText(m_Asset.GetData());
	Dlg.m_dNomAmount = atof(QData.RemoveComma(m_Amount.GetData()));
	Dlg.m_dPrice = atof(QData.RemoveComma(m_Price.GetData()));
	if(m_Fxrate.GetWindowTextLength() == 0)
		Dlg.m_dFxrate = 1;
	else
		Dlg.m_dFxrate = atof(QData.RemoveComma(m_Fxrate.GetData()));
	if(m_BrFee.GetWindowTextLength() == 0)
		Dlg.m_dBrFee = 0;
	else
		Dlg.m_dBrFee = atof(QData.RemoveComma(m_BrFee.GetData()));
	
	if(m_OtherFee.GetWindowTextLength() == 0)
		Dlg.m_dOtherFee = 0;
	else
		Dlg.m_dOtherFee = atof(QData.RemoveComma(m_Price.GetData()));

	Dlg.m_bSecFee = m_SecFee.GetCheck();
	Dlg.m_bOrFee = m_OrFee.GetCheck();
	Dlg.m_PB = m_InvAssignCP.GetData();
	if(Dlg.m_PB.IsEmpty())
		Dlg.m_PB = m_Custodian.GetData();
	Dlg.m_CP = m_CP.GetData();

	if(Dlg.DoModal() == IDOK)
		m_OtherFee.SetData(QData.WriteNumber(Dlg.m_dSecFee + Dlg.m_dOrFee, TRUE));
}

void CFmsTicket::OnEnSetfocusTicketBinaryEdit()
{
	CString TransType;

	TransType = m_TransType.GetData();

	if(TransType == CALL || TransType == PUT)
		m_nCurrID = IDC_TICKET_BINARY_EDIT;
}

void CFmsTicket::OnTransCc()
{
	CString CanTicket, CorrTicket;

	if(!m_Ticket.GetData().IsEmpty() && 
		MessageBox("Are you sure you want to do cancel and correct on this ticket " + m_Ticket.GetData() + " ? ", "Transaction Process", MB_YESNO) == IDYES)
	{
		BeginWaitCursor();
		if(m_Data.CancelCorrect(m_Ticket.GetData(), m_TransNum.GetData(), CanTicket, CorrTicket))
			MessageBox(CanTicket + " is cancellation ticket for " + m_Ticket.GetData() + " and " + CorrTicket + " is correction ticket for " + m_Ticket.GetData() + ".", "Transaction Process", MB_OK);
		EndWaitCursor();
	}
}

void CFmsTicket::OnUpdateTransCc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetData().IsPowerUser());
}
