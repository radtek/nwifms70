// CashForm.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "CashForm.h"
#include "assetdlg.h"
#include "shortcash.h"
#include "coupondlg.h"
#include "odbcdb.h"
#include "errorentrylinkdlg.h"
#include "qdata.h"
#include "cashirsiddlg.h"
#include "oradate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCashForm

IMPLEMENT_DYNCREATE(CCashForm, CFormView)

CCashForm::CCashForm() : CFormView(CCashForm::IDD)
{
	//{{AFX_DATA_INIT(CCashForm)
	m_bFind = FALSE;
	m_Fund = "";
	//}}AFX_DATA_INIT
}

void CCashForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCashForm)
	DDX_Control(pDX, IDC_CASH_LIST, m_SS);
	DDX_Control(pDX, IDC_CASH_MAP_LIST, m_InvSS);
	DDX_Control(pDX, IDC_CASH_TRADE_LIST, m_DataSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCashForm, CFormView)
	//{{AFX_MSG_MAP(CCashForm)
	ON_COMMAND(ID_CASH_ADD, OnCashAdd)
	ON_COMMAND(ID_CASH_CLEAR, OnCashClear)
	ON_COMMAND(ID_CASH_DELETE, OnCashDelete)
	ON_COMMAND(ID_CASH_FIND, OnCashFind)
	ON_COMMAND(ID_CASH_FIND_ASSET, OnCashFindAsset)
	ON_COMMAND(ID_CASH_LOAD, OnCashLoad)
	ON_COMMAND(ID_CASH_LOAD_CASH, OnCashLoadCash)
	ON_COMMAND(ID_CASH_REFRESH, OnCashRefresh)
	ON_COMMAND(ID_CASH_UPDATE, OnCashUpdate)
	ON_UPDATE_COMMAND_UI(ID_CASH_ADD, OnUpdateCashAdd)
	ON_UPDATE_COMMAND_UI(ID_CASH_DELETE, OnUpdateCashDelete)
	ON_UPDATE_COMMAND_UI(ID_CASH_UPDATE, OnUpdateCashUpdate)
	ON_COMMAND(ID_CASH_SHORT_CASH, OnCashShortCash)
	ON_UPDATE_COMMAND_UI(ID_CASH_FIND, OnUpdateCashFind)
	ON_CBN_SELCHANGE(IDC_CASH_PORTFOLIO_COMBO, OnSelchangeCashPortfolioCombo)
	ON_UPDATE_COMMAND_UI(ID_CASH_LOAD_COUPON, OnUpdateCashLoadCoupon)
	ON_COMMAND(ID_CASH_LOAD_COUPON, OnCashLoadCoupon)
	ON_UPDATE_COMMAND_UI(ID_CASH_SWAP, OnUpdateCashSwap)
	ON_COMMAND(ID_CASH_SWAP, OnCashSwap)
	ON_UPDATE_COMMAND_UI(ID_CASH_ERRORLINK, OnUpdateCashErrorlink)
	ON_COMMAND(ID_CASH_ERRORLINK, OnCashErrorlink)
	ON_UPDATE_COMMAND_UI(ID_CASH_LOAD, OnUpdateCashLoad)
	ON_UPDATE_COMMAND_UI(ID_CASH_LOAD_CASH, OnUpdateCashLoadCash)
	ON_CBN_SELCHANGE(IDC_CASH_TYPE_COMBO, OnSelchangeCashTypeCombo)
	ON_EN_CHANGE(IDC_CASH_AMOUNT_EDIT, OnChangeCashAmountEdit)
	ON_COMMAND(ID_CASH_LOADCDSCOUPON, OnCashLoadcdscoupon)
	ON_UPDATE_COMMAND_UI(ID_CASH_LOADCDSCOUPON, OnUpdateCashLoadcdscoupon)
	ON_UPDATE_COMMAND_UI(ID_CASH_CLEAR, OnUpdateCashClear)
	ON_UPDATE_COMMAND_UI(ID_CASH_FIND_ASSET, OnUpdateCashFindAsset)
	ON_UPDATE_COMMAND_UI(ID_CASH_REFRESH, OnUpdateCashRefresh)
	ON_UPDATE_COMMAND_UI(ID_CASH_SHORT_CASH, OnUpdateCashShortCash)
	ON_COMMAND(ID_CASH_FIND_EXACT, OnCashFindExact)
	ON_UPDATE_COMMAND_UI(ID_CASH_FIND_EXACT, OnUpdateCashFindExact)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CASH_LOADIRSCOUPON, &CCashForm::OnCashLoadirscoupon)
	ON_UPDATE_COMMAND_UI(ID_CASH_LOADIRSCOUPON, &CCashForm::OnUpdateCashLoadirscoupon)
	ON_COMMAND(ID_CASH_UPLOADCASHENTRY, &CCashForm::OnCashUploadcashentry)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCashForm diagnostics

#ifdef _DEBUG
void CCashForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CCashForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CCashForm::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNWIFMS70Doc)));
	return (CNWIFMS70Doc*)m_pDocument;
}
#endif //_DEBUG

