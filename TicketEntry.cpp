// TicketEntry.cpp : implementation file
//
#include "stdafx.h"
#include <math.h>
#include "NWIFMS70.h"
#include "TicketEntry.h"
#include "assetdlg.h"
#include "brokerfee.h"
#include "qdata.h"
#include "contractconvertor.h"
#include "optexpdlg.h"
#include "xcurrencydlg.h"
#include "repoprice.h"
#include "Finddlg.h"
#include "ProhibitList.h"
#include "DATEDLG.H"
#include "TrenchCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTicketEntry
IMPLEMENT_DYNCREATE(CTicketEntry, CFormView)

CTicketEntry::CTicketEntry()
	: CFormView(CTicketEntry::IDD)
{
	//{{AFX_DATA_INIT(CTicketEntry)
	m_bAllocValid = TRUE;
	m_bLoaded = FALSE;
	//}}AFX_DATA_INIT
}

void CTicketEntry::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTicketEntry)
	DDX_Control(pDX, IDC_ENTRY_TICKET_LIST, m_SS);
	DDX_Control(pDX, IDC_ENTRY_ALLOCATION_LIST, m_AllocSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTicketEntry, CFormView)
	//{{AFX_MSG_MAP(CTicketEntry)
	ON_CBN_SELCHANGE(IDC_ENTRY_TRANSTYPE_COMBO, OnSelchangeTranstypeCombo)
	ON_COMMAND(ID_ENTRY_ADD, OnEntryAdd)
	ON_COMMAND(ID_ENTRY_DELETE, OnEntryDelete)
	ON_COMMAND(ID_ENTRY_UPDATE, OnEntryUpdate)
	ON_COMMAND(ID_ENTRY_REFRESH, OnEntryRefresh)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_UPDATE, OnUpdateEntryUpdate)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_DELETE, OnUpdateEntryDelete)
	ON_CBN_SELCHANGE(IDC_ENTRY_TRANSDIR_COMBO, OnSelchangeTransdirCombo)
	ON_CBN_SELCHANGE(IDC_ENTRY_CURRENCY_COMBO, OnSelchangeCurrencyCombo)
	ON_COMMAND(ID_ENTRY_FIND_ASSET, OnEntryFindAsset)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_FIND_ASSET, OnUpdateEntryFindAsset)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_ADD, OnUpdateEntryAdd)
	ON_BN_CLICKED(IDC_ENTRY_LISTED_CHECK, OnListedCheck)
	ON_EN_SETFOCUS(IDC_ENTRY_ASSET_EDIT, OnSetfocusAssetEdit)
	ON_EN_CHANGE(IDC_ENTRY_ASSET_EDIT, OnChangeAssetEdit)
	ON_CBN_SELCHANGE(IDC_ENTRY_LEVSOURCE_COMBO, OnSelchangeLevsourceCombo)
	ON_COMMAND(ID_ENTRY_COPY_TICKET, OnEntryCopyTicket)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_COPY_TICKET, OnUpdateEntryCopyTicket)
	ON_COMMAND(ID_ENTRY_ADD_ALLOCATION, OnEntryAddAllocation)
	ON_COMMAND(ID_ENTRY_BROKERFEE, OnEntryBrokerfee)
	ON_COMMAND(ID_ENTRY_REPOPRICE, OnEntryRepoprice)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_REPOPRICE, OnUpdateEntryRepoprice)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_BROKERFEE, OnUpdateEntryBrokerfee) 
	ON_COMMAND(ID_ENTRY_DELETE_ALLOCATION, OnEntryDeleteAllocation)
	ON_COMMAND(ID_ENTRY_UPDATE_ALLOCATION, OnEntryUpdateAllocation)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_DELETE_ALLOCATION, OnUpdateEntryDeleteAllocation)
	ON_EN_SETFOCUS(IDC_ENTRY_AMOUNT_EDIT, OnSetfocusAmountEdit)
	ON_EN_CHANGE(IDC_ENTRY_AMOUNT_EDIT, OnEnChangeEntryAmountEdit)
	ON_EN_SETFOCUS(IDC_ENTRY_ALLOC_NOMAMOUNT_EDIT, OnSetfocusAllocNomamountEdit)
	ON_EN_KILLFOCUS(IDC_ENTRY_ALLOC_NOMAMOUNT_EDIT, OnKillfocusAllocNomamountEdit)
	ON_EN_KILLFOCUS(IDC_ENTRY_DOWNPYMNT_EDIT, OnKillfocusDownpymntEdit)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_UPDATE_ALLOCATION, OnUpdateEntryUpdateAllocation)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_ADD_ALLOCATION, OnUpdateEntryAddAllocation)
	ON_CBN_SELCHANGE(IDC_ENTRY_PORTFOLIO_COMBO, OnSelchangeEntryPortfolioCombo)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_CONTRACT_CONVERTOR, OnUpdateEntryContractConvertor)
	ON_COMMAND(ID_ENTRY_CONTRACT_CONVERTOR, OnEntryContractConvertor)
	ON_COMMAND(ID_ENTRY_LOAD_BLOTTER, OnEntryLoadBlotter)
	ON_COMMAND(ID_ENTRY_LOAD_TRADE, OnEntryLoadTrade)
	ON_COMMAND(ID_ENTRY_OPTION_EXP, OnEntryOptionExp)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_OPTION_EXP, OnUpdateEntryOptionExp)
	ON_BN_CLICKED(IDC_ENTRY_DEAL_CHECK, OnEntryDealCheck)
	ON_EN_KILLFOCUS(IDC_ENTRY_AMOUNT_EDIT, OnKillfocusAmountEdit)
	ON_COMMAND(ID_ENTRY_XCURRENCY, OnEntryXcurrency)
	ON_CBN_SELCHANGE(IDC_ENTRY_AA_COMBO, &CTicketEntry::OnCbnSelchangeEntryAaCombo)
	ON_COMMAND(ID_ENTRY_FORBIDDEN_LIST, OnEntryForbiddenList)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_ENTRY_BINARY_EDIT, &CTicketEntry::OnEnSetfocusEntryBinaryEdit)
	ON_COMMAND(ID_ENTRY_FINDITEM, &CTicketEntry::OnEntryFinditem)
	ON_COMMAND(ID_ENTRY_CANCEL, &CTicketEntry::OnEntryCancel)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_CANCEL, &CTicketEntry::OnUpdateEntryCancel)
	ON_CBN_KILLFOCUS(IDC_ENTRY_AA_COMBO, &CTicketEntry::OnCbnKillfocusEntryAaCombo)
	ON_EN_CHANGE(IDC_ENTRY_PRICE_EDIT, &CTicketEntry::OnEnChangeEntryPriceEdit)
	ON_EN_KILLFOCUS(IDC_ENTRY_PRICE_EDIT, &CTicketEntry::OnEnKillfocusEntryPriceEdit)
	ON_EN_KILLFOCUS(IDC_ENTRY_ASSET_EDIT, &CTicketEntry::OnEnKillfocusEntryAssetEdit)
	ON_COMMAND(ID_ENTRY_TRENCHCOPY_TICKET, &CTicketEntry::OnEntryTrenchcopyTicket)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_TRENCHCOPY_TICKET, &CTicketEntry::OnUpdateEntryTrenchcopyTicket)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTicketEntry diagnostics
#ifdef _DEBUG
void CTicketEntry::AssertValid() const
{
	CFormView::AssertValid();
}

void CTicketEntry::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CTicketEntry::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

BOOL CTicketEntry::AllocIsOK()
{
	COraLoader OraLoader;
	CString Text, Data, TransType, Nom;
	CQData QData;

	OraLoader = GetData().GetOraLoader();
	if(!m_Ticket.GetData().IsEmpty())
	{
		OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKETS "
								"WHERE (PROCESSED IS NOT NULL OR SIGN IS NOT NULL) "
								"AND TICKET_NUM = %s ",	QData.GetQueryNumber(m_Ticket.GetData()));
		if(OraLoader.GetCount() > 0)
			Text = "No change is allowed because the ticket has been approved";
	}

	Data = m_Portfolio.GetData();
	if(Data.IsEmpty())
		Text = "Must select a portfolio";

	Nom = m_AllocAmount.GetData();
	if(m_Data.AllocCheckSum(Data, atof(QData.RemoveComma(Nom))) > 0)
		Text = "Sum of the shares in each portfolios must be equal to the Nominal Amount";

	TransType = m_TransType.GetData();
	if(TransType != FOREX && TransType != INTSWAP && m_Price.GetWindowTextLength() == 0)		
		Text = "Price must be entered";

	if(m_DownPymnt.GetWindowTextLength() == 0)
		Text = "The Percentage of down payment must be entered";

	Data = m_RepoCP.GetData();
	if(!Data.IsEmpty())
	{
		if(atof(QData.RemoveComma(m_DownPymnt.GetData())) == 100)
			Text = "Repo source has been entered. Down payment must be less than 100%.";
	}

	if(fabs(atof(QData.RemoveComma(m_VAR.GetData()))) == 0)
		Text = "VAR cannot be zero";

	if(m_AllocArray.GetSize() == m_AllocSS.GetSheetRows() && m_AllocArray.GetSize() > 0)
	{
		for(int i = 1; i <= m_AllocSS.GetSheetRows(); i ++)
		{
			if(m_AllocArray.GetAt(i - 1)->GetAt(1) != m_AllocSS.GetSheetText(2, i))
			{
				Text = "Auto Allocation Method is violated, You must select AAF Reason";
				break;
			}
		}
	}

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}


void CTicketEntry::EnableAlloc()
{
	m_Portfolio.EnableWindow(!m_Data.GetBlotter());
	m_AllocAmount.SetReadOnly(m_Data.GetBlotter()); // If blotter, read only
//	m_Link.SetReadOnly(m_Data.GetBlotter());
//	m_AssignCP->EnableWindow(m_Data.GetBlotter());
}

void CTicketEntry::EnableAllocCtrls()
{
	m_AllocAmount.EnableWindow(m_Portfolio.GetCurSel() >= 0 ? TRUE : FALSE);
}

