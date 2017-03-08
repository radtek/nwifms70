// TicketProcess.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "TicketProcess.h"
#include "docdata.h"
#include "assetdlg.h"
#include "allocation.h"
#include "finddlg.h"
#include "qdata.h"
#include "ContractConvertor.h"
#include "optexpdlg.h"
#include "xcurrencydlg.h"
#include "optsettlementdlg.h"
#include "setconventiondlg.h"
#include "fxsource.h"
#include "prohibitlist.h"
#include "odbcdb.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTicketProcess

IMPLEMENT_DYNCREATE(CTicketProcess, CFormView)

CTicketProcess::CTicketProcess()
	: CFormView(CTicketProcess::IDD)
{
	//{{AFX_DATA_INIT(CTicketProcess)
	m_nCurrID = 0;

	m_Listed = NULL;
	m_EuroOpt = NULL;
	m_OptAuto = NULL;
	m_TRS = NULL;
	m_IPO = NULL;
	m_DocReq = NULL;
	m_DocRecvd = NULL;
	m_WI = NULL;
	m_Confirm = NULL;
	m_Funded = NULL;
	m_ShortSale = NULL;
	m_CSPBShort = NULL;
	m_SecFee = NULL;
	m_OrFee = NULL;
	m_ETrade = NULL;

    m_TransType = NULL;
	m_CP = NULL;
	m_RepoCP = NULL;
	m_AssignCP = NULL;
    m_Currency = NULL;
	m_RepoType = NULL;
	m_Formula = NULL;
    m_RateBasis = NULL;
    m_PFU = NULL;
    m_USDAcc = NULL;
    m_FxAcc = NULL;
	m_MarginCurrency = NULL;
	m_BestExecution = NULL;
	//}}AFX_DATA_INIT
}

CTicketProcess::~CTicketProcess()
{
	if(m_Listed)
		delete m_Listed;
	if(m_EuroOpt)
		delete m_EuroOpt;
	if(m_OptAuto)
		delete m_OptAuto;
	if(m_TRS)
		delete m_TRS;
	if(m_IPO)
		delete m_IPO;
	if(m_DocReq)
		delete m_DocReq;
	if(m_DocRecvd)
		delete m_DocRecvd;
	if(m_WI)
		delete m_WI;
	if(m_Confirm)
		delete m_Confirm;
	if(m_Funded)
		delete m_Funded;
	if(m_ShortSale)
		delete m_ShortSale;
	if(m_CSPBShort)
		delete m_CSPBShort;
	if(m_SecFee)
		delete m_SecFee;
	if(m_OrFee)
		delete m_OrFee;
	if(m_ETrade)
		delete m_ETrade;

    if(m_TransType)
		delete m_TransType;
	if(m_CP)
		delete m_CP;
	if(m_RepoCP)
		delete m_RepoCP;
	if(m_AssignCP)
		delete m_AssignCP;
    if(m_Currency)
		delete m_Currency;
	if(m_RepoType)
		delete m_RepoType;
    if(m_RateBasis)
		delete m_RateBasis;
	if(m_Formula)
		delete m_Formula;
    if(m_PFU)
		delete m_PFU;
    if(m_USDAcc)
		delete m_USDAcc;
    if(m_FxAcc)
		delete m_FxAcc;
	if(m_MarginCurrency)
		delete m_MarginCurrency;
	if(m_BestExecution)
		delete m_BestExecution;
}

