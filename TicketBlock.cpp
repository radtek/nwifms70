// TicketBlock.cpp : implementation file
//
#include "stdafx.h"
#include <math.h>
#include "NWIFMS70.h"
#include "TicketBlock.h"
#include "assetdlg.h"
#include "brokerfee.h"
#include "qdata.h"
#include "contractconvertor.h"
#include "optexpdlg.h"
#include "xcurrencydlg.h"
#include "optsettlementdlg.h"
#include "repoprice.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTicketBlock
IMPLEMENT_DYNCREATE(CTicketBlock, CFormView)

CTicketBlock::CTicketBlock()
	: CFormView(CTicketBlock::IDD)
{
	//{{AFX_DATA_INIT(CTicketBlock)
	m_Listed = NULL;
	m_EuroOpt = NULL;
	m_Trs = NULL;
	m_IPO = NULL;
	m_WI = NULL;
	m_OptAuto = NULL;
	m_Funded = NULL;
	m_ShortSale = NULL;

    m_TransType = NULL;
    m_CP = NULL;
    m_Currency = NULL;
	m_RepoCP = NULL;
	m_RateBasis = NULL;
	m_FloatRateBasis = NULL;
	m_Portfolio = NULL;
	m_AssignCP = NULL;
	m_MarginCurrency = NULL;
	m_BestExecution = NULL;

	m_bLoaded = FALSE;
	//}}AFX_DATA_INIT
}

CTicketBlock::~CTicketBlock()
{
	if(m_Listed)
		delete m_Listed;
	if(m_EuroOpt)
		delete m_EuroOpt;
	if(m_Trs)
		delete m_Trs;
	if(m_IPO)
		delete m_IPO;
	if(m_WI)
		delete m_WI;
	if(m_OptAuto)
		delete m_OptAuto;
	if(m_Funded)
		delete m_Funded;
	if(m_ShortSale)
		delete m_ShortSale;

    if(m_TransType)
		delete m_TransType;
    if(m_CP)
		delete m_CP;
    if(m_Currency)
		delete m_Currency;
	if(m_RepoCP)
		delete m_RepoCP;
	if(m_RateBasis)
		delete m_RateBasis;
	if(m_FloatRateBasis)
		delete m_FloatRateBasis;
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_AssignCP)
		delete m_AssignCP;
	if(m_MarginCurrency)
		delete m_MarginCurrency;
	if(m_BestExecution)
		delete m_BestExecution;
}