void CTicketEntry::EnableCtrls()
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

	GetDlgItem(IDC_ENTRY_STRIKE_STATIC)->ShowWindow(SW_HIDE);
	m_Strike.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_EXPIRATION_STATIC)->ShowWindow(SW_HIDE);
	m_OptExp.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_OPT_TICKER_STATIC)->ShowWindow(SW_HIDE);
	m_OptTicker.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_OPTSET_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_OPTSET2_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet2.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_BLMID_STATIC)->ShowWindow(SW_HIDE);
	m_OptID.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_MARGIN_STATIC)->ShowWindow(SW_HIDE);
	m_Margin.ShowWindow(SW_HIDE);
	m_MarginCurrency.ShowWindow(SW_HIDE);
	m_MarginAmount.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_SWAP_TICKET_STATIC)->ShowWindow(SW_HIDE);
	m_SwapTicket.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_LEVSOURCE_STATIC)->ShowWindow(SW_HIDE);
	m_RepoCP.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_SWMATURITY_STATIC)->ShowWindow(SW_HIDE);
	m_SWMaturity.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_FLOAT_RATE_STATIC)->ShowWindow(SW_HIDE);
	m_FloatRate.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_FLOAT_RATE_BASIS_STATIC)->ShowWindow(SW_HIDE);
	m_FloatRateBasis.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_FLOAT_MATURITY_STATIC)->ShowWindow(SW_HIDE);
	m_FloatMaturity.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_SWAP_TICKET_STATIC)->ShowWindow(SW_HIDE);
	m_SwapTicket.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ENTRY_DELIVERY_STATIC)->ShowWindow(SW_HIDE);
	m_DeliveryDate.ShowWindow(SW_HIDE);

	EnableRepoCtrls(FALSE);

	m_Listed.ShowWindow(SW_HIDE);
	m_EuroOpt.ShowWindow(SW_HIDE);
	m_Binary.ShowWindow(SW_HIDE);
	m_IPO.ShowWindow(SW_HIDE);
	m_Trs.ShowWindow(SW_HIDE);
	
	RepoCP = m_RepoCP.GetData();
	TransType = m_TransType.GetData();
	Dir = m_Dir.GetData();
	GetData().DefineTransType(TransType, Dir);

	if(TransType == SECURITIES || TransType == CDS || TransType == FOREX || TransType == CALL || TransType == PUT)
		m_AssignCP.EnableWindow(TRUE);
	else
		m_AssignCP.EnableWindow(FALSE);

	if(TransType == INTSWAP || TransType == CASH)
	{
		GetDlgItem(IDC_ENTRY_ASSET_STATIC)->ShowWindow(SW_HIDE);
		m_Asset.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ENTRY_ASSETDESC_STATIC)->ShowWindow(SW_HIDE);
		m_AssetDesc.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ENTRY_CURRENCY_STATIC)->ShowWindow(SW_HIDE);
		m_AssetCurrency.ShowWindow(SW_HIDE);
		m_Price.ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_ENTRY_ASSET_STATIC)->ShowWindow(SW_SHOW);
		m_Asset.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_ASSETDESC_STATIC)->ShowWindow(SW_SHOW);
		m_AssetDesc.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_CURRENCY_STATIC)->ShowWindow(SW_SHOW);
		m_AssetCurrency.ShowWindow(SW_SHOW);
		m_Price.ShowWindow(SW_SHOW);
	}

	if(TransType != FOREX)
	{
		GetDlgItem(IDC_ENTRY_MARGIN_STATIC)->ShowWindow(SW_SHOW);
		m_Margin.ShowWindow(SW_SHOW);
		m_MarginCurrency.ShowWindow(SW_SHOW);
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
		GetDlgItem(IDC_ENTRY_LEVSOURCE_STATIC)->ShowWindow(SW_SHOW);
		m_RepoCP.ShowWindow(SW_SHOW);
		m_IPO.ShowWindow(SW_SHOW);
		OnSelchangeLevsourceCombo();
	}
	
	if(TransType == SECURITIES || TransType == REPO || TransType == LEVERAGE)
		m_Trs.ShowWindow(SW_SHOW);

	if(TransType == SECURITIES || TransType == CDS || TransType == CALL || TransType == PUT)
	{
		GetDlgItem(IDC_ENTRY_OPTSET_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_OPTSET2_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet2.ShowWindow(SW_SHOW);

		if(TransType == SECURITIES && m_Data.GetRawTicket().GetAssetClass() == "CURRENCY FWDS")
		{
			GetDlgItem(IDC_ENTRY_SWAP_TICKET_STATIC)->ShowWindow(SW_SHOW);
			m_SwapTicket.ShowWindow(SW_SHOW);
		}
	}
	
	if(TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
	{
		GetDlgItem(IDC_ENTRY_STRIKE_STATIC)->ShowWindow(SW_SHOW);
		m_Strike.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_EXPIRATION_STATIC)->ShowWindow(SW_SHOW);
		m_OptExp.ShowWindow(SW_SHOW);
		m_Listed.ShowWindow(SW_SHOW);
		m_EuroOpt.ShowWindow(SW_SHOW);
		m_Binary.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_OPT_TICKER_STATIC)->ShowWindow(SW_SHOW);
		m_OptTicker.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_BLMID_STATIC)->ShowWindow(SW_SHOW);
		m_OptID.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_DELIVERY_STATIC)->ShowWindow(SW_SHOW);
		m_DeliveryDate.ShowWindow(SW_SHOW);
	}

	if(TransType == INTSWAP)
	{
		GetDlgItem(IDC_ENTRY_SWMATURITY_STATIC)->ShowWindow(SW_SHOW);
		m_SWMaturity.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_FLOAT_RATE_STATIC)->ShowWindow(SW_SHOW);
		m_FloatRate.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_FLOAT_RATE_BASIS_STATIC)->ShowWindow(SW_SHOW);
		m_FloatRateBasis.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ENTRY_FLOAT_MATURITY_STATIC)->ShowWindow(SW_SHOW);
		m_FloatMaturity.ShowWindow(SW_SHOW);
	}

	if(m_Data.GetBlotter())
	{
		m_TransType.EnableWindow(FALSE);
		m_Dir.EnableWindow(FALSE);
		m_CP.EnableWindow(FALSE);
		m_Currency.EnableWindow(FALSE);
		m_FloatRateBasis.EnableWindow(FALSE);
		m_IPO.EnableWindow(FALSE);
		m_Listed.EnableWindow(FALSE);
		m_EuroOpt.EnableWindow(FALSE);
		m_Binary.EnableWindow(FALSE);
	}
	else
	{
		m_TransType.EnableWindow(TRUE);
		m_Dir.EnableWindow(TRUE);
		m_CP.EnableWindow(TRUE);
		m_Currency.EnableWindow(TRUE);
		m_FloatRateBasis.EnableWindow(TRUE);
		m_IPO.EnableWindow(TRUE);
		m_Listed.EnableWindow(TRUE);
		m_EuroOpt.EnableWindow(TRUE);
		m_Binary.EnableWindow(TRUE);
	}

	if(!(!RepoCP.IsEmpty() || TransType == REPO))
	{
		m_DownPymnt.EnableWindow(FALSE);
		m_DownPymnt.SetData("100");
	}
	else
		m_DownPymnt.EnableWindow(TRUE);
}

void CTicketEntry::EnableRepoCtrls(BOOL bEnable)
{
	m_Trs.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_ENTRY_REPORATE_STATIC)->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_Rate.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_ENTRY_RATE_BASIS_STATIC)->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_RateBasis.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_ENTRY_MATURITY_STATIC)->ShowWindow(bEnable ? SW_SHOW : SW_HIDE);
	m_Maturity.ShowWindow(bEnable ? SW_SHOW : SW_HIDE);

	if(m_Data.GetBlotter())
	{
		m_RepoCP.EnableWindow(FALSE);
		m_RateBasis.EnableWindow(FALSE);
		m_Trs.EnableWindow(FALSE);
	}
	else
	{
		m_RepoCP.EnableWindow(TRUE);
		m_RateBasis.EnableWindow(TRUE);
		m_Trs.EnableWindow(TRUE);
	}	
}

void CTicketEntry::InitControls()
{
	m_SS.SetVisibleCols(11);
	m_SS.SetVisibleRows(7);
	m_SS.SetSheetCols(7);
	m_SS.SetSheetRows(m_SS.GetMaxRows());
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);
	m_SS.SetSheetCurRow(-1);

	m_AllocSS.SetVisibleCols(2);
	m_AllocSS.SetVisibleRows(3);
	m_AllocSS.SetSheetCols(2);
	m_AllocSS.SetSheetRows(m_AllocSS.GetMaxRows());
	m_AllocSS.LockSheet();
	m_AllocSS.SetSheetRows(0);

	m_Trs.Setup(this, IDC_ENTRY_DEAL_CHECK, Y);
	m_IPO.Setup(this, IDC_ENTRY_IPO_CHECK, Y);
	m_Listed.Setup(this, IDC_ENTRY_LISTED_CHECK, Y);
	m_EuroOpt.Setup(this, IDC_ENTRY_EUROOPT_CHECK, Y);
	m_WI.Setup(this, IDC_ENTRY_WI_CHECK, Y);
	m_OptAuto.Setup(this, IDC_ENTRY_AUTO_CHECK, Y);
	m_Funded.Setup(this, IDC_ENTRY_FUNDED_CHECK, Y);
	m_ShortSale.Setup(this, IDC_ENTRY_SHORT_CHECK, Y);
	m_CSPBShort.Setup(this, IDC_ENTRY_CSPBSHORT_CHECK, Y);

    m_TransType.Setup(this, IDC_ENTRY_TRANSTYPE_COMBO);
    m_CP.Setup(this, IDC_ENTRY_COUNTERPARTY_COMBO);
    m_Currency.Setup(this, IDC_ENTRY_CURRENCY_COMBO);
	m_RepoCP.Setup(this, IDC_ENTRY_LEVSOURCE_COMBO, TRUE);
	m_RateBasis.Setup(this, IDC_ENTRY_RATE_BASIS_COMBO, TRUE);
	m_FloatRateBasis.Setup(this, IDC_ENTRY_FLOAT_RATE_BASIS_COMBO, TRUE);
	m_Portfolio.Setup(this, IDC_ENTRY_PORTFOLIO_COMBO);
	m_MarginCurrency.Setup(this, IDC_ENTRY_MARGIN_CURRENCY_COMBO, TRUE);
	m_AAFReason.Setup(this, IDC_ENTRY_AAF_COMBO, TRUE);
	m_BestExecution.Setup(this, IDC_ENTRY_BESTEXECUTION_COMBO);

	m_Ticket.Setup(this, IDC_ENTRY_TICKET_EDIT);
	m_SwapTicket.Setup(this, IDC_ENTRY_SWAP_TICKET_EDIT);
	m_UnWindTicket.Setup(this, IDC_ENTRY_UNWINDTICKET_EDIT);
	m_CancelTicket.Setup(this, IDC_ENTRY_CANCEL_TICKET_EDIT);
	m_CorrectTicket.Setup(this, IDC_ENTRY_CORRECT_TICKET_EDIT);

	m_TradeDate.Setup(this, IDC_ENTRY_TRADEDATE_EDIT, FALSE, "Trade Date must be greater than or equal to Today's date");
	m_TradeDate.SetCompareDate(GetData().GetDate());
	m_ExecTime.Setup(this, IDC_ENTRY_EXEC_TIME_EDIT, FALSE,	"Execution time must be before current time");
	m_ValueDate.Setup(this, IDC_ENTRY_VALUEDATE_EDIT, FALSE);
	m_Maturity.Setup(this, IDC_ENTRY_REPO_MATURITY_EDIT, FALSE, 
						"Maturity Date must be greater than or equal to Value Date", &m_ValueDate);
	m_OptExp.Setup(this, IDC_ENTRY_EXPIRATION_EDIT, FALSE, 
					"Option Expiration must be greater than or equal to Trade Date", &m_TradeDate);
	m_SWBooking.Setup(this, IDC_ENTRY_SWBOOKING_EDIT, FALSE, 
						"Swap Booking Date must be greater than or equal to Value Date");
	m_SWMaturity.Setup(this, IDC_ENTRY_SWMATURITY_EDIT, FALSE, 
						"Swap Maturity Date must be greater than or equal to Swap Booking Date", &m_SWBooking);
	m_DeliveryDate.Setup(this, IDC_ENTRY_DELIVERY_EDIT, FALSE, 
							"Delivery Date must be greater than or equal to option expiration date", &m_OptExp);
	m_FloatMaturity.Setup(this, IDC_ENTRY_FLOAT_MATURITY_EDIT, FALSE, 
							"Maturity Date must be greater than or equal to Value Date", &m_ValueDate);
	m_BrokerFee.Setup(this, IDC_ENTRY_BROKER_FEE_EDIT, NULL, 2);
	m_SoftDollar.Setup(this, IDC_ENTRY_SOFTDOLLAR_EDIT, NULL, 2);
	m_OtherFee.Setup(this, IDC_ENTRY_OTHER_FEE_EDIT, NULL, 2);
	m_VAR.Setup(this, IDC_ENTRY_VAR_EDIT, NULL, 2);
	m_PrevVAR.Setup(this, IDC_ENTRY_PREV_VAR_EDIT, NULL, 2);
	m_CurrVAR.Setup(this, IDC_ENTRY_CURR_VAR_EDIT, NULL, 2);
	m_VARLimit.Setup(this, IDC_ENTRY_VAR_LIMIT_EDIT, NULL, 2);

	m_Amount.Setup(this, IDC_ENTRY_AMOUNT_EDIT, NULL, 2);
	m_Margin.Setup(this, IDC_ENTRY_MARGIN_EDIT);
	m_MarginAmount.Setup(this, IDC_ENTRY_MARGIN_AMOUNT_EDIT, NULL, 2);
	m_Strike.Setup(this, IDC_ENTRY_STRIKE_EDIT);
	m_OptTicker.Setup(this, IDC_ENTRY_OPT_TICKER_EDIT);
	m_OptTicker.LimitText(25);
	m_Dir.Setup(this, IDC_ENTRY_TRANSDIR_COMBO, 2);
	m_Trader.Setup(this, IDC_ENTRY_TRADER_COMBO, 2);
	m_AAMethod.Setup(this, IDC_ENTRY_AA_COMBO, 6, TRUE);
	m_ETrade.Setup(this, IDC_ENTRY_ETRADE_COMBO, 2, TRUE);
	m_OptSet.Setup(this, IDC_ENTRY_OPTSET_COMBO, 3, TRUE);
	m_OptSet2.Setup(this, IDC_ENTRY_OPTSET2_COMBO, 3, TRUE);
	m_OptID.Setup(this, IDC_ENTRY_BLMID_EDIT);
	m_OptID.LimitText(25);
	m_FxRate.Setup(this, IDC_ENTRY_FXRATE_EDIT);
	m_IMFxrate.Setup(this, IDC_ENTRY_IMFX_EDIT);
	m_IMFxrate.SetReadOnly();
	m_Rate.Setup(this, IDC_ENTRY_REPORATE_EDIT);
	m_Rate.LimitText(15);
	m_FloatRate.Setup(this, IDC_ENTRY_FLOAT_RATE_EDIT);
	m_Note.Setup(this, IDC_ENTRY_COMMENT_EDIT, EMPTYSTRING, NOCASE);
	m_Note.LimitText(30);
	m_Note2.Setup(this, IDC_ENTRY_COMMENT2_EDIT, EMPTYSTRING, NOCASE);
	m_Note2.LimitText(30);
	m_Asset.Setup(this, IDC_ENTRY_ASSET_EDIT);
	m_AssetDesc.Setup(this, IDC_ENTRY_ASSETDESC_EDIT);
	m_AssetCurrency.Setup(this, IDC_ENTRY_ASSETCURRENCY_EDIT);
	m_VARStatus.Setup(this, IDC_ENTRY_VAR_STATUS_EDIT);
	m_Ratio.Setup(this, IDC_ENTRY_RATIO_EDIT);
	m_Binary.Setup(this, IDC_ENTRY_BINARY_EDIT);
	m_Binary.LimitText(30);

	m_Price.Setup(this, IDC_ENTRY_PRICE_EDIT);
	m_DownPymnt.Setup(this, IDC_ENTRY_DOWNPYMNT_EDIT);
	m_DV01.Setup(this, IDC_ENTRY_DV01_EDIT);
	m_AssignCP.Setup(this, IDC_ENTRY_ASSIGNCP_COMBO, TRUE);

	CRawTicketRec *pTicket;
	pTicket = &m_Data.GetRawTicket();
	m_Data.Add(&m_Ticket, &pTicket->GetTicket());
	m_Data.Add(&m_Trader, &pTicket->GetTrader());
	m_Data.Add(&m_TradeDate, &pTicket->GetTradeDate());
	m_Data.Add(&m_ExecTime, &pTicket->GetExecTime());
	m_Data.Add(&m_TransType, &pTicket->GetTransType());
	m_Data.Add(&m_Dir, &pTicket->GetDir());
	m_Data.Add(&m_ValueDate, &pTicket->GetValueDate());
	m_Data.Add(&m_Amount, &pTicket->GetNomAmount());
	m_Data.Add(&m_Strike, &pTicket->GetStrike());
	m_Data.Add(&m_OptExp, &pTicket->GetOptExp());
	m_Data.Add(&m_Listed, &pTicket->GetListed());
	m_Data.Add(&m_OptTicker, &pTicket->GetOptTicker());
	m_Data.Add(&m_OptSet, &pTicket->GetOptSetCode());
	m_Data.Add(&m_OptSet2, &pTicket->GetOptSetCode2());
	m_Data.Add(&m_OptID, &pTicket->GetOptID());
	m_Data.Add(&m_EuroOpt, &pTicket->GetEuroOpt());
	m_Data.Add(&m_OptAuto, &pTicket->GetOptAuto());
	m_Data.Add(&m_CP, &pTicket->GetCP());
	m_Data.Add(&m_Currency, &pTicket->GetCurrency());
	m_Data.Add(&m_FxRate, &pTicket->GetFxRate());
	m_Data.Add(&m_Asset, &pTicket->GetAsset());
	m_Data.Add(&m_AssetDesc, &pTicket->GetAssetDesc());
	m_Data.Add(&m_AssetCurrency, &pTicket->GetAssetCurr());
	m_Data.Add(&m_IPO, &pTicket->GetIPO());
	m_Data.Add(&m_Trs, &pTicket->GetTRS());
	m_Data.Add(&m_RepoCP, &pTicket->GetRepoCP());
	m_Data.Add(&m_Rate, &pTicket->GetRate());
	m_Data.Add(&m_RateBasis, &pTicket->GetRateBasis());
	m_Data.Add(&m_Maturity, &pTicket->GetMaturity());
	m_Data.Add(&m_Note, &pTicket->GetNote());
	m_Data.Add(&m_Note2, &pTicket->GetNote2());
	m_Data.Add(&m_SWBooking, &pTicket->GetSWBooking());
	m_Data.Add(&m_SWMaturity, &pTicket->GetSWMaturity());
	m_Data.Add(&m_FloatRate, &pTicket->GetSWFloatRate());
	m_Data.Add(&m_FloatRateBasis, &pTicket->GetSWFloatBasis());
	m_Data.Add(&m_FloatMaturity, &pTicket->GetSWFloatMaturity());
	m_Data.Add(&m_WI, &pTicket->GetWI());
	m_Data.Add(&m_SwapTicket, &pTicket->GetSwapTicket());
	m_Data.Add(&m_DeliveryDate, &pTicket->GetDeliveryDate());
	m_Data.Add(&m_Margin, &pTicket->GetMargin());
	m_Data.Add(&m_MarginCurrency, &pTicket->GetMarginCurrency());
	m_Data.Add(&m_MarginAmount, &pTicket->GetMarginAmount());
	m_Data.Add(&m_Funded, &pTicket->GetFunded());
	m_Data.Add(&m_Binary, &pTicket->GetBinary());
	m_Data.Add(&m_AAMethod, &pTicket->GetAAMethod());
	m_Data.Add(&m_AAFReason, &pTicket->GetAAFReason());
	m_Data.Add(&m_BestExecution, &pTicket->GetBestExecution());
	m_Data.Add(&m_ShortSale, &pTicket->GetShortSale());
	m_Data.Add(&m_CSPBShort, &pTicket->GetCSPBShort());
	m_Data.Add(&m_ETrade, &pTicket->GetETrade());
	m_Data.Add(&m_Price, &pTicket->GetPrice());
	m_Data.Add(&m_DownPymnt, &pTicket->GetDownPymnt());
	m_Data.Add(&m_BrokerFee, &pTicket->GetBrokerFee());
	m_Data.Add(&m_OtherFee, &pTicket->GetOtherFee());
	m_Data.Add(&m_SoftDollar, &pTicket->GetSoftDollar());
	m_Data.Add(&m_AssignCP, &pTicket->GetAssignCP());
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
	m_Data.Add(&m_Rev);
	m_Data.Add(&pTicket->GetOrderID());

	m_AllocAmount.Setup(this, IDC_ENTRY_ALLOC_NOMAMOUNT_EDIT);
	m_Remain.Setup(this, IDC_ENTRY_REMAIN_EDIT);

	CRawInvRec *pInv;
	pInv = &m_Data.GetRawInv();
	m_Data.GetSRowCtrl().Add(&m_Portfolio, &pInv->GetPortfolio());
	m_Data.GetSRowCtrl().Add(&m_AllocAmount, &pInv->GetNomAmount());
	m_Data.GetSRowCtrl().Add(&m_Data.GetCustodian());
}

