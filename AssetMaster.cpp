// AssetMaster.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "AssetMaster.h"
#include "finddlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssetMaster

IMPLEMENT_DYNCREATE(CAssetMaster, CFormView)

CAssetMaster::CAssetMaster()
	: CFormView(CAssetMaster::IDD)
{
	m_Bond = NULL;
	m_Future = NULL;
	m_Accrue = NULL;
	m_PrePaid = NULL;
	m_BelowRating = NULL;
	m_144A = NULL;
	m_1256 = NULL;
	m_Fifo = NULL;
	m_UnListed = NULL;
	m_Inactive = NULL;
	
	m_Country = NULL;
	m_Currency = NULL;
	m_Issuer = NULL;
	m_Industry = NULL;
	m_RateType = NULL;
	m_RateBasis = NULL;
	m_PmntFrq = NULL;
	
	m_Inclusive = NULL;

	m_Category = NULL;
	m_Bucket = NULL;
	m_Class = NULL;
}

CAssetMaster::~CAssetMaster()
{
	if(m_Bond)
		delete m_Bond;
	if(m_Future)
		delete m_Future;
	if(m_Accrue)
		delete m_Accrue;
	if(m_PrePaid)
		delete m_PrePaid;
	if(m_BelowRating)
		delete m_BelowRating;
	if(m_144A)
		delete m_144A;
	if(m_1256)
		delete m_1256;
	if(m_Fifo)
		delete m_Fifo;
	if(m_UnListed)
		delete m_UnListed;
	if(m_Inactive)
		delete m_Inactive;
	
	if(m_Country)
		delete m_Country;
	if(m_Currency)
		delete m_Currency;
	if(m_Issuer)
		delete m_Issuer;
	if(m_Industry)
		delete m_Industry;
	if(m_RateType)
		delete m_RateType;
	if(m_RateBasis)
		delete m_RateBasis;
	if(m_PmntFrq)
		delete m_PmntFrq;
	
	if(m_Inclusive)
		delete m_Inclusive;

	if(m_Category)
		delete m_Category;
	if(m_Bucket)
		delete m_Bucket;
	if(m_Class)
		delete m_Class;
}

void CAssetMaster::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssetMaster)
	DDX_Control(pDX, IDC_ASSET_LIST, m_SS);
	DDX_Control(pDX, IDC_ASSET_INTEREST_LIST, m_IntSS);
	DDX_Control(pDX, IDC_ASSET_CLASS_LIST, m_ClassSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssetMaster, CFormView)
	//{{AFX_MSG_MAP(CAssetMaster)
	ON_COMMAND(ID_ASSET_CLEAR_SCREEN, OnAssetClearScreen)
	ON_COMMAND(ID_ASSET_FIND, OnAssetFind)
	ON_COMMAND(ID_ASSET_ADD, OnAssetAdd)
	ON_COMMAND(ID_ASSET_UPDATE, OnAssetUpdate)
	ON_COMMAND(ID_ASSET_DELETE, OnAssetDelete)
	ON_COMMAND(ID_ASSET_ADD_RATE, OnAssetAddRate)
	ON_COMMAND(ID_ASSET_UPDATE_RATE, OnAssetUpdateRate)
	ON_COMMAND(ID_ASSET_DELETE_RATE, OnAssetDeleteRate)
	ON_COMMAND(ID_ASSET_ADD_CLASS, OnAssetAddClass)
	ON_COMMAND(ID_ASSET_UPDATE_CLASS, OnAssetUpdateClass)
	ON_COMMAND(ID_ASSET_DELETE_CLASS, OnAssetDeleteClass)
	ON_COMMAND(ID_ASSET_FIND_ITEM, OnAssetFindItem)
	ON_COMMAND(ID_ASSET_OTHER, OnAssetOther)
	ON_COMMAND(ID_ASSET_SEC_FEE, OnAssetSecFee)
	ON_UPDATE_COMMAND_UI(ID_ASSET_ADD, OnUpdateAssetAdd)
	ON_UPDATE_COMMAND_UI(ID_ASSET_DELETE, OnUpdateAssetDelete)
	ON_UPDATE_COMMAND_UI(ID_ASSET_FIND, OnUpdateAssetFind)
	ON_UPDATE_COMMAND_UI(ID_ASSET_UPDATE, OnUpdateAssetUpdate)
	ON_UPDATE_COMMAND_UI(ID_ASSET_ADD_CLASS, OnUpdateAssetAddClass)
	ON_UPDATE_COMMAND_UI(ID_ASSET_DELETE_CLASS, OnUpdateAssetDeleteClass)
	ON_UPDATE_COMMAND_UI(ID_ASSET_UPDATE_CLASS, OnUpdateAssetUpdateClass)
	ON_UPDATE_COMMAND_UI(ID_ASSET_ADD_RATE, OnUpdateAssetAddRate)
	ON_UPDATE_COMMAND_UI(ID_ASSET_DELETE_RATE, OnUpdateAssetDeleteRate)
	ON_UPDATE_COMMAND_UI(ID_ASSET_UPDATE_RATE, OnUpdateAssetUpdateRate)
	ON_UPDATE_COMMAND_UI(ID_ASSET_FILL_RATE, OnUpdateAssetFillRate)
	ON_COMMAND(ID_ASSET_FILL_RATE, OnAssetFillRate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssetMaster diagnostics

#ifdef _DEBUG
void CAssetMaster::AssertValid() const
{
	CFormView::AssertValid();
}

void CAssetMaster::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CAssetMaster::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNWIFMS70Doc)));
	return (CNWIFMS70Doc*)m_pDocument;
}
#endif //_DEBUG