void CTicketBlock::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTicketBlock)
	DDX_Control(pDX, IDC_BLOCK_TICKET_LIST, m_SS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTicketBlock, CFormView)
	//{{AFX_MSG_MAP(CTicketBlock)
	ON_CBN_SELCHANGE(IDC_BLOCK_TRANSTYPE_COMBO, OnSelchangeTranstypeCombo)
	ON_COMMAND(ID_BLOCK_ADD, OnBlockAdd)
	ON_COMMAND(ID_BLOCK_DELETE, OnBlockDelete)
	ON_COMMAND(ID_BLOCK_UPDATE, OnBlockUpdate)
	ON_COMMAND(ID_BLOCK_REFRESH, OnBlockRefresh)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_UPDATE, OnUpdateBlockUpdate)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_DELETE, OnUpdateBlockDelete)
	ON_CBN_SELCHANGE(IDC_BLOCK_TRANSDIR_COMBO, OnSelchangeTransdirCombo)
	ON_CBN_SELCHANGE(IDC_BLOCK_CURRENCY_COMBO, OnSelchangeCurrencyCombo)
	ON_COMMAND(ID_BLOCK_FIND_ASSET, OnBlockFindAsset)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_FIND_ASSET, OnUpdateBlockFindAsset)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_ADD, OnUpdateBlockAdd)
	ON_BN_CLICKED(IDC_BLOCK_LISTED_CHECK, OnListedCheck)
	ON_EN_SETFOCUS(IDC_BLOCK_ASSET_EDIT, OnSetfocusAssetEdit)
	ON_EN_CHANGE(IDC_BLOCK_ASSET_EDIT, OnChangeAssetEdit)
	ON_CBN_SELCHANGE(IDC_BLOCK_LEVSOURCE_COMBO, OnSelchangeLevsourceCombo)
	ON_COMMAND(ID_BLOCK_COPY_TICKET, OnBlockCopyTicket)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_COPY_TICKET, OnUpdateBlockCopyTicket)
	ON_COMMAND(ID_BLOCK_BROKERFEE, OnBlockBrokerfee)
	ON_COMMAND(ID_BLOCK_REPOPRICE, OnBlockRepoprice)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_REPOPRICE, OnUpdateBlockRepoprice)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_BROKERFEE, OnUpdateBlockBrokerfee) 
	ON_EN_SETFOCUS(IDC_BLOCK_AMOUNT_EDIT, OnSetfocusAmountEdit)
	ON_EN_KILLFOCUS(IDC_BLOCK_DOWNPYMNT_EDIT, OnKillfocusDownpymntEdit)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_CONTRACT_CONVERTOR, OnUpdateBlockContractConvertor)
	ON_COMMAND(ID_BLOCK_CONTRACT_CONVERTOR, OnBlockContractConvertor)
	ON_COMMAND(ID_BLOCK_LOAD_BLOTTER, OnBlockLoadBlotter)
	ON_COMMAND(ID_BLOCK_LOAD_TRADE, OnBlockLoadTrade)
	ON_COMMAND(ID_BLOCK_OPTION_EXP, OnBlockOptionExp)
	ON_UPDATE_COMMAND_UI(ID_BLOCK_OPTION_EXP, OnUpdateBlockOptionExp)
	ON_BN_CLICKED(IDC_BLOCK_DEAL_CHECK, OnBlockDealCheck)
	ON_EN_KILLFOCUS(IDC_BLOCK_AMOUNT_EDIT, OnKillfocusAmountEdit)
	ON_COMMAND(ID_BLOCK_XCURRENCY, OnBlockXcurrency)
	ON_EN_SETFOCUS(IDC_BLOCK_OPTSET_EDIT, OnSetfocusOptSetEdit)
	ON_EN_SETFOCUS(IDC_BLOCK_OPTSET2_EDIT, OnSetfocusOptSet2Edit)
	ON_COMMAND(ID_BLOCK_SETTLEMENTTIME, &CTicketBlock::OnBlockSettlementtime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTicketBlock diagnostics
#ifdef _DEBUG
void CTicketBlock::AssertValid() const
{
	CFormView::AssertValid();
}

void CTicketBlock::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CTicketBlock::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

void CTicketBlock::EnableCtrls()
{
	CString TransType, Dir, RepoCP;

	m_TradeDate.SetReadOnly(m_Data.GetBlotter());
	m_ValueDate.SetReadOnly(m_Data.GetBlotter());
	m_Amount.SetReadOnly(m_Data.GetBlotter());
	m_Strike.SetReadOnly(m_Data.GetBlotter());
	m_OptExp.SetReadOnly(m_Data.GetBlotter());
	m_OptTicker.SetReadOnly(m_Data.GetBlotter());
	m_FxRate.SetReadOnly(m_Data.GetBlotter());
	m_Rate.SetReadOnly(m_Data.GetBlotter());
	m_Maturity.SetReadOnly(m_Data.GetBlotter());
	m_SWBooking.SetReadOnly(m_Data.GetBlotter());
	m_SWMaturity.SetReadOnly(m_Data.GetBlotter());
	m_FloatRate.SetReadOnly(m_Data.GetBlotter());
	m_FloatMaturity.SetReadOnly(m_Data.GetBlotter());
	m_Note.SetReadOnly(m_Data.GetBlotter());
	m_Note2.SetReadOnly(m_Data.GetBlotter());

	GetDlgItem(IDC_BLOCK_STRIKE_STATIC)->ShowWindow(SW_HIDE);
	m_Strike.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_EXPIRATION_STATIC)->ShowWindow(SW_HIDE);
	m_OptExp.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_OPT_TICKER_STATIC)->ShowWindow(SW_HIDE);
	m_OptTicker.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_OPTSET_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_OPTSET2_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet2.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_BLMID_STATIC)->ShowWindow(SW_HIDE);
	m_OptID.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_MARGIN_STATIC)->ShowWindow(SW_HIDE);
	m_Margin.ShowWindow(SW_HIDE);
	m_MarginCurrency->ShowWindow(SW_HIDE);
	m_MarginAmount.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_SWAP_TICKET_STATIC)->ShowWindow(SW_HIDE);
	m_SwapTicket.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_LEVSOURCE_STATIC)->ShowWindow(SW_HIDE);
	m_RepoCP->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_SWMATURITY_STATIC)->ShowWindow(SW_HIDE);
	m_SWMaturity.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_FLOAT_RATE_STATIC)->ShowWindow(SW_HIDE);
	m_FloatRate.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_FLOAT_RATE_BASIS_STATIC)->ShowWindow(SW_HIDE);
	m_FloatRateBasis->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_FLOAT_MATURITY_STATIC)->ShowWindow(SW_HIDE);
	m_FloatMaturity.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_SWAP_TICKET_STATIC)->ShowWindow(SW_HIDE);
	m_SwapTicket.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BLOCK_DELIVERY_STATIC)->ShowWindow(SW_HIDE);
	m_DeliveryDate.ShowWindow(SW_HIDE);

	EnableRepoCtrls(FALSE);

	m_Listed->ShowWindow(SW_HIDE);
	m_EuroOpt->ShowWindow(SW_HIDE);
	m_Binary.ShowWindow(SW_HIDE);
	m_IPO->ShowWindow(SW_HIDE);
	m_Trs->ShowWindow(SW_HIDE);
	
	m_RepoCP->GetSelString(RepoCP);
	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	GetData().DefineTransType(TransType, Dir);

	if(TransType == SECURITIES || TransType == CDS || TransType == FOREX || TransType == CALL || TransType == PUT)
		m_AssignCP->EnableWindow(TRUE);
	else
		m_AssignCP->EnableWindow(FALSE);

	if(TransType == INTSWAP || TransType == CASH)
	{
		GetDlgItem(IDC_BLOCK_ASSET_STATIC)->ShowWindow(SW_HIDE);
		m_Asset.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BLOCK_ASSETDESC_STATIC)->ShowWindow(SW_HIDE);
		m_AssetDesc.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BLOCK_CURRENCY_STATIC)->ShowWindow(SW_HIDE);
		m_AssetCurrency.ShowWindow(SW_HIDE);
		m_Price.ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BLOCK_ASSET_STATIC)->ShowWindow(SW_SHOW);
		m_Asset.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_ASSETDESC_STATIC)->ShowWindow(SW_SHOW);
		m_AssetDesc.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_CURRENCY_STATIC)->ShowWindow(SW_SHOW);
		m_AssetCurrency.ShowWindow(SW_SHOW);
		m_Price.ShowWindow(SW_SHOW);
	}

	if(TransType != FOREX)
	{
		GetDlgItem(IDC_BLOCK_MARGIN_STATIC)->ShowWindow(SW_SHOW);
		m_Margin.ShowWindow(SW_SHOW);
		m_MarginCurrency->ShowWindow(SW_SHOW);
		m_MarginAmount.ShowWindow(SW_SHOW);
		m_Price.ShowWindow(SW_SHOW);
	}
	else
		m_Price.ShowWindow(SW_HIDE);

	if(TransType == BORROW || TransType == LEND || TransType == LEVERAGE || TransType == REPO || 
		TransType == CASH || TransType == INTSWAP || (TransType == SECURITIES && !RepoCP.IsEmpty()))
		EnableRepoCtrls(TRUE);
	
	if(TransType == SECURITIES)
	{
		GetDlgItem(IDC_BLOCK_LEVSOURCE_STATIC)->ShowWindow(SW_SHOW);
		m_RepoCP->ShowWindow(SW_SHOW);
		m_IPO->ShowWindow(SW_SHOW);
		OnSelchangeLevsourceCombo();
	}
	
	if(TransType == SECURITIES || TransType == REPO || TransType == LEVERAGE)
		m_Trs->ShowWindow(SW_SHOW);

	if(TransType == SECURITIES || TransType == CDS || TransType == CALL || TransType == PUT)
	{
		GetDlgItem(IDC_BLOCK_OPTSET_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_OPTSET2_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet2.ShowWindow(SW_SHOW);

		if(TransType == SECURITIES && m_Data.GetRawTicket().GetAssetClass() == "CURRENCY FWDS")
		{
			GetDlgItem(IDC_BLOCK_SWAP_TICKET_STATIC)->ShowWindow(SW_SHOW);
			m_SwapTicket.ShowWindow(SW_SHOW);
		}
	}
	
	if(TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
	{
		GetDlgItem(IDC_BLOCK_STRIKE_STATIC)->ShowWindow(SW_SHOW);
		m_Strike.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_EXPIRATION_STATIC)->ShowWindow(SW_SHOW);
		m_OptExp.ShowWindow(SW_SHOW);
		m_Listed->ShowWindow(SW_SHOW);
		m_EuroOpt->ShowWindow(SW_SHOW);
		m_Binary.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_OPT_TICKER_STATIC)->ShowWindow(SW_SHOW);
		m_OptTicker.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_BLMID_STATIC)->ShowWindow(SW_SHOW);
		m_OptID.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_DELIVERY_STATIC)->ShowWindow(SW_SHOW);
		m_DeliveryDate.ShowWindow(SW_SHOW);
	}

	if(TransType == INTSWAP)
	{
		GetDlgItem(IDC_BLOCK_SWMATURITY_STATIC)->ShowWindow(SW_SHOW);
		m_SWMaturity.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_FLOAT_RATE_STATIC)->ShowWindow(SW_SHOW);
		m_FloatRate.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_FLOAT_RATE_BASIS_STATIC)->ShowWindow(SW_SHOW);
		m_FloatRateBasis->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BLOCK_FLOAT_MATURITY_STATIC)->ShowWindow(SW_SHOW);
		m_FloatMaturity.ShowWindow(SW_SHOW);
	}

	if(m_Data.GetBlotter())
	{
		m_TransType->EnableWindow(FALSE);
		m_Dir.EnableWindow(FALSE);
		m_CP->EnableWindow(FALSE);
		m_Currency->EnableWindow(FALSE);
		m_FloatRateBasis->EnableWindow(FALSE);
		m_IPO->EnableWindow(FALSE);
		m_Listed->EnableWindow(FALSE);
		m_EuroOpt->EnableWindow(FALSE);
		m_Binary.EnableWindow(FALSE);
	}
	else
	{
		m_TransType->EnableWindow(TRUE);
		m_Dir.EnableWindow(TRUE);
		m_CP->EnableWindow(TRUE);
		m_Currency->EnableWindow(TRUE);
		m_FloatRateBasis->EnableWindow(TRUE);
		m_IPO->EnableWindow(TRUE);
		m_Listed->EnableWindow(TRUE);
		m_EuroOpt->EnableWindow(TRUE);
		m_Binary.EnableWindow(TRUE);
	}

	if(!(!RepoCP.IsEmpty() || TransType == REPO))
	{
		m_DownPymnt.EnableWindow(FALSE);
		m_DownPymnt.SetWindowText("100");
	}
	else
		m_DownPymnt.EnableWindow(TRUE);
}

