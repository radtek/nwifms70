// TicketApproval.cpp : implementation file
//
#include "stdafx.h"
#include "NWIFMS70.h"
#include "TicketApproval.h"
#include "nwivar.h"
#include "qdata.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTicketApproval

IMPLEMENT_DYNCREATE(CTicketApproval, CFormView)

CTicketApproval::CTicketApproval() : CFormView(CTicketApproval::IDD)
{
	//{{AFX_DATA_INIT(CTicketApproval)
	//}}AFX_DATA_INIT
}

void CTicketApproval::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTicketApproval)
	DDX_Control(pDX, IDC_APPROVAL_TICKET_LIST, m_SS);
	DDX_Control(pDX, IDC_APPROVAL_ALLOCATION_LIST, m_AllocSS);
	DDX_Control(pDX, IDC_APPROVAL_POSITION_LIST, m_PositionSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTicketApproval, CFormView)
	//{{AFX_MSG_MAP(CTicketApproval)
	ON_BN_CLICKED(IDC_APPROVAL_SIGN_BUTTON, OnApprovalSignButton)
	ON_COMMAND(ID_APPROVAL_LOAD, OnApprovalLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTicketApproval diagnostics

#ifdef _DEBUG
void CTicketApproval::AssertValid() const
{
	CFormView::AssertValid();
}

void CTicketApproval::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CTicketApproval::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

void CTicketApproval::InitControls()
{
	m_SS.SetVisibleRows(7);
	m_SS.SetVisibleCols(12);
	m_SS.SetMaxRows(500);

	m_AllocSS.SetVisibleRows(3);
	m_AllocSS.SetVisibleCols(8);
	m_PositionSS.SetVisibleRows(3);
	m_PositionSS.SetVisibleCols(7);

	m_Listed.Setup(this, IDC_APPROVAL_LISTED_CHECK, Y);
	m_EuroOpt.Setup(this, IDC_APPROVAL_EUROOPT_CHECK, Y);
	m_TRS.Setup(this, IDC_APPROVAL_DEAL_CHECK, TRS);
	m_IPO.Setup(this, IDC_APPROVAL_IPO_CHECK, Y);
	m_WI.Setup(this, IDC_APPROVAL_WI_CHECK, Y);
	m_Funded.Setup(this, IDC_APPROVAL_FUNDED_CHECK, Y);
	m_OptAuto.Setup(this, IDC_APPROVAL_AUTO_CHECK, Y);
	m_ShortSale.Setup(this, IDC_APPROVAL_SHORT_CHECK, Y);
	m_ETrade.Setup(this, IDC_APPROVAL_ETRADE_EDIT);

	m_Sign.Setup(this, IDC_APPROVAL_SIGN_EDIT);
	m_Ticket.Setup(this, IDC_APPROVAL_TICKET_EDIT, NULL, 0, FALSE); 
	m_Trader.Setup(this, IDC_APPROVAL_TRADER_EDIT);
	m_TradeDate.Setup(this, IDC_APPROVAL_TRADEDATE_EDIT);
	m_ExecTime.Setup(this, IDC_APPROVAL_EXEC_TIME);
	m_DateStamp.Setup(this, IDC_APPROVAL_DATESTAMP_EDIT);
	m_ValueDate.Setup(this, IDC_APPROVAL_VALUEDATE_EDIT);
	m_Dir.Setup(this, IDC_APPROVAL_DIR_EDIT);
	m_TransType.Setup(this, IDC_APPROVAL_TRANSTYPE_EDIT);
	m_Amount.Setup(this, IDC_APPROVAL_AMOUNT_EDIT);
	m_Strike.Setup(this, IDC_APPROVAL_STRIKE_EDIT);
	m_OptExp.Setup(this, IDC_APPROVAL_EXPIRATION_EDIT);
	m_OptSet.Setup(this, IDC_APPROVAL_OPTSET_EDIT);
	m_OptSet2.Setup(this, IDC_APPROVAL_OPTSET2_EDIT);
	m_OptTicker.Setup(this, IDC_APPROVAL_OPT_TICKER_EDIT);
	m_OptID.Setup(this, IDC_APPROVAL_OPTID_EDIT);
	m_CP.Setup(this, IDC_APPROVAL_COUNTERPARTY_EDIT);
	m_Currency.Setup(this, IDC_APPROVAL_CURRENCY_EDIT);
	m_Fxrate.Setup(this, IDC_APPROVAL_FXRATE_EDIT);
	m_RepoCP.Setup(this, IDC_APPROVAL_REPOCP_EDIT);
	m_Rate.Setup(this, IDC_APPROVAL_RATE_EDIT);
	m_RateBasis.Setup(this, IDC_APPROVAL_RATE_BASIS_EDIT);
	m_Maturity.Setup(this, IDC_APPROVAL_MATURITY_EDIT);
	m_SWBooking.Setup(this, IDC_APPROVAL_SWBOOKING_EDIT);
	m_SWMaturity.Setup(this, IDC_APPROVAL_SWMATURITY_EDIT);
	m_FloatRate.Setup(this, IDC_APPROVAL_FLOAT_RATE_EDIT);
	m_FloatBasis.Setup(this, IDC_APPROVAL_FLOAT_RATE_BASIS_EDIT);
	m_FloatMaturity.Setup(this, IDC_APPROVAL_FLOAT_MATURITY_EDIT);
	m_Price.Setup(this, IDC_APPROVAL_PRICE_EDIT);
	m_GiveupCP.Setup(this, IDC_APPROVAL_GIVEUPCP_EDIT);
	m_Note.Setup(this, IDC_APPROVAL_NOTE_EDIT);
	m_Note2.Setup(this, IDC_APPROVAL_NOTE2_EDIT);
	m_SNote.Setup(this, IDC_APPROVAL_SNOTE_EDIT);
	m_SNote.LimitText(80);
	m_AAMethod.Setup(this, IDC_APPROVAL_AAMETHOD_EDIT);
	m_AAFReason.Setup(this, IDC_APPROVAL_AAFREASON_EDIT);
	m_BestExecution.Setup(this, IDC_APPROVAL_BESTEXECUTION_EDIT);
	m_Booker.Setup(this, IDC_APPROVAL_BOOKER_EDIT);
	m_Asset.Setup(this, IDC_APPROVAL_ASSET_EDIT);
	m_AssetDesc.Setup(this, IDC_APPROVAL_ASSETDESC_EDIT);
	m_AssetCurrency.Setup(this, IDC_APPROVAL_ASSETCURRENCY_EDIT);
	m_Country.Setup(this, IDC_APPROVAL_COUNTRY_EDIT);
	m_Industry.Setup(this, IDC_APPROVAL_INDUSTRY_EDIT);
	m_Common.Setup(this, IDC_APPROVAL_COMMON_EDIT);
	m_Cusip.Setup(this, IDC_APPROVAL_MSTC_EDIT);
	m_Isin.Setup(this, IDC_APPROVAL_ISIN_EDIT);
	m_Sedol.Setup(this, IDC_APPROVAL_SEDOL_EDIT);
	m_Class.Setup(this, IDC_APPROVAL_CLASS_EDIT);
	m_Category.Setup(this, IDC_APPROVAL_CATEGORY_EDIT);
	m_Bucket.Setup(this, IDC_APPROVAL_BUCKET_EDIT);
	m_Coupon.Setup(this, IDC_APPROVAL_COUPON_EDIT);
	m_AssetMaturity.Setup(this, IDC_APPROVAL_ASSETMATURITY_EDIT);
	m_Ratio.Setup(this, IDC_APPROVAL_RATIO_EDIT);
	m_VARStatus.Setup(this, IDC_APPROVAL_VAR_STATUS_EDIT);
	m_Binary.Setup(this, IDC_APPROVAL_BINARY_EDIT);
	m_IMFxrate.Setup(this, IDC_APPROVAL_IMFX_EDIT);
	m_IMFxrate.SetReadOnly();

	m_PrevVAR.Setup(this, IDC_APPROVAL_PREV_VAR_EDIT, NULL, 2);
	m_CurrVAR.Setup(this, IDC_APPROVAL_CURR_VAR_EDIT, NULL, 2);
	m_VARLimit.Setup(this, IDC_APPROVAL_VAR_LIMIT_EDIT, NULL, 2);

	m_Data.Add(&m_Sign, &m_Data.GetTicket().GetSign());
	m_Data.Add(&m_Trader, &m_Data.GetTicket().GetTrader());
	m_Data.Add(&m_Ticket, &m_Data.GetTicket().GetTicket());
	m_Data.Add(&m_TradeDate, &m_Data.GetTicket().GetTradeDate());
	m_Data.Add(&m_ExecTime, &m_Data.GetTicket().GetExecTime());
	m_Data.Add(&m_DateStamp);
	m_Data.Add(&m_ValueDate, &m_Data.GetTicket().GetValueDate());
	m_Data.Add(&m_Dir, &m_Data.GetTicket().GetDir());
	m_Data.Add(&m_TransType, &m_Data.GetTicket().GetTransType());
	m_Data.Add(&m_Amount, &m_Data.GetTicket().GetNomAmount());
	m_Data.Add(&m_Strike, &m_Data.GetTicket().GetStrike());
	m_Data.Add(&m_OptExp, &m_Data.GetTicket().GetOptExp());
	m_Data.Add(&m_Listed, &m_Data.GetTicket().GetListed());
	m_Data.Add(&m_OptTicker, &m_Data.GetTicket().GetOptTicker());
	m_Data.Add(&m_OptSet, &m_Data.GetTicket().GetOptSetCode());
	m_Data.Add(&m_OptSet2, &m_Data.GetTicket().GetOptSetCode2());
	m_Data.Add(&m_OptID, &m_Data.GetTicket().GetOptID());
	m_Data.Add(&m_EuroOpt, &m_Data.GetTicket().GetEuroOpt());
	m_Data.Add(&m_OptAuto, &m_Data.GetTicket().GetOptAuto());
	m_Data.Add(&m_Currency, &m_Data.GetTicket().GetCurrency());
	m_Data.Add(&m_Fxrate, &m_Data.GetTicket().GetFxRate());
	m_Data.Add(&m_CP, &m_Data.GetTicket().GetCP());
	m_Data.Add(&m_Asset, &m_Data.GetTicket().GetAsset());
	m_Data.Add(&m_AssetDesc, &m_Data.GetTicket().GetAssetDesc());
	m_Data.Add(&m_AssetCurrency, &m_Data.GetTicket().GetAssetCurr());
	m_Data.Add(&m_Country, &m_Data.GetTicket().GetAssetCountry());
	m_Data.Add(&m_Common, &m_Data.GetTicket().GetAssetCommon());
	m_Data.Add(&m_Cusip, &m_Data.GetTicket().GetAssetCusip());
	m_Data.Add(&m_Isin, &m_Data.GetTicket().GetAssetIsin());
	m_Data.Add(&m_Sedol, &m_Data.GetTicket().GetAssetSedol());
	m_Data.Add(&m_Class, &m_Data.GetTicket().GetAssetClass());
	m_Data.Add(&m_Industry, &m_Data.GetTicket().GetAssetIndustry());
	m_Data.Add(&m_Category, &m_Data.GetTicket().GetAssetCategory());
	m_Data.Add(&m_Bucket, &m_Data.GetTicket().GetAssetBucket());
	m_Data.Add(&m_Coupon, &m_Data.GetTicket().GetAssetCoupon());
	m_Data.Add(&m_AssetMaturity, &m_Data.GetTicket().GetAssetMaturity());
	m_Data.Add(&m_IPO, &m_Data.GetTicket().GetIPO());
	m_Data.Add(&m_TRS, &m_Data.GetTicket().GetTRS());
	m_Data.Add(&m_Funded, &m_Data.GetTicket().GetFunded());
	m_Data.Add(&m_Binary, &m_Data.GetTicket().GetBinary());
	m_Data.Add(&m_RepoCP, &m_Data.GetTicket().GetRepoCP());
	m_Data.Add(&m_Rate, &m_Data.GetTicket().GetRate());
	m_Data.Add(&m_RateBasis, &m_Data.GetTicket().GetRateBasis());
	m_Data.Add(&m_Maturity, &m_Data.GetTicket().GetMaturity());
	m_Data.Add(&m_Price, &m_Data.GetTicket().GetPrice());
	m_Data.Add(&m_GiveupCP, &m_Data.GetTicket().GetAssignCP());
	m_Data.Add(&m_Note, &m_Data.GetTicket().GetNote());
	m_Data.Add(&m_Note2, &m_Data.GetTicket().GetNote2());
	m_Data.Add(&m_AAMethod, &m_Data.GetTicket().GetAAMethod());
	m_Data.Add(&m_AAFReason, &m_Data.GetTicket().GetAAFReason());
	m_Data.Add(&m_SWBooking, &m_Data.GetTicket().GetSWBooking());
	m_Data.Add(&m_SWMaturity, &m_Data.GetTicket().GetSWMaturity());
	m_Data.Add(&m_FloatRate, &m_Data.GetTicket().GetSWFloatRate());
	m_Data.Add(&m_FloatBasis, &m_Data.GetTicket().GetSWFloatBasis());
	m_Data.Add(&m_FloatMaturity, &m_Data.GetTicket().GetSWFloatMaturity());
	m_Data.Add(&m_WI, &m_Data.GetTicket().GetWI());
	m_Data.Add(&m_Booker, &m_Data.GetTicket().GetBroker());
	m_Data.Add(&m_BestExecution, &m_Data.GetTicket().GetBestExecution());
	m_Data.Add(&m_ShortSale, &m_Data.GetTicket().GetShortSale());
	m_Data.Add(&m_ETrade, &m_Data.GetTicket().GetETrade());
	m_Data.Add(&m_Data.GetTicket().GetOrderID());
	m_Data.Add(&m_Data.GetTicket().GetCPTradeID());
	m_Data.Add(&m_Data.GetTicket().GetAssetClass());
	m_Data.Add(&m_Rev);
	m_Data.Add(&m_Ratio);
}

void CTicketApproval::ImpliedFxrate()
{
	CQData QData;

	m_IMFxrate.SetData(QData.GetImpliedFxrate(m_Data.GetTicket().GetAssetClass(), m_Currency.GetData(), m_Rev, m_Price.GetData()));
}

void CTicketApproval::LoadSNote()
{
	if(m_SS.GetSheetCurRow() > 0)
	{
		COraLoader OraLoader;

		OraLoader.SetDB(&theDB);
		OraLoader.Open("SELECT NOTE FROM SEMAM.NW_RAW_TICKET_OPINION WHERE TICKET_NUM = " + m_Ticket.GetData());
		OraLoader.LoadText(m_SNote);
	}
	else
		m_SNote.SetData(EMPTYSTRING);
}

BOOL CTicketApproval::UpdateData(BOOL bSaveAndValid)
{
	BeginWaitCursor();

	if(bSaveAndValid)
	{		
		CString Ticket, SNote;
		COraLoader OraLoader;
		BOOL bOK;
		int Pos;
		
		OraLoader.SetDB(&theDB);
		Ticket = m_Ticket.GetData();
		m_Data.GetTicket().SetSign(m_Sign.GetWindowTextLength() > 0 ? EMPTYSTRING : GetData().GetUser());
		
		Pos = Ticket.Find(MINUS, 0);
		if(Pos >= 0)
		{
			if(m_Data.GetTicket().GetSign().IsEmpty())
				OraLoader.GetSql().Format("UPDATE SEMAM.NW_MATURITY_TRADE SET SIGN = NULL, SIGN_DATE = NULL "
											"WHERE INV_NUM = %s", (LPCTSTR) Ticket.Right(Ticket.GetLength() - Pos - 1));
			else
				OraLoader.GetSql().Format("UPDATE SEMAM.NW_MATURITY_TRADE SET SIGN = '%s', SIGN_DATE = SYSDATE "
											"WHERE INV_NUM = %s", (LPCTSTR) m_Data.GetTicket().GetSign(),
											(LPCTSTR) Ticket.Right(Ticket.GetLength() - Pos - 1));

			bOK = OraLoader.ExecuteSql(OraLoader.GetSql());
			if(bOK)
				m_Sign.SetData(m_Data.GetTicket().GetSign());
		}
		else
		{
			SNote = m_SNote.GetData();
			bOK = m_Data.ApprovalTicket(SNote);
			if(bOK)
				m_Sign.SetData(m_Data.GetTicket().GetSign());
		}
		
		if(m_SS.GetSheetCurRow() > 0 && bOK)
			m_SS.SetSheetText(1, m_SS.GetSheetCurRow(), m_Data.GetTicket().GetSign());
	}
	else
	{
		CQData QData;

		m_Data.LoadTickets();
		m_Data.LoadAllocList();

		SetDlgItemText(IDC_APPROVAL_TOTAL_EDIT, QData.WriteNumber(m_SS.GetSheetRows() >= 0 ? m_SS.GetSheetRows() : 0));
	}

	EndWaitCursor();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CTicketApproval message handlers
void CTicketApproval::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BOOL bTrader;

	InitControls();
	GetData().LoadDates();
	if(GetData().IsSupervisor() || GetData().IsOperation() || GetData().IsGuest())
		bTrader = FALSE;
	else
		bTrader = TRUE;

	m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_AllocSS, GetData().GetBlotter(), bTrader, GetData().GetSign(), 2);
	UpdateData(FALSE);
	GetDlgItem(IDC_APPROVAL_SIGN_BUTTON)->ShowWindow(GetData().GetBlotter() ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_APPROVAL_SNOTE_STATIC)->ShowWindow(GetData().GetBlotter() || GetData().GetSign() ? SW_HIDE : SW_SHOW);
	m_SNote.ShowWindow(GetData().GetBlotter() || GetData().GetSign() ? SW_HIDE : SW_SHOW);
	if(GetData().GetBlotter())
		GetDocument()->SetViewTitle("TICKET BLOTTER");
	else
		if(GetData().GetSign())
			GetDocument()->SetViewTitle("TICKET SIGN");
		else
			GetDocument()->SetViewTitle("TICKET APPROVAL");

	m_Data.Refresh();
}