void CAssetMaster::EnableSubCtrls()
{
	if(m_Asset.GetWindowTextLength() > 0)
	{
		m_FromDate.EnableWindow(TRUE);
		m_ToDate.EnableWindow(TRUE);
		m_SetDate.EnableWindow(TRUE);
		m_Rate.EnableWindow(TRUE);
		m_Amort.EnableWindow(TRUE);
		m_Cap.EnableWindow(TRUE);
		m_Plus.EnableWindow(TRUE);
		m_Inclusive->EnableWindow(TRUE);
		m_IntFrom.EnableWindow(TRUE);
		m_IntTo.EnableWindow(TRUE);
		m_AssetCode.EnableWindow(TRUE);
		m_AssetClassDesc.EnableWindow(TRUE);
		m_Category->EnableWindow(TRUE);
		m_Bucket->EnableWindow(TRUE);
		m_Class->EnableWindow(TRUE);
	}
	else
	{
		m_FromDate.EnableWindow(FALSE);
		m_ToDate.EnableWindow(FALSE);
		m_SetDate.EnableWindow(FALSE);
		m_Rate.EnableWindow(FALSE);
		m_Amort.EnableWindow(FALSE);
		m_Cap.EnableWindow(FALSE);
		m_Plus.EnableWindow(FALSE);
		m_Inclusive->EnableWindow(FALSE);
		m_IntFrom.EnableWindow(FALSE);
		m_IntTo.EnableWindow(FALSE);
		m_AssetCode.EnableWindow(FALSE);
		m_AssetClassDesc.EnableWindow(FALSE);
		m_Category->EnableWindow(FALSE);
		m_Bucket->EnableWindow(FALSE);
		m_Class->EnableWindow(FALSE);
	}
}

BOOL CAssetMaster::IsAssetOK()
{
	CString Text;

	if(m_Asset.GetWindowTextLength() <= 0)
		Text = "Asset must be entered";

	if(m_AssetDesc.GetWindowTextLength() <= 0)
		Text = "Asset Desc must be entered";

	if(m_Source.GetWindowTextLength() <= 0)
		Text = "Price Source must be entered";

	if(m_Country->GetCurSel() < 0)
		Text = "No Country Selected";

	if(m_Issuer->GetCurSel() < 0)
		Text = "No Issuer Selected";

	if(m_Currency->GetCurSel() < 0)
		Text = "No Currency Selected";

	if(m_1256->GetCheck() == 0)
	{
		CString Currency, Class;
		m_Currency->GetSelString(Currency);
		m_Class->GetSelString(Class);
		if(Class == "CURRENCY FWDS" && (Currency == "AUD" || Currency == "CAD" || 
			Currency == "EUR" || Currency == "CHF" || Currency == "GBP" || Currency == "MXN"))
			Text = "1256 was not checked";
	}

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}

