// TicketFXEntry.cpp : implementation file
//
#include "stdafx.h"
#include <math.h>
#include "NWIFMS70.h"
#include "TicketfxEntry.h"
#include "assetdlg.h"
#include "brokerfee.h"
#include "qdata.h"
#include "contractconvertor.h"
#include "optexpdlg.h"
#include "xcurrencydlg.h"
#include "repoprice.h"
#include "Finddlg.h"
#include "ProhibitList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTicketFXEntry
IMPLEMENT_DYNCREATE(CTicketFXEntry, CFormView)

CTicketFXEntry::CTicketFXEntry()
	: CFormView(CTicketFXEntry::IDD)
{
	//{{AFX_DATA_INIT(CTicketFXEntry)
	m_Listed = NULL;
	m_EuroOpt = NULL;
	m_OptAuto = NULL;
	m_ShortSale = NULL;
	m_CSPBShort = NULL;
	m_ETrade = NULL;

    m_TransType = NULL;
    m_CP = NULL;
    m_Currency = NULL;
	m_Portfolio = NULL;
	m_AssignCP = NULL;
	m_MarginCurrency = NULL;
	m_AAFReason = NULL;
	m_BestExecution = NULL;

	m_bAllocValid = TRUE;
	m_bLoaded = FALSE;
	//}}AFX_DATA_INIT
}

CTicketFXEntry::~CTicketFXEntry()
{
	if(m_Listed)
		delete m_Listed;
	if(m_EuroOpt)
		delete m_EuroOpt;
	if(m_OptAuto)
		delete m_OptAuto;
	if(m_ShortSale)
		delete m_ShortSale;
	if(m_CSPBShort)
		delete m_CSPBShort;
	if(m_ETrade)
		delete m_ETrade;

    if(m_TransType)
		delete m_TransType;
    if(m_CP)
		delete m_CP;
    if(m_Currency)
		delete m_Currency;
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_AssignCP)
		delete m_AssignCP;
	if(m_MarginCurrency)
		delete m_MarginCurrency;
	if(m_AAFReason)
		delete m_AAFReason;
	if(m_BestExecution)
		delete m_BestExecution;
}