void CTicketProcess::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTicketProcess)
	DDX_Control(pDX, IDC_PROCESS_TICKET_LIST, m_SS);
	DDX_Control(pDX, IDC_PROCESS_ALLOCATION_LIST, m_AllocSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTicketProcess, CFormView)
	//{{AFX_MSG_MAP(CTicketProcess)
	ON_COMMAND(ID_PROCESS_LOAD, OnProcessLoad)
	ON_COMMAND(ID_PROCESS_ADD, OnProcessAdd)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_ADD, OnUpdateProcessAdd)
	ON_COMMAND(ID_PROCESS_ALLOCATION, OnProcessAllocation)
	ON_COMMAND(ID_PROCESS_DELETE, OnProcessDelete)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_DELETE, OnUpdateProcessDelete)
	ON_COMMAND(ID_PROCESS_FIND_ASSET, OnProcessFindAsset)
	ON_COMMAND(ID_PROCESS_REFRESH, OnProcessRefresh)
	ON_COMMAND(ID_PROCESS_UPDATE, OnProcessUpdate)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_UPDATE, OnUpdateProcessUpdate)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FIND_ASSET, OnUpdateProcessFindAsset)
	ON_EN_SETFOCUS(IDC_PROCESS_ASSET_EDIT, OnSetfocusProcessAssetEdit)
	ON_CBN_SELCHANGE(IDC_PROCESS_TRANSTYPE_COMBO, OnSelchangeProcessTranstypeCombo)
	ON_CBN_SELCHANGE(IDC_PROCESS_TRANSDIR_COMBO, OnSelchangeProcessTransdirCombo)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_ALLOCATION, OnUpdateProcessAllocation)
	ON_CBN_SELCHANGE(IDC_PROCESS_REPOCP_COMBO, OnSelchangeProcessRepocpCombo)
	ON_COMMAND(ID_PROCESS_FIND_ITEM, OnProcessFindItem)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_FIND_ITEM, OnUpdateProcessFindItem)
	ON_BN_CLICKED(IDC_PROCESS_LISTED_CHECK, OnProcessListedCheck)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_OPTION_EXP, OnUpdateProcessOptionExp)
	ON_COMMAND(ID_PROCESS_OPTION_EXP, OnProcessOptionExp)
	ON_UPDATE_COMMAND_UI(ID_PROCESS_CONTRACT_CONVERTOR, OnUpdateProcessContractConvertor)
	ON_COMMAND(ID_PROCESS_CONTRACT_CONVERTOR, OnProcessContractConvertor)
	ON_CBN_SELCHANGE(IDC_PROCESS_CURRENCY_COMBO, OnSelchangeProcessCurrencyCombo)
	ON_COMMAND(ID_PROCESS_XCURRENCY, OnProcessXcurrency)
	ON_EN_SETFOCUS(IDC_PROCESS_AMOUNT_EDIT, OnSetfocusAmountEdit)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PROCESS_FXREFLINK, &CTicketProcess::OnProcessFxreflink)
	ON_BN_CLICKED(IDC_PROCESS_SECFEE_CHECK, &CTicketProcess::OnBnClickedSecCheck)
	ON_EN_CHANGE(IDC_PROCESS_ASSET_EDIT, &CTicketProcess::OnEnChangeProcessAssetEdit)
	ON_EN_KILLFOCUS(IDC_PROCESS_PRICE_EDIT, &CTicketProcess::OnEnKillfocusProcessPriceEdit)
	ON_EN_KILLFOCUS(IDC_PROCESS_FXRATE_EDIT, &CTicketProcess::OnEnKillfocusProcessFxrateEdit)
	ON_EN_KILLFOCUS(IDC_PROCESS_BROKER_FEE_EDIT, &CTicketProcess::OnEnKillfocusProcessBrokerFeeEdit)
	ON_EN_KILLFOCUS(IDC_PROCESS_OTHER_FEE_EDIT, &CTicketProcess::OnEnKillfocusProcessOtherFeeEdit)
	ON_BN_CLICKED(IDC_PROCESS_SECFEE_CHECK, &CTicketProcess::OnBnClickedProcessSecfeeCheck)
	ON_BN_CLICKED(IDC_PROCESS_ORFEE_CHECK, &CTicketProcess::OnBnClickedProcessOrfeeCheck)
	ON_EN_CHANGE(IDC_PROCESS_UNWIND_TICKET_EDIT, &CTicketProcess::OnEnChangeProcessUnwindTicketEdit)
	ON_EN_SETFOCUS(IDC_PROCESS_BINARY_EDIT, &CTicketProcess::OnEnSetfocusProcessBinaryEdit)
	ON_EN_SETFOCUS(IDC_PROCESS_SETCONVENTION_EDIT, &CTicketProcess::OnEnSetfocusProcessSetconventionEdit)
	ON_EN_SETFOCUS(IDC_PROCESS_CT_EDIT, &CTicketProcess::OnEnSetfocusProcessCtEdit)
	ON_EN_SETFOCUS(IDC_PROCESS_REPOCT_EDIT, &CTicketProcess::OnEnSetfocusProcessRepoctEdit)
	ON_EN_SETFOCUS(IDC_PROCESS_ASSIGNCT_EDIT, &CTicketProcess::OnEnSetfocusProcessAssignctEdit)
	ON_COMMAND(ID_PROCESS_FORBIDDEN_LIST, OnProcessForbiddenList)
	ON_COMMAND(ID_PROCESS_AUTOPROCESSTRADE, OnProcessAutoProcessTrade)
	ON_CBN_SELCHANGE(IDC_PROCESS_AA_COMBO, &CTicketProcess::OnCbnSelchangeProcessAaCombo)
	ON_EN_CHANGE(IDC_PROCESS_AMOUNT_EDIT, &CTicketProcess::OnEnChangeProcessAmountEdit)
	ON_EN_KILLFOCUS(IDC_PROCESS_AMOUNT_EDIT, &CTicketProcess::OnEnKillfocusProcessAmountEdit)
	ON_EN_CHANGE(IDC_PROCESS_PRICE_EDIT, &CTicketProcess::OnEnChangeProcessPriceEdit)
	ON_EN_KILLFOCUS(IDC_PROCESS_ASSET_EDIT, &CTicketProcess::OnEnKillfocusProcessAssetEdit)
	ON_CBN_KILLFOCUS(IDC_PROCESS_AA_COMBO, &CTicketProcess::OnCbnKillfocusProcessAaCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTicketProcess diagnostics

#ifdef _DEBUG
void CTicketProcess::AssertValid() const
{
	CFormView::AssertValid();
}

void CTicketProcess::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CTicketProcess::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

BOOL CTicketProcess::AutoProcessTrade(const CString Ticket)
{
	CQData QData;
	COraLoader OraLoader;

	OraLoader = GetData().GetOraLoader();
	
	// Verify whether the has been processed
	OraLoader.GetSql() = "SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKETS "
						 "WHERE ASSIGN_CP IN ('CITI PB', 'DBPB', 'HSBCFX PB') "
						 "AND TICKET_NUM = " + Ticket + 
						 " AND SIGN IS NOT NULL AND PROCESSED IS NULL ";

	if(OraLoader.GetCount() < 1)
		return FALSE;

	if(!OraLoader.ExecuteSql("UPDATE SEMAM.NW_RAW_TICKET_DETAIL A "
						     "SET (PRICE, CUSTODIAN, ACCOUNT) = "
						          "(SELECT B.PRICE, B.ASSIGN_CP, TRIM(B.ASSIGN_CP) "
							      "FROM SEMAM.NW_RAW_TICKETS B "
						          "WHERE B.TICKET_NUM = A.TICKET_NUM) "
						     "WHERE A.TICKET_NUM = " + Ticket))
		return FALSE;

	if(!OraLoader.ExecuteSql("UPDATE SEMAM.NW_RAW_TICKETS SET VERIFIED = 'Y' "
						"WHERE TICKET_NUM = " + Ticket))
		return FALSE;

	return TRUE;
}

void CTicketProcess::ClearCtrls()
{
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	m_SS.ClearSheet();
	EnableCtrls();
}

void CTicketProcess::ChangeShortLabel()
{
	CString Dir;

	m_Dir.GetSelString(Dir);

	if(Dir == P)
		m_ShortSale->SetWindowText("CShort");
	else
		if(Dir == S)
			m_ShortSale->SetWindowText("SShort");
}

void CTicketProcess::EnableCtrls()
{
	CString TransType, Dir, RepoCP;
	CQData QData;

	GetDlgItem(IDC_PROCESS_ASSET_STATIC)->ShowWindow(SW_HIDE);
	m_Asset.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_ASSETDESC_STATIC)->ShowWindow(SW_HIDE);
	m_AssetDesc.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_ASSETCURRENCY_STATIC)->ShowWindow(SW_HIDE);
	m_AssetCurr.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_STRIKE_STATIC)->ShowWindow(SW_HIDE);
	m_Strike.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_EXERCISEDATE_STATIC)->ShowWindow(SW_HIDE);
	m_ExerciseDate.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_OPTEXP_STATIC)->ShowWindow(SW_HIDE);
	m_OptExpDate.ShowWindow(SW_HIDE);
	m_Listed->ShowWindow(SW_HIDE);
	m_EuroOpt->ShowWindow(SW_HIDE);
	m_Binary.ShowWindow(SW_HIDE);
	m_OptAuto->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_OPT_TICK_STATIC)->ShowWindow(SW_HIDE);
	m_OptTicker.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_OPTSET_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_OPTSET2_STATIC)->ShowWindow(SW_HIDE);
	m_OptSet2.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_SETCONVENTION_STATIC)->ShowWindow(SW_HIDE);
	m_SetConvention.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_OPTID_STATIC)->ShowWindow(SW_HIDE);
	m_OptID.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_SWAP_MATURITY_STATIC)->ShowWindow(SW_HIDE);
	m_SWMaturity.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_RATE_STATIC)->ShowWindow(SW_HIDE);
	m_Rate.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_RATEBASIS_STATIC)->ShowWindow(SW_HIDE);
	m_RateBasis->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_INDEX_STATIC)->ShowWindow(SW_HIDE);
	m_Index.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_FORMULA_STATIC)->ShowWindow(SW_HIDE);
	m_Formula->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_MATURITY_STATIC)->ShowWindow(SW_HIDE);
	m_Maturity.ShowWindow(SW_HIDE);
	m_TRS->ShowWindow(SW_HIDE);
	m_IPO->ShowWindow(SW_HIDE);
	m_SecFee->ShowWindow(SW_HIDE);
	m_OrFee->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_MARGIN_STATIC)->ShowWindow(SW_HIDE);
	m_Margin.ShowWindow(SW_HIDE);
	m_MarginCurrency->ShowWindow(SW_HIDE);
	m_MarginAmount.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_SWAP_TICKET_STATIC)->ShowWindow(SW_HIDE);
	m_SwapTicket.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_DELIVERY_STATIC)->ShowWindow(SW_HIDE);
	m_DeliveryDate.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_PROCESS_USDACC_STATIC)->ShowWindow(SW_HIDE);
	m_USDAcc->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROCESS_FXACC_STATIC)->ShowWindow(SW_HIDE);
	m_FxAcc->ShowWindow(SW_HIDE);

	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	TransType = GetData().DefineTransType(TransType, Dir);

	if(TransType == SECURITIES)
		m_IPO->ShowWindow(SW_SHOW);

	if(TransType != FOREX)
	{
		GetDlgItem(IDC_PROCESS_MARGIN_STATIC)->ShowWindow(SW_SHOW);
		m_Margin.ShowWindow(SW_SHOW);
		m_MarginCurrency->ShowWindow(SW_SHOW);
		m_MarginAmount.ShowWindow(SW_SHOW);
	}

	if(TransType == SECURITIES || TransType == CDS || TransType == CALL || TransType == PUT)
	{
		GetDlgItem(IDC_PROCESS_OPTSET_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_OPTSET2_STATIC)->ShowWindow(SW_SHOW);
		m_OptSet2.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_SETCONVENTION_STATIC)->ShowWindow(SW_SHOW);
		m_SetConvention.ShowWindow(SW_SHOW);

		if(TransType != CDS)
		{
			if(Dir == S)
				m_SecFee->ShowWindow(SW_SHOW);
		}

		if(TransType == SECURITIES && m_Data.GetRawTicket().GetAssetClass() == "CURRENCY FWDS")
		{
			GetDlgItem(IDC_PROCESS_SWAP_TICKET_STATIC)->ShowWindow(SW_SHOW);
			m_SwapTicket.ShowWindow(SW_SHOW);
		}
	}

	if(TransType == SECURITIES || TransType == CDS || TransType == REPO || TransType == LEVERAGE || 
		TransType == CALL || TransType == PUT || TransType == INTSWAP || TransType == FOREX)
	{
		GetDlgItem(IDC_PROCESS_ASSET_STATIC)->ShowWindow(SW_SHOW);
		m_Asset.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_ASSETDESC_STATIC)->ShowWindow(SW_SHOW);
		m_AssetDesc.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_ASSETCURRENCY_STATIC)->ShowWindow(SW_SHOW);
		m_AssetCurr.ShowWindow(SW_SHOW);
    }

    if(TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
	{
		GetDlgItem(IDC_PROCESS_EXERCISEDATE_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_OPTEXP_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_STRIKE_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_OPT_TICK_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_OPTID_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_DELIVERY_STATIC)->ShowWindow(SW_SHOW);
		m_Strike.ShowWindow(SW_SHOW);
		m_ExerciseDate.ShowWindow(SW_SHOW);
		m_OptExpDate.ShowWindow(SW_SHOW);
		m_OptTicker.ShowWindow(SW_SHOW);
		m_OptID.ShowWindow(SW_SHOW);
		m_Listed->ShowWindow(SW_SHOW);
		m_EuroOpt->ShowWindow(SW_SHOW);
		m_Binary.ShowWindow(SW_SHOW);
		m_OptAuto->ShowWindow(SW_SHOW);
		m_DeliveryDate.ShowWindow(SW_SHOW);
		if(m_Listed->GetCheck())
			m_OrFee->ShowWindow(SW_SHOW);
	}

	m_RepoCP->GetSelString(RepoCP);
	if(TransType == LEVERAGE || TransType == REPO || TransType == CASH || TransType == INTSWAP ||
		TransType == BORROW || TransType == LEND ||	(TransType == SECURITIES && !RepoCP.IsEmpty()))
	{
		GetDlgItem(IDC_PROCESS_RATE_STATIC)->ShowWindow(SW_SHOW);
		m_Rate.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_RATEBASIS_STATIC)->ShowWindow(SW_SHOW);
		m_RateBasis->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_INDEX_STATIC)->ShowWindow(SW_SHOW);
		m_Index.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_FORMULA_STATIC)->ShowWindow(SW_SHOW);
		m_Formula->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_MATURITY_STATIC)->ShowWindow(SW_SHOW);
		m_Maturity.ShowWindow(SW_SHOW);
	}

	if(TransType == REPO || TransType == LEVERAGE || TransType == SECURITIES)
		m_TRS->ShowWindow(SW_SHOW);

	if(TransType == REPO || TransType == LEVERAGE || TransType == BORROW || TransType == LEND ||
		TransType == SECURITIES || TransType == CDS || TransType == INTSWAP)
	{
		if(TransType == INTSWAP)
		{
			GetDlgItem(IDC_PROCESS_SWAP_MATURITY_STATIC)->ShowWindow(SW_SHOW);
			m_SWMaturity.ShowWindow(SW_SHOW);
		}
	}

	if(TransType == FOREX)
	{
		GetDlgItem(IDC_PROCESS_USDACC_STATIC)->ShowWindow(SW_SHOW);
		m_USDAcc->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROCESS_FXACC_STATIC)->ShowWindow(SW_SHOW);
		m_FxAcc->ShowWindow(SW_SHOW);
	}

	m_AAMethod.EnableWindow(!m_Data.GetRawTicket().GetOriNominal().IsEmpty());
}