void CCashForm::EnableControls()
{
	BOOL bEnabled;
	int UserClass;
	CString Text, PmntType;

	if(!m_bFind)
		m_Portfolio.EnableWindow(!m_CashNum.GetData().IsEmpty() ? FALSE: TRUE);
	else
		m_Portfolio.EnableWindow(TRUE);

	if(!m_bFind && m_Data.IsLoaded() && m_Data.IsCoupon() && m_CashAssetCode.IsEmpty()) // Coupon Pickup from view cannot be edited.
	{
		bEnabled = FALSE;
		m_CashNum.EnableWindow(bEnabled);
		m_PostDate.EnableWindow(bEnabled);
		m_Portfolio.EnableWindow(bEnabled);
		m_Account.EnableWindow(bEnabled);
		m_Currency.EnableWindow(bEnabled);
		m_Amount.EnableWindow(bEnabled);
		m_PayType.EnableWindow(bEnabled);
		m_Asset.EnableWindow(bEnabled);
		m_CloseDate.EnableWindow(bEnabled);
		m_Fxrate.EnableWindow(bEnabled);
		m_Remaining.EnableWindow(bEnabled);
		m_DiffReason.EnableWindow(bEnabled);
		m_Note.EnableWindow(bEnabled);
		m_PayID.EnableWindow(bEnabled);
		m_CustRef.EnableWindow(bEnabled);
		m_Link.EnableWindow(bEnabled);
		m_IRSID.EnableWindow(bEnabled);
		m_AccountTo.EnableWindow(bEnabled);
		m_PortfolioTo.EnableWindow(bEnabled);
		m_ToDate.EnableWindow(bEnabled);
		m_CouponPaid.EnableWindow(bEnabled);
		return;
	}

	UserClass = atoi(m_UserClass);

	if(m_bFind)
	{
		m_CashNum.EnableWindow(TRUE);
		m_PostDate.EnableWindow(TRUE);
		bEnabled = TRUE;
	}
	else
	{
		m_CashNum.EnableWindow(FALSE);
		
		if(GetData().IsPowerUser() || GetData().IsOperation())
		{
			m_PostDate.EnableWindow(TRUE);
			bEnabled = TRUE;
		}
		else
		{
			m_PostDate.EnableWindow(FALSE);
			bEnabled = UserClass > 1 ? FALSE : TRUE;
		}
	}

//	m_Portfolio->EnableWindow(bEnabled);
	m_Account.EnableWindow(bEnabled);
	m_Currency.EnableWindow(bEnabled);
	m_Amount.EnableWindow(bEnabled);

	if(m_bFind)
		bEnabled = TRUE;
	else
		if(!GetData().IsPowerUser())
			bEnabled = UserClass > 2 ? FALSE : TRUE;
		
	m_PayType.EnableWindow(bEnabled);
	m_Asset.EnableWindow(bEnabled);
	m_CloseDate.EnableWindow(bEnabled);
	m_Fxrate.EnableWindow(bEnabled);
	m_Remaining.EnableWindow(bEnabled);
	m_DiffReason.EnableWindow(bEnabled);
	m_Note.EnableWindow(bEnabled);
	m_PayID.EnableWindow(bEnabled);
	m_CustRef.EnableWindow(bEnabled);
	m_Link.EnableWindow(bEnabled);
	
	m_PayType.GetSelString(Text);
	if(Text == "FUTU COST")
		m_IRSID.EnableWindow(Text == "FUTU COST" || bEnabled ? TRUE : FALSE);
	
	if(m_bFind)
		bEnabled = TRUE;
	else
		if(Text == "TRANSFER" && UserClass <= 2)
			bEnabled = TRUE;
		else
			bEnabled = FALSE;

	m_AccountTo.EnableWindow(bEnabled);
	m_PortfolioTo.EnableWindow(bEnabled);

	if(m_bFind || GetData().IsPowerUser() || ((Text == "COUPON PAY" || Text == "SWAP INT" || Text == "DIVIDENT P" || Text == "TAX") && UserClass <= 2))
		bEnabled = TRUE;
	else
		bEnabled = FALSE;

	m_ToDate.EnableWindow(bEnabled);
	m_CouponPaid.EnableWindow(bEnabled);
}