void CTicketFXEntry::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTicketFXEntry)
	DDX_Control(pDX, IDC_FXENTRY_TICKET_LIST, m_SS);
	DDX_Control(pDX, IDC_FXENTRY_ALLOCATION_LIST, m_AllocSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTicketFXEntry, CFormView)
	//{{AFX_MSG_MAP(CTicketFXEntry)
	ON_CBN_SELCHANGE(IDC_FXENTRY_TRANSTYPE_COMBO, OnSelchangeTranstypeCombo)
	ON_COMMAND(ID_ENTRY_ADD, OnEntryAdd)
	ON_COMMAND(ID_ENTRY_DELETE, OnEntryDelete)
	ON_COMMAND(ID_ENTRY_UPDATE, OnEntryUpdate)
	ON_COMMAND(ID_ENTRY_REFRESH, OnEntryRefresh)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_UPDATE, OnUpdateEntryUpdate)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_DELETE, OnUpdateEntryDelete)
	ON_CBN_SELCHANGE(IDC_FXENTRY_TRANSDIR_COMBO, OnSelchangeTransdirCombo)
	ON_CBN_SELCHANGE(IDC_FXENTRY_CURRENCY_COMBO, OnSelchangeCurrencyCombo)
	ON_COMMAND(ID_ENTRY_FIND_ASSET, OnEntryFindAsset)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_FIND_ASSET, OnUpdateEntryFindAsset)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_ADD, OnUpdateEntryAdd)
	ON_BN_CLICKED(IDC_FXENTRY_LISTED_CHECK, OnListedCheck)
	ON_EN_SETFOCUS(IDC_FXENTRY_ASSETCODE_COMBO, OnSelChangeAssetCombo)
	ON_COMMAND(ID_ENTRY_COPY_TICKET, OnEntryCopyTicket)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_COPY_TICKET, OnUpdateEntryCopyTicket)
	ON_COMMAND(ID_ENTRY_ADD_ALLOCATION, OnEntryAddAllocation)
	ON_COMMAND(ID_ENTRY_DELETE_ALLOCATION, OnEntryDeleteAllocation)
	ON_COMMAND(ID_ENTRY_UPDATE_ALLOCATION, OnEntryUpdateAllocation)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_DELETE_ALLOCATION, OnUpdateEntryDeleteAllocation)
	ON_EN_CHANGE(IDC_FXENTRY_AMOUNT_EDIT, OnEnChangeEntryAmountEdit)
	ON_EN_SETFOCUS(IDC_FXENTRY_ALLOC_NOMAMOUNT_EDIT, OnSetfocusAllocNomamountEdit)
	ON_EN_KILLFOCUS(IDC_FXENTRY_ALLOC_NOMAMOUNT_EDIT, OnKillfocusAllocNomamountEdit)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_UPDATE_ALLOCATION, OnUpdateEntryUpdateAllocation)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_ADD_ALLOCATION, OnUpdateEntryAddAllocation)
	ON_CBN_SELCHANGE(IDC_FXENTRY_PORTFOLIO_COMBO, OnSelchangeEntryPortfolioCombo)
	ON_COMMAND(ID_ENTRY_LOAD_BLOTTER, OnEntryLoadBlotter)
	ON_COMMAND(ID_ENTRY_LOAD_TRADE, OnEntryLoadTrade)
	ON_COMMAND(ID_ENTRY_OPTION_EXP, OnEntryOptionExp)
	ON_UPDATE_COMMAND_UI(ID_ENTRY_OPTION_EXP, OnUpdateEntryOptionExp)
	ON_EN_KILLFOCUS(IDC_FXENTRY_AMOUNT_EDIT, OnKillfocusAmountEdit)
	ON_COMMAND(ID_ENTRY_XCURRENCY, OnEntryXcurrency)
	ON_CBN_SELCHANGE(IDC_FXENTRY_AA_COMBO, &CTicketFXEntry::OnCbnSelchangeEntryAaCombo)
	ON_COMMAND(ID_ENTRY_FORBIDDEN_LIST, OnEntryForbiddenList)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_FXENTRY_BINARY_EDIT, &CTicketFXEntry::OnEnSetfocusEntryBinaryEdit)
	ON_COMMAND(ID_ENTRY_FINDITEM, &CTicketFXEntry::OnEntryFinditem)
	ON_CBN_SELCHANGE(IDC_FXENTRY_ASSETCODE_COMBO, &CTicketFXEntry::OnCbnSelchangeFxentryAssetcodeCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTicketFXEntry diagnostics
#ifdef _DEBUG
void CTicketFXEntry::AssertValid() const
{
	CFormView::AssertValid();
}

void CTicketFXEntry::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CTicketFXEntry::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

BOOL CTicketFXEntry::AllocIsOK()
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

	m_Portfolio->GetSelString(Data);
	if(Data.IsEmpty())
		Text = "Must select a portfolio";

	m_AllocAmount.GetWindowText(Nom);
	if(m_Data.AllocCheckSum(Data, atof(QData.RemoveComma(Nom))) > 0)
		Text = "Sum of the shares in each portfolios must be equal to the Nominal Amount";

	m_TransType->GetSelString(TransType);
	if(TransType != FOREX && TransType != INTSWAP && m_Price.GetWindowTextLength() == 0)		
		Text = "Price must be entered";

	m_VAR.GetWindowText(Data);
	if(fabs(atof(QData.RemoveComma(Data))) == 0)
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


void CTicketFXEntry::EnableAlloc()
{
	m_Portfolio->EnableWindow(!m_Data.GetBlotter());
	m_AllocAmount.SetReadOnly(m_Data.GetBlotter()); // If blotter, read only
//	m_Link.SetReadOnly(m_Data.GetBlotter());
//	m_AssignCP->EnableWindow(m_Data.GetBlotter());
}

void CTicketFXEntry::EnableAllocCtrls()
{
	BOOL bEnable;
	CString TransType, Data;

	bEnable = m_Portfolio->GetCurSel() >= 0 ? TRUE : FALSE;

	m_AllocAmount.EnableWindow(bEnable);
	
	m_TransType->GetSelString(TransType);

/*	if(!(TransType == SECURITIES || TransType == CDS || TransType == CALL || TransType == PUT) && bEnable)
		m_Link.SetReadOnly();
	else
		m_Link.SetReadOnly(0); */
}

void CTicketFXEntry::EnableCtrls()
{
	CString TransType, Dir, RepoCP;

	m_TradeDate.SetReadOnly(m_Data.GetBlotter());
	m_ValueDate.SetReadOnly(m_Data.GetBlotter());
	m_Amount.SetReadOnly(m_Data.GetBlotter());
	m_Strike.SetReadOnly(m_Data.GetBlotter());
	m_OptExp.SetReadOnly(m_Data.GetBlotter());
	m_OptTicker.SetReadOnly(m_Data.GetBlotter());
	m_FxRate.SetReadOnly(m_Data.GetBlotter());
	m_SWBooking.SetReadOnly(m_Data.GetBlotter());
	m_Note.SetReadOnly(m_Data.GetBlotter());
	m_Note2.SetReadOnly(m_Data.GetBlotter());

	GetDlgItem(IDC_FXENTRY_STRIKE_STATIC)->ShowWindow(SW_HIDE);
	m_Strike.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_EXPIRATION_STATIC)->ShowWindow(SW_HIDE);
	m_OptExp.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_OPT_TICKER_STATIC)->ShowWindow(SW_HIDE);
	m_OptTicker.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_OPTSET_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_OPTSET2_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet2.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_BLMID_STATIC)->ShowWindow(SW_HIDE);
	m_OptID.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_MARGIN_STATIC)->ShowWindow(SW_HIDE);
	m_Margin.ShowWindow(SW_HIDE);
	m_MarginCurrency->ShowWindow(SW_HIDE);
	m_MarginAmount.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_SWAP_TICKET_STATIC)->ShowWindow(SW_HIDE);
	m_SwapTicket.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_SWAP_TICKET_STATIC)->ShowWindow(SW_HIDE);
	m_SwapTicket.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_DELIVERY_STATIC)->ShowWindow(SW_HIDE);
	m_DeliveryDate.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FXENTRY_BINARY_STATIC)->ShowWindow(SW_HIDE);
	m_Binary.ShowWindow(SW_HIDE);

	m_Listed->ShowWindow(SW_HIDE);
	m_EuroOpt->ShowWindow(SW_HIDE);

	
	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	GetData().DefineTransType(TransType, Dir);

	if(TransType == SECURITIES || TransType == FOREX || TransType == CALL || TransType == PUT)
		m_AssignCP->EnableWindow(TRUE);
	else
		m_AssignCP->EnableWindow(FALSE);

	if(TransType == INTSWAP || TransType == CASH)
	{
		GetDlgItem(IDC_FXENTRY_ASSET_STATIC)->ShowWindow(SW_HIDE);
		m_Asset.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FXENTRY_ASSETDESC_STATIC)->ShowWindow(SW_HIDE);
		m_AssetDesc.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FXENTRY_CURRENCY_STATIC)->ShowWindow(SW_HIDE);
		m_AssetCurrency.ShowWindow(SW_HIDE);
		m_Price.ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_FXENTRY_ASSET_STATIC)->ShowWindow(SW_SHOW);
		m_Asset.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_ASSETDESC_STATIC)->ShowWindow(SW_SHOW);
		m_AssetDesc.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_CURRENCY_STATIC)->ShowWindow(SW_SHOW);
		m_AssetCurrency.ShowWindow(SW_SHOW);
		m_Price.ShowWindow(SW_SHOW);
	}

	if(TransType != FOREX)
	{
		GetDlgItem(IDC_FXENTRY_MARGIN_STATIC)->ShowWindow(SW_SHOW);
		m_Margin.ShowWindow(SW_SHOW);
		m_MarginCurrency->ShowWindow(SW_SHOW);
		m_MarginAmount.ShowWindow(SW_SHOW);
		m_Price.ShowWindow(SW_SHOW);
	}
	else
		m_Price.ShowWindow(SW_HIDE);

	if(TransType == SECURITIES && m_Data.GetRawTicket().GetAssetClass() == "CURRENCY FWDS")
	{
		GetDlgItem(IDC_FXENTRY_SWAP_TICKET_STATIC)->ShowWindow(SW_SHOW);
		m_SwapTicket.ShowWindow(SW_SHOW);
	}
	
	if(TransType == CALL || TransType == PUT)
	{
		GetDlgItem(IDC_FXENTRY_STRIKE_STATIC)->ShowWindow(SW_SHOW);
		m_Strike.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_EXPIRATION_STATIC)->ShowWindow(SW_SHOW);
		m_OptExp.ShowWindow(SW_SHOW);
		m_Listed->ShowWindow(SW_SHOW);
		m_EuroOpt->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_BINARY_STATIC)->ShowWindow(SW_SHOW);
		m_Binary.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_OPT_TICKER_STATIC)->ShowWindow(SW_SHOW);
		m_OptTicker.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_BLMID_STATIC)->ShowWindow(SW_SHOW);
		m_OptID.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_DELIVERY_STATIC)->ShowWindow(SW_SHOW);
		m_DeliveryDate.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_OPTSET_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FXENTRY_OPTSET2_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet2.ShowWindow(SW_SHOW);
	}

	if(m_Data.GetBlotter())
	{
		m_TransType->EnableWindow(FALSE);
		m_Dir.EnableWindow(FALSE);
		m_CP->EnableWindow(FALSE);
		m_Currency->EnableWindow(FALSE);
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
		m_Listed->EnableWindow(TRUE);
		m_EuroOpt->EnableWindow(TRUE);
		m_Binary.EnableWindow(TRUE);
	}
}

