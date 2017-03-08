// OrderEntry.cpp : implementation file
//
#include "stdafx.h"
#include <math.h>
#include "NWIFMS70.h"
#include "OrderEntry.h"
#include "assetdlg.h"
#include "qdata.h"
#include "Finddlg.h"
#include "ContractConvertor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderEntry
IMPLEMENT_DYNCREATE(COrderEntry, CFormView)

COrderEntry::COrderEntry()
	: CFormView(COrderEntry::IDD)
{
	//{{AFX_DATA_INIT(COrderEntry)
	m_TRS = NULL;
	m_IPO = NULL;
	m_MarketOrder = NULL;
	m_SameDay = NULL;
	m_PartialFill = NULL;
	m_Discretionary = NULL;
	m_Expired = NULL;
	m_Cancelled = NULL;

    m_TransType = NULL;
    m_CP = NULL;
    m_Currency = NULL;
	m_BestExecution = NULL;
//	m_Reason = NULL;

	m_bLoaded = FALSE;
	//}}AFX_DATA_INIT
}

COrderEntry::~COrderEntry()
{
	if(m_TRS)
		delete m_TRS;
	if(m_IPO)
		delete m_IPO;
	if(m_MarketOrder)
		delete m_MarketOrder;
	if(m_SameDay)
		delete m_SameDay;
	if(m_PartialFill)
		delete m_PartialFill;
	if(m_Discretionary)
		delete m_Discretionary;
	if(m_Expired)
		delete m_Expired;
	if(m_Cancelled)
		delete m_Cancelled;

    if(m_TransType)
		delete m_TransType;
    if(m_CP)
		delete m_CP;
    if(m_Currency)
		delete m_Currency;
	if(m_BestExecution)
		delete m_BestExecution;
//	if(m_Reason)
//		delete m_Reason;
}