void CTicketBlock::EnableRepoCtrls(BOOL bEnable)
{
	m_Trs->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BLOCK_REPORATE_STATIC)->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_Rate.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BLOCK_RATE_BASIS_STATIC)->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_RateBasis->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BLOCK_MATURITY_STATIC)->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_Maturity.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);

	if(m_Data.GetBlotter())
	{
		m_RepoCP->EnableWindow(FALSE);
		m_RateBasis->EnableWindow(FALSE);
		m_Trs->EnableWindow(FALSE);
	}
	else
	{
		m_RepoCP->EnableWindow(TRUE);
		m_RateBasis->EnableWindow(TRUE);
		m_Trs->EnableWindow(TRUE);
	}	
}

void CTicketBlock::InitControls()
{
	m_SS.SetVisibleCols(11);
	m_SS.SetVisibleRows(15);
	m_SS.SetSheetRows(m_SS.GetMaxRows());
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);
	m_SS.SetSheetCurRow(-1);

	m_Trs = new CCheckBox(this, IDC_BLOCK_DEAL_CHECK, Y);
	m_IPO = new CCheckBox(this, IDC_BLOCK_IPO_CHECK, Y);
	m_Listed = new CCheckBox(this, IDC_BLOCK_LISTED_CHECK, Y);
	m_EuroOpt = new CCheckBox(this, IDC_BLOCK_EUROOPT_CHECK, Y);
	m_WI = new CCheckBox(this, IDC_BLOCK_WI_CHECK, Y);
	m_OptAuto = new CCheckBox(this, IDC_BLOCK_AUTO_CHECK, Y);
	m_Funded = new CCheckBox(this, IDC_BLOCK_FUNDED_CHECK, Y);
	m_ShortSale = new CCheckBox(this, IDC_BLOCK_SHORT_CHECK, Y);

    m_Dir.Setup(this, IDC_BLOCK_TRANSDIR_COMBO);
	m_Trader.Setup(this, IDC_BLOCK_TRADER_COMBO);
	m_TransType = new COptComboBox(this, IDC_BLOCK_TRANSTYPE_COMBO);
    m_CP = new COptComboBox(this, IDC_BLOCK_COUNTERPARTY_COMBO);
    m_Currency = new COptComboBox(this, IDC_BLOCK_CURRENCY_COMBO);
	m_RepoCP = new COptComboBox(this, IDC_BLOCK_LEVSOURCE_COMBO, TRUE);
	m_RateBasis = new COptComboBox(this, IDC_BLOCK_RATE_BASIS_COMBO, TRUE);
	m_FloatRateBasis = new COptComboBox(this, IDC_BLOCK_FLOAT_RATE_BASIS_COMBO, TRUE);
	m_MarginCurrency = new COptComboBox(this, IDC_BLOCK_MARGIN_CURRENCY_COMBO, TRUE);
	m_BestExecution = new COptComboBox(this, IDC_BLOCK_BESTEXECUTION_COMBO);
	m_AssignCP = new COptComboBox(this, IDC_BLOCK_ASSIGNCP_COMBO, TRUE);

	m_Ticket.Setup(this, IDC_BLOCK_TICKET_EDIT);
	m_SwapTicket.Setup(this, IDC_BLOCK_SWAP_TICKET_EDIT);
	m_UnWindTicket.Setup(this, IDC_BLOCK_UNWINDTICKET_EDIT);
	m_CancelTicket.Setup(this, IDC_BLOCK_CANCEL_TICKET_EDIT);
	m_CorrectTicket.Setup(this, IDC_BLOCK_CORRECT_TICKET_EDIT);
	m_Binary.Setup(this, IDC_BLOCK_BINARY_EDIT);

	m_TradeDate.Setup(this, IDC_BLOCK_TRADEDATE_EDIT, TRUE, "Trade Date must be greater than or equal to Today's date");
	m_TradeDate.SetCompareDate(GetData().GetDate());
	m_ValueDate.Setup(this, IDC_BLOCK_VALUEDATE_EDIT, TRUE);
	m_Maturity.Setup(this, IDC_BLOCK_REPO_MATURITY_EDIT, TRUE, 
						"Maturity Date must be greater than or equal to Value Date", &m_ValueDate);
	m_OptExp.Setup(this, IDC_BLOCK_EXPIRATION_EDIT, TRUE, 
					"Option Expiration must be greater than or equal to Trade Date", &m_TradeDate);
	m_SWBooking.Setup(this, IDC_BLOCK_SWBOOKING_EDIT, TRUE, 
						"Swap Booking Date must be greater than or equal to Value Date", &m_ValueDate);
	m_SWMaturity.Setup(this, IDC_BLOCK_SWMATURITY_EDIT, TRUE, 
						"Swap Maturity Date must be greater than or equal to Swap Booking Date", &m_SWBooking);
	m_DeliveryDate.Setup(this, IDC_BLOCK_DELIVERY_EDIT, TRUE, 
							"Delivery Date must be greater than or equal to option expiration date", &m_OptExp);
	m_FloatMaturity.Setup(this, IDC_BLOCK_FLOAT_MATURITY_EDIT, TRUE, 
							"Maturity Date must be greater than or equal to Value Date", &m_ValueDate);
	m_BrokerFee.Setup(this, IDC_BLOCK_BROKER_FEE_EDIT, NULL, 2);
	m_SoftDollar.Setup(this, IDC_BLOCK_SOFTDOLLAR_EDIT, NULL, 2);
	m_OtherFee.Setup(this, IDC_BLOCK_OTHER_FEE_EDIT, NULL, 2);
	m_VAR.Setup(this, IDC_BLOCK_VAR_EDIT, NULL, 2);

	m_Amount.Setup(this, IDC_BLOCK_AMOUNT_EDIT, NULL, 2);
	m_Margin.Setup(this, IDC_BLOCK_MARGIN_EDIT);
	m_MarginAmount.Setup(this, IDC_BLOCK_MARGIN_AMOUNT_EDIT, NULL, 2);
	m_Strike.Setup(this, IDC_BLOCK_STRIKE_EDIT);
	m_OptTicker.Setup(this, IDC_BLOCK_OPT_TICKER_EDIT);
	m_OptTicker.LimitText(25);
	m_OptSet.Setup(this, IDC_BLOCK_OPTSET_EDIT);
	m_OptSet.LimitText(3);
	m_OptSet2.Setup(this, IDC_BLOCK_OPTSET2_EDIT);
	m_OptSet2.LimitText(3);
	m_OptID.Setup(this, IDC_BLOCK_BLMID_EDIT);
	m_OptID.LimitText(25);
	m_FxRate.Setup(this, IDC_BLOCK_FXRATE_EDIT);
	m_Rate.Setup(this, IDC_BLOCK_REPORATE_EDIT);
	m_Rate.LimitText(15);
	m_FloatRate.Setup(this, IDC_BLOCK_FLOAT_RATE_EDIT);
	m_Note.Setup(this, IDC_BLOCK_COMMENT_EDIT, EMPTYSTRING, NOCASE);
	m_Note.LimitText(30);
	m_Note2.Setup(this, IDC_BLOCK_COMMENT2_EDIT, EMPTYSTRING, NOCASE);
	m_Note2.LimitText(30);
	m_Asset.Setup(this, IDC_BLOCK_ASSET_EDIT);
	m_AssetDesc.Setup(this, IDC_BLOCK_ASSETDESC_EDIT);
	m_AssetCurrency.Setup(this, IDC_BLOCK_ASSETCURRENCY_EDIT);
	m_Ratio.Setup(this, IDC_BLOCK_RATIO_EDIT);
	m_Price.Setup(this, IDC_BLOCK_PRICE_EDIT);
	m_DownPymnt.Setup(this, IDC_BLOCK_DOWNPYMNT_EDIT);
	m_DV01.Setup(this, IDC_BLOCK_DV01_EDIT);

	CRawTicketRec *pTicket;
	pTicket = &m_Data.GetRawTicket();
	m_Data.Add(&m_Ticket, &pTicket->GetTicket());
	m_Data.Add(&m_Trader, &pTicket->GetTrader());
	m_Data.Add(&m_TradeDate, &pTicket->GetTradeDate());
	m_Data.Add(m_TransType, &pTicket->GetTransType());
	m_Data.Add(&m_Dir, &pTicket->GetDir());
	m_Data.Add(&m_ValueDate, &pTicket->GetValueDate());
	m_Data.Add(&m_Amount, &pTicket->GetNomAmount());
	m_Data.Add(&m_Strike, &pTicket->GetStrike());
	m_Data.Add(&m_OptExp, &pTicket->GetOptExp());
	m_Data.Add(m_Listed, &pTicket->GetListed());
	m_Data.Add(&m_OptTicker, &pTicket->GetOptTicker());
	m_Data.Add(&m_OptID, &pTicket->GetOptID());
	m_Data.Add(m_EuroOpt, &pTicket->GetEuroOpt());
	m_Data.Add(m_OptAuto, &pTicket->GetOptAuto());
	m_Data.Add(&m_OptSet, &pTicket->GetOptSetCode());
	m_Data.Add(&m_OptSet2, &pTicket->GetOptSetCode2());
	m_Data.Add(m_CP, &pTicket->GetCP());
	m_Data.Add(m_Currency, &pTicket->GetCurrency());
	m_Data.Add(&m_FxRate, &pTicket->GetFxRate());
	m_Data.Add(&m_Asset, &pTicket->GetAsset());
	m_Data.Add(&m_AssetDesc, &pTicket->GetAssetDesc());
	m_Data.Add(&m_AssetCurrency, &pTicket->GetAssetCurr());
	m_Data.Add(m_IPO, &pTicket->GetIPO());
	m_Data.Add(m_Trs, &pTicket->GetTRS());
	m_Data.Add(m_RepoCP, &pTicket->GetRepoCP());
	m_Data.Add(&m_Rate, &pTicket->GetRate());
	m_Data.Add(m_RateBasis, &pTicket->GetRateBasis());
	m_Data.Add(&m_Maturity, &pTicket->GetMaturity());
	m_Data.Add(&m_Note, &pTicket->GetNote());
	m_Data.Add(&m_Note2, &pTicket->GetNote2());
	m_Data.Add(&m_SWBooking, &pTicket->GetSWBooking());
	m_Data.Add(&m_SWMaturity, &pTicket->GetSWMaturity());
	m_Data.Add(&m_FloatRate, &pTicket->GetSWFloatRate());
	m_Data.Add(m_FloatRateBasis, &pTicket->GetSWFloatBasis());
	m_Data.Add(&m_FloatMaturity, &pTicket->GetSWFloatMaturity());
	m_Data.Add(m_WI, &pTicket->GetWI());
	m_Data.Add(&m_SwapTicket, &pTicket->GetSwapTicket());
	m_Data.Add(&m_DeliveryDate, &pTicket->GetDeliveryDate());
	m_Data.Add(&m_Margin, &pTicket->GetMargin());
	m_Data.Add(m_MarginCurrency, &pTicket->GetMarginCurrency());
	m_Data.Add(&m_MarginAmount, &pTicket->GetMarginAmount());
	m_Data.Add(m_Funded, &pTicket->GetFunded());
	m_Data.Add(&m_Binary, &pTicket->GetBinary());
	m_Data.Add(m_BestExecution, &pTicket->GetBestExecution());
	m_Data.Add(m_ShortSale, &pTicket->GetShortSale());
	m_Data.Add(&m_Price, &pTicket->GetPrice());
	m_Data.Add(&m_DownPymnt, &pTicket->GetDownPymnt());
	m_Data.Add(&m_BrokerFee, &pTicket->GetBrokerFee());
	m_Data.Add(&m_OtherFee, &pTicket->GetOtherFee());
	m_Data.Add(&m_SoftDollar, &pTicket->GetSoftDollar());
	m_Data.Add(m_AssignCP, &pTicket->GetAssignCP());
	m_Data.Add(&m_DV01, &pTicket->GetDV01());
	m_Data.Add(&m_VAR, &pTicket->GetVAR());
	m_Data.Add(&m_UnWindTicket, &pTicket->GetUnWindTicket());
	m_Data.Add(&m_CancelTicket, &pTicket->GetCancelTicket());
	m_Data.Add(&m_CorrectTicket, &pTicket->GetCorrectTicket());
	m_Data.Add(&pTicket->GetAssetCountry());
	m_Data.Add(&pTicket->GetAssetCommon());
	m_Data.Add(&pTicket->GetAssetCusip());
	m_Data.Add(&pTicket->GetAssetIsin());
	m_Data.Add(&pTicket->GetAssetSedol());
	m_Data.Add(&pTicket->GetAssetClass());
	m_Data.Add(&pTicket->GetAssetIndustry());
	m_Data.Add(&pTicket->GetAssetCategory());
	m_Data.Add(&pTicket->GetAssetBucket());
	m_Data.Add(&pTicket->GetAssetSource());
	m_Data.Add(&pTicket->GetAssetCoupon());
	m_Data.Add(&pTicket->GetAssetMaturity());
}