BOOL CAssetMaster::IsInterestOK()
{
	CString Text;
	
	if(m_Asset.GetWindowTextLength() <= 0)
		Text = "Asset must be created first";

	if(m_FromDate.GetData().IsEmpty() || m_ToDate.GetData().IsEmpty() || m_SetDate.GetData().IsEmpty())
		Text = "All dates must be filled";

	if(m_Rate.GetWindowTextLength() <= 0)
		Text = "Rate must be entered";

	if(Text.IsEmpty())
		return TRUE;
	
	MessageBox(Text);
	return FALSE;
}

BOOL CAssetMaster::IsClassOK()
{
	CString Text;
	
	if(m_AssetCode.GetWindowTextLength() <= 0)
		Text = "Asset Code must be entered";

	if(m_AssetClassDesc.GetWindowTextLength() <= 0)
		Text = "Asset Desc must be entered";

	if(m_Category->GetCurSel() < 0)
		Text = "No Category Selected";
	
	if(m_Bucket->GetCurSel() < 0)	
		Text = "No Bucket Selected";
	
	if(m_Class->GetCurSel() < 0)
		Text = "No Asset Class Selected";

	if(m_144A->GetCheck() == 1)
	{
		if(m_Cusip.GetWindowTextLength() <= 0)
			Text = "No cusip for 144A asset";
	}

	if(Text.IsEmpty())
		return TRUE;
	
	MessageBox(Text);
	return FALSE;
}