BOOL CTicketEntry::LoadExistingTickets(CString TransType)
{
	CDBRecArray Recs;
	COraLoader OraLoader;
	CQData QData;
	CString sTransType, Trader, Asset, Dir, CP, Nominal, TradeDate, ValueDate, Price, Currency, Fx;
	CString Strike, Rate, FloatRate, Maturity, Sql, Text;
	int i, j;

	OraLoader = GetData().GetOraLoader();
	Trader = m_Trader.GetData();
	Asset = m_Asset.GetData();
	Dir = m_Dir.GetData();
	CP = m_CP.GetData();
	Currency = m_Currency.GetData();
	Nominal = m_Amount.GetData();
	Price = m_Price.GetData();
	Fx = m_FxRate.GetData();
	TradeDate = m_TradeDate.GetData();
	ValueDate = m_ValueDate.GetData();

	sTransType = QData.GetQueryText(TransType);
	Trader = QData.GetQueryText(Trader);
	Asset = QData.GetQueryText(Asset);
	Dir = QData.GetQueryText(Dir);
	CP = QData.GetQueryText(CP);
	Currency = QData.GetQueryText(Currency);
	Nominal = QData.GetQueryNumber(Nominal);
	if(!Price.IsEmpty())
		Price = QData.GetQueryNumber(Price);
	if(!Fx.IsEmpty())
		Fx = QData.GetQueryNumber(Fx);
	TradeDate = QData.GetQueryDate(TradeDate);
	ValueDate = QData.GetQueryDate(ValueDate);

	if(TransType == REPO || TransType == LEVERAGE || TransType == INTSWAP)
	{
		Rate = m_Rate.GetData();
		Maturity = m_Maturity.GetData();
		if(!Rate.IsEmpty())
			Rate = QData.GetQueryNumber(Rate);
		if(!Maturity.IsEmpty())
			Maturity = QData.GetQueryDate(Maturity);
		
		if(TransType == INTSWAP)
			FloatRate = m_FloatRate.GetData();
		if(!FloatRate.IsEmpty())
			FloatRate = QData.GetQueryNumber(FloatRate);
	}
	else
		if(TransType == CALL || TransType == PUT)
		{
			Strike = m_Strike.GetData();
			Maturity = m_OptExp.GetData();
			if(!Strike.IsEmpty())
				Strike = QData.GetQueryNumber(Strike);
			if(!Maturity.IsEmpty())
				Maturity = QData.GetQueryDate(Maturity);
		}

	if(TransType == CDS || TransType == SECURITIES)
		Sql = "SELECT TICKET_NUM, TRADER_INI, TRANS_TYPE, ASSET_CODE, DIR, COUNTERPARTY, NOM_AMOUNT, TRADE_DATE, "
				"VALUE_DATE, PRICE, CURRENCY, FXRATE "
				"FROM SEMAM.NW_RAW_TICKETS A "
				"WHERE TRADER_INI = " + Trader + 
				" AND TRANS_TYPE = " + sTransType + 
				" AND ASSET_CODE = " + Asset + 
				" AND DIR = " + Dir + 
				" AND COUNTERPARTY = " + CP + 
				" AND NOM_AMOUNT = " + Nominal + 
				" AND TRADE_DATE = " + TradeDate + 
				" AND VALUE_DATE = " + ValueDate + 
				" AND PRICE = " + Price + 
				" AND CURRENCY = " + Currency + 
				" AND A.TRANS_TYPE IN ('CDS', 'SECURITIES') ";
	else
		if(TransType == REPO || TransType == LEVERAGE || TransType == INTSWAP)
		{
			Sql = "SELECT TICKET_NUM, TRADER_INI, TRANS_TYPE, ASSET_CODE, DIR, COUNTERPARTY, NOM_AMOUNT, "
					"TRADE_DATE, VALUE_DATE, TO_NUMBER(NULL) \"PRICE\", CURRENCY, FXRATE "
					"FROM SEMAM.NW_RAW_TICKETS A "
					"WHERE TRADER_INI = " + Trader + 
					" AND TRANS_TYPE = " + sTransType + 
					" AND ASSET_CODE = " + Asset + 
					" AND DIR = " + Dir + 
					" AND COUNTERPARTY = " + CP + 
					" AND NOM_AMOUNT = " + Nominal + 
					" AND TRADE_DATE = " + TradeDate + 
					" AND VALUE_DATE = " + ValueDate + 
					" AND CURRENCY = " + Currency + 
					" AND A.TRANS_TYPE IN ('REPO', 'LEVERAGE', 'INT. SWAP') ";
			if(!Sql.IsEmpty() && !Price.IsEmpty())
				Sql += " AND PRICE = " + Price;
			if(!Sql.IsEmpty() && !Rate.IsEmpty())
				Sql += " AND TR_RATE = " + Rate;
			if(!Sql.IsEmpty() && !Maturity.IsEmpty())
				Sql	+= " AND MATURITY = " + Maturity;
		}
		else
			if(TransType == FOREX)
			{
				Sql = "SELECT TICKET_NUM, TRADER_INI, TRANS_TYPE, ASSET_CODE, DIR, COUNTERPARTY, NOM_AMOUNT, "
						"TRADE_DATE, VALUE_DATE, PRICE, CURRENCY, FXRATE "
						"FROM SEMAM.NW_RAW_TICKETS A "
						"WHERE TRADER_INI = " + Trader + 
						" AND TRANS_TYPE = " + sTransType + 
						" AND DIR = " + Dir + 
						" AND COUNTERPARTY = " + CP + 
						" AND NOM_AMOUNT = " + Nominal + 
						" AND TRADE_DATE = " + TradeDate + 
						" AND VALUE_DATE = " + ValueDate + 
						" AND CURRENCY = " + Currency + 
						" AND FXRATE = " + Fx + 
						" AND A.TRANS_TYPE IN ('FOREX') ";
				if(!Sql.IsEmpty() && !Asset.IsEmpty())
					Sql += " AND ASSET_CODE = " + Asset;
			}
			else
				if(TransType == CALL || TransType == PUT)
				{
					Sql = "SELECT TICKET_NUM, TRADER_INI, TRANS_TYPE, ASSET_CODE, DIR, COUNTERPARTY, NOM_AMOUNT, "
							"TRADE_DATE, VALUE_DATE, PRICE, CURRENCY, FXRATE "
							"FROM SEMAM.NW_RAW_TICKETS A "
							"WHERE TRADER_INI = " + Trader + 
							" AND TRANS_TYPE = " + sTransType + 
							" AND ASSET_CODE = " + Asset + 
							" AND DIR = " + Dir + 
							" AND COUNTERPARTY = " + CP + 
							" AND NOM_AMOUNT = " + Nominal + 
							" AND TRADE_DATE = " + TradeDate + 
							" AND VALUE_DATE = " + ValueDate + 
							" AND CURRENCY = " + Currency + 
							" AND PRICE = " + Price + 
							" AND A.TRANS_TYPE IN ('CALL', 'PUT') ";

					if(!Sql.IsEmpty() && !Strike.IsEmpty())
						Sql += " AND STRIKE = " + Strike;
					if(!Sql.IsEmpty() && !Maturity.IsEmpty())
						Sql += " AND OPT_EXPIRATION = " + Maturity;
				}
		
	if(Sql.IsEmpty())
		return TRUE;
	
	Sql += " ORDER BY 1, 2, 3, 4, 5, 6 ";
	OraLoader.Open(Sql);
	OraLoader.LoadDBRecArray(Recs);

	Sql.Empty();
	for(i = 0; i < Recs.GetCount(); i++)
	{
		if(i == 0)
			Sql = "TICKET_NUM\tTRADER_INI\tTRANS_TYPE\tASSET_CODE\tDIR\tCOUNTERPARTY\t"
					"NOM_AMOUNT\tTRADE_DATE\tVALUE_DATE\tPRICE\tCURRENCY\tFXRATE";

		Text.Empty();
		for(j = 0; j < Recs.GetAt(i)->GetSize(); j++)
		{
			if(!Text.IsEmpty())
				Text += TAB;
			Text += Recs.GetAt(i)->GetAt(j);
		}
		if(!Text.IsEmpty())
		{
			Sql += "\n";
			Sql += Text;
		}	
	}

	if(!Sql.IsEmpty())
		if(MessageBox(Sql, "The Tickets may have been entered earlier, Continue ?", MB_YESNO) == IDNO)
			return FALSE;
	return TRUE;
}