void CCashForm::InitControls()
{
	m_SS.SetVisibleCols(8);
	m_SS.SetVisibleRows(9);

	m_InvSS.SetVisibleCols(3);
	m_InvSS.SetVisibleRows(3);

	m_DataSS.SetVisibleCols(9);
	m_DataSS.SetVisibleRows(5);

    m_Portfolio.Setup(this, IDC_CASH_PORTFOLIO_COMBO);
	m_Account.Setup(this, IDC_CASH_ACCOUNT_COMBO);
	m_Currency.Setup(this, IDC_CASH_CURRENCY_COMBO);
	m_PayType.Setup(this, IDC_CASH_TYPE_COMBO);
	m_DiffReason.Setup(this, IDC_CASH_DIFFREASON_COMBO, TRUE);   
    m_AccountTo.Setup(this, IDC_CASH_ACCOUNTTO_COMBO, TRUE);
    m_PortfolioTo.Setup(this, IDC_CASH_TOPORTFOLIO_COMBO, TRUE);
	m_CouponPaid.Setup(this, IDC_CASH_FULL_CHECK);

	m_CashNum.Setup(this, IDC_CASH_CASHNUM_EDIT);
	m_PayID.Setup(this, IDC_CASH_PAYID_EDIT);
	m_Link.Setup(this, IDC_CASH_LINK_EDIT);
	m_IRSID.Setup(this, IDC_CASH_SWAP_ID_EDIT);
	m_CustRef.Setup(this, IDC_CASH_REF_EDIT);
	m_CloseDate.Setup(this, IDC_CASH_CLOSEDATE_EDIT);
	m_PostDate.Setup(this, IDC_CASH_POSTDATE_EDIT, TRUE, 
					"Post Date must be greater than or equal to Close Date", &m_CloseDate);
	m_ToDate.Setup(this, IDC_CASH_TODATE_EDIT);
	m_Amount.Setup(this, IDC_CASH_AMOUNT_EDIT, NULL, 2);
	m_Fxrate.Setup(this, IDC_CASH_FXRATE_EDIT);
	m_Remaining.Setup(this, IDC_CASH_REMAIN_EDIT, NULL, 2);
	m_Note.Setup(this, IDC_CASH_COMMENT_EDIT);
	m_Note.LimitText(40);
	m_Asset.Setup(this, IDC_CASH_ASSET_EDIT);
	m_Asset.LimitText(10);

	m_Data.Add(&m_CashNum, &m_Data.GetRec().GetCashNum());
	m_Data.Add(&m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.Add(&m_Account, &m_Data.GetRec().GetAccount());
	m_Data.Add(&m_PayType, &m_Data.GetRec().GetPayType());
	m_Data.Add(&m_Currency, &m_Data.GetRec().GetCurrency());
	m_Data.Add(&m_Asset, &m_Data.GetRec().GetAsset());
	m_Data.Add(&m_CloseDate, &m_Data.GetRec().GetCloseDate());
	m_Data.Add(&m_PostDate, &m_Data.GetRec().GetPostDate());
	m_Data.Add(&m_ToDate, &m_Data.GetRec().GetToDate());
	m_Data.Add(&m_Amount, &m_Data.GetRec().GetAmount());
	m_Data.Add(&m_Fxrate, &m_Data.GetRec().GetFxRate());
	m_Data.Add(&m_Remaining, &m_Data.GetRec().GetRemain());
	m_Data.Add(&m_DiffReason, &m_Data.GetRec().GetDiffReason());
	m_Data.Add(&m_Note, &m_Data.GetRec().GetComment());
	m_Data.Add(&m_PayID, &m_Data.GetRec().GetPayID());
	m_Data.Add(&m_CustRef, &m_Data.GetRec().GetRef());
	m_Data.Add(&m_AccountTo, &m_Data.GetRec().GetToAccount());
	m_Data.Add(&m_PortfolioTo, &m_Data.GetRec().GetToPortfolio());
	m_Data.Add(&m_Link, &m_Data.GetRec().GetLink());
	m_Data.Add(&m_IRSID, &m_Data.GetRec().GetIRSID());
	m_Data.Add(&m_CouponPaid, &m_Data.GetRec().GetCouponPaid());
	m_Data.Add(&m_Data.GetRec().GetBookDate());
	m_Data.Add(&m_UserClass);
	m_Data.Add(&m_CashAssetCode);

	m_Data.GetCashSubData().Add(&m_PayID, &m_Data.GetRec().GetPayID());
	m_Data.GetCashSubData().Add(&m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.GetCashSubData().Add(&m_Account, &m_Data.GetRec().GetAccount());
	m_Data.GetCashSubData().Add(&m_PayType, &m_Data.GetRec().GetPayType());
	m_Data.GetCashSubData().Add(&m_Currency, &m_Data.GetRec().GetCurrency());
	m_Data.GetCashSubData().Add(&m_Asset, &m_Data.GetRec().GetAsset());
	m_Data.GetCashSubData().Add(&m_CloseDate, &m_Data.GetRec().GetCloseDate());
	m_Data.GetCashSubData().Add(&m_Amount, &m_Data.GetRec().GetAmount());
	m_Data.GetCashSubData().Add(&m_Fxrate, &m_Data.GetRec().GetFxRate());
	m_Data.GetCashSubData().Add(&m_Remaining, &m_Data.GetRec().GetRemain());
	m_Data.GetCashSubData().Add(&m_DiffReason, &m_Data.GetRec().GetDiffReason());
	m_Data.GetCashSubData().Add(&m_Note, &m_Data.GetRec().GetComment());
	m_Data.GetCashSubData().Add(&m_CustRef, &m_Data.GetRec().GetRef());
	m_Data.GetCashSubData().Add(&m_AccountTo, &m_Data.GetRec().GetToAccount());
	m_Data.GetCashSubData().Add(&m_UserClass);
//	m_Data.GetCashSubData().Add(m_PortfolioTo, &m_Data.GetRec().GetToPortfolio());
//	m_Data.GetCashSubData().Add(&m_Link, &m_Data.GetRec().GetLink());
//	m_Data.GetCashSubData().Add(&m_IRSID, &m_Data.GetRec().GetIRSID());
//	m_Data.GetCashSubData().Add(m_CouponPaid, &m_Data.GetRec().GetCouponPaid());

	m_Data.GetSRowCtrl().Add(&m_InvNum);
	m_Data.GetSRowCtrl().Add(&m_Data.GetInvRec().GetTransNum());
	m_Data.GetSRowCtrl().Add(&m_Data.GetInvRec().GetAmount());
}

BOOL CCashForm::IsOK()
{
    CString Data, Text, PayType, Sql;
	COraLoader OraLoader;
	CQData QData;

	OraLoader = GetData().GetOraLoader();

	if(m_Currency.GetCurSel() < 0)
		Text = "Must select a currency";

	PayType = m_PayType.GetData();
	if(PayType.IsEmpty())
		Text = "Must select a Payment Type";

	Data = m_Remaining.GetData();
	if(atof(Data) != 0)
	{
		if(m_DiffReason.GetCurSel() < 0)
			Text = "Must select a reason for difference";
	}
	else
		if(m_DiffReason.GetCurSel() >= 0)
			Text = "Must not select a reason for difference if there is no difference";

	if(m_CloseDate.GetData().IsEmpty())
		Text = "Must enter a close date";

	if(m_PostDate.GetData().IsEmpty())
		Text = "Must enter a post date";
	
	if(Text.IsEmpty())
	{
		Data = m_PostDate.GetData();		
		COraDate OraDate;
		OraDate.Setup(Data);

		if(PayType == "DIVIDENT P" || PayType == "TAX")
		{
			if(OraDate.CompareDate(m_CloseDate.GetData()) < 0)
				Text = "Close Date must be less than or equal to Post Date";
		}
		else
			if(OraDate.CompareDate(m_CloseDate.GetData()) <= 0)
				Text = "Close Date must be less than Post Date";
			else
				Text.Empty();
	}

	if(m_Amount.GetWindowTextLength() == 0)
		Text = "Must enter a cash amount";

	if(PayType == "SWAP INT" || PayType == "COUPON PAY" || PayType == "DIVIDENT P")
	{
		if(m_PayID.GetData().IsEmpty())
		{
			if(m_ToDate.GetData().IsEmpty())
				Text = "Must enter Coupon Date";
			else{
				Data = m_Asset.GetData();
				if(Data.IsEmpty())
					Text = "Must enter Asset.";
				else
				{
					Data = QData.GetQueryText(Data);
					
					if(PayType == "DIVIDENT P" || PayType == "COUPON PAY")
						Sql = "SELECT COUNT(*) FROM SEMAM.NW_ASSETS WHERE ASS_CODE = " + Data;
					else
						if(PayType == "COUPON PAY")
							Sql = "SELECT COUNT(*) FROM SEMAM.NW_ASSETS WHERE ASS_ACCRUABLE = 'Y' AND ASS_CODE = " + Data;
						else
						{
							Sql = "SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
									"WHERE B.ASS_CODE = A.ASSET_CODE AND ASS_ACCRUABLE = 'Y' "
									"AND A.TRANS_TYPE = 'INT. SWAP' "
									"AND A.ASSET_CODE = " + Data + 
									" AND A.TRANS_NUM = ";
							Sql += QData.GetQueryNumber(m_IRSID.GetData());
						}
		
					if(OraLoader.GetCount(Sql) <= 0)
						Text = "Must Enter Asset and/or Trans Num.";
			
				}
			}
		}
	}

	if(PayType == "TRANSACT")
	{
		if(m_PayID.GetData().IsEmpty() && m_InvSS.GetSheetRows() <= 0)
			Text = "Must enter an entry from open trade or cash payment";
	}
	
	if(PayType == "OTHERASSET")
	{
		if(m_Asset.GetWindowTextLength() <= 0)
			Text = "Must enter an Asset Code";
	}

	if(!Text.IsEmpty())
		MessageBox(Text);

	return Text.IsEmpty();
}

void CCashForm::SetInCurrMonth()
{
	COraLoader OraLoader;
	CString CloseDate;

	CloseDate = m_CloseDate.GetData();
	OraLoader = GetData().GetOraLoader();
	m_bInCurrMonth = OraLoader.InCurrentMonth(CloseDate);
}

void CCashForm::SetSameDate()
{
	COraLoader OraLoader;
	CString Today;

	OraLoader = GetData().GetOraLoader();
	OraLoader.Today(Today);
	m_bSameDate = (m_Data.GetRec().GetBookDate() == Today);
}

BOOL CCashForm::UpdateData(BOOL bSaveandValid)
{
	if(bSaveandValid)
	{
		if(!IsOK())
			return FALSE;
		
		if(m_CashNum.GetData().IsEmpty())
		{
			CString CashNum;

			GetData().GetOraLoader().GenerateUniqueID(CASHNUM, CashNum);
			m_CashNum.SetData(CashNum);
			m_Data.AddRow();
		}
		else
			m_Data.UpdateRow();
		SetInCurrMonth();
	}
	else
	{
		CQData QData;

		GetData().LoadSupportData();
		GetData().LoadCashData();
		GetData().GetPortfolioArr().CopyToComboBox(m_Portfolio);
		GetData().GetPortfolioArr().CopyToComboBox(m_PortfolioTo);
		GetData().GetCurrencyArr().CopyToComboBox(m_Currency);
		GetData().GetAccountArr().CopyToComboBox(m_Account);
		GetData().GetAccountArr().CopyToComboBox(m_AccountTo);
		GetData().GetCashTypeArr().CopyToComboBox(m_PayType);

		OnCashFind();
		OnCashClear();
		m_Data.SetInit(FALSE);
	}

	return TRUE;
}

void CCashForm::UpdateTotalCash(long Row)
{
	CQData QData;
	double Amount;
		
	Amount = atof(QData.RemoveComma(m_Amount.GetData()));

	Amount -= atof(QData.RemoveComma(m_InvSS.GetSheetText(3,Row)));

	m_Amount.SetData(QData.WriteNumber(Amount, TRUE, 2));
}
/////////////////////////////////////////////////////////////////////////////
// CCashForm message handlers

BEGIN_EVENTSINK_MAP(CCashForm, CFormView)
    //{{AFX_EVENTSINK_MAP(CCashForm)
	ON_EVENT(CCashForm, IDC_CASH_LIST, 2 /* BlockSelected */, OnBlockSelectedCashList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CCashForm, IDC_CASH_LIST, 11 /* DblClick */, OnDblClickCashList, VTS_I4 VTS_I4)
	ON_EVENT(CCashForm, IDC_CASH_MAP_LIST, 11 /* DblClick */, OnDblClickCashMapList, VTS_I4 VTS_I4)
	ON_EVENT(CCashForm, IDC_CASH_TRADE_LIST, 11 /* DblClick */, OnDblClickCashTradeList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCashForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BeginWaitCursor();
	InitControls();
	m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_InvSS, 0);
	m_Data.GetCashSubData().Setup(&m_DataSS);

	GetData().LoadDates();

	if(GetData().GetOraLoader().Open("SELECT DIFF_REASON FROM SEMAM.NW_REMAINING_REASONS ORDER BY DIFF_REASON"))
		GetData().GetOraLoader().LoadComboBox(m_DiffReason);

	if(GetData().GetOraLoader().Open("SELECT DEF_ACCOUNT FROM SEMAM.NW_COMBINED_PORTFOLIOS "
									"WHERE STATUS IS NULL ORDER BY PORTFOLIO "))
		GetData().GetOraLoader().LoadStringArray(m_DefAccounts);
	
	OnCashRefresh();
	GetDocument()->SetViewTitle("Cash Entry");
	UpdateData(FALSE);
	m_bFind = FALSE;
	m_AccountTo.EnableWindow(FALSE);
	m_PortfolioTo.EnableWindow(FALSE);
	
	EndWaitCursor();
}

void CCashForm::OnCashAdd() 
{
	if(UpdateData(TRUE))
	{
		m_Data.SetCurrentRow(-1);
		OnCashRefresh();
	} 
}

void CCashForm::OnCashClear() 
{
	m_Data.Refresh();
	m_DataSS.ClearSheet();
	m_InvSS.ClearSheet();
	m_Portfolio.SetData(m_Fund);
	m_bFind = TRUE;
	m_bInCurrMonth = TRUE;
	m_bSameDate = TRUE;
	EnableControls();
}

void CCashForm::OnCashDelete() 
{
	if(MessageBox("Are you sure you want this entry? ", "Warning", MB_YESNO) == IDYES)
		m_Data.DeleteRow();
}

void CCashForm::OnCashFind() 
{
	BeginWaitCursor();
	m_Data.SetFindExact(FALSE);
	m_Data.LoadDBData();
	m_bFind = FALSE;
	EndWaitCursor();
}

void CCashForm::OnCashFindExact() 
{
	BeginWaitCursor();
	m_Data.SetFindExact(FALSE);
	m_Data.LoadDBData();
	m_bFind = FALSE;
	EndWaitCursor();
}

void CCashForm::OnCashFindAsset() 
{
	CAssetDlg Dlg;

	Dlg.m_pData = &GetData();
	Dlg.m_FindData.GetRec().GetAsset() = m_Asset.GetData();
	Dlg.m_bWithNew = FALSE;
	Dlg.m_bCash = TRUE;

	if(Dlg.DoModal() != IDOK)
		return;

	m_Asset.SetData(Dlg.m_FindData.GetRec().GetAsset());
}

void CCashForm::OnCashLoad() 
{
	CString Portfolio, Account, Currency, PostDate, Asset, PmntType;
	
	BeginWaitCursor();
	m_InvSS.ClearSheet();
	Portfolio = m_Portfolio.GetData();
	Account = m_Account.GetData();
	Currency = m_Currency.GetData();
	PostDate = m_PostDate.GetData();
	Asset = m_Asset.GetData();
	PmntType = m_PayType.GetData();
	
	m_Data.LoadOpenTrade(Portfolio, Account, Currency, PostDate, Asset, PmntType);
	EndWaitCursor();
}

void CCashForm::OnUpdateCashLoad(CCmdUI* pCmdUI) 
{
	CString PayType;

	PayType = m_PayType.GetData();
	pCmdUI->Enable(!m_Fund.IsEmpty() && PayType == "TRANSACT"||PayType == "FUTU COST" && m_Account.GetCurSel() >= 0 && m_Currency.GetCurSel() >= 0 ? TRUE : FALSE);
}

void CCashForm::OnCashLoadCash() 
{
	CString Portfolio, Account, Currency, PostDate;

	BeginWaitCursor();
	m_InvSS.ClearSheet();
	Portfolio = m_Portfolio.GetData();
	Account = m_Account.GetData();
	Currency = m_Currency.GetData();
	PostDate = m_PostDate.GetData();

	m_Data.LoadCash(Portfolio, Account, Currency, PostDate);
	EndWaitCursor();
}

void CCashForm::OnUpdateCashLoadCash(CCmdUI* pCmdUI) 
{
	if(GetData().IsGuest())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!m_Fund.IsEmpty());
}