void CTicketFXEntry::InitControls()
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

	m_Listed = new CCheckBox(this, IDC_FXENTRY_LISTED_CHECK, Y);
	m_EuroOpt = new CCheckBox(this, IDC_FXENTRY_EUROOPT_CHECK, Y);
	m_OptAuto = new CCheckBox(this, IDC_FXENTRY_AUTO_CHECK, Y);
	m_ShortSale = new CCheckBox(this, IDC_FXENTRY_SHORT_CHECK, Y);
	m_CSPBShort = new CCheckBox(this, IDC_FXENTRY_CSPBSHORT_CHECK, Y);
	m_ETrade = new CCheckBox(this, IDC_FXENTRY_ETRADE_CHECK, Y);

    m_TransType = new COptComboBox(this, IDC_FXENTRY_TRANSTYPE_COMBO);
    m_CP = new COptComboBox(this, IDC_FXENTRY_COUNTERPARTY_COMBO);
    m_Currency = new COptComboBox(this, IDC_FXENTRY_CURRENCY_COMBO);
	m_Portfolio = new COptComboBox(this, IDC_FXENTRY_PORTFOLIO_COMBO);
	m_MarginCurrency = new COptComboBox(this, IDC_FXENTRY_MARGIN_CURRENCY_COMBO, TRUE);
	m_AAFReason = new COptComboBox(this, IDC_FXENTRY_AAF_COMBO, TRUE);
	m_BestExecution = new COptComboBox(this, IDC_FXENTRY_BESTEXECUTION_COMBO);

	m_Ticket.Setup(this, IDC_FXENTRY_TICKET_EDIT);
	m_SwapTicket.Setup(this, IDC_FXENTRY_SWAP_TICKET_EDIT);
	m_UnWindTicket.Setup(this, IDC_FXENTRY_UNWINDTICKET_EDIT);

	m_TradeDate.Setup(this, IDC_FXENTRY_TRADEDATE_EDIT, FALSE, "Trade Date must be greater than or equal to Today's date");
	m_TradeDate.SetCompareDate(GetData().GetDate());
	m_ExecTime.Setup(this, IDC_FXENTRY_EXEC_TIME_EDIT, FALSE,	"Execution time must be before current time");
	m_ValueDate.Setup(this, IDC_FXENTRY_VALUEDATE_EDIT, FALSE);
	m_OptExp.Setup(this, IDC_FXENTRY_EXPIRATION_EDIT, FALSE, 
					"Option Expiration must be greater than or equal to Trade Date", &m_TradeDate);
	m_SWBooking.Setup(this, IDC_FXENTRY_SWBOOKING_EDIT, FALSE, 
						"Swap Booking Date must be greater than or equal to Value Date");
	m_DeliveryDate.Setup(this, IDC_FXENTRY_DELIVERY_EDIT, FALSE, 
							"Delivery Date must be greater than or equal to option expiration date", &m_OptExp);
	m_VAR.Setup(this, IDC_FXENTRY_VAR_EDIT, NULL, 2);
	m_PrevVAR.Setup(this, IDC_FXENTRY_PREV_VAR_EDIT, NULL, 2);
	m_CurrVAR.Setup(this, IDC_FXENTRY_CURR_VAR_EDIT, NULL, 2);
	m_VARLimit.Setup(this, IDC_FXENTRY_VAR_LIMIT_EDIT, NULL, 2);

	m_Amount.Setup(this, IDC_FXENTRY_AMOUNT_EDIT, NULL, 2);
	m_Margin.Setup(this, IDC_FXENTRY_MARGIN_EDIT);
	m_MarginAmount.Setup(this, IDC_FXENTRY_MARGIN_AMOUNT_EDIT, NULL, 2);
	m_Strike.Setup(this, IDC_FXENTRY_STRIKE_EDIT);
	m_OptTicker.Setup(this, IDC_FXENTRY_OPT_TICKER_EDIT);
	m_OptTicker.LimitText(25);
    m_Dir.Setup(this, IDC_FXENTRY_TRANSDIR_COMBO);
	m_Trader.Setup(this, IDC_FXENTRY_TRADER_COMBO);
	m_AAMethod.Setup(this, IDC_FXENTRY_AA_COMBO, 6, TRUE);
	m_OptSet.Setup(this, IDC_FXENTRY_OPTSET_COMBO, 3, TRUE);
	m_OptSet2.Setup(this, IDC_FXENTRY_OPTSET2_COMBO, 3, TRUE);
	m_OptID.Setup(this, IDC_FXENTRY_BLMID_EDIT);
	m_OptID.LimitText(25);
	m_FxRate.Setup(this, IDC_FXENTRY_FXRATE_EDIT);
	m_Note.Setup(this, IDC_FXENTRY_COMMENT_EDIT, EMPTYSTRING, NOCASE);
	m_Note.LimitText(30);
	m_Note2.Setup(this, IDC_FXENTRY_COMMENT2_EDIT, EMPTYSTRING, NOCASE);
	m_Note2.LimitText(30);
	m_Asset.Setup(this, IDC_FXENTRY_ASSETCODE_COMBO, 14, TRUE);
	m_AssetDesc.Setup(this, IDC_FXENTRY_ASSETDESC_EDIT);
	m_AssetCurrency.Setup(this, IDC_FXENTRY_ASSETCURRENCY_EDIT);
	m_VARStatus.Setup(this, IDC_FXENTRY_VAR_STATUS_EDIT);
	m_Ratio.Setup(this, IDC_FXENTRY_RATIO_EDIT);
	m_Binary.Setup(this, IDC_FXENTRY_BINARY_EDIT);
	m_Binary.LimitText(30);

	m_Price.Setup(this, IDC_FXENTRY_PRICE_EDIT);
	m_DV01.Setup(this, IDC_FXENTRY_DV01_EDIT);
	m_AssignCP = new COptComboBox(this, IDC_FXENTRY_ASSIGNCP_COMBO, TRUE);

	CRawTicketRec *pTicket;
	pTicket = &m_Data.GetRawTicket();
	m_Data.Add(&m_Ticket, &pTicket->GetTicket());
	m_Data.Add(&m_Trader, &pTicket->GetTrader());
	m_Data.Add(&m_TradeDate, &pTicket->GetTradeDate());
	m_Data.Add(&m_ExecTime, &pTicket->GetExecTime());
	m_Data.Add(m_TransType, &pTicket->GetTransType());
	m_Data.Add(&m_Dir, &pTicket->GetDir());
	m_Data.Add(&m_ValueDate, &pTicket->GetValueDate());
	m_Data.Add(&m_Amount, &pTicket->GetNomAmount());
	m_Data.Add(&m_Strike, &pTicket->GetStrike());
	m_Data.Add(&m_OptExp, &pTicket->GetOptExp());
	m_Data.Add(m_Listed, &pTicket->GetListed());
	m_Data.Add(&m_OptTicker, &pTicket->GetOptTicker());
	m_Data.Add(&m_OptSet, &pTicket->GetOptSetCode());
	m_Data.Add(&m_OptSet2, &pTicket->GetOptSetCode2());
	m_Data.Add(&m_OptID, &pTicket->GetOptID());
	m_Data.Add(m_EuroOpt, &pTicket->GetEuroOpt());
	m_Data.Add(m_OptAuto, &pTicket->GetOptAuto());
	m_Data.Add(m_CP, &pTicket->GetCP());
	m_Data.Add(m_Currency, &pTicket->GetCurrency());
	m_Data.Add(&m_FxRate, &pTicket->GetFxRate());
	m_Data.Add(&pTicket->GetAsset());
	m_Data.Add(&m_AssetDesc, &pTicket->GetAssetDesc());
	m_Data.Add(&m_AssetCurrency, &pTicket->GetAssetCurr());
	m_Data.Add(&pTicket->GetIPO());
	m_Data.Add(&pTicket->GetTRS());
	m_Data.Add(&pTicket->GetSoftDollar());
	m_Data.Add(&pTicket->GetRepoCP());
	m_Data.Add(&pTicket->GetRate());
	m_Data.Add(&pTicket->GetRateBasis());
	m_Data.Add(&pTicket->GetMaturity());
	m_Data.Add(&m_Note, &pTicket->GetNote());
	m_Data.Add(&m_Note2, &pTicket->GetNote2());
	m_Data.Add(&m_SWBooking, &pTicket->GetSWBooking());
	m_Data.Add(&pTicket->GetSWMaturity());
	m_Data.Add(&pTicket->GetSWFloatRate());
	m_Data.Add(&pTicket->GetSWFloatBasis());
	m_Data.Add(&pTicket->GetSWFloatMaturity());
	m_Data.Add(&pTicket->GetWI());
	m_Data.Add(&m_SwapTicket, &pTicket->GetSwapTicket());
	m_Data.Add(&m_DeliveryDate, &pTicket->GetDeliveryDate());
	m_Data.Add(&m_Margin, &pTicket->GetMargin());
	m_Data.Add(m_MarginCurrency, &pTicket->GetMarginCurrency());
	m_Data.Add(&m_MarginAmount, &pTicket->GetMarginAmount());
	m_Data.Add(&pTicket->GetFunded());
	m_Data.Add(&m_Binary, &pTicket->GetBinary());
	m_Data.Add(&m_AAMethod, &pTicket->GetAAMethod());
	m_Data.Add(m_AAFReason, &pTicket->GetAAFReason());
	m_Data.Add(m_BestExecution, &pTicket->GetBestExecution());
	m_Data.Add(m_ShortSale, &pTicket->GetShortSale());
	m_Data.Add(m_CSPBShort, &pTicket->GetCSPBShort());
	m_Data.Add(m_ETrade, &pTicket->GetETrade());
	m_Data.Add(&m_Price, &pTicket->GetPrice());
	m_Data.Add(m_AssignCP, &pTicket->GetAssignCP());
	m_Data.Add(&pTicket->GetDownPymnt());
	m_Data.Add(&pTicket->GetBrokerFee());
	m_Data.Add(&pTicket->GetOtherFee());
	m_Data.Add(&pTicket->GetSoftDollar());
	m_Data.Add(m_AssignCP, &pTicket->GetAssignCP());
	m_Data.Add(&m_DV01, &pTicket->GetDV01());
	m_Data.Add(&m_VAR, &pTicket->GetVAR());
	m_Data.Add(&m_UnWindTicket, &pTicket->GetUnWindTicket());
	m_Data.Add(&pTicket->GetCancelTicket());
	m_Data.Add(&pTicket->GetCorrectTicket());
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
	m_Data.Add(&m_CPKey);

	m_AllocAmount.Setup(this, IDC_FXENTRY_ALLOC_NOMAMOUNT_EDIT);
	m_Remain.Setup(this, IDC_FXENTRY_REMAIN_EDIT);

	CRawInvRec *pInv;
	pInv = &m_Data.GetRawInv();
	m_Data.GetSRowCtrl().Add(m_Portfolio, &pInv->GetPortfolio());
	m_Data.GetSRowCtrl().Add(&m_AllocAmount, &pInv->GetNomAmount());
}