void COrderEntry::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderEntry)
	DDX_Control(pDX, IDC_ORDER_LIST, m_SS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COrderEntry, CFormView)
	//{{AFX_MSG_MAP(COrderEntry)
	ON_CBN_SELCHANGE(IDC_ORDER_TRANSTYPE_COMBO, OnSelchangeTranstypeCombo)
	ON_COMMAND(ID_ORDER_ADD, OnOrderAdd)
	ON_COMMAND(ID_ORDER_UPDATE, OnOrderUpdate)
	ON_COMMAND(ID_ORDER_REFRESH, OnOrderRefresh)
	ON_CBN_SELCHANGE(IDC_ORDER_DIR_COMBO, OnSelchangeDirCombo)
	ON_COMMAND(ID_ORDER_FIND_ASSET, OnOrderFindAsset)
	ON_BN_CLICKED(IDC_ORDER_IPO_CHECK, OnIPOCheck)
	ON_EN_SETFOCUS(IDC_ORDER_ASSET_EDIT, OnSetfocusAssetEdit)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_ORDER_CURRENCY_COMBO, &COrderEntry::OnCbnSelchangeOrderCurrencyCombo)
	ON_BN_CLICKED(IDC_ORDER_TRS_CHECK, &COrderEntry::OnBnClickedOrderTrsCheck)
	ON_COMMAND(ID_ORDER_CONTRACT, &COrderEntry::OnOrderContract)
	ON_CBN_SELCHANGE(IDC_ORDER_AA_COMBO, &COrderEntry::OnCbnSelchangeOrderAaCombo)
	ON_CBN_KILLFOCUS(IDC_ORDER_AA_COMBO, &COrderEntry::OnCbnKillfocusOrderAaCombo)
	ON_EN_CHANGE(IDC_ORDER_AMOUNT_EDIT, &COrderEntry::OnEnChangeOrderAmountEdit)
	ON_EN_KILLFOCUS(IDC_ORDER_AMOUNT_EDIT, &COrderEntry::OnEnKillfocusOrderAmountEdit)
	ON_EN_CHANGE(IDC_ORDER_PRICE_EDIT, &COrderEntry::OnEnChangeOrderPriceEdit)
	ON_EN_KILLFOCUS(IDC_ORDER_PRICE_EDIT, &COrderEntry::OnEnKillfocusOrderPriceEdit)
	ON_EN_CHANGE(IDC_ORDER_ASSET_EDIT, &COrderEntry::OnEnChangeOrderAssetEdit)
	ON_EN_KILLFOCUS(IDC_ORDER_ASSET_EDIT, &COrderEntry::OnEnKillfocusOrderAssetEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderEntry diagnostics
#ifdef _DEBUG
void COrderEntry::AssertValid() const
{
	CFormView::AssertValid();
}

void COrderEntry::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* COrderEntry::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

void COrderEntry::EnableCtrls()
{
	CString TransType, Dir;

	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	GetData().DefineTransType(TransType, Dir);

	GetDlgItem(IDC_ORDER_STRIKE_STATIC)->ShowWindow(TransType == CALL || TransType == PUT ? TRUE : FALSE);
	m_Strike.ShowWindow(TransType == CALL || TransType == PUT ? TRUE : FALSE);
	GetDlgItem(IDC_ORDER_RATE_STATIC)->ShowWindow(TransType == REPO || TransType == INTSWAP || (TransType == SECURITIES && m_TRS->GetCheck()) ? TRUE : FALSE);
	m_Rate.ShowWindow(TransType == REPO || TransType == INTSWAP || (TransType == SECURITIES && m_TRS->GetCheck()) ? TRUE : FALSE);
	GetDlgItem(IDC_ORDER_PRICE_STATIC)->ShowWindow(TransType == FOREX ? FALSE : TRUE);
	m_Price.ShowWindow(TransType == FOREX ? FALSE : TRUE);
	GetDlgItem(IDC_ORDER_MATURITY_STATIC)->ShowWindow(TransType == CALL || TransType == PUT || TransType == REPO || TransType == INTSWAP || (TransType == SECURITIES && (Dir == B || Dir == L || m_TRS->GetCheck())));
	m_Maturity.ShowWindow(TransType == CALL || TransType == PUT || TransType == REPO || TransType == INTSWAP || (TransType == SECURITIES && (Dir == B || Dir == L || m_TRS->GetCheck())));
}

void COrderEntry::InitControls()
{
	m_SS.SetVisibleCols(10);
	m_SS.SetVisibleRows(12);
	m_SS.SetSheetCols(7);
	m_SS.SetSheetRows(m_SS.GetMaxRows());
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);
	m_SS.SetSheetCurRow(-1);

	m_TRS = new CCheckBox(this, IDC_ORDER_TRS_CHECK, TRS);
	m_IPO = new CCheckBox(this, IDC_ORDER_IPO_CHECK, Y);
	m_MarketOrder = new CCheckBox(this, IDC_ORDER_MARKET_CHECK, Y);
	m_SameDay = new CCheckBox(this, IDC_ORDER_SAMEDAY_CHECK, Y);
	m_PartialFill = new CCheckBox(this, IDC_ORDER_PARTIAL_CHECK, Y);
	m_Discretionary = new CCheckBox(this, IDC_ORDER_DISCRETIONARY_CHECK, Y);
	m_Discretionary->SetCheck(1);
	m_Expired = new CCheckBox(this, IDC_ORDER_EXPIRED_CHECK, Y);
	m_Cancelled = new CCheckBox(this, IDC_ORDER_CANCEL_CHECK, Y);

    m_TransType = new COptComboBox(this, IDC_ORDER_TRANSTYPE_COMBO);
    m_CP = new COptComboBox(this, IDC_ORDER_COUNTERPARTY_COMBO);
    m_Currency = new COptComboBox(this, IDC_ORDER_CURRENCY_COMBO);
	m_BestExecution = new COptComboBox(this, IDC_ORDER_BESTEXECUTION_COMBO);
//	m_Reason = new COptComboBox(this, IDC_ORDER_REASON_COMBO);

	m_OrderID.Setup(this, IDC_ORDER_ID_EDIT);
	m_OrderDate.Setup(this, IDC_ORDER_DATE_EDIT, FALSE);
	m_Maturity.Setup(this, IDC_ORDER_MATURITY_EDIT, FALSE, "Maturity Date must be greater than or equal to Order Date", &m_OrderDate);

	m_Fxrate.Setup(this, IDC_ORDER_FXRATE_EDIT);
	m_Amount.Setup(this, IDC_ORDER_AMOUNT_EDIT, NULL, 2);
	m_Strike.Setup(this, IDC_ORDER_STRIKE_EDIT);
	m_Rate.Setup(this, IDC_ORDER_RATE_EDIT);
	m_Rate.LimitText(15);
	m_Price.Setup(this, IDC_ORDER_PRICE_EDIT);

	m_Trader.Setup(this, IDC_ORDER_TRADER_COMBO, 2);
	m_AAMethod.Setup(this, IDC_ORDER_AA_COMBO, 6);
	m_Dir.Setup(this, IDC_ORDER_DIR_COMBO, 2);

	m_Note.Setup(this, IDC_ORDER_COMMENT_EDIT, EMPTYSTRING);
	m_Note.LimitText(200);

	m_Asset.Setup(this, IDC_ORDER_ASSET_EDIT);
	m_AssetDesc.Setup(this, IDC_ORDER_ASSETDESC_EDIT);
	m_AssetCurrency.Setup(this, IDC_ORDER_ASSETCURRENCY_EDIT);

	COrderRec *pOrderRec;
	pOrderRec = &m_Data.GetOrderRec();
	m_Data.Add(&m_OrderDate, &pOrderRec->GetOrderDate());
	m_Data.Add(&m_OrderID, &pOrderRec->GetOrderID());
	m_Data.Add(m_TransType, &pOrderRec->GetTransType());
	m_Data.Add(&m_Dir, &pOrderRec->GetDir());
	m_Data.Add(&m_Asset, &pOrderRec->GetAsset());
	m_Data.Add(&m_Strike, &pOrderRec->GetStrike());
	m_Data.Add(&m_Rate, &pOrderRec->GetRate());;
	m_Data.Add(&m_Maturity, &pOrderRec->GetMaturity());
	m_Data.Add(m_Currency, &pOrderRec->GetCurrency());
	m_Data.Add(&m_Fxrate, &pOrderRec->GetFxrate());
	m_Data.Add(&m_Amount, &pOrderRec->GetNomAmount());
	m_Data.Add(&m_Price, &pOrderRec->GetPrice());
	m_Data.Add(m_CP, &pOrderRec->GetCP());
	m_Data.Add(&pOrderRec->GetRefTicket());
	m_Data.Add(&m_Trader, &pOrderRec->GetTrader());
//	m_Data.Add(m_Reason, &pOrderRec->GetReason());
	m_Data.Add(&m_Note, &pOrderRec->GetNote());
	m_Data.Add(&m_AAMethod, &pOrderRec->GetAAMethod());
	m_Data.Add(m_BestExecution, &pOrderRec->GetBestExecution());
	m_Data.Add(m_TRS, &pOrderRec->GetTRS());
	m_Data.Add(m_IPO, &pOrderRec->GetIPO());
	m_Data.Add(m_MarketOrder, &pOrderRec->GetMarketOrder());
	m_Data.Add(m_SameDay, &pOrderRec->GetSameDay());
	m_Data.Add(m_PartialFill, &pOrderRec->GetPartialFill());
	m_Data.Add(m_Discretionary, &pOrderRec->GetDiscretionary());
	m_Data.Add(m_Expired, &pOrderRec->GetExpired());
	m_Data.Add(m_Cancelled, &pOrderRec->GetCancelled());
}