BOOL CTicketEntry::IsOK(BOOL bCopy)
{
	double dNom;
	CString Fund, TransType, Text, Data, Amount, Currency, Fxrate;
	CQData QData;
	int i;
	COraLoader OraLoader;

	OraLoader.SetDB(&theDB);
	TransType = m_TransType.GetData();

	if(m_Data.GetSRowCtrl().GetSS()->GetSheetRows() <= 0)
		Text = "Invalid Ticket, Must enter Allocation";

	if(m_Data.AllocCheckSum())
		Text = "Sum of the shares in each portfolios must be equal to the Nominal Amount";

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

	OraLoader.Today(Data);
	if(m_TradeDate.GetData().IsEmpty() || m_TradeDate.GetData() != Data)
		Text = "Invalid Trade Date.  Must enter Correct Trade Date.";

	if(m_ExecTime.GetData().IsEmpty())
		Text = "Invalid Exection Time.  Must enter Exection Time.";

	Data = m_Dir.GetData();
	if(Data.IsEmpty())
		Text = "Invalid Transaction Direction.  Must enter Transaction direction.";

	Amount = m_Amount.GetData();
	dNom = atof(QData.RemoveComma(Amount));

	if(dNom <= 0)
		Text = "Invalid Nominal Amount.  Must enter Nominal Amount.";
	
	if(fabs(dNom - m_Data.ComputeAllocAmount()) > 0.00001)
		Text = "Nominal Amount and Sum of Allocated Nominal Amounts does not match";
	
	if(TransType == CALL || TransType == PUT)
	{
		if(m_dParValue > 0 && dNom > 0 && fmod(dNom, m_dParValue) > 0)
			Text = "Invalid number of contracts (Fraction of a contract) ";
	}

	Data = m_CP.GetData();
	if(Data.IsEmpty())
		Text = "Invalid Counterparty.  Must enter a couterparty.";

	if(m_Data.GetRawTicket().GetDefTrader().IsEmpty() && m_Data.GetRawTicket().GetTrader().IsEmpty())
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
					if(m_OptSet.GetCurSel() <= 0)
						Text = "You must select a option settlement code";
	}

	if(TransType == FOREX)
	{
		if(m_FxRate.GetWindowTextLength() <= 0)
			Text = "You must enter Fxrate";
	}
	else
		if(m_Price.GetWindowTextLength() <= 0)
			Text = "You must enter price";

	if(TransType != FOREX && m_Margin.GetWindowTextLength() <= 0 && m_MarginAmount.GetWindowTextLength() <= 0)
		Text = "You must enter Margin percentage or Margin Amount";

	if(m_BestExecution.GetCurSel() < 0)
		Text = "You must choose Best Execution Method";

	if(m_AAMethod.GetCurSel() < 0)
	{
		if(m_AAFReason.GetCurSel() < 0)
			Text = "Must choose a reason for not Auto allocation";
	}
	else
		for(i = 1; i <= m_AllocSS.GetSheetRows() && m_AllocArray.GetSize(); i ++)
		{
			if(m_AllocArray.GetAt(i - 1)->GetAt(1) != m_AllocSS.GetSheetText(2, i) && m_AAFReason.GetCurSel() < 0)
			{
				Text = "Auto Allocation Method is violated, You must select AAF Reason";
				break;
			}
		}

	if(!bCopy)
		if(!LoadExistingTickets(TransType))
			Text = "Dulicated Ticket(s)";

	if(!Text.IsEmpty())
	{
		MessageBox(Text, "Ticket Entry");
		return FALSE;
	}

	// Risk Control
	Fund.Empty();

	for(i = 1; i <= m_AllocSS.GetSheetRows(); i ++)
	{
		if(m_AllocSS.GetSheetText(1, i) == "BNAMTR")
			Fund = "BNAMTR";
	}

	if(!Fund)
		return TRUE;

	switch(VerifyRisk(Text))
	{
		case 1:
			MessageBox(Text, "Ticket Entry");
			return TRUE;
		case 3:
			MessageBox(Text, "Ticket Entry");
			return FALSE;
		default:
			return TRUE;
	}
}

void CTicketEntry::ProcessVerifyRisk()
{
	CString Text;

	m_nRiskLevel = VerifyRisk(Text);
	if(!Text.IsEmpty() && !m_bWarned)
	{
		MessageBox(Text, "Ticket Entry");
		m_bWarned = TRUE;
	}
}

void CTicketEntry::UpdateAlloc(int Action)
{
	CString Port, Text;
	CQData QData;
	double NomAmount, AllocAmount;

	m_dNomAmount = atof(QData.RemoveComma(m_Amount.GetData()));

	switch(Action)
	{
		case 0: // Update After Load
			NomAmount = m_Data.ComputeAllocAmount();
//			m_SoftDAmount.SetData(QData.WriteNumber(m_Data.GetSoftDollar()));
//			m_OtherFee.SetData(QData.WriteNumber(m_Data.GetOtherFee()));
//			m_VAR.SetData(QData.WriteNumber(m_Data.GetVAR()));
			break;
		case 1: // Add New 
		case 3: // Delete
			NomAmount = m_Data.ComputeAllocAmount();
			break;
		case 2: // Update
			Port = m_Portfolio.GetData();
			Text = m_AllocAmount.GetData();
			AllocAmount = atof(QData.RemoveComma(Text));
			if(Port.IsEmpty())
				MessageBox("Cannot get portfolio");
//			NomAmount = m_Data.ComputeAllocAmount(Port, AllocAmount, BrokerFee, SoftDollar, OtherFee, VAR);
			NomAmount = m_Data.ComputeAllocAmount(Port, AllocAmount);
			break;
		default:
			break;
	}

	UpdateNumber(GetDlgItem(IDC_ENTRY_TOTAL_ALLOC_EDIT), NomAmount);
	UpdateNumber(&m_Remain, m_dNomAmount - NomAmount);
}

BOOL CTicketEntry::UpdateData(BOOL bSaveandValid)
{
	if(bSaveandValid)
	{
		m_Try = 0;
		m_SS.SetFocus();
		m_Data.UpdateRow();
		m_Risk.UpdateRisk(m_Ticket.GetData());
	}
	else
	{
		BeginWaitCursor();
		COraLoader OraLoader;

		OraLoader = m_Data.GetOraLoader();
		m_Data.LoadTickets();
		m_Data.LoadAllocList();

		if(!m_bLoaded)
		{
			GetData().LoadSupportData();
			GetData().GetPortfolioArr().CopyToComboBox(m_Portfolio); /* Portfolio */
			GetData().GetCurrencyArr().CopyToComboBox(m_Currency);	/* Currency */
			GetData().GetCurrencyArr().CopyToComboBox(m_MarginCurrency); /* Margin Currency */
			GetData().GetRateBasisArr().CopyToComboBox(m_RateBasis); /* RateBasis */
			GetData().GetRateBasisArr().CopyToComboBox(m_FloatRateBasis); /* FloatRateBasis */
			GetData().GetContactList().CopyKeyToComboBox(m_CP); /* Counterparty */
			GetData().GetContactList().CopyKeyToComboBox(m_RepoCP); /* Repo Counterparty */
			GetData().GetContactList().CopyKeyToComboBox(m_AssignCP); /* Giveup Counterparty */
			GetData().GetTransTypeArr().CopyToComboBox(m_TransType); /* Trans Type */
			GetData().GetBestExecutionArr().CopyToComboBox(m_BestExecution);
			
			GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);	/* TransDir */
			GetData().GetTraderArr().CopyToMCComboBox(m_Trader); /* Trader */
			GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet);
			GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet2);
			GetData().GetETradeTypeArr().CopyToMCComboBox(m_ETrade);
			OraLoader.Open("SELECT AA_METHOD, AA_DESC, RATIO_A, RATIO_B, RATIO_C, RATIO_D FROM SEMAM.NW_AA_METHOD_V ORDER BY 1");
			m_AAMethod.ResetContent();
			OraLoader.LoadMCComboBox(m_AAMethod);

			OraLoader.Open("SELECT AA_REASON FROM SEMAM.NW_AA_REASON ORDER BY 1");
			m_AAFReason.ResetContent();
			OraLoader.LoadComboBox(m_AAFReason);
			m_bLoaded = TRUE;
		}
		EndWaitCursor();
	}

	UpdateNumber(GetDlgItem(IDC_ENTRY_TOTAL_EDIT), m_SS.GetSheetRows() >= 0 ? m_SS.GetSheetRows() : 0, TRUE);
	m_SS.SetSheetCurRow(-1);
	EnableAllocCtrls();

	Invalidate();
	UpdateWindow();
	OnEntryRefresh();
	return TRUE;
}

void CTicketEntry::UpdateNumber(CWnd* Ctrl, double Number, BOOL WholeNumber)
{
	CQData QData;
	
	Ctrl->SetWindowText(QData.WriteNumber(Number, TRUE, WholeNumber ? 0 : -1));
}

void CTicketEntry::UpdateLink()
{
	CString TransType, Asset;

	TransType = m_TransType.GetData();
	if((TransType == CDS || TransType == SECURITIES || TransType == CALL || TransType == PUT) && 
		!m_UnWindTicket.GetData().IsEmpty())
	{	
		COraLoader OraLoader;
		CQData QData;
		CString Ticket;

		Asset = m_Asset.GetData();
		Ticket = m_UnWindTicket.GetData();
		OraLoader = GetData().GetOraLoader();

		OraLoader.GetSql().Format("SELECT PORTFOLIO, TRANS_NUM "
							"FROM SEMAM.NW_POSITION_V WHERE ASSET_CODE = %s AND TICKET_NUM = '%s' ", 
							QData.GetQueryText(Asset), (LPCTSTR) Ticket);
		OraLoader.Open();		
	}
}

void CTicketEntry::UpdateRatio()
{
	CString TransType, Asset, Ticket;
	COraLoader OraLoader;
	CQData QData;

	OraLoader = GetData().GetOraLoader();
	m_Ratio.SetData(":");
	TransType = m_TransType.GetData();
	Asset = m_Asset.GetData();
	Ticket = m_UnWindTicket.GetData();
	if(Ticket.IsEmpty())
	{
		if(TransType == CDS || TransType == SECURITIES)
		{
			OraLoader.GetSql().Format("SELECT TO_CHAR(TRUNC(EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(PERFISO_WT, 0)) "
									"FROM SEMAM.NW_EMCFPERFISO_ALLOC_V WHERE ASSET_CODE = %s", 
									QData.GetQueryText(Asset));
		}
	}
	else
	{
		Asset = Asset + "|" + Ticket;
		OraLoader.GetSql().Format("SELECT TO_CHAR(TRUNC(EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(PERFISO_WT, 0)) "
					"FROM SEMAM.NW_EMCFPERFISO_ALLOC_V WHERE ASSET_CODE = %s", QData.GetQueryText(Asset));
	}

	if(!OraLoader.GetSql().IsEmpty())
	{
		OraLoader.Open();
		OraLoader.LoadText(m_Ratio);
	}
}

void CTicketEntry::UpdateTradeDates(BOOL bEuroBond) 
{
	CString Date;

	m_TradeDate.SetData(GetData().GetOraLoader().Today(Date));

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, m_TransType.GetData(), m_Asset.GetData(), m_Listed.GetCheck()));
}