BOOL CAssetMaster::UpdateData(BOOL bSaveandValid)
{
	m_SS.SetFocus();
	if(bSaveandValid)
	{
		BeginWaitCursor();
		if(strlen(m_Data.GetKey()) == 0)
			m_Data.AddRow();
		else
			m_Data.UpdateRow();
		EndWaitCursor();
	}
	else
	{	
		BeginWaitCursor();
		CQData QData;

		m_Data.LoadDBData(0);

		GetData().LoadSupportData();
		GetData().LoadAssetData();

		QData.CopyDBRecArrayToComboBox(GetData().GetCountryArr(), *m_Country);
		QData.CopyDBRecArrayToComboBox(GetData().GetCurrencyArr(), *m_Currency);
		QData.CopyDBRecArrayToComboBox(GetData().GetIssuerArr(), *m_Issuer);
		QData.CopyDBRecArrayToComboBox(GetData().GetIndustryArr(), *m_Industry);
		QData.CopyDBRecArrayToComboBox(GetData().GetRateTypeArr(), *m_RateType);
		QData.CopyDBRecArrayToComboBox(GetData().GetRateBasisArr(), *m_RateBasis);
		QData.CopyDBRecArrayToComboBox(GetData().GetPmntFrqArr(), *m_PmntFrq);

		QData.CopyDBRecArrayToComboBox(GetData().GetCategoryArr(), *m_Category);
		QData.CopyDBRecArrayToComboBox(GetData().GetBucketArr(), *m_Bucket);
		QData.CopyDBRecArrayToComboBox(GetData().GetAssetTypeArr(), *m_Class);
		
		if(m_SS.GetSheetRows() == 1)
			OnDblClickAssetList(1, 1);
		EndWaitCursor();

		return TRUE;
	}

	if(m_SS.GetSheetRows() > 0)
		m_SS.SetSheetActiveCell(1, m_SS.GetSheetRows());

	m_SS.SetSheetCurRow(-1);
	EnableSubCtrls();	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CAssetMaster message handlers

void CAssetMaster::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BeginWaitCursor();
	m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_IntSS);
	m_Data.GetAssetClass().Setup(&m_ClassSS);
	m_Data.SetHeaders();
	
	m_SS.SetVisibleCols(9);
	m_SS.SetVisibleRows(6);
	m_IntSS.SetVisibleCols(9);
	m_IntSS.SetVisibleRows(4);
	m_ClassSS.SetVisibleCols(5);
	m_ClassSS.SetVisibleRows(4);

	m_Asset.Setup(this, IDC_ASSET_ASSET_EDIT, EMPTYSTRING, UPPERCASE);
	m_Asset.LimitText(10);
	m_AssetDesc.Setup(this, IDC_ASSET_DESC_EDIT, EMPTYSTRING, UPPERCASE);
	m_AssetDesc.LimitText(40);
	m_Par.Setup(this, IDC_ASSET_PAR_EDIT);
	m_Asset2.Setup(this, IDC_ASSET_ASSET2_EDIT, EMPTYSTRING, UPPERCASE);
	m_Asset2.LimitText(10);
	m_Common.Setup(this, IDC_ASSET_COMMON_EDIT, EMPTYSTRING, UPPERCASE);
	m_Common.LimitText(13);
	m_Cusip.Setup(this, IDC_ASSET_CUSIP_EDIT, EMPTYSTRING, UPPERCASE);
	m_Cusip.LimitText(10);
	m_Sedol.Setup(this, IDC_ASSET_SEDOL_EDIT, EMPTYSTRING, UPPERCASE);
	m_Sedol.LimitText(10);
	m_Isin.Setup(this, IDC_ASSET_ISIN_EDIT, EMPTYSTRING, UPPERCASE);
	m_Isin.LimitText(12);
	m_PID.Setup(this, IDC_ASSET_PID_EDIT, EMPTYSTRING, UPPERCASE);
	m_PID.LimitText(20);
	m_IssueDate.Setup(this, IDC_ASSET_ISSUE_DATE_EDIT);
	m_CDSFeeType.Setup(this, IDC_ASSET_DF_FEE_TYPE_EDIT, EMPTYSTRING, UPPERCASE);
	m_CDSFeeType.LimitText(1);
	m_Maturity.Setup(this, IDC_ASSET_MATURITY_EDIT, TRUE, 
					"Maturity date must be greater than or equal to Issue Date", &m_IssueDate);
	m_RollDate.Setup(this, IDC_ASSET_ROLLDATE_EDIT, TRUE, 
					"Roll date must be greater than or equal to Issue Date", &m_IssueDate);
	m_RefIndex.Setup(this, IDC_ASSET_FLOAT_INDEX_EDIT, EMPTYSTRING, UPPERCASE);
	m_RefIndex.LimitText(10);
	m_Formula.Setup(this, IDC_ASSET_FLOAT_FORM_EDIT, EMPTYSTRING, UPPERCASE);
	m_Formula.LimitText(20);
	m_Source.Setup(this, IDC_ASSET_SOURCE_EDIT, EMPTYSTRING, UPPERCASE);
	m_Source.LimitText(12);
	
	m_Country = (COptComboBox*) new COptComboBox(this, IDC_ASSET_COUNTRY_COMBO);
	m_Currency = (COptComboBox*) new COptComboBox(this, IDC_ASSET_CURRENCY_COMBO);
	m_Issuer = (COptComboBox*) new COptComboBox(this, IDC_ASSET_ISSUER_COMBO);
	m_Industry = (COptComboBox*) new COptComboBox(this, IDC_ASSET_INDUSTRY_COMBO, TRUE);
	m_RateType = (COptComboBox*) new COptComboBox(this, IDC_ASSET_RATE_TYPE_COMBO, TRUE);
	m_RateBasis = (COptComboBox*) new COptComboBox(this, IDC_ASSET_RATEBASIS_COMBO, TRUE);
	m_PmntFrq = (COptComboBox*) new COptComboBox(this, IDC_ASSET_PMNT_FRQ_COMBO, TRUE);
	m_AdjFactor.Setup(this, IDC_ASSET_ADJUST_FACTOR_EDIT);

	m_Bond = (CCheckBox *) new CCheckBox(this, IDC_ASSET_BOND_CHECK, "100", "1");
	m_Accrue = (CCheckBox *) new CCheckBox(this, IDC_ASSET_ACCRUE_CHECK, "Y");
	m_PrePaid = (CCheckBox *) new CCheckBox(this, IDC_ASSET_PPAY_CHECK, "Y");
	m_Future = (CCheckBox *) new CCheckBox(this, IDC_ASSET_FUTURE_CHECK, "Y");
	m_1256 = (CCheckBox *) new CCheckBox(this, IDC_ASSET_1256_CHECK, "Y");
	m_BelowRating = (CCheckBox *) new CCheckBox(this, IDC_ASSET_BELOW_RATING_CHECK, "Y");
	m_144A = (CCheckBox *) new CCheckBox(this, IDC_ASSET_144A_CHECK, "Y");
	m_Fifo = (CCheckBox *) new CCheckBox(this, IDC_ASSET_FIFO_CHECK, "FIFO", "AVCOST");
	m_UnListed = (CCheckBox *) new CCheckBox(this, IDC_ASSET_LISTED_CHECK, "Y");
	m_Inactive = (CCheckBox *) new CCheckBox(this, IDC_ASSET_ACTIVE_CHECK, "Y");

	m_FromDate.Setup(this, IDC_ASSET_FROM_EDIT);
	m_ToDate.Setup(this, IDC_ASSET_TO_EDIT, TRUE, 
					"To Date must be greater than or equal to from Date", &m_FromDate);
	m_SetDate.Setup(this, IDC_ASSET_SET_DATE_EDIT);
	m_Rate.Setup(this, IDC_ASSET_RATE_EDIT);
	m_Amort.Setup(this, IDC_ASSET_AMORT_EDIT);
	m_Cap.Setup(this, IDC_ASSET_CAP_EDIT);
	m_Plus.Setup(this, IDC_ASSET_PLUS_AMT_EDIT);
	m_Inclusive = (CCheckBox *) new CCheckBox(this, IDC_ASSET_INCLUSIVE_CHECK, "Y");
	m_IntFrom.Setup(this, IDC_ASSET_INTFROM_EDIT);
	m_IntTo.Setup(this, IDC_ASSET_INTTO_EDIT, TRUE, 
					"To Date must be greater than or equal to from Date", &m_IntFrom);

	m_AssetCode.Setup(this, IDC_ASSET_ASSETCODE_EDIT, EMPTYSTRING, UPPERCASE);
	m_AssetCode.LimitText(10);
	m_AssetClassDesc.Setup(this, IDC_ASSET_ASSETDESC_EDIT, EMPTYSTRING, UPPERCASE);
	m_AssetClassDesc.LimitText(40);
	m_AssetMaster.Setup(this, IDC_ASSET_ASSETMASTER_EDIT, EMPTYSTRING, UPPERCASE);
	m_AssetMaster.LimitText(10);
	m_Category = (COptComboBox*) new COptComboBox(this, IDC_ASSET_CATEGORY_COMBO);
	m_Bucket = (COptComboBox*) new COptComboBox(this, IDC_ASSET_BUCKET_COMBO);
	m_Class = (COptComboBox*) new COptComboBox(this, IDC_ASSET_CLASS_COMBO);
				
	m_Data.Add(&m_Asset, &m_Data.GetAssetRec().GetAsset());
	m_Data.Add(&m_AssetDesc, &m_Data.GetAssetRec().GetDesc());
	m_Data.Add(&m_Par, &m_Data.GetAssetRec().GetPar());
	m_Data.Add(&m_Asset2, &m_Data.GetAssetRec().GetUnderline());
	m_Data.Add(&m_Common, &m_Data.GetAssetRec().GetCommon());
	m_Data.Add(&m_Cusip, &m_Data.GetAssetRec().GetCusip());
	m_Data.Add(&m_Sedol, &m_Data.GetAssetRec().GetSedol());
	m_Data.Add(&m_Isin, &m_Data.GetAssetRec().GetIsin());
	m_Data.Add(&m_PID, &m_Data.GetAssetRec().GetPID());
	m_Data.Add(&m_IssueDate, &m_Data.GetAssetRec().GetIssueDate());
	m_Data.Add(&m_CDSFeeType, &m_Data.GetAssetRec().GetCDSFeeType());
	m_Data.Add(&m_Maturity, &m_Data.GetAssetRec().GetMaturity());
	m_Data.Add(&m_RollDate, &m_Data.GetAssetRec().GetRollDate());
	m_Data.Add(&m_RefIndex, &m_Data.GetAssetRec().GetRefIndex());
	m_Data.Add(&m_Formula, &m_Data.GetAssetRec().GetFormula());
	m_Data.Add(&m_Source, &m_Data.GetAssetRec().GetSource());
	m_Data.Add(m_Country, &m_Data.GetAssetRec().GetCountry());
	m_Data.Add(m_Currency, &m_Data.GetAssetRec().GetCurrency());
	m_Data.Add(m_Issuer, &m_Data.GetAssetRec().GetIssuer());
	m_Data.Add(m_Industry, &m_Data.GetAssetRec().GetIndustry());
	m_Data.Add(m_RateType, &m_Data.GetAssetRec().GetRateType());
	m_Data.Add(m_RateBasis, &m_Data.GetAssetRec().GetRateBasis());
	m_Data.Add(m_PmntFrq, &m_Data.GetAssetRec().GetPmntFrq());
	m_Data.Add(&m_AdjFactor, &m_Data.GetAssetRec().GetAdjFactor());
	m_Data.Add(m_Bond, &m_Data.GetAssetRec().GetFactor());
	m_Data.Add(m_Accrue, &m_Data.GetAssetRec().GetAccrue());
	m_Data.Add(m_PrePaid, &m_Data.GetAssetRec().GetPrePaid());
	m_Data.Add(m_Future, &m_Data.GetAssetRec().GetFuture());
	m_Data.Add(m_1256, &m_Data.GetAssetRec().Get1256());
	m_Data.Add(m_BelowRating, &m_Data.GetAssetRec().GetBelowRating());
	m_Data.Add(m_144A, &m_Data.GetAssetRec().Get144A());
	m_Data.Add(m_Fifo, &m_Data.GetAssetRec().GetMethod());
	m_Data.Add(m_UnListed, &m_Data.GetAssetRec().GetUnlisted());
	m_Data.Add(m_Inactive, &m_Data.GetAssetRec().GetInactive());
	m_Data.SetKeyIndex(0);

	m_Data.GetSRowCtrl().Add(&m_FromDate, &m_Data.GetIntRec().GetAssetFrom());
	m_Data.GetSRowCtrl().Add(&m_ToDate, &m_Data.GetIntRec().GetAssetTo());
	m_Data.GetSRowCtrl().Add(&m_SetDate, &m_Data.GetIntRec().GetSetDate());
	m_Data.GetSRowCtrl().Add(&m_Rate, &m_Data.GetIntRec().GetRate());
	m_Data.GetSRowCtrl().Add(&m_Amort, &m_Data.GetIntRec().GetAmort());
	m_Data.GetSRowCtrl().Add(&m_Cap, &m_Data.GetIntRec().GetCap());
	m_Data.GetSRowCtrl().Add(&m_Plus, &m_Data.GetIntRec().GetPlus());
	m_Data.GetSRowCtrl().Add(m_Inclusive, &m_Data.GetIntRec().GetInclusive());
	m_Data.GetSRowCtrl().Add(&m_IntFrom, &m_Data.GetIntRec().GetFrom());
	m_Data.GetSRowCtrl().Add(&m_IntTo, &m_Data.GetIntRec().GetTo());

	m_Data.GetAssetClass().Add(&m_AssetCode, &m_Data.GetClassRec().GetAssetCode());
	m_Data.GetAssetClass().Add(&m_AssetClassDesc, &m_Data.GetClassRec().GetAssetDesc());
	m_Data.GetAssetClass().Add(&m_AssetMaster, &m_Data.GetClassRec().GetAsset());
	m_Data.GetAssetClass().Add(m_Category, &m_Data.GetClassRec().GetCategory());
	m_Data.GetAssetClass().Add(m_Bucket, &m_Data.GetClassRec().GetBucket());
	m_Data.GetAssetClass().Add(m_Class, &m_Data.GetClassRec().GetClass());

	GetData().LoadDates();
	m_Data.Refresh();
	UpdateData(FALSE);
	EndWaitCursor();
}