BOOL CTicketBlock::IsOK()
{
	CString TransType, Text, Data;
	CQData QData;
	
	m_TransType->GetSelString(TransType);

	if(TransType.IsEmpty())
		Text = "Invalid Transaction Type. Must enter Transaction Type.";
	
	if(TransType == SECURITIES ||TransType == CDS || TransType == PUT || 
		TransType == CALL || TransType == REPO || TransType == LEVERAGE)
	{		
		if(m_Asset.GetWindowTextLength() == 0 && m_AssetDesc.GetWindowTextLength() == 0)
			Text = "Invalid Asset Code. Must enter either Asset Code or Asset Description.";
	}

	if(TransType == INTSWAP && m_DV01.GetWindowTextLength() == 0)
		Text = "You must enter DV01 for IRS";

	if(m_TradeDate.GetData().IsEmpty())
		Text = "Invalid Trade Date.  Must enter Trade Date.";

	m_Dir.GetSelString(Data);
	if(Data.IsEmpty())
		Text = "Invalid Transaction Direction.  Must enter Transaction direction.";
		
	if(m_Amount.GetWindowTextLength() == 0)
		Text = "Invalid Nominal Amount.  Must enter Nominal Amount.";
	
	m_CP->GetSelString(Data);
	if(Data.IsEmpty())
		Text = "Invalid Counterparty.  Must enter a couterparty.";

	if(m_Trader.GetCurSel() < 0)
		Text = "You must choose a trader.";

	Data = m_Data.GetRawTicket().GetAssetClass();
	if(Data == "CURRENCY OPTION" && (TransType == CALL || TransType == PUT))
	{
		if(m_OptTicker.GetWindowTextLength() <= 0)
			Text = "Invalid Option Ticker";
		else
			if(m_OptID.GetWindowTextLength() <= 0)
				Text = "Invalid Option ID";
			else
				if(m_DeliveryDate.GetData().IsEmpty())
					Text = "Invalid Option Delivery Date";
				else
					if(m_OptSet.GetWindowTextLength() <= 0)
						Text = "You must select a option settlement code";
	}

	if(TransType != FOREX && m_Margin.GetWindowTextLength() <= 0 && m_MarginAmount.GetWindowTextLength() <= 0)
		Text = "You must enter Margin percentage or Margin Amount";

	if(m_BestExecution->GetCurSel() < 0)
		Text = "You must choose Best Execution Method";

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}