BOOL CTicketFXEntry::IsOK()
{
	CString TransType, Text, Data;
	CQData QData;
	int i;

	m_TransType->GetSelString(TransType);
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

	if(m_TradeDate.GetData().IsEmpty())
		Text = "Invalid Trade Date.  Must enter Trade Date.";

	if(m_ExecTime.GetData().IsEmpty())
		Text = "Invalid Exection Time.  Must enter Exection Time.";

	m_Dir.GetSelString(Data);
	if(Data.IsEmpty())
		Text = "Invalid Transaction Direction.  Must enter Transaction direction.";

	double dNom;

	m_Amount.GetWindowText(Data);
	dNom = atof(QData.RemoveComma(Data));

	if(dNom <= 0)
		Text = "Invalid Nominal Amount.  Must enter Nominal Amount.";
	
	if(fabs(dNom - m_Data.ComputeAllocAmount()) > 0.00001)
		Text = "Nominal Amount and Sum of Allocated Nominal Amounts does not match";
	
	if(TransType == CALL || TransType == PUT)
	{
		if(fmod(dNom, m_dParValue) > 0)
			Text = "Invalid number of contracts (Fraction of a contract) ";
	}

	m_CP->GetSelString(Data);
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

	if(m_BestExecution->GetCurSel() < 0)
		Text = "You must choose Best Execution Method";

	if(m_AAMethod.GetCurSel() < 0)
	{
		if(m_AAFReason->GetCurSel() < 0)
			Text = "Must choose a reason for not Auto allocation";
	}
	else
		for(i = 1; i <= m_AllocSS.GetSheetRows() && m_AllocArray.GetSize(); i ++)
		{
			if(m_AllocArray.GetAt(i - 1)->GetAt(1) != m_AllocSS.GetSheetText(2, i) && m_AAFReason->GetCurSel() < 0)
			{
				Text = "Auto Allocation Method is violated, You must select AAF Reason";
				break;
			}
		}
		
	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}

void CTicketFXEntry::LoadAssetData(BOOL bFind)
{
	CRawTicketRec *pTicket;
	COraLoader OraLoader;
	CQData QData;
	CString Text;
	int i;

	OraLoader.SetDB(&theDB);
	pTicket = &m_Data.GetRawTicket();
	m_Asset.ResetContent();

	if(pTicket->GetAsset().IsEmpty())
	{
		m_AssetDesc.SetWindowText(EMPTYSTRING);
		m_AssetCurrency.SetWindowText(EMPTYSTRING);
		pTicket->SetAssetCountry(EMPTYSTRING);
		pTicket->SetAssetCommon(EMPTYSTRING);
		pTicket->SetAssetCusip(EMPTYSTRING);
		pTicket->SetAssetIsin(EMPTYSTRING);
		pTicket->SetAssetSedol(EMPTYSTRING);
		pTicket->SetAssetClass(EMPTYSTRING);
		pTicket->SetAssetIndustry(EMPTYSTRING);
		pTicket->SetAssetCategory(EMPTYSTRING);
		pTicket->SetAssetBucket(EMPTYSTRING);
		pTicket->SetAssetSource(EMPTYSTRING);
		pTicket->SetAssetCoupon(EMPTYSTRING);
		pTicket->SetAssetMaturity(EMPTYSTRING);
		return;
	}

	if(pTicket->GetAsset() != "NEW ASSET")
	{
		Text = QData.GetQueryText(pTicket->GetAsset());

		OraLoader.Open("SELECT ASS_CODE, ASS_DESC, ASS_CURRENCY, ASS_COUNTRY, "
						"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, "
						"ASS_SEDOL_NUM, ASS_INDUSTRY, ASS_IND_AREA, ASS_CATEGORY, "
						"BUCKET, SOURCE, ASS_MATURITY_DATE FROM SEMAM.NW_ASSETS "
						"WHERE ASS_CODE = " + Text);
		OraLoader.LoadMCComboBox(m_Asset);
	}
	else
	{
		if(bFind)
		{
			CStringArray AssetData;

			AssetData.Add(pTicket->GetAsset());
			AssetData.Add(pTicket->GetAssetDesc());
			AssetData.Add(pTicket->GetAssetCurr());
			AssetData.Add(pTicket->GetAssetCommon());
			AssetData.Add(pTicket->GetAssetCusip());
			AssetData.Add(pTicket->GetAssetIsin());
			AssetData.Add(pTicket->GetAssetSedol());
			AssetData.Add(pTicket->GetAssetClass());
			AssetData.Add(pTicket->GetAssetIndustry());
			AssetData.Add(pTicket->GetAssetCategory());
			AssetData.Add(pTicket->GetAssetBucket());
			AssetData.Add(pTicket->GetAssetSource());
			AssetData.Add(pTicket->GetAssetMaturity());

			m_Asset.AddString(AssetData);
		}
		else
			if(m_CPKey.IsEmpty() && m_SS.GetSheetCurRow() > 0)
			{
				m_CPKey = m_SS.GetSheetText(m_SS.GetSheetCols(), m_SS.GetSheetCurRow());
				if(m_CPKey.IsEmpty())
					return;

				Text = QData.GetQueryText(m_CPKey);
				OraLoader.Open("SELECT A.ASSET_CODE, NVL(A.ASSET, B.ASS_DESC) \"ASSET\", "
						"NVL(A.ASSET_CURRENCY, B.ASS_CURRENCY) \"ASSET_CURRENCY\", "
						"B.ASS_COUNTRY, B.ASS_COMMON_CODE, B.ASS_MSTC_CODE, "
						"B.ASS_ISIN_CODE, B.ASS_SEDOL_NUM, "
						"NVL(A.ASSET_CLASS, B.ASS_INDUSTRY) \"ASSET_CLASS\", "
						"B.ASS_IND_AREA, NVL(A.CATEGORY, B.ASS_CATEGORY) \"CATEGORY\", "
						"NVL(A.BUCKET, B.BUCKET) \"BUCKET\", B.SOURCE, "
						"NVL(A.ASSET_MATURITY, B.ASS_MATURITY_DATE) \"ASSET_MATURITY\" "
						"FROM SEMAM.NW_PRE_RAW_ASSETS A, SEMAM.NW_ASSETS B "
						"WHERE B.ASS_CODE(+) = A.ASSET_CODE AND A.CP_KEY = " + Text);
				OraLoader.LoadMCComboBox(m_Asset);
			}
	}
	
	if(m_Asset.GetCount() <= 0)
		return;

	m_Asset.SelectString(0, pTicket->GetAsset());
	for(i = 0; i < m_Asset.GetColumnCount(); i ++)
	{
		m_Asset.GetLBText(m_Asset.GetCurSel(), i, Text);
		switch(i)
		{
			case 0:
				break;
			case 1:
				m_AssetDesc.SetWindowText(Text);
				pTicket->SetAssetDesc(Text);
				break;
			case 2:
				m_AssetCurrency.SetWindowText(Text);
				pTicket->SetAssetCurr(Text);
				break;
			case 3:
				pTicket->SetAssetCountry(Text);
				break;
			case 4:
				pTicket->SetAssetCommon(Text);
				break;
			case 5:
				pTicket->SetAssetCusip(Text);
				break;
			case 6:
				pTicket->SetAssetIsin(Text);
				break;
			case 7:
				pTicket->SetAssetSedol(Text);
				break;
			case 8:
				pTicket->SetAssetClass(Text);
				break;
			case 9:
				pTicket->SetAssetIndustry(Text);
				break;
			case 10:
				pTicket->SetAssetCategory(Text);
				break;
			case 11:
				pTicket->SetAssetBucket(Text);
				break;
			case 12:
				pTicket->SetAssetSource(Text);
				break;
			case 13:
				pTicket->SetAssetMaturity(Text);
				break;
			default:
				break;
		}
	}
}