BOOL COrderEntry::IsOK()
{
	CString Text, Method, TransType, Currency, Nominal, Price, Fxrate;
	
	if(m_TransType->GetCurSel() < 0)
		Text = "Invalid Trans Type, Must enter Trans Type";
	if(m_Dir.GetCurSel() < 0)
		Text = "Invalid direction, Must enter dir";
	if(m_Currency->GetCurSel() < 0)
		Text = "Invalid currency, Must enter currency";
	if(m_Amount.GetWindowTextLength() <= 0)
		Text = "Invalid nominal, Must enter nominal";
	if(m_CP->GetCurSel() < 0)
		Text = "Invalid counterparty, Must enter counterparty";
	if(m_Trader.GetCurSel() < 0)
		Text = "Invalid trader, Must enter trade";
//	if(m_Reason->GetCurSel() < 0)
//		Text = "Invalid reason, Must enter reason";
	if(m_AAMethod.GetCurSel() < 0)
		Text = "Invalid AA method, Must enter AA method";
	if(m_BestExecution->GetCurSel() < 0)
		Text = "Invalid Best Execution, Must enter Best Execution";

	if(!Text.IsEmpty())
	{
		MessageBox(Text, "ORDER ENTRY");
		return FALSE;
	}

	// Risk Control
	switch(VerifyRisk(Text))
	{
		case 1:
			MessageBox(Text, "ORDER ENTRY");
			return TRUE;
		case 3:
			MessageBox(Text, "ORDER ENTRY");
			return FALSE;
		default:
			return TRUE;
	}
}