void CTicketProcess::InitControls()
{
	m_SS.SetVisibleCols(10);
	m_SS.SetVisibleRows(6);
	m_SS.LockSheet();

	m_AllocSS.SetVisibleCols(8);
	m_AllocSS.SetVisibleRows(3);	
	m_AllocSS.LockSheet();

	m_Listed = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_LISTED_CHECK);
	m_EuroOpt = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_EUROOPT_CHECK);
	m_OptAuto = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_AUTO_CHECK);
	m_IPO = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_IPO_CHECK);
	m_TRS = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_TRS_CHECK, Y);
	m_DocReq = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_DOC_REQUIRED_CHECK);
	m_DocRecvd = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_DOC_RECEIVED_CHECK);
	m_WI = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_WI_CHECK);
	m_Confirm = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_CONFIRM_CHECK);
	m_Funded = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_FUNDED_CHECK, Y);
	m_ShortSale = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_SHORT_CHECK, Y);
	m_CSPBShort = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_CSPBSHORT_CHECK, Y);
	m_SecFee = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_SECFEE_CHECK, Y);
	m_OrFee = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_ORFEE_CHECK, Y);
	m_ETrade = (CCheckBox*) new CCheckBox(this, IDC_PROCESS_ETRADE_CHECK, Y);

	m_TransType = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_TRANSTYPE_COMBO);
	m_CP = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_COUNTERPARTY_COMBO);
	m_RepoCP = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_REPOCP_COMBO, TRUE);
	m_AssignCP = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_ASSIGNCP_COMBO, TRUE);
    m_Currency = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_CURRENCY_COMBO);
    m_RateBasis = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_RATEBASIS_COMBO, TRUE);
	m_RepoType = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_REPOTYPE_COMBO, TRUE);
	m_Formula = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_FORMULA_COMBO, TRUE, TRUE);
	m_Formula->LimitText(20);
    m_PFU = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_PFU_COMBO, TRUE);
    m_USDAcc = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_USDACC_COMBO, TRUE);
    m_FxAcc = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_FXACC_COMBO, TRUE);
	m_MarginCurrency = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_MARGIN_CURRENCY_COMBO, TRUE);
	m_BestExecution = (COptComboBox*) new COptComboBox(this, IDC_PROCESS_BESTEXECUTION_COMBO);

	m_Amount.Setup(this, IDC_PROCESS_AMOUNT_EDIT);
	m_FxRate.Setup(this, IDC_PROCESS_FXRATE_EDIT);

	m_TradeDate.Setup(this, IDC_PROCESS_TRADEDATE_EDIT);
	m_ExecTime.Setup(this, IDC_PROCESS_EXECTIME_EDIT);
	m_ValueDate.Setup(this, IDC_PROCESS_VALUEDATE_EDIT);
	m_Maturity.Setup(this, IDC_PROCESS_MATURITY_EDIT, TRUE, 
					"Exercise Date must be greated than or equal to Value Date", &m_ValueDate);
	m_ExerciseDate.Setup(this, IDC_PROCESS_EXERCISEDATE_EDIT, TRUE, 
					"Exercise Date must be greater than or equal to Trade Date", &m_TradeDate);
	m_OptExpDate.Setup(this, IDC_PROCESS_OPTEXP_EDIT, TRUE, 
					"Option Expiration Date must be greater or equal to Trade Date", &m_TradeDate);
	m_DeliveryDate.Setup(this, IDC_PROCESS_DELIVERY_EDIT, TRUE, 
						"Delivery Date must be greater than Expiration Date", &m_OptExpDate);

	m_SWBooking.Setup(this, IDC_PROCESS_SWAP_BOOKING_EDIT);
	m_SWMaturity.Setup(this, IDC_PROCESS_SWAP_MATURITY_EDIT, TRUE, 
					"Swap Maturity Date must be greater than or equal to Swap Booking Date", &m_SWBooking);
	m_FXDate.Setup(this, IDC_PROCESS_FXDATE_EDIT);

	m_Rate.Setup(this, IDC_PROCESS_RATE_EDIT);
	m_Margin.Setup(this, IDC_PROCESS_MARGIN_EDIT);
	m_MarginAmount.Setup(this, IDC_PROCESS_MARGIN_AMOUNT_EDIT);
	m_AssignCT.Setup(this, IDC_PROCESS_ASSIGNCT_EDIT);
	m_AssignCT.LimitText(3);
	m_Strike.Setup(this, IDC_PROCESS_STRIKE_EDIT);
	m_CT.Setup(this, IDC_PROCESS_CT_EDIT);
	m_RepoCT.Setup(this, IDC_PROCESS_REPOCT_EDIT);
	m_OptTicker.Setup(this, IDC_PROCESS_OPT_TICK_EDIT);
	m_OptTicker.LimitText(25);
	m_SetConvention.Setup(this, IDC_PROCESS_SETCONVENTION_EDIT);
	m_SetConvention.LimitText(15);

    m_Dir.Setup(this, IDC_PROCESS_TRANSDIR_COMBO, 2);
    m_Trader.Setup(this, IDC_PROCESS_TRADER_COMBO, 2);
	m_Index.Setup(this, IDC_PROCESS_INDEX_COMBO, 2, TRUE);
	m_OptSet.Setup(this, IDC_PROCESS_OPTSET_COMBO, 3, TRUE);
	m_OptSet2.Setup(this, IDC_PROCESS_OPTSET2_COMBO, 3, TRUE);
	m_AAMethod.Setup(this, IDC_PROCESS_AA_COMBO);
	
	m_OptID.Setup(this, IDC_PROCESS_OPTID_EDIT);
	m_OptID.LimitText(25);
	m_Binary.Setup(this, IDC_PROCESS_BINARY_EDIT);
	m_Binary.LimitText(30);
	
	m_Ticket.Setup(this, IDC_PROCESS_TICKETNUM_EDIT);
	m_SwapTicket.Setup(this, IDC_PROCESS_SWAP_TICKET_EDIT);
	m_OptTicket.Setup(this, IDC_PROCESS_UNWIND_TICKET_EDIT);
	m_CancelTicket.Setup(this, IDC_PROCESS_CANCEL_TICKET_EDIT);
	m_CorrectTicket.Setup(this, IDC_PROCESS_CORRECT_TICKET_EDIT);

	m_Asset.Setup(this, IDC_PROCESS_ASSET_EDIT);
	m_AssetDesc.Setup(this, IDC_PROCESS_ASSETDESC_EDIT);
	m_AssetCurr.Setup(this, IDC_PROCESS_ASSETCURRENCY_EDIT);
	m_Note.Setup(this, IDC_PROCESS_COMMENT_EDIT, EMPTYSTRING, NOCASE);
	m_Note.LimitText(30);
	m_Note2.Setup(this, IDC_PROCESS_COMMENT2_EDIT, EMPTYSTRING, NOCASE);
	m_Note2.LimitText(30);
	m_Booker.Setup(this, IDC_PROCESS_BOOKER_EDIT);

	m_Price.Setup(this, IDC_PROCESS_PRICE_EDIT);
	m_NetPrice.Setup(this, IDC_PROCESS_NETPRICE_EDIT);
	m_DownPymnt.Setup(this, IDC_PROCESS_DOWNPAY_EDIT);
	m_BrokerFee.Setup(this, IDC_PROCESS_BROKER_FEE_EDIT);
	m_SoftDollar.Setup(this, IDC_PROCESS_SOFTDOLLAR_EDIT);
	m_OtherFee.Setup(this, IDC_PROCESS_OTHER_FEE_EDIT);
	m_VAR.Setup(this, IDC_PROCESS_VAR_EDIT);
	m_DV01.Setup(this, IDC_PROCESS_DV01_EDIT);

	m_SetCurrency.Setup(this, IDC_PROCESS_SETCURRENCY_EDIT);
	m_Cash.Setup(this, IDC_PROCESS_PRINCIPLE_EDIT);
	m_PrePaid.Setup(this, IDC_PROCESS_INTEREST_EDIT);
	m_Accretion.Setup(this, IDC_PROCESS_ACCRETION_EDIT);
	m_Total.Setup(this, IDC_PROCESS_TOTAL_AMT_EDIT);

	CRawTicketRec *pTicket;
	pTicket = &m_Data.GetRawTicket();
	m_Data.Add(&m_Ticket, &pTicket->GetTicket());
	m_Data.Add(m_CP, &pTicket->GetCP());
	m_Data.Add(&m_Asset, &pTicket->GetAsset());
	m_Data.Add(&m_Trader, &pTicket->GetTrader());
	m_Data.Add(&m_TradeDate, &pTicket->GetTradeDate());
	m_Data.Add(&m_ExecTime, &pTicket->GetExecTime());
	m_Data.Add(&m_ValueDate, &pTicket->GetValueDate());
	m_Data.Add(m_TransType, &pTicket->GetTransType());
	m_Data.Add(&m_Dir, &pTicket->GetDir());
	m_Data.Add(&m_Amount, &pTicket->GetNomAmount());
	m_Data.Add(&m_Strike, &pTicket->GetStrike());
	m_Data.Add(&m_OptExpDate, &pTicket->GetOptExp());
	m_Data.Add(&m_FXDate, &pTicket->GetFXDate());
	m_Data.Add(m_Listed, &pTicket->GetListed());
	m_Data.Add(&m_OptTicker, &pTicket->GetOptTicker());
	m_Data.Add(&m_OptSet, &pTicket->GetOptSetCode());
	m_Data.Add(&m_OptSet2, &pTicket->GetOptSetCode2());
	m_Data.Add(&m_SetConvention, &pTicket->GetSetConvention());
	m_Data.Add(&m_OptID, &pTicket->GetOptID());
	m_Data.Add(m_EuroOpt, &pTicket->GetEuroOpt());
	m_Data.Add(m_OptAuto, &pTicket->GetOptAuto());
	m_Data.Add(m_Currency, &pTicket->GetCurrency());
	m_Data.Add(&m_FxRate, &pTicket->GetFxRate());
	m_Data.Add(&m_AssetDesc, &pTicket->GetAssetDesc());
	m_Data.Add(&m_AssetCurr, &pTicket->GetAssetCurr());
	m_Data.Add(m_IPO, &pTicket->GetIPO());
	m_Data.Add(m_TRS, &pTicket->GetTRS());
	m_Data.Add(m_RepoCP, &pTicket->GetRepoCP());
	m_Data.Add(&m_Rate, &pTicket->GetRate());
	m_Data.Add(m_RateBasis, &pTicket->GetRateBasis());
	m_Data.Add(&m_Maturity, &pTicket->GetMaturity());
	m_Data.Add(&m_SWBooking, &pTicket->GetSWBooking());
	m_Data.Add(&m_SWMaturity, &pTicket->GetSWMaturity());
	m_Data.Add(&m_Note, &pTicket->GetNote());
	m_Data.Add(&m_Note2, &pTicket->GetNote2());
	m_Data.Add(&m_RepoCT, &pTicket->GetRepoCT());
	m_Data.Add(m_RepoType, &pTicket->GetRepoType());
	m_Data.Add(&m_Index, &m_Data.GetTicket().GetIndex());
	m_Data.Add(m_Formula, &m_Data.GetTicket().GetFormula());
	m_Data.Add(m_USDAcc, &m_Data.GetTicket().GetUSDAcc());
	m_Data.Add(m_FxAcc, &m_Data.GetTicket().GetFxAcc());
	m_Data.Add(m_PFU, &m_Data.GetTicket().GetPFU());
	m_Data.Add(&m_ExerciseDate, &m_Data.GetTicket().GetExerciseDate());
	m_Data.Add(&m_CT, &m_Data.GetTicket().GetCT());
	m_Data.Add(m_DocReq, &m_Data.GetTicket().GetDocReq());
	m_Data.Add(m_DocRecvd, &m_Data.GetTicket().GetDocRecvd());
	m_Data.Add(m_WI, &pTicket->GetWI());
	m_Data.Add(m_Confirm, &m_Data.GetTicket().GetConfirm());
	m_Data.Add(&m_SwapTicket, &pTicket->GetSwapTicket());
	m_Data.Add(&m_DeliveryDate, &pTicket->GetDeliveryDate());
	m_Data.Add(&m_Margin, &pTicket->GetMargin());
	m_Data.Add(m_MarginCurrency, &pTicket->GetMarginCurrency());
	m_Data.Add(&m_MarginAmount, &pTicket->GetMarginAmount());
	m_Data.Add(m_Funded, &pTicket->GetFunded());
	m_Data.Add(&m_Binary, &pTicket->GetBinary());
	m_Data.Add(&m_AAMethod, &pTicket->GetAAMethod());
	m_Data.Add(&pTicket->GetAAFReason());
	m_Data.Add(m_BestExecution, &pTicket->GetBestExecution());
	m_Data.Add(m_ShortSale, &pTicket->GetShortSale());
	m_Data.Add(m_CSPBShort, &pTicket->GetCSPBShort());
	m_Data.Add(&m_Price, &pTicket->GetPrice());
	m_Data.Add(&m_DownPymnt, &pTicket->GetDownPymnt());
	m_Data.Add(&m_BrokerFee, &pTicket->GetBrokerFee());
	m_Data.Add(&m_SoftDollar, &pTicket->GetSoftDollar());
	m_Data.Add(&m_OtherFee, &pTicket->GetOtherFee());
	m_Data.Add(&m_VAR, &pTicket->GetVAR());
	m_Data.Add(&m_DV01, &pTicket->GetDV01());
	m_Data.Add(m_AssignCP, &pTicket->GetAssignCP());
	m_Data.Add(&m_AssignCT, &pTicket->GetAssignCT());
	m_Data.Add(&m_OptTicket, &pTicket->GetUnWindTicket());
	m_Data.Add(&m_CancelTicket, &pTicket->GetCancelTicket());
	m_Data.Add(&m_CorrectTicket, &pTicket->GetCorrectTicket());
	m_Data.Add(m_SecFee, &pTicket->GetSecFee());
	m_Data.Add(m_OrFee, &pTicket->GetOrFee());
	m_Data.Add(m_ETrade, &pTicket->GetETrade());
	m_Data.Add(&m_Booker, &m_Data.GetBooker());
	m_Data.Add(&m_Data.GetBookDate());
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
	m_Data.Add(&m_Par);
	m_Data.Add(&pTicket->GetOriNominal());
	m_Data.Add(&pTicket->GetCPTradeID());
	m_Data.Add(&m_NetPrice, &pTicket->GetNetPrice());

	CRawInvRec *pInv;
	pInv = &m_Data.GetRawInv();
	m_Data.GetSRowCtrl().Add(&m_Data.GetTicket().GetTransNum());
	m_Data.GetSRowCtrl().Add(&pInv->GetPortfolio());
	m_Data.GetSRowCtrl().Add(&pInv->GetNomAmount());
	m_Data.GetSRowCtrl().Add(&pInv->GetCustodian());
	m_Data.GetSRowCtrl().Add(&pInv->GetAccount());
	m_Data.GetSRowCtrl().Add(&pInv->GetPrice());
	m_Data.GetSRowCtrl().Add(&pInv->GetLink());
	m_Data.GetSRowCtrl().Add(&pInv->GetDownPymnt());
}