BEGIN_EVENTSINK_MAP(CTicketApproval, CFormView)
    //{{AFX_EVENTSINK_MAP(CTicketApproval)
	ON_EVENT(CTicketApproval, IDC_APPROVAL_TICKET_LIST, 11 /* DblClick */, OnDblClickApprovalTicketList, VTS_I4 VTS_I4)
	ON_EVENT(CTicketApproval, IDC_APPROVAL_ALLOCATION_LIST, 11, CTicketApproval::OnDblClickApprovalAllocationList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTicketApproval::OnApprovalSignButton() 
{
	UpdateData(TRUE);
}

void CTicketApproval::OnDblClickApprovalTicketList(long Col, long Row) 
{
	CString Asset, Dir;

	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row, m_Data.GetAllocList());
		
		Asset = m_Asset.GetData();
		if(m_CurrAsset != Asset)
		{
			m_CurrAsset = Asset;
			if(m_CurrAsset != "NEW ASSET")
			{
				BeginWaitCursor();
				m_Data.LoadPositionList(m_CurrAsset, m_PositionSS);
				EndWaitCursor();
			}
			else
				m_PositionSS.ClearSheet();
		}
	}
	else
	{
		m_Data.SetCurrentRow(-1);
		m_PositionSS.ClearSheet();
	}

	Dir = m_Dir.GetData();
	if(Dir == P)
		m_ShortSale.SetWindowText("CShort");
	else
		if(Dir == S)
			m_ShortSale.SetWindowText("SShort");

	GetDlgItem(IDC_APPROVAL_SIGN_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0);
	if(m_SS.GetSheetCurRow() > 0 && !(GetData().GetBlotter() || GetData().GetSign()))
		LoadSNote();

	ImpliedFxrate();
}

void CTicketApproval::OnApprovalLoad() 
{
	m_Data.Refresh();

	UpdateData(FALSE);	
	GetDlgItem(IDC_APPROVAL_SIGN_BUTTON)->EnableWindow(FALSE);
}

void CTicketApproval::OnDblClickApprovalAllocationList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_AllocSS.GetSheetRows())
	{
		CString Portfolio, Category;
		CStringArray Rec;
		double VAR;
		CQData QData;

		m_Data.GetSRowCtrl().SetCurrentRow(Row);

		Portfolio = m_AllocSS.GetSheetText(1, Row);
		Category = m_Category.GetData();
		VAR = atof(QData.RemoveComma(m_AllocSS.GetSheetText(8, Row)));
		if(m_Data.LoadVARs(Rec, Portfolio, Category, VAR) > 0)
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
	else
		m_Data.GetSRowCtrl().SetCurrentRow(-1);
}