BEGIN_EVENTSINK_MAP(CAssetMaster, CFormView)
    //{{AFX_EVENTSINK_MAP(CAssetMaster)
	ON_EVENT(CAssetMaster, IDC_ASSET_LIST, 2 /* BlockSelected */, OnBlockSelectedAssetList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CAssetMaster, IDC_ASSET_LIST, 11 /* DblClick */, OnDblClickAssetList, VTS_I4 VTS_I4)
	ON_EVENT(CAssetMaster, IDC_ASSET_INTEREST_LIST, 2 /* BlockSelected */, OnBlockSelectedAssetInterestList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CAssetMaster, IDC_ASSET_INTEREST_LIST, 11 /* DblClick */, OnDblClickAssetInterestList, VTS_I4 VTS_I4)
	ON_EVENT(CAssetMaster, IDC_ASSET_CLASS_LIST, 2 /* BlockSelected */, OnBlockSelectedAssetClassList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CAssetMaster, IDC_ASSET_CLASS_LIST, 11 /* DblClick */, OnDblClickAssetClassList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAssetMaster::OnBlockSelectedAssetList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickAssetList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CAssetMaster::OnDblClickAssetList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row);
		m_Data.GetAssetClass().SetCurrentRow(m_ClassSS.GetSheetRows() >= 1 ? 1 : -1);
	}
		
	EnableSubCtrls();
}