void CCashForm::OnCashRefresh() 
{
	CString Date;
	
	m_Data.Refresh();
	m_DataSS.ClearSheet();
	m_InvSS.ClearSheet();
	
	m_Portfolio.SetData(m_Fund);
	m_Data.GetOraLoader().Today(Date);
	m_PostDate.SetData(Date);
	
	m_Data.GetOraLoader().TMinus1(Date, Date, "USD");
	m_bFind = FALSE;
	m_bInCurrMonth = TRUE;
	m_CloseDate.SetData(Date);
	EnableControls();
}

void CCashForm::OnCashUpdate() 
{
	UpdateData(TRUE);
}

void CCashForm::OnBlockSelectedCashList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickCashList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CCashForm::OnDblClickCashList(long Col, long Row) 
{
	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		BeginWaitCursor();
		m_Data.SetCurrentRow(Row);
		m_Data.LoadSheet();
		SetInCurrMonth();
		SetSameDate();
		EnableControls();
		EndWaitCursor();
	}
	else
		m_Data.SetCurrentRow(-1);
}

void CCashForm::OnDblClickCashMapList(long Col, long Row) 
{
	if(Row > 0 && Row <= m_InvSS.GetSheetRows())
	{
		m_Data.GetInvRec().SetID(m_InvSS.GetSheetText(1, Row));
		if(MessageBox("Are you sure you want to remove this entry? ", "WARNING", MB_YESNO) == IDYES)
		{
			m_Data.DeleteSRow(Row);
			UpdateTotalCash(Row);
			m_InvSS.DeleteSheetRow(Row);
		}
	}
}