BOOL CTicketProcess::IsOK()
{
	CString TransType, Dir, Asset, Text, Data, Sql, Method, Currency, Nominal, Price, Fxrate;
	COraLoader OraLoader;
	CQData QData;
	double dNom, dParValue;

	OraLoader = GetData().GetOraLoader();

	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	TransType = GetData().DefineTransType(TransType, Dir);
	if(TransType.IsEmpty()) // TransType
		Text = "Invalid Trans Type";

	if(Dir.IsEmpty())  // TransDir
		Text = "Invalid Trans Dir";

	if(m_TradeDate.GetData().IsEmpty()) // TradeDate
		Text = "Invalid Trade Date";

	if(m_ValueDate.GetData().IsEmpty()) // ValueDate
		Text = "Invalid Value Date";

	if((TransType == REPO || TransType == LEVERAGE || TransType == SECURITIES || 
		TransType == CDS || TransType == CALL || TransType == PUT || 
		TransType == SPRSWCLL || TransType == SPRSWPUT || TransType == INTSWAP))
	{
		m_Asset.GetWindowText(Asset);
		if(Asset.IsEmpty() || Asset == NEWASSET)
			Text = "Invalid Asset";
	}

	m_Amount.GetWindowText(Data);
	dNom = atof(QData.RemoveComma(Data));
	if(dNom <= 0)
		Text = "Invalid Nominal Amount.  Must enter Nominal Amount.";

	if(!m_TRS->GetCheck() && fabs(dNom - m_Data.ComputeAllocAmount()) > 0.00001)
			Text = "Nominal Amount and Sum of Allocated Nominal Amounts does not match";
	
	if(TransType == CALL || TransType == PUT)
	{
		dParValue = atof(QData.RemoveComma(m_Par));
		if(dParValue > 0 && dNom > 0 && fmod(dNom, dParValue) > 0)
			Text = "Invalid number of contracts (Fraction of a contract) ";
	}

	if(TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT) // Opt Expiration
	{
		if(m_OptExpDate.GetData().IsEmpty())
			Text = "Invalid Opt Expiration Date";
	}

	if(TransType == REPO || TransType == LEVERAGE || TransType == CASH || TransType == INTSWAP) // RateBasis
	{										
		if(m_RateBasis->GetCurSel() < 0)
			Text = "Invalid RateBasis";
	}

	if(m_PFU->GetCurSel() < 0)
		Text = "Invalid PFU";

	if(m_CP->GetCurSel() < 0)
		Text = "Invalid Counterparty";

	if(m_CT.GetWindowTextLength() <= 0)
		Text = "Invalid Counterparty Contact";

	if(m_RepoCP->GetCurSel() >= 0)
	{
		if(m_RepoCT.GetWindowTextLength() == 0)
			Text = "Invalid Repo Contact";
		
		if(m_RepoType->GetCurSel() < 0)
			Text = "Invalid Repo Type";
	}

	Asset = m_Data.GetRawTicket().GetAssetClass();
	if(Asset == "CURRENCY OPTION" && (TransType == CALL || TransType == PUT))
	{
		if(m_OptTicker.GetWindowTextLength() <= 0)
			Text = "Invalid Option Ticker";
		else
			if(m_OptID.GetWindowTextLength() <= 0)
				Text = "Invalid Option ID";
			else
				if(m_OptSet.GetWindowTextLength() <= 0)
					Text = "Invalid Option Settlement Code";
	}

	m_Asset.GetWindowText(Asset);
	if(!m_OptTicket.GetData().IsEmpty() && !Asset.IsEmpty() && 
		(TransType == SECURITIES || TransType == CALL || TransType == PUT))
	{
		Asset = QData.GetQueryText(Asset);

		Sql = "SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS A "
				"WHERE TRANS_TYPE IN ('CALL', 'PUT') "
				"AND ASSET_CODE = " + Asset;

		if(TransType == CALL || TransType == PUT)
		{
			Sql += " AND TICKET_NUM = ";
			Sql += QData.GetQueryNumber(m_OptTicket.GetData());
		
			Sql += " AND TRANS_TYPE = ";
			Sql += QData.GetQueryText(TransType);

			Sql += " AND TRANS_DIRECTION != "; 
			Sql += QData.GetQueryText(Dir); 

			m_Asset.GetWindowText(Asset);
			Sql += " AND ASSET_CODE = ";
			Sql += QData.GetQueryText(Asset);

			m_Strike.GetWindowText(Data);
			Sql += " AND EXERCISE_PRICE = ";
			Sql += QData.GetQueryNumber(Data);
	
			m_OptExpDate.GetWindowText(Data);
			Sql	+= " AND OPT_EXPIRATION = ";
			Sql += QData.GetQueryDate(Data);
		
			if(m_Listed->GetCheck())
				Sql += " AND LISTED = 'Y' ";

			if(m_EuroOpt->GetCheck())
				Sql += " AND EURO_OPT = 'Y' ";
		}
			
		if(OraLoader.GetCount(Sql) <= 0)
			Text = "There are mismatch between option and unwinded option";
	}

	if(TransType != FOREX && m_Margin.GetWindowTextLength() <= 0 && m_MarginAmount.GetWindowTextLength() <= 0)
		Text = "Invalid Margin or Margin Amount";

	if(m_BestExecution->GetCurSel() < 0)
		Text = "Must choose a best execution method";

/*	m_NetPrice.GetWindowText(Data);
	if(!Data.IsEmpty())
	{
		CString Data2;
		for(int i = 1; i <= m_AllocSS.GetSheetRows(); i++)
		{
			Data2 = m_AllocSS.GetSheetText(6, i);
			if(Data2 != Data)
				Text = "Net Price and Allocation Price are not match";
		}
	} */

	if(!Text.IsEmpty())
	{
		MessageBox(Text);
		return FALSE;
	}

	m_AAMethod.GetSelString(Method);

	if(Method != "BNAMTR")
		return TRUE;

	m_Currency->GetSelString(Currency);
	m_Amount.GetWindowTextA(Nominal);
	m_Price.GetWindowTextA(Price);
	m_FxRate.GetWindowText(Fxrate);

	switch(m_Risk.VerifyRisk(Text, Method, TransType, m_ValueDate.GetData(), Currency, Nominal, Price, Fxrate))
	{
		case 1:
			MessageBox(Text, "Ticket Processing");
			return TRUE;
		case 3:
			MessageBox(Text, "Ticket Processing");
			return FALSE;
		default:
			return TRUE;
	}
}