void CAssetMaster::OnBlockSelectedAssetInterestList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickAssetInterestList(m_IntSS.GetActiveCol(), m_IntSS.GetActiveRow());
}

void CAssetMaster::OnDblClickAssetInterestList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_IntSS.GetSheetRows())
		m_Data.GetSRowCtrl().SetCurrentRow(Row);
	else
	{
		m_Data.GetSRowCtrl().SetCurrentRow(-1);
		m_Data.GetSRowCtrl().Refresh();
	}
}

void CAssetMaster::OnBlockSelectedAssetClassList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickAssetClassList(m_ClassSS.GetActiveCol(), m_ClassSS.GetActiveRow());
}

void CAssetMaster::OnDblClickAssetClassList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_ClassSS.GetSheetRows())
		m_Data.GetAssetClass().SetCurrentRow(Row);
	else
	{
		m_Data.GetAssetClass().SetCurrentRow(-1);
		m_Data.GetAssetClass().Refresh();
	}
}

void CAssetMaster::OnAssetClearScreen() 
{
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	m_Data.GetAssetClass().SetCurrentRow(-1);
	m_Data.GetAssetClass().Refresh();
	EnableSubCtrls();
}

void CAssetMaster::OnAssetFind() 
{
	UpdateData(FALSE);	
}