void CCashForm::OnDblClickCashTradeList(long Col, long Row) 
{
	if(Row <= 0 || Row > m_DataSS.GetSheetRows())
	{
		m_Data.GetCashSubData().SetCurrentRow(-1);
		return;
	}

	CString PayType, CloseDate;
	int i;
	double Amount;
	CQData QData;

	switch(m_Data.GetCashStatus())
	{
		case 1: // Open Trade
			CloseDate = m_CloseDate.GetData();
			m_Data.GetCashSubData().SetCurrentRow(Row);
			if(!CloseDate.IsEmpty())
				m_CloseDate.SetData(CloseDate);
			m_Data.GetInvRec().SetID(m_Data.GetCashSubData().GetID());
			m_PayID.SetData(EMPTYSTRING);
			m_Data.LoadCashDetail();
			m_PayType.SetData("TRANSACT");
			Amount = 0;
			for(i = 1; i <= m_InvSS.GetSheetRows(); i++)
				Amount += atof(QData.RemoveComma(m_InvSS.GetSheetText(3, i)));
			m_Amount.SetData(QData.WriteNumber(Amount, TRUE, 2));
			m_Remaining.SetData("0");
			EnableControls();
			break;
		case 2: // Payment instruction
			m_Data.GetCashSubData().SetCurrentRow(Row);
			m_Data.LoadCashDetail();
			PayType = m_Data.GetCashSubData().GetSS()->GetSheetText(8, Row);
			m_PayType.SetData(PayType);
			if(PayType == "COUPON PAY")
				m_CouponPaid.SetCheck(1);
			EnableControls();
			break;
		case 3:
			if(Row > 0 && Row <= m_DataSS.GetSheetRows())
			{
				m_IRSID.SetData(m_Data.GetCashSubData().GetIDArray().GetAt(Row -1)->GetID());
				m_Portfolio.SetData(m_DataSS.GetSheetText(1, Row));
				m_Account.SetData(m_DataSS.GetSheetText(2, Row));
				m_PayType.SetData("FUTU COST");
				m_Currency.SetData(m_DataSS.GetSheetText(4, Row));
				m_Asset.SetData(m_DataSS.GetSheetText(5, Row));
				m_ToDate.SetData(EMPTYSTRING);
				m_Amount.SetData(m_DataSS.GetSheetText(7, Row));
				m_Fxrate.SetData(EMPTYSTRING);
				m_Remaining.SetData(EMPTYSTRING);
				m_DiffReason.SetCurSel(-1);
				m_Note.SetData(EMPTYSTRING);
				m_PayID.SetData(EMPTYSTRING);
				m_CustRef.SetData(EMPTYSTRING);
				m_AccountTo.SetCurSel(-1);
				m_PortfolioTo.SetCurSel(-1);
				m_Link.SetData(EMPTYSTRING);
				m_CouponPaid.SetCheck(0);
				m_UserClass = "1";
			}
			EnableControls();
			break;
		default:
			break;
	}
}