BOOL CTicketBlock::UpdateData(BOOL bSaveandValid)
{
	if(bSaveandValid)
	{
		m_Try = 0;
		m_SS.SetFocus();
		m_Data.UpdateRow();
	}
	else
	{
		BeginWaitCursor();
		CQData QData;
		COraLoader OraLoader;

		OraLoader = m_Data.GetOraLoader();
		m_Data.LoadTickets();

		if(!m_bLoaded)
		{
			GetData().LoadSupportData();
			GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);	/* TransDir */
			GetData().GetTraderArr().CopyToMCComboBox(m_Trader); /* Trader */
			QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_Currency, 0, FALSE);	/* Currency */
			GetData().GetCurrencyArr().CopyToComboBox(*m_MarginCurrency); /* Margin Currency */
			GetData().GetRateBasisArr().CopyToComboBox(*m_RateBasis); /* RateBasis */
			GetData().GetRateBasisArr().CopyToComboBox(*m_FloatRateBasis); /* FloatRateBasis */
			QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_CP, FALSE); /* Counterparty */
			QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_RepoCP, FALSE); /* Repo Counterparty */
			QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_AssignCP); /* Giveup Counterparty */
			GetData().GetTransTypeArr().CopyToComboBox(*m_TransType); /* Trans Type */
			GetData().GetBestExecutionArr().CopyToComboBox(*m_BestExecution);
		
			m_bLoaded = TRUE;
		}
		EndWaitCursor();
	}

	m_SS.SetSheetCurRow(-1);

	Invalidate();
	UpdateWindow();
	OnBlockRefresh();
	return TRUE;
}