void CTicketFXEntry::UpdateAlloc(int Action)
{
	CString Port, Text;
	CQData QData;
	double NomAmount, AllocAmount;

	m_Amount.GetWindowText(Text);
	m_dNomAmount = atof(QData.RemoveComma(Text));

	switch(Action)
	{
		case 0: // Update After Load
			NomAmount = m_Data.ComputeAllocAmount();
//			m_SoftDAmount.SetWindowText(QData.WriteNumber(m_Data.GetSoftDollar()));
//			m_OtherFee.SetWindowText(QData.WriteNumber(m_Data.GetOtherFee()));
//			m_VAR.SetWindowText(QData.WriteNumber(m_Data.GetVAR()));
			break;
		case 1: // Add New 
		case 3: // Delete
			NomAmount = m_Data.ComputeAllocAmount();
			break;
		case 2: // Update
			m_Portfolio->GetSelString(Port);
			m_AllocAmount.GetWindowText(Text);
			AllocAmount = atof(QData.RemoveComma(Text));
			if(Port.IsEmpty())
				MessageBox("Cannot get portfolio");
//			NomAmount = m_Data.ComputeAllocAmount(Port, AllocAmount, BrokerFee, SoftDollar, OtherFee, VAR);
			NomAmount = m_Data.ComputeAllocAmount(Port, AllocAmount);
			break;
		default:
			break;
	}

	UpdateNumber(GetDlgItem(IDC_FXENTRY_TOTAL_ALLOC_EDIT), NomAmount);
	UpdateNumber(&m_Remain, m_dNomAmount - NomAmount);
}

BOOL CTicketFXEntry::UpdateData(BOOL bSaveandValid)
{
	if(bSaveandValid)
	{
		m_Try = 0;
		m_SS.SetFocus();
		m_Data.UpdateRow();
		UpdateETrade();
	}
	else
	{
		BeginWaitCursor();
		CQData QData;
		COraLoader OraLoader;

		OraLoader = m_Data.GetOraLoader();
		m_Data.LoadTickets();
		m_Data.LoadAllocList();

		if(!m_bLoaded)
		{
			GetData().LoadSupportData();
			GetData().GetPortfolioArr().CopyToComboBox(*m_Portfolio); /* Portfolio */
			QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_Currency, 0, FALSE);	/* Currency */
			GetData().GetCurrencyArr().CopyToComboBox(*m_MarginCurrency); /* Margin Currency */
			QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_CP, FALSE); /* Counterparty */
			QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_AssignCP); /* Giveup Counterparty */
			GetData().GetTransTypeArr().CopyToComboBox(*m_TransType); /* Trans Type */
			GetData().GetBestExecutionArr().CopyToComboBox(*m_BestExecution);
			
			GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);	/* TransDir */
			GetData().GetTraderArr().CopyToMCComboBox(m_Trader); /* Trader */
			GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet);
			GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet2);
			OraLoader.Open("SELECT AA_METHOD, AA_DESC, RATIO_A, RATIO_B, RATIO_C, RATIO_D FROM SEMAM.NW_AA_METHOD_V ORDER BY 1");
			m_AAMethod.ResetContent();
			OraLoader.LoadMCComboBox(m_AAMethod);

			OraLoader.Open("SELECT AA_REASON FROM SEMAM.NW_AA_REASON ORDER BY 1");
			m_AAFReason->ResetContent();
			OraLoader.LoadComboBox(*m_AAFReason);
			m_bLoaded = TRUE;
		}
		EndWaitCursor();
	}

	UpdateNumber(GetDlgItem(IDC_FXENTRY_TOTAL_EDIT), m_SS.GetSheetRows() >= 0 ? m_SS.GetSheetRows() : 0, TRUE);
	m_SS.SetSheetCurRow(-1);
	EnableAllocCtrls();

	Invalidate();
	UpdateWindow();
	OnEntryRefresh();
	return TRUE;
}

void CTicketFXEntry::UpdateETrade()
{
	COraLoader OraLoader;
	CQData QData;
	CString Text;

	OraLoader.SetDB(&theDB);

	if(m_CPKey.IsEmpty())
	{
		if(m_SS.GetSheetCurRow() <= 0)
			return;
		else
			m_CPKey = m_SS.GetSheetText(m_SS.GetSheetCols(), m_SS.GetSheetCurRow());
	}
	
	if(m_CPKey.IsEmpty())
		return;

	Text = QData.GetQueryText(m_CPKey);

	OraLoader.ExecuteSql("UPDATE SEMAM.NW_PRE_RAW_TICKETS SET PROCESSED = 'Y' "
						 "WHERE CP_KEY = " + Text);
}

void CTicketFXEntry::UpdateNumber(CWnd* Ctrl, double Number, BOOL WholeNumber)
{
	CQData QData;
	
	Ctrl->SetWindowText(QData.WriteNumber(Number, TRUE, WholeNumber ? 0 : -1));
}