int CTicketEntry::VerifyRisk(CString &Text)
{
	CString Method, Asset, TransType, Dir, Currency, ValueDate, Nominal, Price, Fxrate;

	Method = m_AAMethod.GetData();

	if(Method != "BNAMTR"||m_Trs.GetCheck())
		return 0;

	Asset = m_Asset.GetData();
	if(Asset != m_Risk.GetAsset())
	{
		if(m_Ticket.GetData().IsEmpty())
			m_Risk.LoadRisk(Asset, m_Data.GetRawTicket().GetOrderID(), FALSE);
		else
			m_Risk.LoadRisk(Asset, m_Ticket.GetData());
	}

	TransType = m_TransType.GetData();
	Dir = m_Dir.GetData();
	Currency = m_Currency.GetData();
	ValueDate = m_ValueDate.GetData();
	Nominal = m_Amount.GetData();
	Price = m_Price.GetData();
	Fxrate = m_FxRate.GetData();
		
	if(Dir == S)
		Nominal = "-" + Nominal;

	m_bWarned = FALSE;

	return m_Risk.VerifyRisk(Text, Method, TransType, ValueDate, Currency, Nominal, Price, Fxrate);
}

/////////////////////////////////////////////////////////////////////////////
// CTicketEntry message handlers
void CTicketEntry::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BeginWaitCursor();

	InitControls();
	if(GetData().IsTrader())
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_AllocSS, GetData().GetTraderIni(), GetData().GetOraUser(), 0);
	else
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_AllocSS, EMPTYSTRING, GetData().GetOraUser(), 0);
	
	m_Risk.Setup(GetData().GetOraLoader());
	GetData().LoadDates();
	if(GetData().GetBlock())
		GetDocument()->SetViewTitle("TICKET BLOCK ALLOC");
	else
		GetDocument()->SetViewTitle("TICKET ENTRY");
	
	m_Data.SetBlock(GetData().GetBlock());
	m_Try = 0;

	UpdateData(FALSE);
	EndWaitCursor();
	
	OnEntryLoadTrade();
	EnableAlloc();
}

BEGIN_EVENTSINK_MAP(CTicketEntry, CFormView)
    //{{AFX_EVENTSINK_MAP(CTicketEntry)
	ON_EVENT(CTicketEntry, IDC_ENTRY_TICKET_LIST, 2 /* BlockSelected */, OnBlockSelectedTicketList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTicketEntry, IDC_ENTRY_TICKET_LIST, 11 /* DblClick */, OnDblClickTicketList, VTS_I4 VTS_I4)
	ON_EVENT(CTicketEntry, IDC_ENTRY_ALLOCATION_LIST, 11 /* DblClick */, OnDblClickEntryAllocationList, VTS_I4 VTS_I4)
	ON_EVENT(CTicketEntry, IDC_ENTRY_ALLOCATION_LIST, 2 /* BlockSelected */, OnBlockSelectedEntryAllocationList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTicketEntry::OnSelchangeTranstypeCombo() 
{
	OnSelchangeTransdirCombo();
	OnListedCheck();
	m_DownPymnt.SetData("100");
	
	CString TransType;
	
	TransType = m_TransType.GetData();

	if(TransType != CALL && TransType != PUT)
		m_OptTicker.SetData(EMPTYSTRING);

	UpdateTradeDates();

	ProcessVerifyRisk();
}

void CTicketEntry::OnSelchangeTransdirCombo() 
{
	CString TransType, Dir;
	
	TransType = m_TransType.GetData();
	Dir = m_Dir.GetData();
	GetData().DefineTransType(TransType, Dir);
	if(TransType.IsEmpty())
		return;

	if(TransType == REPO || TransType == LEVERAGE || TransType == BORROW || TransType == LEND)
	{
		if(m_RateBasis.GetCurSel() < 0)		
			m_RateBasis.SelectString(-1, "A/360");
	}

	if(Dir == P)
		m_ShortSale.SetWindowText("CShort");
	else
		if(Dir == S)
			m_ShortSale.SetWindowText("SShort");

	EnableCtrls();
	
	ProcessVerifyRisk();
}

void CTicketEntry::OnSelchangeCurrencyCombo() 
{	
	CString Text;
	
	Text = m_Currency.GetData();
	if(Text.IsEmpty())
		return;
	
	if(Text == "USD")
	{
		m_FxRate.SetData("1");
		return;
	}
	
	CString Fxrate;

	Fxrate = m_FxRate.GetData();
	if(Fxrate == "1")
		m_FxRate.SetData(EMPTYSTRING);

	ProcessVerifyRisk();
}

void CTicketEntry::OnEntryFindAsset() 
{	
	CAssetDlg Dlg;
	CString TransType, Dir, Text;
	BOOL bChange = FALSE, bValueDateChangable = TRUE;
	CQData QData;

	TransType = m_TransType.GetData();
	
	m_Try = 1;

	Dlg.m_pData = &GetData();
	Dlg.m_FindData.SetTicketBooking(TRUE);
	Dlg.m_FindData.GetRec().GetAsset() = m_Asset.GetData();
	Dlg.m_FindData.GetRec().GetDesc() = m_AssetDesc.GetData();
	Dlg.m_FindData.GetRec().GetOptTicker() =  m_OptTicker.GetData();
	Dlg.m_bTicket = TRUE;

	if(Dlg.DoModal() == IDOK)
	{
		CRawTicketRec *pTicket;

		pTicket = &m_Data.GetRawTicket();
		pTicket->FromAssetRec(Dlg.m_FindData.GetRec());
		m_Asset.SetData(pTicket->GetAsset());
		m_AssetDesc.SetData(pTicket->GetAssetDesc());
		m_AssetCurrency.SetData(pTicket->GetAssetCurr());
		m_Rev = Dlg.m_Rev;
		m_Trader.SetData(Dlg.m_FindData.GetTrader());
		m_Data.GetCustodian() = Dlg.m_FindData.GetCustodian();
		if(!Dlg.m_FindData.GetTransType().IsEmpty())
			TransType = Dlg.m_FindData.GetTransType();

		m_Future = Dlg.m_Future;
		CString Currency, Fxrate;

		Currency = m_Currency.GetData();

		if(Currency.IsEmpty() || 
			(Currency != pTicket->GetAssetCurr() &&
			MessageBox("Note: Trading Currency is Different from Asset Currency! Do you want to synchonize them ?", 
						"Warning", MB_YESNO) == IDYES))
		{
			m_Currency.SetData(pTicket->GetAssetCurr());
		}

		Currency = m_Currency.GetData();
		if(m_FxRate.GetWindowTextLength() == 0)
			m_FxRate.SetData(Currency == USD ? "1" : EMPTYSTRING);
		
		m_MarginCurrency.SetData(Currency);
		Fxrate = m_FxRate.GetData();
		if(Currency != USD && Fxrate == "1")
			m_FxRate.SetData(EMPTYSTRING);

		if(TransType == CALL || TransType == PUT)
		{
			m_EuroOpt.SetCheck(pTicket->GetAssetClass().Find("CURRENCY", 0) >= 0 ? 1 : 0);
		}
		
		if(Dlg.m_nActionID == FINDACTION_UNWIND || Dlg.m_nActionID == FINDACTION_OPTEX)
		{
			Text = m_Amount.GetData();
			if(Dlg.m_FindData.GetNominal() != Text && m_Amount.GetWindowTextLength() > 0)
			{
				if(MessageBox("Do you want to change current nom_amount (Y/N)?", "Ticket Entry", MB_YESNO) == IDYES)
					bChange = TRUE;
			}
		}
		
		m_dParValue = atoi(QData.RemoveComma(Dlg.m_FindData.GetRec().GetPar()));

		if(!Dlg.m_FindData.GetTransType().IsEmpty())
			m_TransType.SetData(Dlg.m_FindData.GetTransType());

		Dir = Dlg.m_FindData.GetDir();
		if(TransType == SECURITIES && Dlg.m_FindData.GetRec().GetClass() == "CURRENCY FWDS")
			bValueDateChangable = FALSE;

		switch(Dlg.m_nActionID)
		{
			case FINDACTION_ADD:
				if(TransType == CDS || TransType == SECURITIES || TransType == CALL || TransType == PUT)
				{
					m_Dir.SelectString(0, Dir);
					m_TransType.SetData(TransType);
					m_AssignCP.SetData(Dlg.m_FindData.GetAssignCP());
					
					if(TransType == CALL ||	TransType == PUT)
					{
						m_Strike.SetData(Dlg.m_FindData.GetStrike());
						m_OptExp.SetData(Dlg.m_FindData.GetRec().GetMaturity());
						m_OptTicker.SetData(Dlg.m_FindData.GetOptTick());
						m_OptID.SetData(Dlg.m_FindData.GetOptID());
						m_OptSet.SetData(Dlg.m_FindData.GetOptSetCode());
						m_DeliveryDate.SetData(Dlg.m_FindData.GetDeliveryDate());
						m_Listed.SetCheck(Dlg.m_OptListed);
					}
				}

				break;
			case FINDACTION_UNWIND:
				if(TransType == CDS || TransType == SECURITIES || TransType == CALL || TransType == PUT)
				{
					m_Dir.SelectString(0, Dir == P ? S : P);
					if(bChange)
						m_Amount.SetData(Dlg.m_FindData.GetNominal());
					m_TransType.SetData(TransType);
					m_AssignCP.SetData(Dlg.m_FindData.GetAssignCP());
				
					if(TransType == CALL || TransType == PUT)
					{
						m_Strike.SetData(Dlg.m_FindData.GetStrike());
						m_OptExp.SetData(Dlg.m_FindData.GetRec().GetMaturity());
						m_OptTicker.SetData(Dlg.m_FindData.GetOptTick());
						m_OptID.SetData(Dlg.m_FindData.GetOptID());
						m_DeliveryDate.SetData(Dlg.m_FindData.GetDeliveryDate());
						m_OptSet.SetData(Dlg.m_FindData.GetOptSetCode());
						m_UnWindTicket.SetData(Dlg.m_FindData.GetTicket());
						m_Listed.SetCheck(Dlg.m_OptListed);
					}
				}
				break;
			case FINDACTION_OPTEX:
				if(TransType == CALL || TransType == PUT)
				{
					if(Dir == P) // We bought options
					{
						if(TransType == CALL)
							m_Dir.SetData(P);
						else
							if(TransType == PUT)
								m_Dir.SetData(S);
					}
					else
					{
						if(TransType == CALL)
							m_Dir.SetData(S);
						else
							if(TransType == PUT)
								m_Dir.SetData(P);
					}
					
					if(bChange)
						m_Amount.SetData(Dlg.m_FindData.GetNominal());
					m_TransType.SetData(SECURITIES);
					m_Price.SetData(Dlg.m_FindData.GetStrike());
					m_UnWindTicket.SetData(Dlg.m_FindData.GetTicket());
					m_AssignCP.SetData(Dlg.m_FindData.GetAssignCP());
				}
				break;
			default:
				break;
		}
	}

	if(bValueDateChangable)
		UpdateTradeDates(Dlg.m_EuropeBond == "Y" ? TRUE : FALSE);
	else
		m_ValueDate.SetData(Dlg.m_FindData.GetRec().GetMaturity());

	Text = m_Asset.GetData();

	if(TransType == FOREX && Text == "NEW ASSET")
	{
		m_Asset.SetData(EMPTYSTRING);
		m_AssetDesc.SetData(EMPTYSTRING);
		m_AssetCurrency.SetData(NULL);
	}

	EnableCtrls();

	if(!m_Price.GetData().IsEmpty())
		OnEnChangeEntryPriceEdit();

	UpdateRatio();
	m_Note.SetFocus();
}

void CTicketEntry::OnBlockSelectedTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickTicketList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CTicketEntry::OnDblClickTicketList(long Col, long Row) 
{
	CString Method, Asset;

	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row, m_Data.GetAllocList());
		if(m_AAMethod.GetCurSel() >= 0 && !m_Data.GetRawTicket().GetOrderID().IsEmpty())
			OnCbnSelchangeEntryAaCombo();

		if(m_ValueDate.GetData().IsEmpty())
			m_ValueDate.SetData(GetData().GetPlus3Date());

		m_AAMethod.GetSelString(m_sAAMethod);
		UpdateAlloc(0);
		EnableCtrls();
	}
	else
	{
		OnEntryRefresh();
		m_dNomAmount = 0;
		m_sAAMethod.Empty();
	}

	OnEnChangeEntryPriceEdit();
}

void CTicketEntry::OnEntryAdd() 
{	
   	if(!IsOK(FALSE))
		return;

	CString Ticket;
	int Row = 0;

	if(!m_Data.GetRawTicket().GetOrderID().IsEmpty())
		Row = m_Data.GetCurrentRow();

	m_SS.SetFocus();
	GetData().GetOraLoader().GenerateUniqueID(TICKETNUM, Ticket);
	m_Ticket.SetData(Ticket);
	m_Data.SetKey(EMPTYSTRING);

	UpdateData(TRUE);
	OnEntryLoadTrade();
	
}