void CTicketBlock::UpdateNumber(CWnd* Ctrl, double Number, BOOL WholeNumber)
{
	CQData QData;
	
	Ctrl->SetWindowText(QData.WriteNumber(Number, TRUE, WholeNumber ? 0 : -1));
}

void CTicketBlock::UpdateRatio()
{
	CString TransType, Asset;
	COraLoader OraLoader;
	CQData QData;

	m_Ratio.SetWindowText(":");
	m_TransType->GetSelString(TransType);
	if(TransType == CDS || TransType == SECURITIES)
	{
		OraLoader = GetData().GetOraLoader();
		m_Asset.GetWindowText(Asset);
		OraLoader.GetSql().Format("SELECT TO_CHAR(TRUNC(EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(PERFISO_WT, 0)) "
					"FROM SEMAM.NW_EMCFPERFISO_ALLOC_V WHERE ASSET_CODE = %s", QData.GetQueryText(Asset));
		OraLoader.Open();
		OraLoader.LoadText(m_Ratio);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CTicketBlock message handlers
void CTicketBlock::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BeginWaitCursor();

	InitControls();
	if(GetData().IsTrader())
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, GetData().GetTraderIni(), GetData().GetOraUser());
	else
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, EMPTYSTRING, GetData().GetOraUser());
	
	GetData().LoadDates();
	GetDocument()->SetViewTitle("TICKET BLOCK");
	m_Try = 0;

	UpdateData(FALSE);
	m_Data.Refresh();
	EndWaitCursor();

	OnBlockLoadTrade();
}

BEGIN_EVENTSINK_MAP(CTicketBlock, CFormView)
    //{{AFX_EVENTSINK_MAP(CTicketBlock)
	ON_EVENT(CTicketBlock, IDC_BLOCK_TICKET_LIST, 2 /* BlockSelected */, OnBlockSelectedTicketList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTicketBlock, IDC_BLOCK_TICKET_LIST, 11 /* DblClick */, OnDblClickTicketList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTicketBlock::OnSelchangeTranstypeCombo() 
{
	OnSelchangeTransdirCombo();
	OnListedCheck();
	m_DownPymnt.SetWindowText("100");
}

void CTicketBlock::OnSelchangeTransdirCombo() 
{
	CString TransType, Dir;
	
	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	GetData().DefineTransType(TransType, Dir);
	if(TransType.IsEmpty())
		return;

	if(TransType == REPO || TransType == LEVERAGE || TransType == BORROW || TransType == LEND)
	{
		if(m_RateBasis->GetCurSel() < 0)		
			m_RateBasis->SelectString(-1, "A/360");
	}

	if(TransType == FOREX)
	{
		m_Asset.SetWindowText("");
		m_AssetDesc.SetWindowText("");
		m_AssetCurrency.SetWindowText("");
	}

	if(Dir == P)
		m_ShortSale->SetWindowText("CShort");
	else
		if(Dir == S)
			m_ShortSale->SetWindowText("SShort");

	EnableCtrls();
}

void CTicketBlock::OnSelchangeCurrencyCombo() 
{	
	CString Text;
	
	m_Currency->GetSelString(Text);
	if(Text.IsEmpty())
		return;
	
	if(Text == "USD")
	{
		m_FxRate.SetWindowText("1");
		return;
	}
	
	CString Fxrate;

	m_FxRate.GetWindowText(Fxrate);
	if(Fxrate = "1")
		m_FxRate.SetWindowText(EMPTYSTRING);
}

void CTicketBlock::OnBlockFindAsset() 
{	
	CAssetDlg Dlg;
	CString TransType, Text;

	TransType = m_TransType->GetSelString(TransType);
	
	m_Try = 1;

	Dlg.m_pData = &GetData();
	m_Asset.GetWindowText(Dlg.m_FindData.GetRec().GetAsset());
	m_AssetDesc.GetWindowText(Dlg.m_FindData.GetRec().GetDesc());
	m_OptTicker.GetWindowText(Dlg.m_FindData.GetRec().GetOptTicker());

	if(Dlg.DoModal() == IDOK)
	{
		CRawTicketRec *pTicket;

		pTicket = &m_Data.GetRawTicket();
		pTicket->FromAssetRec(Dlg.m_FindData.GetRec());
		m_Asset.SetWindowText(pTicket->GetAsset());
		m_AssetDesc.SetWindowText(pTicket->GetAssetDesc());
		m_AssetCurrency.SetWindowText(pTicket->GetAssetCurr());
		m_Trader.SelectString(0, Dlg.m_FindData.GetTrader());

		m_Future = Dlg.m_Future;
		m_UnWindTicket.SetData(Dlg.m_FindData.GetTicket());

		CString Currency, Fxrate;

		m_Currency->GetSelString(Currency);
		if(Currency.IsEmpty() || 
			(Currency != pTicket->GetAssetCurr() &&
			MessageBox("Note: Trading Currency is Different from Asset Currency! Do you want to synchonize them ?", 
						"Warning", MB_YESNO) == IDYES))
			m_Currency->SelectString(-1, pTicket->GetAssetCurr());

		m_Currency->GetSelString(Currency);
		if(m_FxRate.GetWindowTextLength() == 0)
			m_FxRate.SetWindowText(Currency == USD ? "1" : EMPTYSTRING);

		m_FxRate.GetWindowText(Fxrate);
		if(Currency != USD && Fxrate == "1")
			m_FxRate.SetWindowText(EMPTYSTRING);

		if(TransType == CALL || TransType == PUT)
		{
			if(pTicket->GetAssetClass().Find("CURRENCY", 0) >= 0)
				m_EuroOpt->SetCheck(1);
			else
				m_EuroOpt->SetCheck(0);
		}
	}

	m_Asset.GetWindowText(Text);

	if(TransType == FOREX && Text == "NEW ASSET")
	{
		m_Asset.SetWindowText("");
		m_AssetDesc.SetWindowText("");
		m_AssetCurrency.SetWindowText(NULL);
	}
	
	EnableCtrls();
	UpdateRatio();
	m_Note.SetFocus();
}

void CTicketBlock::OnBlockSelectedTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickTicketList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CTicketBlock::OnDblClickTicketList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row);

		EnableCtrls();
	}
	else
	{
		OnBlockRefresh();
		m_dNomAmount = 0;
	}
}