void CCashForm::OnUpdateCashAdd(CCmdUI* pCmdUI) 
{
	if((m_bInCurrMonth && GetData().IsOperation() && !GetData().IsGuest()) || GetData().IsPowerUser())
		pCmdUI->Enable(!m_Fund.IsEmpty() && m_CashNum.GetData().IsEmpty());
	else
		pCmdUI->Enable(FALSE);

/*	if(GetData().IsGuest())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!m_Fund.IsEmpty() && m_CashNum.GetData().IsEmpty()); */
}

void CCashForm::OnUpdateCashDelete(CCmdUI* pCmdUI) 
{
	if((GetData().IsOperation() && !GetData().IsGuest()) || GetData().IsPowerUser())
	{
		if(m_bSameDate || GetData().IsPowerUser())
			pCmdUI->Enable(!m_Fund.IsEmpty() && !m_CashNum.GetData().IsEmpty() && m_SS.GetIsBlockSelected());
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);

/*	if(GetData().IsGuest())
		pCmdUI->Enable(FALSE);
	else
	{
		if(!m_bSameDate)
			pCmdUI->Enable(GetData().IsPowerUser() && !m_Fund.IsEmpty() && 
					!m_CashNum.GetData().IsEmpty() && m_SS.GetIsBlockSelected());
		else
			pCmdUI->Enable(!m_Fund.IsEmpty() && !m_CashNum.GetData().IsEmpty() && m_SS.GetIsBlockSelected());
	} */
}

void CCashForm::OnUpdateCashUpdate(CCmdUI* pCmdUI) 
{
	if((m_bInCurrMonth && GetData().IsOperation() && !GetData().IsGuest()) || GetData().IsPowerUser())
		pCmdUI->Enable(!m_Fund.IsEmpty() && !m_CashNum.GetData().IsEmpty() && m_Data.GetCurrentRow() > 0);
	else
		pCmdUI->Enable(FALSE);

/*	if(GetData().IsGuest())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!m_Fund.IsEmpty() && !m_CashNum.GetData().IsEmpty() && m_Data.GetCurrentRow() > 0); */
}

void CCashForm::OnCashShortCash()
{
	CShortCash Dlg;

	Dlg.m_pData = &GetData();
	Dlg.DoModal();
}

void CCashForm::OnUpdateCashFind(CCmdUI* pCmdUI) 
{
	CString Text;
	BOOL bEnable = FALSE;

    if(!m_Fund.IsEmpty() || m_Currency.GetCurSel() >= 0 || 
		m_PayType.GetCurSel() >= 0)
		bEnable = TRUE;
	
	if(m_Remaining.GetWindowTextLength() > 0 || m_DiffReason.GetCurSel() >= 0)
		bEnable = TRUE;

	if(!m_CloseDate.GetData().IsEmpty() || !m_PostDate.GetData().IsEmpty())
		bEnable = TRUE;
	
	if(m_Amount.GetWindowTextLength() > 0)
		bEnable = TRUE;
	
	if(m_AccountTo.GetCurSel() >= 0 || m_PortfolioTo.GetCurSel() >= 0)
		bEnable = TRUE;
	
	if(!m_CashNum.GetData().IsEmpty())
		bEnable = TRUE;

	pCmdUI->Enable(bEnable);
}

void CCashForm::OnUpdateCashFindExact(CCmdUI* pCmdUI) 
{
	BOOL bEnable = FALSE;

    if(!m_Fund.IsEmpty() || m_Currency.GetCurSel() >= 0 || 
		m_PayType.GetCurSel() >= 0)
		bEnable = TRUE;
	
	if(m_Remaining.GetWindowTextLength() > 0 || m_DiffReason.GetCurSel() >= 0)
		bEnable = TRUE;

	if(!m_CloseDate.GetData().IsEmpty() || !m_PostDate.GetData().IsEmpty())
		bEnable = TRUE;
	
	if(m_Amount.GetWindowTextLength() > 0)
		bEnable = TRUE;
	
	if(m_AccountTo.GetCurSel() >= 0 || m_PortfolioTo.GetCurSel() >= 0)
		bEnable = TRUE;
	
	if(!m_CashNum.GetData().IsEmpty())
		bEnable = TRUE;

	pCmdUI->Enable(bEnable);
}

void CCashForm::OnSelchangeCashPortfolioCombo() 
{	
	if(m_bFind)
		return;

	int Sel;

	Sel = m_Portfolio.GetCurSel();
	if(Sel >= 0)
	{
		m_Fund = m_Portfolio.GetData();
		OnCashRefresh();
		
		CString Account;
		
		m_Portfolio.SetCurSel(Sel);
		Account = m_DefAccounts.GetAt(Sel);
		m_Account.SetData(Account);
		m_PayType.SetFocus();
	}
	else
		m_Fund.Empty();
}