void CTicketEntry::OnEntryDelete() 
{
	if(MessageBox("Are you sure you want to delete these tickets ?",
	  			  "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	m_Try = 0;
	m_Data.DeleteRow();
	m_Risk.DeleteRisk(m_Ticket.GetData());
}

void CTicketEntry::OnEntryUpdate() 
{
	if(!IsOK(FALSE))
		return;

	m_SS.SetFocus();
	UpdateData(TRUE);
}

void CTicketEntry::OnEntryRefresh() 
{
	BeginWaitCursor();
	
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	m_TradeDate.SetData(GetData().GetDate());
	m_ValueDate.SetData(GetData().GetPlus3Date());
	m_TransType.SetData(SECURITIES);
	m_FxRate.SetData("1");
	m_Currency.SetCurSel(m_Currency.FindString(-1, USD));

	m_SS.SheetAutoFit();

	SetDlgItemText(IDC_ENTRY_TOTAL_ALLOC_EDIT, EMPTYSTRING);
	
	m_nCurrID = 0;
	EnableCtrls();
	EndWaitCursor();
}

void CTicketEntry::OnUpdateEntryAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter() && m_Ticket.GetData().IsEmpty());
//	pCmdUI->Enable(!m_Data.GetBlotter() && m_SS.GetSheetCurRow() <= 0 && 
//					m_AllocSS.GetSheetRows() > 0 ? TRUE : FALSE);	
}

void CTicketEntry::OnUpdateEntryDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Ticket.GetData().IsEmpty() && !m_Data.GetBlotter() && 
					m_SS.GetIsBlockSelected() && m_SS.GetSheetCurRow() >= 1);
}

void CTicketEntry::OnUpdateEntryFindAsset(CCmdUI* pCmdUI) 
{
	CString TransType;

	TransType = m_TransType.GetData();
	pCmdUI->Enable(m_Data.GetBlotter() || TransType == CASH || TransType == INTSWAP ? FALSE : TRUE);
}

void CTicketEntry::OnUpdateEntryUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Ticket.GetData().IsEmpty() && !m_Data.GetBlotter() && m_SS.GetSheetCurRow() >= 1 ? TRUE : FALSE);
}

void CTicketEntry::OnUpdateEntryCopyTicket(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Ticket.GetData().IsEmpty() && m_SS.GetSheetCurRow() >= 1 ? TRUE : FALSE);
}

void CTicketEntry::OnListedCheck() 
{
	CString Date, TransType;

	Date = m_TradeDate.GetData();
	if(Date != GetData().GetDate()) // Automatic change dates only when entry date is today
		return;

	TransType = m_TransType.GetData();

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, TransType, m_Asset.GetData(), m_Listed.GetCheck()));
}

void CTicketEntry::OnSetfocusAssetEdit() 
{
	CString TransType;

	TransType = m_TransType.GetData();
	if((TransType == FOREX && m_Try) || m_Asset.GetWindowTextLength() > 0)
		return;

	m_Asset.SetData(NEWASSET);	
	OnEntryFindAsset();	
}

void CTicketEntry::OnSelchangeLevsourceCombo() 
{
	EnableRepoCtrls(m_RepoCP.GetCurSel() >= 0 ? TRUE : FALSE);
}

void CTicketEntry::OnEntryCopyTicket() 
{
   	if(m_Ticket.GetData().IsEmpty())
		return;

	CString Ticket;

	m_SS.SetFocus();
	GetData().GetOraLoader().GenerateUniqueID(TICKETNUM, Ticket);
	m_Ticket.SetData(Ticket);
	m_ExecTime.SetData(EMPTYSTRING);
	m_Price.SetData(EMPTYSTRING);
	m_Amount.SetData("0");
	m_BestExecution.SetCurSel(-1);
	m_UnWindTicket.SetData(EMPTYSTRING);
	m_Data.SetKey(EMPTYSTRING);

	UpdateData(TRUE);
}

void CTicketEntry::OnUpdateEntryAddAllocation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocAmount.GetWindowTextLength() > 0 ? TRUE : FALSE);
}

void CTicketEntry::OnUpdateEntryBrokerfee(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter());
}

void CTicketEntry::OnEntryAddAllocation() 
{
	if(m_bAllocValid && !AllocIsOK())
		return;

	m_Data.GetSRowCtrl().AddSheetRow();
	UpdateAlloc(1);
}

void CTicketEntry::OnEntryBrokerfee() 
{
	CBrokerFee BrFee;

	BrFee.m_sAmount = m_Amount.GetData();
	if(BrFee.DoModal() == IDOK)
		m_BrokerFee.SetData(BrFee.m_Total);
}

void CTicketEntry::OnEntryUpdateAllocation() 
{
	if(!AllocIsOK())
		return;

	m_Data.GetSRowCtrl().UpdateSheetRow(FALSE);
	UpdateAlloc(2);
}

void CTicketEntry::OnUpdateEntryUpdateAllocation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocSS.GetSheetCurRow() >= 1);
}

void CTicketEntry::OnEntryDeleteAllocation() 
{
	if(MessageBox("Are you sure you want to delete these allocations ?", "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	
	m_Data.GetSRowCtrl().DeleteSheetRow(m_Data.GetSRowCtrl().GetCurrentRow());

	UpdateAlloc(3);
}

void CTicketEntry::OnUpdateEntryDeleteAllocation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocSS.GetIsBlockSelected() && m_AllocSS.GetSheetCurRow() >= 1);
}

void CTicketEntry::OnDblClickEntryAllocationList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_AllocSS.GetSheetRows())
	{
		m_Data.GetSRowCtrl().SetCurrentRow(Row);
		
		CString Portfolio, Text, AllocNom;
		CStringArray Rec;
		double VAR;
		CQData QData;

		Portfolio = m_Portfolio.GetData();
		Text = m_VAR.GetData();
		AllocNom = m_AllocAmount.GetData();
		VAR = atof(QData.RemoveComma(m_Data.GetRawTicket().Alloc(Text, Text, AllocNom)));
		if(m_Data.LoadVARs(Rec, Portfolio, m_Data.GetRawTicket().GetAssetCategory(), VAR) > 0)
		{
			m_PrevVAR.SetData(Rec.GetAt(0));
			m_CurrVAR.SetData(Rec.GetAt(1));
			m_VARLimit.SetData(Rec.GetAt(2));
			m_VARStatus.SetData(Rec.GetAt(3));
		}
		else
		{
			m_PrevVAR.SetData(EMPTYSTRING);
			m_CurrVAR.SetData(EMPTYSTRING);
			m_VARLimit.SetData(EMPTYSTRING);
			m_VARStatus.SetData(EMPTYSTRING);
		}
	}
	EnableAllocCtrls();
}

void CTicketEntry::OnBlockSelectedEntryAllocationList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickEntryAllocationList(m_AllocSS.GetActiveCol(), m_AllocSS.GetActiveRow());
}

void CTicketEntry::OnKillfocusAllocNomamountEdit() 
{
	CString Port, Text;
	CQData QData;

	Port = m_Portfolio.GetData();
	Text = m_AllocAmount.GetData();
	if(Port.IsEmpty() || Text.IsEmpty())
		return;

	if(m_Data.AllocCheckSum(Port, atof(QData.RemoveComma(Text))) > 0)
	{
		MessageBox("Sum of the shares in each portfolios exceeds the Nominal Amount");
		m_AllocAmount.SetFocus();
	}
}