void CTicketBlock::OnBlockAdd() 
{	
   	if(!IsOK())
		return;

	CString Ticket;

	m_SS.SetFocus();
	GetData().GetOraLoader().GenerateUniqueID(TICKETNUM, Ticket);
	m_Ticket.SetData(Ticket);
	m_Data.GetRawTicket().SetID(Ticket);

	UpdateData(TRUE);
}

void CTicketBlock::OnBlockDelete() 
{
	if(MessageBox("Are you sure you want to delete these tickets ?",
	  			  "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	m_Try = 0;
	m_Data.DeleteRow();
}

void CTicketBlock::OnBlockUpdate() 
{
	if(!IsOK())
		return;

	m_SS.SetFocus();
	UpdateData(TRUE);
}

void CTicketBlock::OnBlockRefresh() 
{
	BeginWaitCursor();

	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();

	m_TradeDate.SetData(GetData().GetDate());
	m_ValueDate.SetData(GetData().GetPlus3Date());
	m_TransType->SelectString(-1, SECURITIES);
	m_FxRate.SetWindowText("1");
	m_Currency->SetCurSel(m_Currency->FindString(-1, USD));
	m_SS.SheetAutoFit();
	m_OptSetID = 0;

	EnableCtrls();

	EndWaitCursor();
}

void CTicketBlock::OnUpdateBlockAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter() && m_SS.GetSheetCurRow() <= 0);	
}

void CTicketBlock::OnUpdateBlockDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter() && 
					m_SS.GetIsBlockSelected() && m_SS.GetSheetCurRow() >= 1);
}

void CTicketBlock::OnUpdateBlockFindAsset(CCmdUI* pCmdUI) 
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	pCmdUI->Enable(m_Data.GetBlotter() || TransType == CASH || TransType == INTSWAP ? FALSE : TRUE);
}

void CTicketBlock::OnUpdateBlockUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter() && m_SS.GetSheetCurRow() >= 1 ? TRUE : FALSE);	
}

void CTicketBlock::OnUpdateBlockCopyTicket(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SS.GetSheetCurRow() >= 1 ? TRUE : FALSE);	
}

void CTicketBlock::OnListedCheck() 
{
	CString Date, TransType;

	Date = m_TradeDate.GetData();
	if(Date != GetData().GetDate()) // Automatic change dates only when entry date is today
		return;

	m_TransType->GetSelString(TransType);

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, TransType, m_Listed->GetCheck()));
}

void CTicketBlock::OnSetfocusAssetEdit() 
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	if((TransType == FOREX && m_Try) || m_Asset.GetWindowTextLength() > 0)
		return;

	m_Asset.SetWindowText(NEWASSET);	
	OnBlockFindAsset();	
}

void CTicketBlock::OnSelchangeLevsourceCombo() 
{
	EnableRepoCtrls(m_RepoCP->GetCurSel() >= 0 ? TRUE : FALSE);
}

void CTicketBlock::OnBlockCopyTicket() 
{
	OnBlockAdd();
}

void CTicketBlock::OnUpdateBlockBrokerfee(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter());
}

void CTicketBlock::OnBlockBrokerfee() 
{
	CBrokerFee BrFee;

	m_Amount.GetWindowText(BrFee.m_sAmount);
	if(BrFee.DoModal() == IDOK)
		m_BrokerFee.SetWindowText(BrFee.m_Total);
}

void CTicketBlock::OnKillfocusDownpymntEdit() 
{
	CString Text;
	CQData QData;

	m_DownPymnt.GetWindowText(Text);
	if(Text.IsEmpty())
		return;

	double DownPymnt;

	DownPymnt = atof(QData.RemoveComma(Text));
	if(DownPymnt < 0 || DownPymnt > 100)
	{
		MessageBox("Down Payment must be between 0 ~ 100%");
		m_DownPymnt.SetFocus();
	}
}

void CTicketBlock::OnBlockLoadBlotter() 
{
	m_Data.SetBlotter(TRUE);
	UpdateData(FALSE);
}