void COrderEntry::ProcessVerifyRisk()
{
	CString Text;

	m_nRiskLevel = VerifyRisk(Text);
	if(!Text.IsEmpty() && !m_bWarned)
	{
		MessageBox(Text, "Order Entry");
		m_bWarned = TRUE;
	}
}

BOOL COrderEntry::UpdateData(BOOL bSaveandValid)
{
	if(!bSaveandValid)
	{
		BeginWaitCursor();
		CQData QData;
		COraLoader OraLoader;
		CStringArray Data;

		OraLoader = GetData().GetOraLoader();
		m_Data.LoadDBData();

		if(!m_bLoaded)
		{
			GetData().LoadSupportData();

			GetData().GetTransTypeArr().CopyToComboBox(*m_TransType); /* Trans Type */
			GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);
			Data.Add("SS");
			Data.Add("SELL SHORT");
			m_Dir.AddString(Data);
			Data.RemoveAll();
			Data.Add("CS");
			Data.Add("COVER SHORT");
			m_Dir.AddString(Data);
			QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_Currency, 0, FALSE);	/* Currency */
			GetData().GetContactList().CopyKeyToComboBox(*m_CP);
			OraLoader.Open("SELECT TRADER_INI, TRADER FROM SEMAM.NW_TRADERS WHERE ACTIVE = 'Y' ORDER BY 1 ");
			OraLoader.LoadMCComboBox(m_Trader); /* Trader */
			OraLoader.Open("SELECT AA_METHOD, AA_DESC, RATIO_A, RATIO_B, RATIO_C, RATIO_D FROM SEMAM.NW_AA_METHOD_V ORDER BY 1");
			m_AAMethod.ResetContent();
			OraLoader.LoadMCComboBox(m_AAMethod);
			GetData().GetBestExecutionArr().CopyToComboBox(*m_BestExecution);
//			OraLoader.Open("SELECT REASON FROM SEMAM.NW_ORDER_REASON ORDER BY 1 ");
//			OraLoader.LoadComboBox(*m_Reason);
			m_bLoaded = TRUE;
		}

		EndWaitCursor();
	}

	m_SS.SetSheetCurRow(-1);

	Invalidate();
	UpdateWindow();
	return TRUE;
}


int COrderEntry::VerifyRisk(CString &Text)
{
	CString Method, Dir, Asset, TransType, ValueDate, Currency, Nominal, Price, Fxrate;
	
	m_AAMethod.GetSelString(Method);

	if(Method != "BNAMTR"||m_TRS->GetCheck())
		return 0;

	m_Asset.GetWindowTextA(Asset);
	if(Asset != m_Risk.GetAsset())
		m_Risk.LoadRisk(Asset, m_OrderID.GetData(), FALSE);

	m_TransType->GetSelString(TransType);
	m_Dir.GetSelString(Dir);
	m_Currency->GetSelString(Currency);
	m_Amount.GetWindowText(Nominal);
	m_Price.GetWindowText(Price);
	m_Fxrate.GetWindowText(Fxrate);
	ValueDate = m_OrderDate.GetData();

	if(Dir == S || Dir == "SS")
		Nominal = "-" + Nominal;

	m_bWarned = FALSE;
	
	return m_Risk.VerifyRisk(Text, Method, TransType, ValueDate, Currency, Nominal, Price, Fxrate);
}

/////////////////////////////////////////////////////////////////////////////
// COrderEntry message handlers
void COrderEntry::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	COraLoader OraLoader;

	BeginWaitCursor();

	InitControls();
	OraLoader.SetDB(&theDB);
	if(GetData().IsTrader())
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, GetData().GetTraderIni());
	else
		m_Data.Setup(GetData().GetOraLoader(), &m_SS, EMPTYSTRING);
	m_Risk.Setup(GetData().GetOraLoader());

	UpdateData(FALSE);
	OnOrderRefresh();
	EndWaitCursor();
}