void CTicketEntry::OnKillfocusDownpymntEdit() 
{
	CString Text;
	CQData QData;

	Text = m_DownPymnt.GetData();
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

void CTicketEntry::OnSelchangeEntryPortfolioCombo() 
{
	CString Port;
	
	Port = m_Portfolio.GetData();
	if(Port.IsEmpty())
		return;

	if(m_Data.FindPortfolio(Port))
	{
		MessageBox("Duplicated portfolio selection");
		m_Portfolio.SetCurSel(-1);
	}

	EnableAllocCtrls();
}

void CTicketEntry::OnEntryLoadBlotter() 
{
	m_Data.SetBlotter(TRUE);
	UpdateData(FALSE);
}

void CTicketEntry::OnEntryLoadTrade() 
{
	m_Data.SetBlotter(FALSE);
	UpdateData(FALSE);
}

void CTicketEntry::OnEntryOptionExp()
{
	COptExpDlg Dlg;

	Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
	if(Dlg.DoModal() == IDOK)
		m_OptExp.SetData(Dlg.m_Date);
}

void CTicketEntry::OnUpdateEntryOptionExp(CCmdUI* pCmdUI)
{
	CString TransType;

	TransType = m_TransType.GetData();
	pCmdUI->Enable(TransType == PUT || TransType == CALL ? TRUE : FALSE);
}

void CTicketEntry::OnUpdateEntryContractConvertor(CCmdUI* pCmdUI)
{
	CString TransType;

	TransType = m_TransType.GetData();
	pCmdUI->Enable(TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F") ? TRUE : FALSE);
}

void CTicketEntry::OnEntryContractConvertor()
{
	CContractConvertor Dlg;
	int Status;

	m_Asset.SetFocus();
	Dlg.m_AssetStr = m_Asset.GetData();
	Dlg.m_OraLoader.SetDB(GetData().GetOraLoader().GetDB());
	Status = Dlg.DoModal();
	
	if(Status != IDOK)
		return;

	switch(m_nNomID)
	{
		case IDC_ENTRY_AMOUNT_EDIT:
			m_Amount.SetData(Dlg.m_NominalStr);
			break;
		case IDC_ENTRY_ALLOC_NOMAMOUNT_EDIT:
			m_AllocAmount.SetData(Dlg.m_NominalStr);
			break;
		default:
			break;
	}
}

void CTicketEntry::OnSetfocusAmountEdit()
{
	CString TransType;

	TransType = m_TransType.GetData();

	m_nNomID = IDC_ENTRY_AMOUNT_EDIT;

	if((TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F")) && 
		m_Amount.GetWindowTextLength() == 0)
	{
		OnEntryContractConvertor();
		if(m_Amount.GetWindowTextLength() > 0)
			m_Amount.SetFocus();
	}
}

void CTicketEntry::OnSetfocusAllocNomamountEdit()
{
	CString TransType;

	TransType = m_TransType.GetData();

	m_nNomID = IDC_ENTRY_ALLOC_NOMAMOUNT_EDIT;

	if((TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F")) && 
		m_AllocAmount.GetWindowTextLength() == 0)
	{
		OnEntryContractConvertor();
		if(m_AllocAmount.GetWindowTextLength() > 0)
			m_AllocAmount.SetFocus();
	}
}

void CTicketEntry::OnEntryDealCheck() 
{
	BOOL bChecked;
	CString Text;

	bChecked = m_Trs.GetCheck();
	m_DownPymnt.EnableWindow(bChecked);
	
	Text = m_DownPymnt.GetData();
	if(bChecked)
	{
		if(Text == "100")
			m_DownPymnt.SetData("0");
	}
	else
		if(Text == "0")
			m_DownPymnt.SetData("100");
}

void CTicketEntry::OnKillfocusAmountEdit() 
{
	if(m_Amount.GetWindowTextLength() > 0 && 
		m_Data.GetRawTicket().GetAssetCategory() == "G20 FX ALPHA")
	{
		m_AAMethod.SetData("EMCF_B");
		m_Trader.SetData("IM");
		OnCbnSelchangeEntryAaCombo();
	}

	UpdateAlloc(0);

	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Amount.SetFocus();
}

void CTicketEntry::OnEnChangeEntryAmountEdit()
{
	CString AAMethod, Nom;
	CQData QData;

	if(m_AAFReason.GetCurSel() >= 0 && MessageBox("Do you want to change current manual allocation ?", "Ticket Entry", MB_YESNO) == IDNO)
		return;

	Nom = m_Amount.GetData();
	QData.RemoveComma(Nom);
	m_AAMethod.GetSelString(AAMethod);
	if(m_sAAMethod == AAMethod && atof(Nom) == m_dNomAmount)
		return;
	
	m_AAFReason.SetCurSel(-1);
	m_AllocSS.ClearSheet();

	if(m_Amount.GetModify())
		ProcessVerifyRisk();
}

void CTicketEntry::OnEntryXcurrency() 
{
	CXCurrencyDlg Dlg;

	Dlg.m_pData = &GetData();
	Dlg.m_UserClass = 1;
	Dlg.DoModal();
}

void CTicketEntry::OnEntryRepoprice()
{
	CRepoPrice Dlg;
	CString	ValueDate, RateBasis, Formula;
	
	Dlg.m_Asset = m_Asset.GetData();
	if(Dlg.m_Asset.IsEmpty() || Dlg.m_Asset == "NEW ASSET")
		return;

	Dlg.m_HairCut = m_Margin.GetData();
	if(Dlg.m_HairCut.IsEmpty())
	{
		if(m_Trs.GetCheck())
			Dlg.m_HairCut = "0";
		else
			Dlg.m_HairCut = "10";
	}
	
	Dlg.m_NWINetPrice = m_Price.GetData();
	ValueDate = m_ValueDate.GetData();
	RateBasis = m_RateBasis.GetData(); // Lev Rate Basis
	Dlg.m_Val.Setup(GetData().GetOraLoader(), REPO, P, Dlg.m_Asset, ValueDate, ValueDate, "0", "0", "1", RateBasis, "1", Formula);
					
	if(Dlg.DoModal() == IDOK)
	{
		m_Price.SetData(Dlg.m_NWINetPrice);
		m_Margin.SetData(Dlg.m_HairCut);
	}
}

void CTicketEntry::OnUpdateEntryRepoprice(CCmdUI* pCmdUI)
{
	CString Asset;

	Asset = m_Asset.GetData();

	pCmdUI->Enable(m_TransType.GetData() == REPO && (!Asset.IsEmpty() && Asset != "NEW ASSET"));
}

void CTicketEntry::OnChangeAssetEdit()
{
	CString Asset, TransType;
	CRawTicketRec *pTicket;

	Asset = m_Asset.GetData();
	TransType = m_TransType.GetData();

	pTicket = &m_Data.GetRawTicket();

	if(TransType == SECURITIES && pTicket->GetAssetClass() == "CURRENCY FWDS")
	{
		if(m_Data.GetMargin(Asset))
		{
			if(strlen(pTicket->GetMarginCurrency()) > 0)
				m_MarginCurrency.SetData(pTicket->GetMarginCurrency());
			if(strlen(pTicket->GetMarginAmount()) > 0)
				m_MarginAmount.SetData(pTicket->GetMarginAmount());
			if(strlen(pTicket->GetMargin()) > 0)
				m_Margin.SetData(pTicket->GetMargin());
		}
	}

	if(m_Asset.GetModify())
		ProcessVerifyRisk();
}

void CTicketEntry::OnCbnSelchangeEntryAaCombo()
{
	CDBRec DBRec;
	COraLoader OraLoader;
	CString sMethod, sAmount, Ticket;

	sAmount = m_Amount.GetData();
	sMethod = m_AAMethod.GetData();

	if(m_AAMethod.GetCurSel() >= 0)
	{
		m_AAFReason.SetCurSel(-1);
		m_AAFReason.EnableWindow(FALSE);
	}
	else
		m_AAFReason.EnableWindow(TRUE);

	if(sAmount.IsEmpty() || sMethod.IsEmpty())
		return;

	Ticket = m_UnWindTicket.GetData();

	OraLoader = GetData().GetOraLoader();
	
	OraLoader.GetSql() = m_Data.GetAutoAllocationSql(sMethod, sAmount, Ticket);

	m_AllocArray.RemoveAll();

	if(OraLoader.Open())
	{
		m_AllocSS.ClearSheet();
		OraLoader.MoveFirst();
		while(!OraLoader.IsEOF())
		{
			DBRec.RemoveAll();
			m_Portfolio.SetData(OraLoader.GetDBString(0));
			DBRec.Add(OraLoader.GetDBString(0));
			m_AllocAmount.SetData(OraLoader.GetDBString(1));
			DBRec.Add(OraLoader.GetDBString(1));
//			m_Link.SetData(OraLoader.GetDBString(2));
			m_bAllocValid = FALSE;
			m_AllocArray.Add(DBRec);
			OnEntryAddAllocation();
			OraLoader.MoveNext();
		}
	}

	ProcessVerifyRisk();
}

void CTicketEntry::OnCbnKillfocusEntryAaCombo()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_AAMethod.SetFocus();
}

void CTicketEntry::OnEnSetfocusEntryBinaryEdit()
{
	CString TransType;
	
	TransType = m_TransType.GetData();
	if((TransType == CALL || TransType == PUT))
		m_nCurrID = IDC_ENTRY_BINARY_EDIT;
}

void CTicketEntry::OnEntryFinditem()
{
	int CurrID;
	CFindDlg FindDlg;

	CurrID = m_nCurrID;
	m_nCurrID = 0;

	FindDlg.m_OraLoader = m_Data.GetOraLoader();
	
	switch(CurrID)
	{
		case IDC_ENTRY_BINARY_EDIT:
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
		default:
			return;
	}
}

void CTicketEntry::OnEntryForbiddenList()
{
	CProhibitList Dlg;

	Dlg.m_OraLoader = GetData().GetOraLoader();
	Dlg.DoModal();
}

void CTicketEntry::OnEntryCancel()
{
	CString OrigTicket, Ticket, Sql;
	CDateDlg TktDlg;

	TktDlg.m_nInputType = 2;
	TktDlg.m_DlgHeader = "Input Ticket";
	TktDlg.m_Prompt = "Enter cancel ticket:";

	if(TktDlg.DoModal() != IDOK)
		return;

	COraLoader OraLoader = GetData().GetOraLoader();
	
	OraLoader.GenerateUniqueID("TICKET_NUM", Ticket);
	OrigTicket = TktDlg.m_Data;

	if(MessageBox("Are sure you want to cancel " + OrigTicket + " ? ", "Ticket Entry", MB_YESNOCANCEL) != IDYES)
		return;
	
	Sql.Format("INSERT INTO SEMAM.NW_RAW_TICKETS (TICKET_NUM, TRADER_INI, TRADE_DATE, VALUE_DATE, DIR, TRANS_TYPE, NOM_AMOUNT, "
				"STRIKE, OPT_EXPIRATION, OTC, CURRENCY, FXRATE, COUNTERPARTY, BROKER, ASSET_CODE, ASSET_DESC, ASSET_COUNTRY, "
				"ASSET_CURRENCY, ASSET_CLASS, ASSET_CATEGORY, ASSET_BUCKET, ASSET_IND_AREA, ASSET_MATURITY, ASSET_COMMON_C, "
				"ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, IPO, TRS, REPO_CP, TR_RATE, RATE_BASIS, MATURITY, "
				"SW_BOOKING, SW_MATURITY, FLOAT_RATE, FLOAT_RATE_BAS, FLOAT_MATURITY, NOTE, NOTE2, SOURCE, OPT_TICK, EURO_OPT, "
				"WI, FX_DATE, ASSET_PID, MARGIN, OPT_AUTO, OPT_SET_CODE, SWAP_TICKET, DELIVERY_DATE, MARGIN_CURRENCY, "
				"MARGIN_AMOUNT, FUNDED_SWAP, BINARY, OPT_SET_CODE2, SET_CONVENTION, BEST_EXECUTION, SHORT_SALE, PRICE, DOWN_PYMNT, "
				"BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, EXEC_TIME, "
				"ETRADE, CSPB_SHORT) "
				"SELECT '%s', TRADER_INI, TRADE_DATE, VALUE_DATE, DECODE(DIR, 'P', 'S', 'S', 'P'), TRANS_TYPE, SUM(NOM_AMOUNT), "
				"EXERCISE_PRICE, OPT_EXPIRATION, DECODE(LISTED, 'Y', NULL, 'Y'), CURRENCY, FXRATE, COUNTERPARTY, BROKER, "
				"ASSET_CODE, ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
				"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, NEW_ISSUE, DEAL_TYPE, NULL \"REPO_CP\", TR_RATE, "
				"RATE_BASIS, MATURITY_DATE, ORIG_SW_BOOKING, ORIG_SW_MATURITY, TO_NUMBER(NULL) \"FLOAT_RATE\", NULL \"FLOAT_RATE_BASIS\", "
				"TO_DATE(NULL) \"FLOAT_MATURITY\", 'CANCEL '||A.TICKET_NUM, NULL \"NOTE2\", SOURCE, OPT_TICK, EURO_OPT, WI, FX_DATE, "
				"ASS_PID, MARGIN, OPT_AUTO, OPT_SET_CODE, SWAP_TICKET, DELIVERY_DATE, MARGIN_CURRENCY, SUM(MARGIN_AMOUNT), FUNDED_SWAP, "
				"BINARY, OPT_SET_CODE2, SET_CONVENTION, BEST_EXECUTION, "
				"SHORT_SALE, A.PRICE + DECODE(DIR, 'P', -1, 1)*SUM(NVL(BR_FEES, 0) + NVL(LEV_RATE, 0))/SUM(NOM_AMOUNT) \"PRICE\", "
				"100 \"DOWN_PYMNT\", SUM(BR_FEES) \"BROKER_FEE\", SUM(LEV_RATE) \"OTHER_FEE\", SUM(SOFTDOLLAR), ASSIGN_CP, DV01, "
				"SUM(VAR), A.TICKET_NUM \"CANCEL_TICKET\", CORRECT_TICKET, SEC_FEE, OR_FEE, A.EXEC_TIME, ETRADE, CSPB_SHORT "
				"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B "
				"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
				"AND A.TRANS_TYPE IN ('CALL', 'CDS', 'SECURITIES', 'PUT', 'REPO') "
				"AND A.DEAL_TYPE IS NULL "
				"AND A.TR_DESC = 'BOOKING' "
				"AND A.DIR IN ('P', 'S') "
				"AND A.TICKE_NUM = '%s' "
				"GROUP BY TRADER_INI, TRADE_DATE, VALUE_DATE, DECODE(DIR, 'P', 'S', 'S', 'P'), TRANS_TYPE, EXERCISE_PRICE, OPT_EXPIRATION, "
				"DECODE(LISTED, 'Y', NULL, 'Y'), CURRENCY, FXRATE, COUNTERPARTY, BROKER, ASSET_CODE, ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, "
				"ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, "
				"ASS_SEDOL_NUM, NEW_ISSUE, DEAL_TYPE, TR_RATE, RATE_BASIS, MATURITY_DATE, ORIG_SW_BOOKING, ORIG_SW_MATURITY, SOURCE, "
				"OPT_TICK, EURO_OPT, WI, FX_DATE, ASS_PID, MARGIN, OPT_AUTO, OPT_SET_CODE, SWAP_TICKET, DELIVERY_DATE, MARGIN_CURRENCY, "
				"FUNDED_SWAP, BINARY, OPT_SET_CODE2, SET_CONVENTION, BEST_EXECUTION, SHORT_SALE, A.PRICE, ASSIGN_CP, DV01, TICKET_NUM, "
				"CORRECT_TICKET, SEC_FEE, OR_FEE, A.EXEC_TIME, ETRADE, CSPB_SHORT "
				"UNION "
				"SELECT '%s', A.TRADER_INI, A.TRADE_DATE, A.VALUE_DATE, DECODE(A.DIR, 'P', 'S', 'S', 'P'), A.TRANS_TYPE, SUM(A.NOM_AMOUNT), "
				"A.EXERCISE_PRICE, A.OPT_EXPIRATION, DECODE(A.LISTED, 'Y', NULL, 'Y'), A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, "
				"A.ASSET_CODE, ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
				"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, NULL \"REPO_CP\", A.TR_RATE, "
				"A.RATE_BASIS, A.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, C.TR_RATE, C.RATE_BASIS, C.MATURITY_DATE, "
				"'CANCEL '||A.TICKET_NUM, NULL \"NOTE2\", A.SOURCE, A.OPT_TICK, A.EURO_OPT, A.WI, A.FX_DATE, ASS_PID, A.MARGIN, A.OPT_AUTO, "
				"A.OPT_SET_CODE, A.SWAP_TICKET, A.DELIVERY_DATE, A.MARGIN_CURRENCY, SUM(A.MARGIN_AMOUNT), A.FUNDED_SWAP, A.BINARY, "
				"A.OPT_SET_CODE2, A.SET_CONVENTION, A.BEST_EXECUTION, A.SHORT_SALE, A.PRICE, 100 \"DOWN_PYMNT\", SUM(A.BR_FEES) \"BROKER_FEE\", "
				"SUM(A.LEV_RATE) \"OTHER_FEE\", SUM(A.SOFTDOLLAR), A.ASSIGN_CP, A.DV01, SUM(A.VAR), A.TICKET_NUM \"CANCEL_TICKET\", "
				"A.CORRECT_TICKET, A.SEC_FEE, A.OR_FEE, A.EXEC_TIME, A.ETRADE, A.CSPB_SHORT "
				"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_TICKETS C "
				"WHERE B.ASS_CODE = A.ASSET_CODE "
				"AND B.ASS_INT_TYPE = 'FIXED' "
				"AND C.TICKET_NUM = A.TICKET_NUM "
				"AND C.PORTFOLIO = A.PORTFOLIO "
				"AND C.TRADE_DATE = A.TRADE_DATE "
				"AND C.TRANS_NUM != A.TRANS_NUM "
				"AND A.TRANS_TYPE IN ('INT. SWAP') "
				"AND A.TR_DESC = 'MATURITY' "
				"AND A.DIR IN ('P', 'S') "
				"AND A.TICKE_NUM = '%s' "
				"GROUP BY A.TRADER_INI, A.TRADE_DATE, A.VALUE_DATE, DECODE(A.DIR, 'P', 'S', 'S', 'P'), A.TRANS_TYPE, A.EXERCISE_PRICE, "
				"A.OPT_EXPIRATION, DECODE(A.LISTED, 'Y', NULL, 'Y'), A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, A.ASSET_CODE, ASS_DESC, "
				"ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, ASS_COMMON_CODE, "
				"ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, A.TR_RATE, A.RATE_BASIS, A.MATURITY_DATE, "
				"A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, C.TR_RATE, C.RATE_BASIS, C.MATURITY_DATE, SOURCE, A.OPT_TICK, A.EURO_OPT, A.WI, "
				"A.FX_DATE, ASS_PID, A.MARGIN, A.OPT_AUTO, A.OPT_SET_CODE, A.SWAP_TICKET, A.DELIVERY_DATE, A.MARGIN_CURRENCY, A.FUNDED_SWAP, "
				"A.BINARY, A.OPT_SET_CODE2, A.SET_CONVENTION, A.BEST_EXECUTION, A.SHORT_SALE, A.PRICE, A.ASSIGN_CP, A.DV01, A.TICKET_NUM, "
				"A.CORRECT_TICKET, A.SEC_FEE, A.OR_FEE, A.EXEC_TIME, A.ETRADE, A.CSPB_SHORT "
				"UNION "
				"SELECT '%s', A.TRADER_INI, A.TRADE_DATE, A.VALUE_DATE, DECODE(A.DIR, 'P', 'S', 'S', 'P'), A.TRANS_TYPE, SUM(A.NOM_AMOUNT), "
				"A.EXERCISE_PRICE, A.OPT_EXPIRATION, DECODE(A.LISTED, 'Y', NULL, 'Y'), A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, "
				"A.ASSET_CODE, ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
				"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, C.COUNTERPARTY \"REPO_CP\", "
				"C.TR_RATE, C.RATE_BASIS, C.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, TO_NUMBER(NULL) \"TR_RATE\", "
				"NULL \"RATE_BASIS\", TO_DATE(NULL) \"MATURITY_DATE\", 'CANCEL '||A.TICKET_NUM, NULL \"NOTE2\", A.SOURCE, A.OPT_TICK, "
				"A.EURO_OPT, A.WI, A.FX_DATE, ASS_PID, A.MARGIN, A.OPT_AUTO, A.OPT_SET_CODE, A.SWAP_TICKET, A.DELIVERY_DATE, "
				"A.MARGIN_CURRENCY, SUM(A.MARGIN_AMOUNT), A.FUNDED_SWAP, A.BINARY, A.OPT_SET_CODE2, A.SET_CONVENTION, A.BEST_EXECUTION, "
				"A.SHORT_SALE, A.PRICE + DECODE(DIR, 'P', -1, 1)*SUM(NVL(BR_FEES, 0) + NVL(LEV_RATE, 0))/SUM(NOM_AMOUNT) \"PRICE\", "
				"DECODE(C.TICKET_NUM, NULL, 100, 0) \"DOWN_PYMNT\", SUM(A.BR_FEES) \"BROKER_FEE\", SUM(A.LEV_RATE) \"OTHER_FEE\", "
				"SUM(A.SOFTDOLLAR), A.ASSIGN_CP, A.DV01, SUM(A.VAR), A.TICKET_NUM \"CANCEL_TICKET\", A.CORRECT_TICKET, A.SEC_FEE, A.OR_FEE, "
				"A.EXEC_TIME, A.ETRADE, A.CSPB_SHORT "
				"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_TICKETS C "
				"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
				"AND C.TICKET_NUM(+) = A.TICKET_NUM "
				"AND C.PORTFOLIO(+) = A.PORTFOLIO "
				"AND C.TRADE_DATE(+) = A.TRADE_DATE "
				"AND C.TRANS_TYPE(+) = 'LEVERAGE' "
				"AND A.TRANS_TYPE IN ('SECURITIES') "
				"AND A.DIR IN ('P', 'S') "
				"AND A.DEAL_TYPE = 'TRS' "
				"AND A.TR_DESC = 'BOOKING' "
				"AND A.TICKE_NUM = '%s' "
				"GROUP BY A.TRADER_INI, A.TRADE_DATE, A.VALUE_DATE, DECODE(A.DIR, 'P', 'S', 'S', 'P'), A.TRANS_TYPE, A.EXERCISE_PRICE, "
				"A.OPT_EXPIRATION, DECODE(A.LISTED, 'Y', NULL, 'Y'), A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, A.ASSET_CODE, "
				"ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, ASS_COMMON_CODE, "
				"ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, C.TR_RATE, C.RATE_BASIS, C.MATURITY_DATE, "
				"A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, SOURCE, A.OPT_TICK, A.EURO_OPT, A.WI, A.FX_DATE, ASS_PID, A.MARGIN, A.OPT_AUTO, "
				"A.OPT_SET_CODE, A.SWAP_TICKET, A.DELIVERY_DATE, A.MARGIN_CURRENCY, A.FUNDED_SWAP, A.BINARY, A.OPT_SET_CODE2, "
				"A.SET_CONVENTION, A.BEST_EXECUTION, A.SHORT_SALE, A.PRICE, DECODE(C.TICKET_NUM, NULL, 100, 0), A.ASSIGN_CP, A.DV01, "
				"A.TICKET_NUM, A.CORRECT_TICKET, A.SEC_FEE, A.OR_FEE, A.EXEC_TIME, A.ETRADE, A.CSPB_SHORT ", (LPCTSTR) Ticket, 
				(LPCTSTR) OrigTicket, (LPCTSTR) Ticket, (LPCTSTR) OrigTicket, (LPCTSTR) Ticket, (LPCTSTR) OrigTicket);
		if(!OraLoader.ExecuteSql(Sql))
			return;
		
		Sql.Format("INSERT INTO SEMAM.NW_RAW_TICKET_DETAIL (TICKET_NUM, PORTFOLIO, NOM_AMOUNT, PRICE, OPT_BACK, CUSTODIAN, ACCOUNT) "
				"SELECT '%s', PORTFOLIO, NOM_AMOUNT, PRICE, TRANS_NUM, CUSTODIAN, ACC_CODE "
				"SEMAM.ALL_TICKET_INV_V A "
				"WHERE TICKET_NUM = '%s' "
				"AND A.TRANS_TYPE != 'INT. SWAP' "
				"AND A.TR_DESC = 'BOOKING' "
				"UNION "
				"SELECT '%s', PORTFOLIO, NOM_AMOUNT, PRICE, TRANS_NUM, CUSTODIAN, ACC_CODE "
				"SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B "
				"WHERE B.ASS_CODE = A.ASSET_CODE "
				"AND B.ASS_INT_TYPE = 'FIXED' "
				"AND TICKET_NUM = '%s' "
				"AND A.TRANS_TYPE = 'INT. SWAP' "
				"AND A.TR_DESC = 'MATURITY' ", (LPCTSTR) Ticket, (LPCTSTR) OrigTicket, (LPCTSTR) Ticket, (LPCTSTR) OrigTicket);
		OraLoader.ExecuteSql(Sql);
}


void CTicketEntry::OnUpdateEntryCancel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetData().IsOperation() || GetData().IsPowerUser());
}

void CTicketEntry::OnEnChangeEntryPriceEdit()
{
	if(m_Price.GetModify())
		ProcessVerifyRisk();

	CQData QData;

	m_IMFxrate.SetData(QData.GetImpliedFxrate(m_Data.GetRawTicket().GetAssetClass(), m_Currency.GetData(), m_Rev, m_Price.GetData()));
}

void CTicketEntry::OnEnKillfocusEntryPriceEdit()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Price.SetFocus();
}