void CTicketFXEntry::UpdateLink()
{
	CString TransType, Asset;

	m_TransType->GetSelString(TransType);
	if((TransType == CDS || TransType == SECURITIES || TransType == CALL || TransType == PUT) && 
		!m_UnWindTicket.GetData().IsEmpty())
	{	
		COraLoader OraLoader;
		CQData QData;
		CString Ticket;

		m_Asset.GetWindowText(Asset);
		Ticket = m_UnWindTicket.GetData();
		OraLoader = GetData().GetOraLoader();

		OraLoader.GetSql().Format("SELECT PORTFOLIO, TRANS_NUM "
							"FROM SEMAM.NW_POSITION_V WHERE ASSET_CODE = %s AND TICKET_NUM = '%s' ", 
							QData.GetQueryText(Asset), (LPCTSTR) Ticket);
		OraLoader.Open();		
	}
}

void CTicketFXEntry::UpdateRatio()
{
	CString TransType, Asset, Ticket;
	COraLoader OraLoader;
	CQData QData;

	m_Ratio.SetWindowText(":");
	m_TransType->GetSelString(TransType);
	OraLoader = GetData().GetOraLoader();
	m_Asset.GetWindowText(Asset);
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
/////////////////////////////////////////////////////////////////////////////
// CTicketFXEntry message handlers
void CTicketFXEntry::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BeginWaitCursor();

	InitControls();
	if(GetData().IsTrader())
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_AllocSS, GetData().GetTraderIni(), GetData().GetOraUser(), 0);
	else
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_AllocSS, EMPTYSTRING, GetData().GetOraUser(), 0);
	m_Data.GetFXEntry() = TRUE;

	GetData().LoadDates();
	GetDocument()->SetViewTitle("TICKET FX ENTRY");
	
	m_Data.SetBlock(GetData().GetBlock());
	m_Try = 0;

	UpdateData(FALSE);
	EndWaitCursor();
	
	OnEntryLoadTrade();
	EnableAlloc();
}

BEGIN_EVENTSINK_MAP(CTicketFXEntry, CFormView)
    //{{AFX_EVENTSINK_MAP(CTicketFXEntry)
	ON_EVENT(CTicketFXEntry, IDC_FXENTRY_TICKET_LIST, 2 /* BlockSelected */, OnBlockSelectedTicketList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTicketFXEntry, IDC_FXENTRY_TICKET_LIST, 11 /* DblClick */, OnDblClickTicketList, VTS_I4 VTS_I4)
	ON_EVENT(CTicketFXEntry, IDC_FXENTRY_ALLOCATION_LIST, 11 /* DblClick */, OnDblClickEntryAllocationList, VTS_I4 VTS_I4)
	ON_EVENT(CTicketFXEntry, IDC_FXENTRY_ALLOCATION_LIST, 2 /* BlockSelected */, OnBlockSelectedEntryAllocationList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTicketFXEntry::OnSelchangeTranstypeCombo() 
{
	OnSelchangeTransdirCombo();
	OnListedCheck();

	CString Date, TransType;
	
	m_TransType->GetSelString(TransType);
	m_TradeDate.SetData(GetData().GetOraLoader().Today(Date));

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, TransType, m_Listed->GetCheck()));
}

void CTicketFXEntry::OnSelchangeTransdirCombo() 
{
	CString TransType, Dir;
	
	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	GetData().DefineTransType(TransType, Dir);
	if(TransType.IsEmpty())
		return;

	if(Dir == P)
		m_ShortSale->SetWindowText("CShort");
	else
		if(Dir == S)
			m_ShortSale->SetWindowText("SShort");

	EnableCtrls();
}

void CTicketFXEntry::OnSelchangeCurrencyCombo() 
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

void CTicketFXEntry::OnEntryFindAsset() 
{	
	CAssetDlg Dlg;
	CString TransType, Dir, Text;
	BOOL bChange = FALSE;
	CQData QData;

	TransType = m_TransType->GetSelString(TransType);
	
	m_Try = 1;

	Dlg.m_pData = &GetData();
	m_Asset.GetWindowText(Dlg.m_FindData.GetRec().GetAsset());
	m_AssetDesc.GetWindowText(Dlg.m_FindData.GetRec().GetDesc());
	m_OptTicker.GetWindowText(Dlg.m_FindData.GetRec().GetOptTicker());
	Dlg.m_bTicket = TRUE;

	if(Dlg.DoModal() == IDOK)
	{
		CRawTicketRec *pTicket;

		pTicket = &m_Data.GetRawTicket();
		pTicket->FromAssetRec(Dlg.m_FindData.GetRec());
		LoadAssetData();

		m_Future = Dlg.m_Future;
		CString Currency, Fxrate;

		m_Currency->GetSelString(Currency);

		if(Currency.IsEmpty() || 
			(Currency != pTicket->GetAssetCurr() &&
			MessageBox("Note: Trading Currency is Different from Asset Currency! Do you want to synchonize them ?", 
						"Warning", MB_YESNO) == IDYES))
		{
			m_Currency->SelectString(-1, pTicket->GetAssetCurr());
		}

		m_Currency->GetSelString(Currency);
		m_Trader.SelectString(0, Dlg.m_FindData.GetTrader());
		if(m_FxRate.GetWindowTextLength() == 0)
			m_FxRate.SetWindowText(Currency == USD ? "1" : EMPTYSTRING);
		
		m_MarginCurrency->SelectString(-1, Currency);
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
		
		if(Dlg.m_nActionID == FINDACTION_UNWIND || Dlg.m_nActionID == FINDACTION_OPTEX)
		{
			m_Amount.GetWindowText(Text);
			if(Dlg.m_FindData.GetNominal() != Text && m_Amount.GetWindowTextLength() > 0)
			{
				if(MessageBox("Do you want to change current nom_amount (Y/N)?", "Ticket Entry", MB_YESNO) == IDYES)
					bChange = TRUE;
			}
		}
		
		m_dParValue = atoi(QData.RemoveComma(Dlg.m_FindData.GetRec().GetPar()));

		TransType = Dlg.m_FindData.GetTransType();
		Dir = Dlg.m_FindData.GetDir();

		switch(Dlg.m_nActionID)
		{
			case FINDACTION_ADD:
				if(TransType == CDS || TransType == SECURITIES || 
					TransType == CALL || TransType == PUT)
				{
					m_Dir.SelectString(0, Dir);
					m_TransType->SelectString(0, TransType);
					m_AssignCP->SelectString(0, Dlg.m_FindData.GetAssignCP());
				}

				if(TransType == CALL ||	TransType == PUT)
				{
					m_Strike.SetWindowText(Dlg.m_FindData.GetStrike());
					m_OptExp.SetData(Dlg.m_FindData.GetRec().GetMaturity());
					m_OptTicker.SetWindowText(Dlg.m_FindData.GetOptTick());
					m_OptID.SetWindowText(Dlg.m_FindData.GetOptID());
					m_DeliveryDate.SetWindowText(Dlg.m_FindData.GetDeliveryDate());
					m_OptSet.SelectString(0, Dlg.m_FindData.GetOptSetCode());
				}
				else
					if(TransType == SECURITIES && 
						Dlg.m_FindData.GetRec().GetClass() == "CURRENCY FWDS")
						m_ValueDate.SetData(Dlg.m_FindData.GetRec().GetMaturity());
				break;
			case FINDACTION_UNWIND:
				if(TransType == CDS || TransType == SECURITIES || 
					TransType == CALL || TransType == PUT)
				{
					m_Dir.SelectString(0, Dir == P ? S : P);
					if(bChange)
						m_Amount.SetWindowText(Dlg.m_FindData.GetNominal());
					m_TransType->SelectString(0, TransType);
					m_AssignCP->SelectString(0, Dlg.m_FindData.GetAssignCP());
				}
					
				if(TransType == CALL || TransType == PUT)
				{
					m_Strike.SetWindowText(Dlg.m_FindData.GetStrike());
					m_OptExp.SetWindowText(Dlg.m_FindData.GetRec().GetMaturity());
					m_OptTicker.SetWindowText(Dlg.m_FindData.GetOptTick());
					m_OptID.SetWindowText(Dlg.m_FindData.GetOptID());
					m_DeliveryDate.SetWindowText(Dlg.m_FindData.GetDeliveryDate());
					m_OptSet.SelectString(0, Dlg.m_FindData.GetOptSetCode());
					m_UnWindTicket.SetData(Dlg.m_FindData.GetTicket());
				}
				else
					if(TransType == SECURITIES && 
						Dlg.m_FindData.GetRec().GetClass() == "CURRENCY FWDS")
						m_ValueDate.SetData(Dlg.m_FindData.GetRec().GetMaturity());
				break;
			case FINDACTION_OPTEX:
				if(TransType == CALL || TransType == PUT)
				{
					if(Dir == P) // We bought options
					{
						if(TransType == CALL)
							m_Dir.SelectString(0, P);
						else
							if(TransType == PUT)
								m_Dir.SelectString(0, S);
					}
					else
					{
						if(TransType == CALL)
							m_Dir.SelectString(0, S);
						else
							if(TransType == PUT)
								m_Dir.SelectString(0, P);
					}
					
					if(bChange)
						m_Amount.SetWindowText(Dlg.m_FindData.GetNominal());
					m_TransType->SelectString(0, SECURITIES);
					m_Price.SetWindowText(Dlg.m_FindData.GetStrike());
					m_UnWindTicket.SetData(Dlg.m_FindData.GetTicket());
					m_AssignCP->SelectString(0, Dlg.m_FindData.GetAssignCP());
				}
				break;
			default:
				break;
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

void CTicketFXEntry::OnBlockSelectedTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickTicketList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CTicketFXEntry::OnDblClickTicketList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row, m_Data.GetAllocList());
		UpdateAlloc(0);
		EnableCtrls();
	}
	else
	{
		OnEntryRefresh();
		m_dNomAmount = 0;
	}
	LoadAssetData();
}