void CTicketProcess::ProcessVerifyRisk()
{
	CString Text;

	m_nRiskLevel = VerifyRisk(Text);
	if(!Text.IsEmpty() && !m_bWarned)
	{
		MessageBox(Text, "Ticket Process");
		m_bWarned = TRUE;
	}
}

void CTicketProcess::UpdateAsset()
{
	m_Data.SetupAssetInfo();
}

void CTicketProcess::UpdateCash()
{
	CString Asset, Data, Data2;
	CQData QData;

	m_Asset.GetWindowText(Asset);
	if(Asset == NEWASSET)
		return;
	
	m_Data.SetupAssetInfo();
	m_Data.ComputeValue(m_SecFee->GetCheck(), m_OrFee->GetCheck());
	if(m_FxRate.GetWindowTextLength() > 0)
		m_SetCurrency.SetWindowText(USD);
	else
		m_SetCurrency.SetWindowText(m_Currency->GetSelString(Asset));

	Data = QData.WriteNumber(m_Data.GetNetPrice(), TRUE);
	m_NetPrice.SetWindowText(Data);
	m_Cash.SetWindowText(QData.WriteNumber(m_Data.GetCash(), TRUE, 2));
	m_PrePaid.SetWindowText(QData.WriteNumber(m_Data.GetPrePaid(), TRUE, 2));
	m_Accretion.SetWindowText(QData.WriteNumber(m_Data.GetAccretion(), TRUE, 2));
	m_Total.SetWindowText(QData.WriteNumber(m_Data.GetCash() + m_Data.GetPrePaid() + m_Data.GetAccretion(), TRUE, 2));
	m_NetPrice.GetWindowText(Data);
	m_Price.GetWindowText(Data);
	Data2.Format("%.2lf", m_Data.ComputeOtherFee(atof(Data), m_SecFee->GetCheck(), m_OrFee->GetCheck()));
	if(m_SecFee->GetCheck() || m_OrFee->GetCheck())
		m_OtherFee.SetWindowText(Data2);

/*	if(m_Data.GetNetPrice() > 0)
	{
		for(int i = 1; i <= m_AllocSS.GetSheetRows(); i++)
		{
			Data2 = m_AllocSS.GetSheetText(6, i);
			if(!Data2.IsEmpty() && Data2 != Data)
				m_AllocSS.SetSheetText(6, i, Data);
		}
	} */
}

void CTicketProcess::UpdateCPCT(COptComboBox *CP, CCntlEdit &CT)
{
	CFindDlg FindDlg;
	CQData QData;
	CString sCP;

	FindDlg.m_OraLoader = GetData().GetOraLoader();

	CP->GetSelString(sCP);
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
		CT.SetWindowText(FindDlg.m_Key);
}

BOOL CTicketProcess::UpdateData(BOOL bSaveandValid)
{
	BeginWaitCursor();
	if(bSaveandValid)
	{
		m_SS.SetFocus();
		m_Data.UpdateRow();
		m_Risk.UpdateRisk(m_Ticket.GetData());
	}
	else
	{
		CQData QData;

		m_Data.LoadTickets();
		m_Data.LoadAllocList();

		GetData().LoadSupportData();
		QData.CopyDBRecArrayToComboBox(GetData().GetTransTypeArr(), *m_TransType);
		QData.CopyDBRecArrayToComboBox(GetData().GetRateBasisArr(), *m_RateBasis);
		QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_Currency, 0, FALSE);
		QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_MarginCurrency);
		QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_CP);
		QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_AssignCP);
		QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_RepoCP);
		
		QData.CopyDBRecArrayToComboBox(GetData().GetPFUArr(), *m_PFU);
		QData.CopyDBRecArrayToComboBox(GetData().GetAccountArr(), *m_USDAcc, 0, FALSE);
		QData.CopyDBRecArrayToComboBox(GetData().GetAccountArr(), *m_FxAcc);
		GetData().GetOraLoader().Open("SELECT AA_METHOD, AA_DESC, RATIO_A, RATIO_B, RATIO_C, RATIO_D FROM SEMAM.NW_AA_METHOD_V ORDER BY 1");
		m_AAMethod.ResetContent();
		GetData().GetOraLoader().LoadMCComboBox(m_AAMethod);
		QData.CopyDBRecArrayToComboBox(GetData().GetBestExecutionArr(), *m_BestExecution);

		GetData().GetTraderArr().CopyToMCComboBox(m_Trader); /* Trader */
		GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);	/* TransDir */
		GetData().GetIndexArr().CopyToMCComboBox(m_Index);
		GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet);
		GetData().GetOptSetArr().CopyToMCComboBox(m_OptSet2);
	}

	CQData QData;

	SetDlgItemText(IDC_PROCESS_COUNT_EDIT, QData.WriteNumber(m_SS.GetSheetRows() >= 0 ? m_SS.GetSheetRows() : 0));
	EndWaitCursor();

	return TRUE;
}

void CTicketProcess::UpdateTradeDates(BOOL bEuroBond) 
{
	CString Date, TransType;

	m_TransType->GetSelString(TransType);

	m_TradeDate.SetData(GetData().GetOraLoader().Today(Date));

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, TransType, m_Listed->GetCheck(), m_Future == "F" ? TRUE : FALSE));
	
	if(bEuroBond)
		m_ValueDate.SetData(GetData().GetPlus2Date());
}