BEGIN_EVENTSINK_MAP(COrderEntry, CFormView)
    //{{AFX_EVENTSINK_MAP(COrderEntry)
	ON_EVENT(COrderEntry, IDC_ORDER_LIST, 2 /* BlockSelected */, OnBlockSelectedOrderList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(COrderEntry, IDC_ORDER_LIST, 11 /* DblClick */, OnDblClickOrderList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void COrderEntry::OnSelchangeTranstypeCombo() 
{
	EnableCtrls();
	ProcessVerifyRisk();
}

void COrderEntry::OnSelchangeDirCombo() 
{
	EnableCtrls();
	ProcessVerifyRisk();
}

void COrderEntry::OnOrderFindAsset() 
{	
	CAssetDlg Dlg;
	CAssetRec *pRec;
	CString TransType, Dir, Nominal, Asset, Text;
	BOOL bChange = FALSE;
	CQData QData;

	TransType = m_TransType->GetSelString(TransType);
	
	Dlg.m_pData = &GetData();
	m_Asset.GetWindowText(Dlg.m_FindData.GetRec().GetAsset());
	m_AssetDesc.GetWindowText(Dlg.m_FindData.GetRec().GetDesc());

	Dlg.m_bTicket = TRUE;

	if(Dlg.DoModal() == IDOK)
	{
		pRec = &Dlg.m_FindData.GetRec();
		m_Asset.SetWindowText(pRec->GetAsset());
		m_AssetDesc.SetWindowText(pRec->GetDesc());
		m_AssetCurrency.SetWindowText(pRec->GetCurrency());
		
		m_Currency->GetSelString(Text);

		if(Text.IsEmpty() || (Text != pRec->GetCurrency() && 
			MessageBox("Note: Trading Currency is Different from Asset Currency! Do you want to synchonize them ?", "Warning", MB_YESNO) == IDYES))
			m_Currency->SelectString(-1, pRec->GetCurrency());
		
		m_Currency->GetSelString(Text);
		m_Fxrate.GetWindowText(Nominal);
		if(Text != "USD" && Nominal == "1")
			m_Fxrate.SetWindowText(EMPTYSTRING);
		
		if(Dlg.m_nActionID == FINDACTION_UNWIND || Dlg.m_nActionID == FINDACTION_OPTEX)
		{
			m_Amount.GetWindowText(Text);
			if(Dlg.m_FindData.GetNominal() != Text && m_Amount.GetWindowTextLength() > 0)
			{
				if(MessageBox("Do you want to change current nom_amount (Y/N)?", "Order Entry", MB_YESNO) == IDYES)
					bChange = TRUE;
			}
		}
		
		TransType = Dlg.m_FindData.GetTransType();
		m_TransType->SelectString(0, TransType);
		Dir = Dlg.m_FindData.GetDir();
		Nominal = Dlg.m_FindData.GetNominal();

		switch(Dlg.m_nActionID)
		{
			case FINDACTION_ADD:
				if(TransType == CDS || TransType == SECURITIES || TransType == CALL || TransType == PUT)
				{
					if(Dir == P || Dir.IsEmpty())
						m_Dir.SelectString(0, P);
					else
						m_Dir.SelectString(0, "SS");
				}

				if(TransType == CALL ||	TransType == PUT)
				{
					m_Strike.SetWindowText(Dlg.m_FindData.GetStrike());
					m_Maturity.SetData(Dlg.m_FindData.GetRec().GetMaturity());
				}
				else
					if(TransType == SECURITIES && Dlg.m_FindData.GetRec().GetClass() == "CURRENCY FWDS")
						m_Maturity.SetData(Dlg.m_FindData.GetRec().GetMaturity());
				break;
			case FINDACTION_UNWIND:
				if(TransType == CDS || TransType == SECURITIES || TransType == CALL || TransType == PUT)
				{
					if(Dir == P)
						m_Dir.SelectString(0, S);
					else
						m_Dir.SelectString(0, "CS");

					if(bChange || m_Amount.GetWindowTextLength() == 0)
						m_Amount.SetWindowText(Dlg.m_FindData.GetNominal());
				}
					
				if(TransType == CALL || TransType == PUT)
				{
					m_Strike.SetWindowText(Dlg.m_FindData.GetStrike());
					m_Maturity.SetWindowText(Dlg.m_FindData.GetRec().GetMaturity());
					m_Data.GetOrderRec().GetRefTicket() = Dlg.m_FindData.GetTicket();
				}
				else
					if(TransType == SECURITIES && Dlg.m_FindData.GetRec().GetClass() == "CURRENCY FWDS")
						m_Maturity.SetData(Dlg.m_FindData.GetRec().GetMaturity());
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
					
					m_TransType->SelectString(0, SECURITIES);
					m_Amount.SetWindowText(Dlg.m_FindData.GetNominal());
					m_Price.SetWindowText(Dlg.m_FindData.GetStrike());
					m_Data.GetOrderRec().GetRefTicket() = Dlg.m_FindData.GetTicket();
				}
				break;
			default:
				break;
		}
	}

	m_Asset.GetWindowText(Asset);
	if(TransType == FOREX && Asset == "NEW ASSET")
	{
		m_Asset.SetWindowText("");
		m_AssetDesc.SetWindowText("");
		m_AssetCurrency.SetWindowText("");
	}
	
	EnableCtrls();
	m_Note.SetFocus();
}

void COrderEntry::OnBlockSelectedOrderList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickOrderList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void COrderEntry::OnDblClickOrderList(long Col, long Row) 
{
	CString Method, Asset;

	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row);

		EnableCtrls();
	}
	else
		OnOrderRefresh();
}