void CTicketFXEntry::OnEntryAdd() 
{	
   	if(!IsOK())
		return;

	CString Ticket;

	m_SS.SetFocus();
	GetData().GetOraLoader().GenerateUniqueID(TICKETNUM, Ticket);
	m_Ticket.SetData(Ticket);
	m_Data.SetKey(EMPTYSTRING);

	UpdateData(TRUE);
}

void CTicketFXEntry::OnEntryDelete() 
{
	if(MessageBox("Are you sure you want to delete these tickets ?",
	  			  "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	m_Try = 0;
	m_Data.DeleteRow();
}

void CTicketFXEntry::OnEntryUpdate() 
{
	if(!IsOK())
		return;

	m_SS.SetFocus();
	UpdateData(TRUE);
}

void CTicketFXEntry::OnEntryRefresh() 
{
	BeginWaitCursor();
	
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	m_Asset.SetCurSel(-1);
	m_TradeDate.SetData(GetData().GetDate());
	m_ValueDate.SetData(GetData().GetPlus3Date());
	m_TransType->SelectString(-1, SECURITIES);
	m_FxRate.SetWindowText("1");
	m_Currency->SetCurSel(m_Currency->FindString(-1, USD));
	m_AllocAmount.SetWindowText(EMPTYSTRING);
	m_Remain.SetWindowText(EMPTYSTRING);

	m_SS.SheetAutoFit();

	SetDlgItemText(IDC_FXENTRY_TOTAL_ALLOC_EDIT, EMPTYSTRING);
	
	m_nCurrID = 0;
	EnableCtrls();
	EndWaitCursor();
}

void CTicketFXEntry::OnUpdateEntryAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter() && m_SS.GetSheetCurRow() <= 0 && 
					m_AllocSS.GetSheetRows() > 0 ? TRUE : FALSE);	
}

void CTicketFXEntry::OnUpdateEntryDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter() && 
					m_SS.GetIsBlockSelected() && m_SS.GetSheetCurRow() >= 1);
}

void CTicketFXEntry::OnUpdateEntryFindAsset(CCmdUI* pCmdUI) 
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	pCmdUI->Enable(m_Data.GetBlotter() || TransType == CASH || TransType == INTSWAP ? FALSE : TRUE);
}

void CTicketFXEntry::OnUpdateEntryUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Data.GetBlotter() && m_SS.GetSheetCurRow() >= 1 ? TRUE : FALSE);	
}

void CTicketFXEntry::OnUpdateEntryCopyTicket(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SS.GetSheetCurRow() >= 1 ? TRUE : FALSE);
}

void CTicketFXEntry::OnListedCheck() 
{
	CString Date, TransType;

	Date = m_TradeDate.GetData();
	if(Date != GetData().GetDate()) // Automatic change dates only when entry date is today
		return;

	m_TransType->GetSelString(TransType);

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, TransType, m_Listed->GetCheck()));
}

void CTicketFXEntry::OnSelChangeAssetCombo() 
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	if((TransType == FOREX && m_Try) || m_Asset.GetWindowTextLength() > 0)
		return;

	m_Asset.SetWindowText(NEWASSET);	
	OnEntryFindAsset();	
}

void CTicketFXEntry::OnEntryCopyTicket() 
{
   	if(!IsOK())
		return;

	CString Ticket;

	m_SS.SetFocus();
	GetData().GetOraLoader().GenerateUniqueID(TICKETNUM, Ticket);
	m_Ticket.SetData(Ticket);
	m_Data.SetKey(EMPTYSTRING);
	m_Price.SetWindowTextA(EMPTYSTRING);
	m_Amount.SetWindowTextA("0");
	m_BestExecution->SetCurSel(-1);

	UpdateData(TRUE);
}

void CTicketFXEntry::OnUpdateEntryAddAllocation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocAmount.GetWindowTextLength() > 0 ? TRUE : FALSE);
}

void CTicketFXEntry::OnEntryAddAllocation() 
{
	if(m_bAllocValid && !AllocIsOK())
		return;

	m_Data.GetSRowCtrl().AddSheetRow();
	UpdateAlloc(1);
}

void CTicketFXEntry::OnEntryUpdateAllocation() 
{
	if(!AllocIsOK())
		return;

	m_Data.GetSRowCtrl().UpdateSheetRow(FALSE);
	UpdateAlloc(2);
}

void CTicketFXEntry::OnUpdateEntryUpdateAllocation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocSS.GetSheetCurRow() >= 1);
}

void CTicketFXEntry::OnEntryDeleteAllocation() 
{
	if(MessageBox("Are you sure you want to delete these allocations ?", "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	
	m_Data.GetSRowCtrl().DeleteSheetRow(m_Data.GetSRowCtrl().GetCurrentRow());

	UpdateAlloc(3);
}

void CTicketFXEntry::OnUpdateEntryDeleteAllocation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocSS.GetIsBlockSelected() && m_AllocSS.GetSheetCurRow() >= 1);
}