int CTicketProcess::VerifyRisk(CString &Text)
{
	CString Method, Asset, TransType, Dir, Currency, ValueDate, Nominal, Price, Fxrate;

	m_AAMethod.GetSelString(Method);

	if(Method != "BNAMTR"||m_TRS->GetCheck())
		return 0;

	m_Asset.GetWindowTextA(Asset);
	if(Asset != m_Risk.GetAsset())
		m_Risk.LoadRisk(Asset, m_Ticket.GetData());

	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	m_Currency->GetSelString(Currency);
	ValueDate = m_ValueDate.GetData();
	m_Amount.GetWindowText(Nominal);
	m_Price.GetWindowText(Price);
	m_FxRate.GetWindowText(Fxrate);

	if(Dir == S)
		Nominal = "-" + Nominal;

	m_bWarned = FALSE;
	return m_Risk.VerifyRisk(Text, Method, TransType, ValueDate, Currency, Nominal, Price, Fxrate);
}

/////////////////////////////////////////////////////////////////////////////
// CTicketProcess message handlers

BEGIN_EVENTSINK_MAP(CTicketProcess, CFormView)
    //{{AFX_EVENTSINK_MAP(CTicketProcess)
	ON_EVENT(CTicketProcess, IDC_PROCESS_TICKET_LIST, 2 /* BlockSelected */, OnBlockSelectedProcessTicketList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTicketProcess, IDC_PROCESS_TICKET_LIST, 11 /* DblClick */, OnDblClickProcessTicketList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTicketProcess::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BeginWaitCursor();

	InitControls();

	GetData().LoadDates();
	m_RepoType->AddString(NONE);
	m_RepoType->AddString(LEVERAGE);
	m_RepoType->AddString(REPO);
	m_Formula->AddString(NONE);
	m_Formula->AddString(DAILYREPO);
	m_Formula->AddString(DAILYREPOA);
	m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_AllocSS, GetData().GetOraUser(), 0);
	m_Risk.Setup(GetData().GetOraLoader());
	EndWaitCursor();

	m_Data.Refresh();
	UpdateData(FALSE);
	GetDocument()->SetTitle(GetData().GetUser());
	GetDocument()->SetViewTitle("TICKET PROCESS");

	GetData().SetTicketNumber(EMPTYSTRING);
}

void CTicketProcess::OnBlockSelectedProcessTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickProcessTicketList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CTicketProcess::OnDblClickProcessTicketList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row, m_Data.GetAllocList());
		GetData().SetTicketNumber(m_Ticket.GetData());
		if(m_Data.IsTicketProcessed())
		{
			MessageBox("This ticket has been processed.  Please reload tickets.", m_Data.GetRawTicket().GetTicket());
			return;
		}
	}
	else
	{
		m_Data.SetCurrentRow(-1);
		OnProcessRefresh();
	}

	ChangeShortLabel();

	UpdateAsset();
	UpdateCash();
	EnableCtrls();
}

void CTicketProcess::OnProcessLoad() 
{
	UpdateData(FALSE);
}

void CTicketProcess::OnProcessAdd() 
{
	if(!IsOK())
		return;

	if(m_Ticket.GetData().IsEmpty())
	{		
		CString Ticket;
		GetData().GetOraLoader().GenerateUniqueID(TICKETNUM, Ticket);
		m_Ticket.SetData(Ticket);
	}

	m_Data.SetKey(EMPTYSTRING);
	UpdateData(TRUE);
}

void CTicketProcess::OnProcessDelete() 
{
	if(MessageBox("Are you sure you want to delete this ticket", NULL, MB_YESNOCANCEL) == IDYES)
	{
		m_Data.DeleteRow();
		m_Risk.DeleteRisk(m_Ticket.GetData());
	}
}

void CTicketProcess::OnProcessUpdate() 
{
	if(!IsOK())
		return;
	
	UpdateData(TRUE);
}

void CTicketProcess::OnUpdateProcessAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocSS.GetSheetRows() > 0 && !m_AllocSS.GetSheetText(4, 1).IsEmpty() && 
					m_SS.GetSheetCurRow() >= 1 && m_AllocSS.GetSheetRows() > 0 ? TRUE : FALSE);
}

void CTicketProcess::OnProcessAllocation() 
{
	CAllocation Dlg;
	CString TransType, Dir;
	CKeyDBRecArray *pArray;
	int i;

	m_Data.UpdateData();
	m_Data.SetKey();
	Dlg.m_Data.Setup(GetData().GetOraLoader(), m_Data.GetRawTicket(), m_Data.GetTicket().GetFormula());
	m_NetPrice.GetWindowText(m_Data.GetRawTicket().GetNetPrice());
	Dlg.m_Data.GetTicket() = m_Data.GetRawTicket();
	m_DownPymnt.GetWindowText(Dlg.m_DownPay);
	pArray = m_Data.GetAllocList().GetAt(m_Data.GetKey());
	if(!pArray || Dlg.m_DownPay.IsEmpty())
		return;

	Dlg.m_SS.GetFieldArray() = m_AllocSS.GetFieldArray();
	Dlg.m_RecArray = *pArray;
	m_TransType->GetSelString(TransType);

	Dir = m_Data.GetTicket().GetDir();
	Dlg.m_TransType = GetData().DefineTransType(TransType, Dir);

	Dlg.m_pData = &GetData();
	m_Formula->GetSelString(Dlg.m_Formula);
	Dlg.m_Category = m_Data.GetRawTicket().GetAssetCategory();
	m_Margin.GetWindowText(Dlg.m_Margin);
	m_PFU->GetSelString(Dlg.m_PFU);
	Dlg.m_sCustodian = m_Custodian;

	if(Dlg.DoModal() == IDOK)
	{		
		i = m_Data.GetAllocList().Find(Dlg.m_RecArray.GetKey());
		if(i >= 0)
			m_Data.GetAllocList().SetAt(i, Dlg.m_RecArray);
		m_Data.GetAllocList().CopyDataToRowCtrl(Dlg.m_RecArray.GetKey(), m_Data.GetSRowCtrl());
		m_Margin.SetWindowText(Dlg.m_Margin);
	}
}

void CTicketProcess::OnUpdateProcessAllocation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_AllocSS.GetSheetRows() == 0 && 
			(m_Amount.GetWindowTextLength() == 0 || m_TransType->GetCurSel() < 0) ? FALSE : TRUE);
}

void CTicketProcess::OnUpdateProcessDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SS.GetSheetCurRow() >= 1 && m_SS.GetIsBlockSelected() && strlen(m_Data.GetKey()) > 0);
}

void CTicketProcess::OnProcessFindAsset() 
{
	CAssetDlg Dlg;
	CString TransType, Dir, Text;
	BOOL bChange = FALSE;
	CQData QData;

	Dlg.m_pData = &GetData();
	m_Asset.GetWindowText(Dlg.m_FindData.GetRec().GetAsset());
	m_AssetDesc.GetWindowText(Dlg.m_FindData.GetRec().GetDesc());
	m_OptTicker.GetWindowText(Dlg.m_FindData.GetRec().GetOptTicker());

	m_TransType->GetSelString(TransType);
	Dlg.m_bTicket = TRUE;

	if(Dlg.DoModal() == IDOK)
	{
		m_Asset.SetWindowText(Dlg.m_FindData.GetRec().GetAsset());
		m_AssetDesc.SetWindowText(Dlg.m_FindData.GetRec().GetDesc());
		m_AssetCurr.SetWindowText(Dlg.m_FindData.GetRec().GetCurrency());
		m_Trader.SelectString(0, Dlg.m_FindData.GetTrader());
		m_Custodian = Dlg.m_FindData.GetCustodian();

		m_Data.GetRawTicket().SetAssetClass(Dlg.m_FindData.GetRec().GetClass());
		m_Future = Dlg.m_Future;

		CString Currency, Text;
		if(TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
			m_OptTicker.SetWindowText(m_Data.GetRawTicket().GetOptTicker()); // Set Opt Ticker
		else
			m_OptTicker.SetWindowText(EMPTYSTRING);		

		if(TransType == "CALL" || TransType == "PUT")
		{
			if(m_Data.GetRawTicket().GetAssetClass().Find("CURRENCY", 0) >= 0)
				m_EuroOpt->SetCheck(1);
			else
				m_EuroOpt->SetCheck(0);
		}

		m_Currency->GetSelString(Currency);
		m_AssetCurr.GetWindowText(Text);
		if(Currency.IsEmpty() || 
			(Currency != Text &&
			MessageBox("Note: Trading Currency is Different from Asset Currency! Do you want to synchonize them ?", 
					"Warning", MB_YESNO) == IDYES))
			m_Currency->SelectString(-1, Text);

		m_Currency->GetSelString(Currency);
		if(m_FxRate.GetWindowTextLength() == 0 && Currency == USD)
			m_FxRate.SetWindowText("1");
	
		m_FxRate.GetWindowText(Text);
		if(Currency != USD && Text == "1")
			m_FxRate.SetWindowText(EMPTYSTRING);

		if(Dlg.m_nActionID == FINDACTION_UNWIND || Dlg.m_nActionID == FINDACTION_OPTEX)
		{
			m_Amount.GetWindowText(Text);
			if(Dlg.m_FindData.GetNominal() != Text && m_Amount.GetWindowTextLength() > 0)
			{
				if(MessageBox("Do you want to change current nom_amount (Y/N)?", "Ticket Process", MB_YESNO) == IDYES)
					bChange = TRUE;
			}
		}

		m_Par = Dlg.m_FindData.GetRec().GetPar();

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
					m_OptExpDate.SetData(Dlg.m_FindData.GetRec().GetMaturity());
					m_OptTicker.SetWindowText(Dlg.m_FindData.GetOptTick());
					m_OptID.SetWindowText(Dlg.m_FindData.GetOptID());
					m_DeliveryDate.SetWindowText(Dlg.m_FindData.GetDeliveryDate());
					m_OptSet.SelectString(0, Dlg.m_FindData.GetOptSetCode());
					m_Listed->SetCheck(Dlg.m_OptListed);
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
					m_OptExpDate.SetWindowText(Dlg.m_FindData.GetRec().GetMaturity());
					m_OptTicker.SetWindowText(Dlg.m_FindData.GetTicket());
					m_OptID.SetWindowText(Dlg.m_FindData.GetOptID());
					m_DeliveryDate.SetWindowText(Dlg.m_FindData.GetDeliveryDate());
					m_OptSet.SelectString(0, Dlg.m_FindData.GetOptSetCode());
					m_OptTicket.SetData(Dlg.m_FindData.GetTicket());
					m_Listed->SetCheck(Dlg.m_OptListed);
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
					m_OptTicket.SetData(Dlg.m_FindData.GetTicket());
					m_AssignCP->SelectString(0, Dlg.m_FindData.GetAssignCP());
				}
				break;
			default:
				break;
		}
	}

	if(m_TradeDate.GetData().IsEmpty())
		m_TradeDate.SetData(GetData().GetDate());

	UpdateTradeDates(Dlg.m_EuropeBond == "Y");
	
	m_Asset.GetWindowText(Text);

	if(TransType == FOREX && Text == "NEW ASSET")
		m_Asset.SetWindowText("");

	EnableCtrls();
	m_TransType->SetFocus();
}