void CCashForm::OnCashLoadCoupon() 
{
	CCouponDlg CouponDlg;
	CQData QData;
	CString PmntType;

	CouponDlg.m_OraLoader = m_Data.GetOraLoader();
	CouponDlg.m_Portfolio = m_Portfolio.GetData();
	CouponDlg.m_Currency = m_Currency.GetData();
	PmntType = m_PayType.GetData();

	if(PmntType == "COUPON PAY")
		CouponDlg.m_CouponType = 3; // None CDS/IRS Coupon
	else
		if(PmntType == "DIVIDENT P")
			CouponDlg.m_CouponType = 4; // Dividend
		else
			if(PmntType == "TAX")
				CouponDlg.m_CouponType = 5; // Tax
			else
				return;

	if(CouponDlg.DoModal() == IDOK)
	{
		m_Asset.SetData(CouponDlg.m_Asset);
		m_Currency.SetData(CouponDlg.m_Currency);
		if(CouponDlg.m_Fxrate == "1")
			m_Fxrate.SetData("");
		else
			m_Fxrate.SetData(CouponDlg.m_Fxrate);
		m_ToDate.SetData(CouponDlg.m_Date);
		m_Amount.SetData(CouponDlg.m_Amount);
		m_IRSID.SetData(CouponDlg.m_TransNum);

		m_Remaining.SetData(PmntType == "TAX" ? "0" : CouponDlg.m_Remaining);
		if(atof(CouponDlg.m_Remaining) != 0)
			m_DiffReason.SetData("ROUNDING");
		else
			m_DiffReason.SetCurSel(-1);
		m_CouponPaid.SetCheck(1);
		m_UserClass = "1";
		EnableControls();
	}
}

void CCashForm::OnUpdateCashLoadCoupon(CCmdUI* pCmdUI) 
{
	if(GetData().IsGuest())
	{
		pCmdUI->Enable(FALSE);
		return;	
	}

	CString PayType;

	PayType = m_PayType.GetData();
	pCmdUI->Enable(!m_Fund.IsEmpty() && (PayType == "COUPON PAY" || PayType == "SWAP INT" || "DIVIDENT P" || "TAX") && m_Currency.GetCurSel() >= 0);
}

void CCashForm::OnUpdateCashSwap(CCmdUI* pCmdUI) 
{
	if(GetData().IsGuest())
	{
		pCmdUI->Enable(FALSE);
		return;	
	}
	
	CString PayType;

	PayType = m_PayType.GetData();
	pCmdUI->Enable(!m_Fund.IsEmpty() && (PayType == "SWAP INT" || PayType == "FX PVALUE"));
}

void CCashForm::OnCashSwap() 
{
	CCashIRSIDDlg Dlg;

	Dlg.m_OraLoader = m_Data.GetOraLoader();
	Dlg.m_Portfolio = m_Portfolio.GetData();
	Dlg.m_PayType = m_PayType.GetData();
	Dlg.m_Currency = m_Currency.GetData();
	
	if(Dlg.DoModal() == IDOK)
		m_IRSID.SetData(Dlg.m_TransNum);
}

void CCashForm::OnUpdateCashErrorlink(CCmdUI* pCmdUI) 
{
	if(GetData().IsGuest())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!m_Fund.IsEmpty());
}

void CCashForm::OnCashErrorlink() 
{
	CErrorEntryLinkDlg Dlg;

	Dlg.m_OraLoader = m_Data.GetOraLoader();
	Dlg.m_Portfolio = m_Portfolio.GetData();
	Dlg.m_Account = m_Account.GetData();
	Dlg.m_Currency = m_Currency.GetData();
	
	if(Dlg.DoModal() == IDOK)
		m_Link.SetData(Dlg.m_CashNum);
}

void CCashForm::OnSelchangeCashTypeCombo() 
{
	if (m_bFind)
		return;

	CString Text;
	int UserClass;

	UserClass = atoi(m_UserClass);

	Text = m_PayType.GetData();
	if(Text == "TRANSFER" && UserClass <= 2)
	{
		m_AccountTo.EnableWindow(TRUE);
		m_PortfolioTo.EnableWindow(TRUE);
	}
	else
	{
		m_AccountTo.EnableWindow(FALSE);
		m_PortfolioTo.EnableWindow(FALSE);
	}
}

void CCashForm::OnChangeCashAmountEdit() 
{
	CQData QData;
	double Amount;
	CString Text;
	int i;

	Text = m_Amount.GetData();
	Amount = atof(QData.RemoveComma(Text));

	if(m_InvSS.GetSheetRows() > 1)
	{
		for(i = 1; i <= m_InvSS.GetSheetRows(); i++)
			Amount -= atof(QData.RemoveComma(m_InvSS.GetSheetText(3,i)));

		m_Remaining.SetData(QData.WriteNumber(Amount, TRUE, 2));
	}
	else
		m_Remaining.SetData("0");
}


void CCashForm::OnCashLoadcdscoupon() 
{
	CCouponDlg CouponDlg;

	CouponDlg.m_OraLoader = m_Data.GetOraLoader();
	CouponDlg.m_Portfolio = m_Portfolio.GetData();
	CouponDlg.m_Currency = m_Currency.GetData();
	CouponDlg.m_CouponType = 1; // CDS Coupon
	if(CouponDlg.DoModal() == IDOK)
	{
		m_Asset.SetData(CouponDlg.m_Asset);
		m_Currency.SetData(CouponDlg.m_Currency);
		if(CouponDlg.m_Fxrate == "1")
			m_Fxrate.SetData("");
		else
			m_Fxrate.SetData(CouponDlg.m_Fxrate);
		m_ToDate.SetData(CouponDlg.m_Date);
		m_Amount.SetData(CouponDlg.m_Amount);
		m_IRSID.SetData(EMPTYSTRING);
		m_Remaining.SetData(CouponDlg.m_Remaining);
		if(atof(CouponDlg.m_Remaining) != 0)
			m_DiffReason.SetData("ROUNDING");
		else
			m_DiffReason.SetCurSel(-1);
		m_CouponPaid.SetCheck(1);
		m_UserClass = "1";
		EnableControls();
	}	
}

void CCashForm::OnUpdateCashLoadcdscoupon(CCmdUI* pCmdUI) 
{
	if(GetData().IsGuest())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CString PayType;

	PayType = m_PayType.GetData();
	pCmdUI->Enable(!m_Fund.IsEmpty() && PayType == "COUPON PAY" && m_Currency.GetCurSel() >= 0);
}