void CAssetMaster::OnAssetAdd() 
{
	if(IsAssetOK())
	{
		m_Data.SetKey(EMPTYSTRING);
		UpdateData(TRUE);
	}
}

void CAssetMaster::OnAssetUpdate() 
{
	if(m_SS.GetSheetCurRow() <= 0)
		return;
		
	if(IsAssetOK())
	{
		m_Data.SetKey();
		UpdateData(TRUE);
	}
}

void CAssetMaster::OnAssetDelete() 
{
	if(MessageBox("Are you sure you want to delete this asset ?", "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	
	BeginWaitCursor();
	m_Data.DeleteRow();
	EndWaitCursor();
}

void CAssetMaster::OnAssetAddRate() 
{
	if(IsInterestOK())
		m_Data.AddSRow();
}

void CAssetMaster::OnAssetUpdateRate() 
{
	if(IsInterestOK())
		m_Data.UpdateSRow();
}

void CAssetMaster::OnAssetDeleteRate() 
{
	if(MessageBox("Are you sure you want to delete these interest ?", "Delete", MB_OKCANCEL) == IDOK)
		m_Data.DeleteSRow();
}

void CAssetMaster::OnAssetAddClass() 
{
	if(IsClassOK())
	{
		m_Data.GetAssetClass().UpdateData();
		m_Data.GetClassRec().SetAsset(m_Data.GetKey());
		if(m_Data.GetClassRec().AddRec(GetData().GetOraLoader()))
			m_Data.GetAssetClass().AddSheetRow();
	}
}

void CAssetMaster::OnAssetUpdateClass() 
{
	if(IsClassOK())
	{
		m_Data.GetAssetClass().UpdateData();
		m_Data.GetClassRec().SetID(m_Data.GetAssetClass().GetID());
		if(m_Data.GetClassRec().UpdateRec(GetData().GetOraLoader(), TRUE))
			m_Data.GetAssetClass().UpdateSheetRow(FALSE);
	}
}

void CAssetMaster::OnAssetDeleteClass() 
{
	if(MessageBox("Are you sure you want to delete these classification ?", "Delete", MB_OKCANCEL) == IDOK)
	{
		m_Data.GetAssetClass().UpdateData();
		m_Data.GetClassRec().SetID(m_Data.GetAssetClass().GetID());
		if(m_Data.GetClassRec().DeleteRec(GetData().GetOraLoader(), TRUE))
			m_Data.GetAssetClass().DeleteSheetRow();
	}
}

void CAssetMaster::OnAssetFindItem() 
{
/*	CFindDlg FindDlg;

	FindDlg.m_OraLoader = GetData().GetOraLoader();
	FindDlg.m_OraLoader.GetSql() = Sql;
	
	Edit.GetWindowText(FindDlg.m_Key);
	if(FindDlg.DoModal() == IDOK)
		Edit.SetWindowText(FindDlg.m_Key); */
}

void CAssetMaster::OnAssetOther() 
{
	// TODO: Add your command handler code here
	
}

void CAssetMaster::OnAssetSecFee() 
{
	// TODO: Add your command handler code here
	
}

void CAssetMaster::OnUpdateAssetAdd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_Asset.GetWindowTextLength() > 0);
}