void CTicketProcess::OnUpdateProcessFindAsset(CCmdUI* pCmdUI) 
{
	CString TransType;

	TransType = m_TransType->GetSelString(TransType);
	pCmdUI->Enable((TransType.IsEmpty() || TransType == CASH || TransType == SPRSWCLL || TransType == SPRSWPUT) ? 
					FALSE : TRUE);
}

void CTicketProcess::OnProcessRefresh() 
{
	const int Sel = -1;
	
	BeginWaitCursor();
	m_Data.SetCurrentRow(Sel);
	m_Data.Refresh();
	m_TradeDate.SetData(GetData().GetDate());
	m_ValueDate.SetData(GetData().GetPlus3Date());     
	m_TransType->SelectString(Sel, SECURITIES);
	m_Currency->SelectString(Sel, USD);
	m_FxRate.SetWindowText("1");
	m_RateBasis->SelectString(Sel, "A/360");
	m_PFU->SelectString(Sel, "P");
	m_SS.SheetAutoFit();	
	EndWaitCursor();
}

void CTicketProcess::OnUpdateProcessUpdate(CCmdUI* pCmdUI) 
{
	CString Text;

	if(m_SS.GetSheetCurRow() >= 1 && m_Data.GetSRowCtrl().GetSS()->GetSheetRows() > 0) 
		Text = m_Data.GetSRowCtrl().GetSS()->GetSheetText(1, 1);
	pCmdUI->Enable(!Text.IsEmpty());
}

void CTicketProcess::OnSetfocusProcessAssetEdit() 
{
	if(m_Asset.GetWindowTextLength() <= 0)
	{
		m_Asset.SetWindowText(NEWASSET);
		OnProcessFindAsset();
	}
}

void CTicketProcess::OnSelchangeProcessTranstypeCombo() 
{	
	OnSelchangeProcessTransdirCombo();
	UpdateTradeDates();
}

void CTicketProcess::OnSelchangeProcessTransdirCombo()
{
	CString TransType, Dir;

	m_TransType->GetSelString(TransType);
	if(TransType.IsEmpty())
		return;

	if(TransType != CALL && TransType != PUT)
		m_OptTicker.SetWindowText(EMPTYSTRING);

	m_Dir.GetSelString(Dir);
	TransType = GetData().DefineTransType(TransType, Dir);

	if(TransType == REPO || TransType == LEVERAGE || TransType == BORROW || TransType == LEND)
	{
		if(m_RateBasis->GetCurSel() < 0)
			m_RateBasis->SelectString(-1, "A/360");
	}

	ChangeShortLabel();

	EnableCtrls();

	ProcessVerifyRisk();
}

void CTicketProcess::OnSelchangeProcessRepocpCombo() 
{
	if(m_RepoCP->GetCurSel() < 0)
	{
		m_RepoCT.SetWindowText(EMPTYSTRING);
		m_RepoType->SetCurSel(-1);
	}

	EnableCtrls();
}

void CTicketProcess::OnProcessFindItem() 
{
	CFindDlg FindDlg;
	CSetConventionDlg SetDlg;
	CString CP;
	CQData QData;
	int CurrID;

	CurrID = m_nCurrID;
	m_nCurrID = 0;

	FindDlg.m_OraLoader = GetData().GetOraLoader();
	switch(CurrID)
	{
		case IDC_PROCESS_CT_EDIT:
			UpdateCPCT(m_CP, m_CT);
			break;
		case IDC_PROCESS_REPOCT_EDIT:
			UpdateCPCT(m_RepoCP, m_RepoCT);
			break;
		case IDC_PROCESS_ASSIGNCT_EDIT:
			UpdateCPCT(m_AssignCP, m_AssignCT);
			break;
		case IDC_PROCESS_SETCONVENTION_EDIT:
			m_Asset.GetWindowText(SetDlg.m_sAsset);
			SetDlg.m_sPortfolio.Empty();
			SetDlg.m_pData = &GetData();
			if(SetDlg.DoModal() == IDOK)
				m_SetConvention.SetWindowText(SetDlg.m_sSetConvention);
			break;
		case IDC_PROCESS_BINARY_EDIT:
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
			break;
		default:
			break;
	} 
}

void CTicketProcess::OnUpdateProcessFindItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_nCurrID == IDC_PROCESS_CT_EDIT && m_CP->GetCurSel() > 0) ||
					(m_nCurrID == IDC_PROCESS_REPOCT_EDIT && m_RepoCP->GetCurSel() > 0) || m_nCurrID > 0);
}

void CTicketProcess::OnProcessListedCheck() 
{
	CString Date, TransType;

	Date = m_TradeDate.GetData();
	if(Date != GetData().GetDate()) // Automatic change dates only when entry date is today
		return;

	m_TransType->GetSelString(TransType);

	m_ValueDate.SetData(GetData().GetOraLoader().GetValueDate(Date, TransType, m_Listed->GetCheck()));

	m_OrFee->ShowWindow(m_Listed->GetCheck() ? SW_SHOW : SW_HIDE);
}

void CTicketProcess::OnProcessOptionExp()
{
	COptExpDlg Dlg;

	Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
	if(Dlg.DoModal() == IDOK)
		m_OptExpDate.SetData(Dlg.m_Date);
}

void CTicketProcess::OnUpdateProcessOptionExp(CCmdUI* pCmdUI)
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	pCmdUI->Enable(TransType == CALL || TransType == PUT ? TRUE : FALSE);
}

void CTicketProcess::OnProcessContractConvertor()
{
	CContractConvertor Dlg;
	CQData QData;

	m_Asset.SetFocus();
	m_Asset.GetWindowText(Dlg.m_AssetStr);
	Dlg.m_OraLoader.SetDB(GetData().GetOraLoader().GetDB());
	if(Dlg.DoModal() == IDOK)
		m_Amount.SetWindowText(QData.WriteNumber(Dlg.m_dNominal));
}

void CTicketProcess::OnSetfocusAmountEdit()
{
	CString TransType;

	m_TransType->GetSelString(TransType);

	if((TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F")) && 
		m_Amount.GetWindowTextLength() == 0)
	{
		OnProcessContractConvertor();
		if(m_Amount.GetWindowTextLength() > 0)
			m_Amount.SetFocus();
	}
}

void CTicketProcess::OnUpdateProcessContractConvertor(CCmdUI* pCmdUI)
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	pCmdUI->Enable(TransType == CALL || TransType == PUT || (TransType == SECURITIES && m_Future == "F") ? TRUE : FALSE);
}

void CTicketProcess::OnSelchangeProcessCurrencyCombo() 
{
	CString Text;
	
	m_Currency->GetSelString(Text);
	if(Text.IsEmpty())
		return;
	
	if(Text == USD)
	{
		m_FxRate.SetWindowText("1");
		return;
	}
	
	CString Fxrate;

	m_FxRate.GetWindowText(Fxrate);
	if(Fxrate = "1")
		m_FxRate.SetWindowText(EMPTYSTRING);

	ProcessVerifyRisk();
}

void CTicketProcess::OnProcessXcurrency() 
{
	CXCurrencyDlg Dlg;

	Dlg.m_pData = &GetData();
	Dlg.m_UserClass = 2;
	Dlg.DoModal();
}

void CTicketProcess::OnEnSetfocusProcessSetconventionEdit()
{
	m_nCurrID = IDC_PROCESS_SETCONVENTION_EDIT;
}

void CTicketProcess::OnEnSetfocusProcessCtEdit()
{
	m_nCurrID = IDC_PROCESS_CT_EDIT;
}

void CTicketProcess::OnEnSetfocusProcessRepoctEdit() 
{
	m_nCurrID = IDC_PROCESS_REPOCT_EDIT;
}

void CTicketProcess::OnEnSetfocusProcessAssignctEdit()
{
	m_nCurrID = IDC_PROCESS_ASSIGNCT_EDIT;
}