void COrderEntry::OnOrderAdd() 
{	
   	if(!IsOK())
		return;

	CString OrderID;

	m_SS.SetFocus();
	GetData().GetOraLoader().GenerateUniqueID("ORDER_ID", OrderID);
	m_OrderID.SetData(OrderID);

	m_Data.AddRow();
	m_Risk.UpdateRisk(OrderID, FALSE);
}

void COrderEntry::OnOrderUpdate() 
{
	if(!IsOK())
		return;

	m_SS.SetFocus();
	m_Data.UpdateRow();
	m_Risk.UpdateRisk(m_OrderID.GetData(), FALSE);
}

void COrderEntry::OnOrderRefresh() 
{
	BeginWaitCursor();
	
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	GetData().LoadDates();
	m_OrderDate.SetData(GetData().GetDate());
	m_TransType->SelectString(-1, SECURITIES);
	m_Currency->SetCurSel(m_Currency->FindString(-1, USD));
	m_Fxrate.SetWindowText("1");
	m_Trader.SelectString(-1, GetData().GetTraderIni());
	m_MarketOrder->SetCheck(1);
	m_IPO->SetCheck(0);
	OnIPOCheck();
	m_SS.SheetAutoFit();

	EnableCtrls();
	EndWaitCursor();
}

void COrderEntry::OnIPOCheck() 
{
	m_MarketOrder->SetCheck(1);

	if(m_IPO->GetCheck())
	{
		m_SameDay->SetCheck(0);
		m_PartialFill->SetCheck(1);
	}
	else
	{
		m_SameDay->SetCheck(1);
		m_PartialFill->SetCheck(1);
	}
}

void COrderEntry::OnSetfocusAssetEdit() 
{
	CString TransType;

	m_TransType->GetSelString(TransType);
	if(TransType == FOREX || m_Asset.GetWindowTextLength() > 0)
		return;

	m_Asset.SetWindowText(NEWASSET);	
	OnOrderFindAsset();	
}


void COrderEntry::OnCbnSelchangeOrderCurrencyCombo()
{
	CString Curr;

	m_Currency->GetSelString(Curr);
	
	if(Curr == USD)
		m_Fxrate.SetWindowText("1");
}


void COrderEntry::OnBnClickedOrderTrsCheck()
{
	EnableCtrls();
}


void COrderEntry::OnOrderContract()
{
	CContractConvertor Dlg;
	int Status;

	m_Asset.GetWindowText(Dlg.m_AssetStr);
	Dlg.m_OraLoader.SetDB(GetData().GetOraLoader().GetDB());
	Status = Dlg.DoModal();
	
	if(Status != IDOK)
		return;

	m_Amount.SetWindowText(Dlg.m_NominalStr);
}


void COrderEntry::OnCbnSelchangeOrderAaCombo()
{
	ProcessVerifyRisk();
}


void COrderEntry::OnCbnKillfocusOrderAaCombo()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_AAMethod.SetFocus();
}


void COrderEntry::OnEnChangeOrderAmountEdit()
{
	if(m_Amount.GetModify())
		ProcessVerifyRisk();
}


void COrderEntry::OnEnKillfocusOrderAmountEdit()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Amount.SetFocus();
}


void COrderEntry::OnEnChangeOrderPriceEdit()
{
	if(m_Price.GetModify())
		ProcessVerifyRisk();
}


void COrderEntry::OnEnKillfocusOrderPriceEdit()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Price.SetFocus();
}


void COrderEntry::OnEnChangeOrderAssetEdit()
{
	if(m_Asset.GetModify())
		ProcessVerifyRisk();
}


void COrderEntry::OnEnKillfocusOrderAssetEdit()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Asset.SetFocus();
}