void CTicketEntry::OnEnKillfocusEntryAssetEdit()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Asset.SetFocus();
}


void CTicketEntry::OnEntryTrenchcopyTicket()
{
	CTrenchCopyDlg Dlg;
	COraLoader OraLoader, OraLoader2;
	CString Ticket, Asset, Asset2;

	Dlg.m_sTicket = m_Ticket.GetData();

	if(Dlg.DoModal() == IDOK)
	{
		OraLoader.SetDB(&theDB);
		OraLoader2.SetDB(&theDB);

		OraLoader.Open("SELECT A.ASSET_CODE, B.ASSET_CODE FROM SEMAM.NW_TRENCH_ASSETS A, SEMAM.NW_RAW_TICKETS B "
						"WHERE B.ASSET_CODE(+) = A.ASSET_CODE AND B.TICKET_NUM(+) = '" + Dlg.m_sTicket + 
						"' AND A.TRENCH_CODE = '" + Dlg.m_sTrenchCode + "' ORDER BY 1 ");
		OraLoader.MoveFirst();
		while(!OraLoader.IsEOF())
		{
			Asset = OraLoader.GetDBString(0);
			Asset2 = OraLoader.GetDBString(1);
			if(Asset != Asset2)
			{
				GetData().GetOraLoader().GenerateUniqueID(TICKETNUM, Ticket);
				OraLoader2.ExecuteSql("INSERT INTO SEMAM.NW_RAW_TICKETS (TICKET_NUM, ASSET_CODE, TRADER_INI, TRADE_DATE, VALUE_DATE, "
								"DIR, TRANS_TYPE, NOM_AMOUNT, IMG_NOMINAL, STRIKE, OPT_EXPIRATION, OTC, BINARY, CURRENCY, FXRATE, "
							"FX_DATE, COUNTERPARTY, BROKER, ASSET_DESC, ASSET_COUNTRY, ASSET_CURRENCY, ASSET_CLASS, ASSET_CATEGORY, "
							"ASSET_BUCKET, ASSET_IND_AREA, ASSET_COUPON, ASSET_MATURITY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, "
							"ASSET_ISIN_CODE, ASSET_SEDOL_NUM, ASSET_PID, ASSET_IMAGINE_CODE, IPO, TRS, REPO_CP, TR_RATE, RATE_BASIS, "
							"MATURITY, TR_TENOR, SW_BOOKING, SW_MATURITY, FLOAT_RATE, FLOAT_RATE_BASIS, FLOAT_MATURITY, NOTE, NOTE2, "
							"SOURCE, OPT_TICK, EURO_OPT, OPT_AUTO, OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, WI, SWAP_TICKET, "
							"DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, AA_METHOD, AA_FREASON, BEST_EXECUTION, "
							"SHORT_SALE, CSPB_SHORT, PRICE, DOWN_PYMNT, BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, "
							"CORRECT_TICKET, SEC_FEE, OR_FEE, VERIFIED, TRADER_SIGN, TRADER_SIGN_DATE, ETRADE, IMAGINE_EXEC_ID, "
							"CP_TRADE_ID, ORDER_ID, BOOKER, BOOK_DATE, EXEC_TIME) "
							"SELECT '" + Ticket + "', '" + Asset + "', TRADER_INI, TRADE_DATE, VALUE_DATE, DIR, TRANS_TYPE, "
							"NOM_AMOUNT, IMG_NOMINAL, STRIKE, OPT_EXPIRATION, OTC, BINARY, CURRENCY, FXRATE, FX_DATE, COUNTERPARTY, "
							"BROKER, ASSET_DESC, ASSET_COUNTRY, ASSET_CURRENCY, ASSET_CLASS, ASSET_CATEGORY, ASSET_BUCKET, ASSET_IND_AREA, "
							"ASSET_COUPON, ASSET_MATURITY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, "
							"ASSET_PID, ASSET_IMAGINE_CODE, IPO, TRS, REPO_CP, TR_RATE, RATE_BASIS, MATURITY, TR_TENOR, SW_BOOKING, "
							"SW_MATURITY, FLOAT_RATE, FLOAT_RATE_BASIS, FLOAT_MATURITY, NOTE, NOTE2, SOURCE, OPT_TICK, EURO_OPT, "
							"OPT_AUTO, OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, WI, SWAP_TICKET, DELIVERY_DATE, MARGIN, "
							"MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, AA_METHOD, AA_FREASON, BEST_EXECUTION, SHORT_SALE, CSPB_SHORT, "
							"PRICE, DOWN_PYMNT, BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, CORRECT_TICKET, SEC_FEE, "
							"OR_FEE, VERIFIED, TRADER_SIGN, TRADER_SIGN_DATE, ETRADE, IMAGINE_EXEC_ID, CP_TRADE_ID, ORDER_ID, BOOKER, "
							"BOOK_DATE, EXEC_TIME FROM SEMAM.NW_RAW_TICKETS A WHERE TICKET_NUM = '" + Dlg.m_sTicket + "' ");
			
				OraLoader2.ExecuteSql("INSERT INTO SEMAM.NW_RAW_TICKET_DETAIL (TICKET_NUM, PORTFOLIO, NOM_AMOUNT) "
										"SELECT '" + Ticket + "', PORTFOLIO, NOM_AMOUNT FROM SEMAM.NW_RAW_TICKET_DETAIL "
										"WHERE TICKET_NUM = '" + Dlg.m_sTicket + "' ");
			}
			
			OraLoader.MoveNext();
		}

		OnEntryLoadTrade();
	}
}

void CTicketEntry::OnUpdateEntryTrenchcopyTicket(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetData().IsOperation() && m_SS.GetSheetCurRow() > 0);
}