void CTicketProcess::OnProcessFxreflink()
{
	CFxSource Dlg;
	CString Dir, TransType;
	CQData QData;

	m_TransType->GetSelString(TransType);
//	if(TransType != FOREX || (TransType == FOREX && m_Asset.GetWindowTextLength() > 0))
//		return;

	Dlg.m_pData = &GetData();
	Dlg.m_sTicket = m_Ticket.GetData();
	m_Currency->GetSelString(Dlg.m_sCurrency);
	m_ValueDate.GetWindowText(Dlg.m_sValueDate);
	m_FxRate.GetWindowText(Dlg.m_sFxrate);
	
	m_Dir.GetSelString(Dir);
	m_Amount.GetWindowText(Dlg.m_sNomAmount);
	if(Dir == S)
		Dlg.m_sNomAmount = MINUS + Dlg.m_sNomAmount;
	Dlg.m_sNomAmount = QData.RemoveComma(Dlg.m_sNomAmount);

	m_Data.GetFxRecArray().RemoveAll();
	if(Dlg.DoModal() == IDOK)
		m_Data.GetFxRecArray().Copy(Dlg.m_RecArray);
}

void CTicketProcess::OnBnClickedSecCheck()
{
	UpdateCash();
}

void CTicketProcess::OnEnChangeProcessAssetEdit()
{
	UpdateAsset();
}

void CTicketProcess::OnEnKillfocusProcessPriceEdit()
{
	UpdateCash();
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Price.SetFocus();
}

void CTicketProcess::OnEnKillfocusProcessFxrateEdit()
{
	UpdateCash();
}

void CTicketProcess::OnEnKillfocusProcessBrokerFeeEdit()
{
	UpdateCash();
}

void CTicketProcess::OnEnKillfocusProcessOtherFeeEdit()
{
	UpdateCash();
}

void CTicketProcess::OnBnClickedProcessSecfeeCheck()
{
	double Price, OtherFee;
	CQData QData;
	CString sPrice;

	m_Price.GetWindowText(sPrice);
	Price = atof(QData.RemoveComma(sPrice));

	OtherFee = m_Data.ComputeOtherFee(Price, m_SecFee->GetCheck(), m_OrFee->GetCheck());
	m_OtherFee.SetWindowText(QData.WriteNumber(OtherFee, TRUE, 2));

	UpdateCash();
}

void CTicketProcess::OnBnClickedProcessOrfeeCheck()
{
	OnBnClickedProcessSecfeeCheck();
}

void CTicketProcess::OnEnChangeProcessUnwindTicketEdit()
{
	CString OptTicket, Ticket;

	Ticket = m_Ticket.GetData();
	OptTicket = m_OptTicket.GetData();
	if(m_SS.GetSheetCurRow() > 0 && OptTicket != m_Data.GetRawTicket().GetUnWindTicket())
	{
		m_Data.ReLoadAllocList(Ticket, OptTicket);
		m_Data.GetAllocList().CopyDataToDBSheet(Ticket, m_AllocSS);
	}
}

void CTicketProcess::OnEnSetfocusProcessBinaryEdit()
{
	CString TransType;

	m_TransType->GetSelString(TransType);

	if(TransType == CALL || TransType == PUT)
		m_nCurrID = IDC_PROCESS_BINARY_EDIT;
}

void CTicketProcess::OnProcessForbiddenList()
{
	CProhibitList Dlg;

	Dlg.m_OraLoader = GetData().GetOraLoader();
	Dlg.DoModal();
}

void CTicketProcess::OnProcessAutoProcessTrade()
{
	CQData QData;
	CODBCDB DB;
	CStringArray Tickets;
	CString Data, Ticket, Path = "Excel Files (*.xls*) || ";
	int Count = 0;
	LPCTSTR Template = "Template";
	LPCTSTR XLS = "XLS";

	if(!QData.GetFileName(AfxGetApp(), Path, Template, "AUTO PROCESSING", XLS))
		return;
	
	if(!DB.Open(Path))
		return;

	CRecordset DBRec((CDatabase *) &DB);

	DBRec.Open(CRecordset::forwardOnly, "SELECT TICKET_NUM FROM MATCHED_TICKETS "
				"WHERE TICKET_NUM IS NOT NULL ORDER BY 1 ", CRecordset::readOnly);

	while(!DBRec.IsEOF())
	{
		DBRec.GetFieldValue((short) 0, Data);
		if(!Data.IsEmpty())
		{
			Data = QData.WriteNumber(Data, FALSE, 0);
			Ticket = QData.GetQueryNumber(Data);

			if(AutoProcessTrade(Ticket))
			{
				Tickets.Add(Data);
				Count++;
			}
		}
		DBRec.MoveNext();
	}

	DBRec.Close();

	Ticket.Format("Total %d Tickets Processed. ", Count);
	for(int i = 0; i < Tickets.GetCount(); i++)
	{
		if(i > 0)
			Ticket += ", ";
		Ticket += Tickets.GetAt(i);
	}

	MessageBox(Ticket, "Ticket Process");
	OnProcessLoad();
}

void CTicketProcess::OnCbnSelchangeProcessAaCombo()
{
	CQData QData;
	COraLoader OraLoader;
	CString sPortfolio, sMethod, sAmount, sTicket, Text;

	if(m_Data.GetRawTicket().GetOriNominal().IsEmpty() || m_AAMethod.GetCurSel() < 0)
		return;

	ProcessVerifyRisk();
	sAmount = QData.RemoveComma(m_Data.GetRawTicket().GetOriNominal());
	m_AAMethod.GetSelString(sMethod);
	sMethod = QData.GetQueryText(sMethod);

	OraLoader = GetData().GetOraLoader();

	m_Ticket.GetWindowText(sTicket);
	sTicket = QData.GetQueryNumber(sTicket);

	OraLoader.Open("SELECT PORTFOLIO FROM SEMAM.NW_RAW_TICKET_DETAIL WHERE ROWNUM = 1 AND TICKET_NUM = " + sTicket);
	OraLoader.LoadTextString(sPortfolio);

	if(sPortfolio.IsEmpty())
		return;

	sPortfolio = QData.GetQueryText(sPortfolio);
	OraLoader.Open("SELECT ROUND(" + sAmount + "/ALLOC*100, 0) FROM SEMAM.NW_AA_ALLOC A "
				"WHERE FROM_DATE <= TRUNC(SYSDATE) "
				"AND NVL(TO_DATE, TRUNC(SYSDATE) + 1) > TRUNC(SYSDATE) "
				"AND AA_METHOD = " + sMethod + 
				"AND A.PORTFOLIO = " + sPortfolio);
	OraLoader.LoadTextString(sAmount);

	if(sAmount.IsEmpty())
		return;

	Text = "Total Nominal: " + sAmount + "\n";

	sAmount = QData.GetQueryNumber(sAmount);
	OraLoader.Open("SELECT PORTFOLIO, ROUND(" + sAmount + "*ALLOC/100, 0) FROM SEMAM.NW_AA_ALLOC A "
						"WHERE FROM_DATE <= TRUNC(SYSDATE) "
						"AND NVL(TO_DATE, TRUNC(SYSDATE) + 1) > TRUNC(SYSDATE) "
						"AND AA_METHOD = " + sMethod);
	
	OraLoader.MoveFirst();
	while(!OraLoader.IsEOF())
	{
		Text += OraLoader.GetDBString(0) + ": ";
		Text += OraLoader.GetDBString(1) + "\n";
		OraLoader.MoveNext();
	}

	Text += "Do you want to keep these allocation (Yes/No)?";
	if(MessageBox(Text, NULL, MB_YESNO) != IDYES)
	{
		m_AAMethod.SetCurSel(-1);
		return;
	}

	OraLoader.ExecuteSql("DELETE SEMAM.NW_RAW_TICKET_DETAIL WHERE TICKET_NUM = " + sTicket);

	OraLoader.ExecuteSql("INSERT INTO SEMAM.NW_RAW_TICKET_DETAIL (TICKET_NUM, PORTFOLIO, NOM_AMOUNT) "
						"SELECT " + sTicket + ", PORTFOLIO, ROUND(" + sAmount + "*ALLOC/100, 0) "
						"FROM SEMAM.NW_AA_ALLOC A "
						"WHERE FROM_DATE <= TRUNC(SYSDATE) "
						"AND NVL(TO_DATE, TRUNC(SYSDATE) + 1) > TRUNC(SYSDATE) "
						"AND AA_METHOD = " + sMethod);
	
	m_Amount.SetWindowText(sAmount);
	OraLoader.ExecuteSql("UPDATE SEMAM.NW_RAW_TICKETS SET NOM_AMOUNT = " + sAmount + 
						 "WHERE TICKET_NUM = " + sTicket);
	
	m_Data.LoadAllocList();
	OnDblClickProcessTicketList(1, m_SS.GetSheetCurRow());
}

void CTicketProcess::OnCbnKillfocusProcessAaCombo()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_AAMethod.SetFocus();
}

void CTicketProcess::OnEnChangeProcessAmountEdit()
{
	if(m_Amount.GetModify())
		ProcessVerifyRisk();
}


void CTicketProcess::OnEnKillfocusProcessAmountEdit()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Amount.SetFocus();
}


void CTicketProcess::OnEnChangeProcessPriceEdit()
{
	if(m_Price.GetModify())
		ProcessVerifyRisk();
}

void CTicketProcess::OnEnKillfocusProcessAssetEdit()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Asset.SetFocus();
}