void CAssetMaster::OnUpdateAssetDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_SS.GetIsBlockSelected());
}

void CAssetMaster::OnUpdateAssetUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
}

void CAssetMaster::OnUpdateAssetFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CAssetMaster::OnUpdateAssetAddClass(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_AssetCode.GetWindowTextLength() > 0 ? TRUE : FALSE);
}

void CAssetMaster::OnUpdateAssetDeleteClass(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_ClassSS.GetIsBlockSelected());	
}

void CAssetMaster::OnUpdateAssetUpdateClass(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_Data.GetAssetClass().GetCurrentRow() > 0 ? TRUE : FALSE);
}

void CAssetMaster::OnUpdateAssetAddRate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && !m_FromDate.GetData().IsEmpty() ? TRUE : FALSE);
}

void CAssetMaster::OnUpdateAssetDeleteRate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_IntSS.GetIsBlockSelected());
}

void CAssetMaster::OnUpdateAssetUpdateRate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetData().IsGuest() && m_Data.GetSRowCtrl().GetCurrentRow() > 0 ? TRUE : FALSE);	
}

void CAssetMaster::OnUpdateAssetFillRate(CCmdUI* pCmdUI) 
{
	CString RateType, Freq, AssetClass;

	m_RateType->GetSelString(RateType);
	m_PmntFrq->GetSelString(Freq);
	m_Class->GetSelString(AssetClass);
	
	pCmdUI->Enable(m_Asset.GetWindowTextLength() > 0 && !m_Maturity.GetData().IsEmpty() && 
		          (RateType == "FIXED" || AssetClass.Find("CDS SWAPS") >= 0) && 
				  !Freq.IsEmpty() && m_IntSS.GetSheetRows() > 0);	
}

void CAssetMaster::OnAssetFillRate() 
{
	CString Asset, AssetClass, RateType, Freq, Maturity, IntToDate, Rate;

	m_Asset.GetWindowText(Asset);
	m_Class->GetSelString(AssetClass);
	m_RateType->GetSelString(RateType);
	m_PmntFrq->GetSelString(Freq);
	m_Maturity.GetWindowText(Maturity);
	IntToDate = m_ToDate.GetData();
	m_Rate.GetWindowText(Rate);
	
	m_Data.GenerateFixedRates(Asset, AssetClass, RateType, Freq, Maturity, IntToDate, Rate);	
}