void CTicketFXEntry::OnDblClickEntryAllocationList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_AllocSS.GetSheetRows())
	{
		m_Data.GetSRowCtrl().SetCurrentRow(Row);
		
		CString Portfolio, Text, AllocNom;
		CStringArray Rec;
		double VAR;
		CQData QData;

		m_Portfolio->GetSelString(Portfolio);
		m_VAR.GetWindowText(Text);
		m_AllocAmount.GetWindowText(AllocNom);
		VAR = atof(QData.RemoveComma(m_Data.GetRawTicket().Alloc(Text, Text, AllocNom)));
		if(m_Data.LoadVARs(Rec, Portfolio, m_Data.GetRawTicket().GetAssetCategory(), VAR) > 0)
		{
			m_PrevVAR.SetWindowText(Rec.GetAt(0));
			m_CurrVAR.SetWindowText(Rec.GetAt(1));
			m_VARLimit.SetWindowText(Rec.GetAt(2));
			m_VARStatus.SetWindowText(Rec.GetAt(3));
		}
		else
		{
			m_PrevVAR.SetWindowText(EMPTYSTRING);
			m_CurrVAR.SetWindowText(EMPTYSTRING);
			m_VARLimit.SetWindowText(EMPTYSTRING);
			m_VARStatus.SetWindowText(EMPTYSTRING);
		}
	}
	EnableAllocCtrls();
}

void CTicketFXEntry::OnBlockSelectedEntryAllocationList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickEntryAllocationList(m_AllocSS.GetActiveCol(), m_AllocSS.GetActiveRow());
}

void CTicketFXEntry::OnKillfocusAllocNomamountEdit() 
{
	CString Port, Text;
	CQData QData;

	m_Portfolio->GetSelString(Port);
	m_AllocAmount.GetWindowText(Text);
	if(Port.IsEmpty() || Text.IsEmpty())
		return;

	if(m_Data.AllocCheckSum(Port, atof(QData.RemoveComma(Text))) > 0)
	{
		MessageBox("Sum of the shares in each portfolios exceeds the Nominal Amount");
		m_AllocAmount.SetFocus();
	}
}

void CTicketFXEntry::OnSelchangeEntryPortfolioCombo() 
{
	CString Port;
	
	m_Portfolio->GetSelString(Port);
	if(Port.IsEmpty())
		return;

	if(m_Data.FindPortfolio(Port))
	{
		MessageBox("Duplicated portfolio selection");
		m_Portfolio->SetCurSel(-1);
	}

	EnableAllocCtrls();
}

void CTicketFXEntry::OnEntryLoadBlotter() 
{
	m_Data.SetBlotter(TRUE);
	UpdateData(FALSE);
}

void CTicketFXEntry::OnEntryLoadTrade() 
{
	m_Data.SetBlotter(FALSE);
	UpdateData(FALSE);
}

void CTicketFXEntry::OnEntryOptionExp()
{
	COptExpDlg Dlg;

	Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
	if(Dlg.DoModal() == IDOK)
		m_OptExp.SetData(Dlg.m_Date);
}

void CTicketFXEntry::OnUpdateEntryOptionExp(CCmdUI* pCmdUI)
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	pCmdUI->Enable(TransType == PUT || TransType == CALL ? TRUE : FALSE);
}

/*
void CTicketFXEntry::OnSetfocusAmountEdit()
{
	CString TransType;

	m_TransType->GetSelString(TransType);

	m_nNomID = IDC_FXENTRY_AMOUNT_EDIT;

	if((TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F")) && 
		m_Amount.GetWindowTextLength() == 0)
	{
		if(m_Amount.GetWindowTextLength() > 0)
			m_Amount.SetFocus();
	}
} */

void CTicketFXEntry::OnSetfocusAllocNomamountEdit()
{
	CString TransType;

	m_TransType->GetSelString(TransType);

	m_nNomID = IDC_FXENTRY_ALLOC_NOMAMOUNT_EDIT;

	if((TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F")) && 
		m_AllocAmount.GetWindowTextLength() == 0)
	{
		if(m_AllocAmount.GetWindowTextLength() > 0)
			m_AllocAmount.SetFocus();
	}
}

void CTicketFXEntry::OnKillfocusAmountEdit() 
{
	if(m_Amount.GetWindowTextLength() > 0 && 
		m_Data.GetRawTicket().GetAssetCategory() == "G20 FX ALPHA")
	{
		m_AAMethod.SelectString(0, "EMCF_B");
		m_Trader.SelectString(0, "IM");
		OnCbnSelchangeEntryAaCombo();
	}

	UpdateAlloc(0);
}

void CTicketFXEntry::OnEnChangeEntryAmountEdit()
{
	
	if(m_AllocSS.GetSheetRows() > 0 && 	
		m_Data.GetRawTicket().GetAssetCategory() != "G20 FX ALPHA" &&
		MessageBox("Trade has been allocated, do you want to reallocate?", 
					"TICKET ENTRY", MB_YESNO) == IDYES)
	{
		m_AAMethod.SetCurSel(-1);
		m_Portfolio->SetCurSel(-1);
		m_AllocAmount.SetWindowText(EMPTYSTRING);
		m_AAFReason->SetCurSel(-1);
		m_AllocSS.ClearSheet();
		m_AllocSS.SetSheetRows(0);
	}
}

void CTicketFXEntry::OnEntryXcurrency() 
{
	CXCurrencyDlg Dlg;

	Dlg.m_pData = &GetData();
	Dlg.m_UserClass = 1;
	Dlg.DoModal();
}
/*
void CTicketFXEntry::OnChangeAssetEdit()
{
	CString Asset, TransType;
	CRawTicketRec *pTicket;

	Asset = m_Asset.GetData();
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
} */

void CTicketFXEntry::OnCbnSelchangeEntryAaCombo()
{
	CDBRec DBRec;
	COraLoader OraLoader;
	CString sMethod, sAmount, Ticket;

	m_Amount.GetWindowText(sAmount);
	m_AAMethod.GetSelString(sMethod);

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
			m_Portfolio->SelectString(0, OraLoader.GetDBString(0));
			DBRec.Add(OraLoader.GetDBString(0));
			m_AllocAmount.SetWindowText(OraLoader.GetDBString(1));
			DBRec.Add(OraLoader.GetDBString(1));
//			m_Link.SetData(OraLoader.GetDBString(2));
			m_bAllocValid = FALSE;
			m_AllocArray.Add(DBRec);
			OnEntryAddAllocation();
			OraLoader.MoveNext();
		}
	}
}

void CTicketFXEntry::OnEnSetfocusEntryBinaryEdit()
{
	CString TransType;
	
	m_TransType->GetSelString(TransType);
	if((TransType == CALL || TransType == PUT))
		m_nCurrID = IDC_FXENTRY_BINARY_EDIT;
}

void CTicketFXEntry::OnEntryFinditem()
{
	int CurrID;
	CFindDlg FindDlg;

	CurrID = m_nCurrID;
	m_nCurrID = 0;

	FindDlg.m_OraLoader = m_Data.GetOraLoader();
	
	switch(CurrID)
	{
		case IDC_FXENTRY_BINARY_EDIT:
			FindDlg.m_OraLoader.Open("SELECT OPT_TYPE, OPT_TYPE_DESC FROM SEMAM.NW_EXOTIC_OPTTYPE ORDER BY 1 ");
		
			if(FindDlg.DoModal() == IDOK)
			{
				CString Text;

				m_Binary.GetWindowText(Text);
				Text.MakeUpper();
				Text = Text.SpanExcluding("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				Text = FindDlg.m_Key + Text;
				m_Binary.SetWindowText(Text);
			}
		default:
			return;
	}
}

void CTicketFXEntry::OnEntryForbiddenList()
{
	CProhibitList Dlg;

	Dlg.m_OraLoader = GetData().GetOraLoader();
	Dlg.DoModal();
}

void CTicketFXEntry::OnCbnSelchangeFxentryAssetcodeCombo()
{
	if(m_Asset.GetCurSel() >= 0)
	{
		m_Asset.GetLBText(m_Asset.GetCurSel(), m_Data.GetRawTicket().GetAsset());
		LoadAssetData();
	}
}