void CCashForm::OnUpdateCashClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_Fund.IsEmpty());
}

void CCashForm::OnUpdateCashFindAsset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest());
}

void CCashForm::OnUpdateCashRefresh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest());
}

void CCashForm::OnUpdateCashShortCash(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest());
}

void CCashForm::OnCashLoadirscoupon()
{
	CCouponDlg CouponDlg;
	CQData QData;

	CouponDlg.m_OraLoader = m_Data.GetOraLoader();
	CouponDlg.m_Portfolio = m_Portfolio.GetData();
	CouponDlg.m_Currency = m_Currency.GetData();
	CouponDlg.m_CouponType = 2; // IRS Coupon
	if(CouponDlg.DoModal() == IDOK)
	{
		m_Asset.SetData(CouponDlg.m_Asset);
		m_Currency.SetData(CouponDlg.m_Currency);
		if(CouponDlg.m_Fxrate == "1")
			m_Fxrate.SetData("");
		else
			m_Fxrate.SetData(CouponDlg.m_Fxrate);
		m_ToDate.SetData(CouponDlg.m_Date);
		m_Amount.SetData(CouponDlg.m_Amount);
		m_IRSID.SetData(QData.RemoveComma(CouponDlg.m_TransNum));
		m_Remaining.SetData(CouponDlg.m_Remaining);
		if(atof(CouponDlg.m_Remaining) != 0)
			m_DiffReason.SetData("ROUNDING");
		else
			m_DiffReason.SetCurSel(-1);
		m_CouponPaid.SetCheck(1);
		m_UserClass = "1";
		EnableControls();
	}
}

void CCashForm::OnUpdateCashLoadirscoupon(CCmdUI *pCmdUI)
{
	if(GetData().IsGuest())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CString PayType;

	PayType = m_PayType.GetData();
	pCmdUI->Enable(!m_Fund.IsEmpty() && PayType == "SWAP INT" && m_Currency.GetCurSel() >= 0);
}

void CCashForm::OnCashUploadcashentry()
{
	CODBCDB DB;
	CQData QData;
	CString Path = "Excel Files (*.xls*) || ";
	int Count = 0;
	LPCTSTR Template = "Template";
	LPCTSTR XLS = "XLS";
	CDBVariant CloseDate;
	CString Seq, Today, Fund, Account, Currency, PmntType, sCloseDate, Asset, Cash, CashNum, SQL;
	COraLoader OraLoader, OraLoader2;

	if(!QData.GetFileName(AfxGetApp(), Path, Template, "CASH UPLOAD", XLS))
		return;

	if(!DB.Open(Path))
		return;

	OraLoader.SetDB(&theDB);
	OraLoader2.SetDB(&theDB);

	OraLoader.Today(Today);
	OraLoader.Open("SELECT CASH_NUM, PORTFOLIO, ACC_CODE, CURRENCY, PMNT_TYPE, "
					"CLOSE_DATE, POST_DATE, ASS_CODE, AMOUNT FROM SEMAM.NW_CASH_PMNTS ", 
					ODYNASET_DEFAULT);
	CRecordset DBRec((CDatabase *) &DB);

	DBRec.Open(CRecordset::dynaset, "SELECT SEQ, FUND, ACCOUNT, CURRENCY, PMNT_TYPE, "
				"CLOSE_DATE, ASSET_CODE, CASH, CASH_NUM FROM CASH_ITEMS "
				"WHERE CASH IS NOT NULL AND CASH_NUM IS NULL ", CRecordset::none);

	while(!DBRec.IsEOF())
	{
		DBRec.GetFieldValue(short (0), Seq);
		DBRec.GetFieldValue(short (1), Fund);
		DBRec.GetFieldValue(short (2), Account);
		DBRec.GetFieldValue(short (3), Currency);
		DBRec.GetFieldValue(short (4), PmntType);
		DBRec.GetFieldValue(short (5), CloseDate);
		DBRec.GetFieldValue(short (6), Asset);
		DBRec.GetFieldValue(short (7), Cash);
		
		if(!Cash.IsEmpty())
		{
			if(OraLoader2.Open("SELECT MAX(CASH_NUM) + 1 FROM SEMAM.NW_CASH_PMNTS "))
			{
				CashNum = OraLoader2.GetDBString(0);
				CashNum = QData.WriteNumber(CashNum, FALSE, 0);
				OraLoader2.Close();
			}

			if(!CashNum.IsEmpty())
			{
				if(OraLoader.AddNewRecord() == OSUCCESS)
				{
					OraLoader.SetFieldValue(0, (LPCTSTR) CashNum);
					OraLoader.SetFieldValue(1, (LPCTSTR) Fund);
					OraLoader.SetFieldValue(2, (LPCTSTR) Account);
					OraLoader.SetFieldValue(3, (LPCTSTR) Currency);
					OraLoader.SetFieldValue(4, (LPCTSTR) PmntType);
					sCloseDate.Format("%02d/%02d/%04d", CloseDate.m_pdate->month, 
									CloseDate.m_pdate->day, CloseDate.m_pdate->year);
					OraLoader.SetFieldValue(5, (LPCTSTR) sCloseDate);
					OraLoader.SetFieldValue(6, (LPCTSTR) Today);
					OraLoader.SetFieldValue(7, (LPCTSTR) Asset);
					OraLoader.SetFieldValue(8, (LPCTSTR) Cash);
					if(OraLoader.Update() == OSUCCESS)
						Count++;
				}
			
				if(DBRec.CanUpdate())
				{
					SQL = "UPDATE CASH_ITEMS SET CASH_NUM = " + CashNum + 
							" WHERE SEQ = " + Seq;
					DB.ExecuteSQL(SQL);
				}
			}
		}
		DBRec.MoveNext();
	}

	DBRec.Close();
	OraLoader.Close();

	Fund.Format("Total %d Cash Items Processed. ", Count);

	MessageBox(Fund, "Cash Upload");
}