void CTicketBlock::OnBlockLoadTrade() 
{
	m_Data.SetBlotter(FALSE);
	UpdateData(FALSE);
}

void CTicketBlock::OnBlockOptionExp()
{
	COptExpDlg Dlg;

	Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
	if(Dlg.DoModal() == IDOK)
		m_OptExp.SetData(Dlg.m_Date);
}

void CTicketBlock::OnUpdateBlockOptionExp(CCmdUI* pCmdUI)
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	pCmdUI->Enable(TransType == PUT || TransType == CALL ? TRUE : FALSE);
}

void CTicketBlock::OnUpdateBlockContractConvertor(CCmdUI* pCmdUI)
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	pCmdUI->Enable(TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F") ? TRUE : FALSE);
}

void CTicketBlock::OnBlockContractConvertor()
{
	CContractConvertor Dlg;
	int Status;

	m_Asset.SetFocus();
	m_Asset.GetWindowText(Dlg.m_AssetStr);
	Dlg.m_OraLoader.SetDB(GetData().GetOraLoader().GetDB());
	Status = Dlg.DoModal();
	
	if(Status != IDOK)
		return;

	if(m_nNomID == IDC_BLOCK_AMOUNT_EDIT)
		m_Amount.SetWindowText(Dlg.m_NominalStr);
}

void CTicketBlock::OnSetfocusAmountEdit()
{
	CString TransType;

	m_TransType->GetSelString(TransType);

	m_nNomID = IDC_BLOCK_AMOUNT_EDIT;

	if((TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F")) && 
		m_Amount.GetWindowTextLength() == 0)
	{
		OnBlockContractConvertor();
		if(m_Amount.GetWindowTextLength() > 0)
			m_Amount.SetFocus();
	}
}

void CTicketBlock::OnBlockDealCheck() 
{
	BOOL bChecked;
	CString Text;

	bChecked = m_Trs->GetCheck();
	m_DownPymnt.EnableWindow(bChecked);
	
	m_DownPymnt.GetWindowText(Text);
	if(bChecked)
	{
		if(Text == "100")
			m_DownPymnt.SetWindowText("0");
	}
	else
		if(Text == "0")
			m_DownPymnt.SetWindowText("100");
}

void CTicketBlock::OnKillfocusAmountEdit() 
{
}

void CTicketBlock::OnBlockXcurrency() 
{
	CXCurrencyDlg Dlg;

	Dlg.m_pData = &GetData();
	Dlg.m_UserClass = 1;
	Dlg.DoModal();
}

void CTicketBlock::OnSetfocusOptSetEdit()
{
	m_OptSetID = IDC_BLOCK_OPTSET_EDIT;
}

void CTicketBlock::OnSetfocusOptSet2Edit()
{
	m_OptSetID = IDC_BLOCK_OPTSET2_EDIT;
}

void CTicketBlock::OnBlockRepoprice()
{
	CRepoPrice Dlg;
	CString	ValueDate, RateBasis, Formula;
	
	m_Asset.GetWindowText(Dlg.m_Asset);
	if(Dlg.m_Asset.IsEmpty() || Dlg.m_Asset == "NEW ASSET")
		return;

	m_Margin.GetWindowText(Dlg.m_HairCut);
	if(Dlg.m_HairCut.IsEmpty())
	{
		if(m_Trs->GetCheck())
			Dlg.m_HairCut = "0";
		else
			Dlg.m_HairCut = "10";
	}
	
	m_Price.GetWindowText(Dlg.m_NWINetPrice);
	ValueDate = m_ValueDate.GetData();
	m_RateBasis->GetSelString(RateBasis); // Lev Rate Basis
	Dlg.m_Val.Setup(GetData().GetOraLoader(), REPO, P, Dlg.m_Asset, ValueDate, ValueDate, 
					"0", "0", "1", RateBasis, "1", Formula);
					
	if(Dlg.DoModal() == IDOK)
	{
		m_Price.SetWindowText(Dlg.m_NWINetPrice);
		m_Margin.SetWindowText(Dlg.m_HairCut);
	}
}

void CTicketBlock::OnUpdateBlockRepoprice(CCmdUI* pCmdUI)
{
	CString TransType, Asset;

	m_Asset.GetWindowText(Asset);
	m_TransType->GetSelString(TransType);

	pCmdUI->Enable(TransType == REPO && (!Asset.IsEmpty() && Asset != "NEW ASSET"));
}

void CTicketBlock::OnChangeAssetEdit()
{
	CString Asset, TransType;
	CRawTicketRec *pTicket;

	m_Asset.GetWindowText(Asset);
	m_TransType->GetSelString(TransType);

	pTicket = &m_Data.GetRawTicket();

	if(TransType == SECURITIES && pTicket->GetAssetClass() == "CURRENCY FWDS")
	{
		if(m_Data.GetMargin(Asset))
		{
			if(strlen(pTicket->GetMarginCurrency()) > 0)
				m_MarginCurrency->SelectString(-1, pTicket->GetMarginCurrency());
			if(strlen(pTicket->GetMarginAmount()) > 0)
				m_MarginAmount.SetWindowText(pTicket->GetMarginAmount());
			if(strlen(pTicket->GetMargin()) > 0)
				m_Margin.SetWindowText(pTicket->GetMargin());
		}
	}
}

void CTicketBlock::OnBlockSettlementtime()
{
	COptSettlementDlg Dlg;

	Dlg.m_OraLoader = m_Data.GetOraLoader();
	switch(m_OptSetID)
	{
		case IDC_BLOCK_OPTSET_EDIT:
			m_OptSet.GetWindowText(Dlg.m_SetCode);
			Dlg.m_Title = "Settlement Time";
			if(Dlg.DoModal() == IDOK)
				m_OptSet.SetWindowText(Dlg.m_SetCode);
			break;
		case IDC_BLOCK_OPTSET2_EDIT:
			m_OptSet2.GetWindowText(Dlg.m_SetCode);
			Dlg.m_Title = "Settlement Time 2";
			if(Dlg.DoModal() == IDOK)
				m_OptSet2.SetWindowText(Dlg.m_SetCode);
		default:
			return;
	}
}

