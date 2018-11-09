// Maintenance.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "Maintenance.h"
#include "finddlg.h"
#include "othermaint.h"
#include "onefield.h"
#include "twofield.h"
#include "threefield.h"
#include "fourfield.h"
#include "cpdialog.h"
#include "portfolio.h"
#include "portfolioinfo.h"
#include "portfolioaccount.h"
#include "assetsupplement.h"
#include "country.h"
#include "OrFeeDlg.h"
#include "secfee.h"
#include "investordialog.h"
#include "claimcp.h"
#include "accountdlg.h"
#include "agentdlg.h"
#include "bankdlg.h"
#include "PaymentTemplateDlg.h"
#include "paymentrestriction.h"
#include "autoallocationdlg.h"
#include "currencydlg.h"
#include "assetclassdlg.h"
#include "clearingfeedlg.h"
#include "issuerdlg.h"
#include "optmap.h"
#include "refIndexdlg.h"
#include "PrimeCPMargin.h"
#include "CPFutureCommDlg.h"
#include "CPOTCFeeDlg.h"
#include "OptSettlementDlg.h"
#include "FutureFeesDlg.h"
#include "GiveupFeeDlg.h"
#include "TraderDlg.h"
#include "OPsDlg.h"
#include "YieldCurveDlg.h"
#include "qdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaintenance
IMPLEMENT_DYNCREATE(CMaintenance, CFormView)

CMaintenance::CMaintenance() : CFormView(CMaintenance::IDD)
{
	//{{AFX_DATA_INIT(CMaintenance)
	m_CurrID = 0;
	m_bLoaded = FALSE;
	//}}AFX_DATA_INIT
}

void CMaintenance::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenance)
	DDX_Control(pDX, IDC_MAINT_ASSET_LIST, m_SS);
	DDX_Control(pDX, IDC_MAINT_INTEREST_LIST, m_IntSS);
	DDX_Control(pDX, IDC_MAINT_HOLDING_LIST, m_HoldingSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMaintenance, CFormView)
	//{{AFX_MSG_MAP(CMaintenance)
	ON_COMMAND(ID_MAINT_ADD, OnMaintAdd)
	ON_UPDATE_COMMAND_UI(ID_MAINT_ADD, OnUpdateMaintAdd)
	ON_COMMAND(ID_MAINT_CLEAR_SCREEN, OnMaintClearScreen)
	ON_UPDATE_COMMAND_UI(ID_MAINT_DELETE, OnUpdateMaintDelete)
	ON_COMMAND(ID_MAINT_DELETE, OnMaintDelete)
	ON_COMMAND(ID_MAINT_FIND, OnMaintFind)
	ON_COMMAND(ID_MAINT_UPDATE, OnMaintUpdate)
	ON_UPDATE_COMMAND_UI(ID_MAINT_UPDATE, OnUpdateMaintUpdate)
	ON_BN_CLICKED(IDC_MAINT_BOND_CHECK, OnMaintBondCheck)
	ON_EN_CHANGE(IDC_MAINT_ASSET_CODE, OnChangeMaintAsset)
	ON_COMMAND(ID_MAINT_ADD_RATE, OnMaintAddRate)
	ON_UPDATE_COMMAND_UI(ID_MAINT_ADD_RATE, OnUpdateMaintAddRate)
	ON_COMMAND(ID_MAINT_DELETE_RATE, OnMaintDeleteRate)
	ON_UPDATE_COMMAND_UI(ID_MAINT_DELETE_RATE, OnUpdateMaintDeleteRate)
	ON_COMMAND(ID_MAINT_UPDATE_RATE, OnMaintUpdateRate)
	ON_UPDATE_COMMAND_UI(ID_MAINT_UPDATE_RATE, OnUpdateMaintUpdateRate)
	ON_COMMAND(ID_MAINT_FIND_ITEM, OnMaintFindItem)
	ON_UPDATE_COMMAND_UI(ID_MAINT_FIND_ITEM, OnUpdateMaintFindItem)
	ON_COMMAND(ID_MAINT_OTHER, OnMaintOther)
	ON_COMMAND(ID_MAINT_SEC_FEE, OnMaintSecFee)
	ON_CBN_SELCHANGE(IDC_MAINT_ASSET_TYPE_COMBO, OnSelchangeMaintAssetTypeCombo)
	ON_EN_SETFOCUS(IDC_MAINT_UNDERLINE_EDIT, OnSetfocusMaintUnderlineEdit)
	ON_UPDATE_COMMAND_UI(ID_MAINT_OTHER, OnUpdateMaintOther)
	ON_UPDATE_COMMAND_UI(ID_MAINT_FILL_RATE, OnUpdateMaintFillRate)
	ON_UPDATE_COMMAND_UI(ID_MAINT_SEC_FEE, OnUpdateMaintSecFee)
	ON_COMMAND(ID_MAINT_FILL_RATE, OnMaintFillRate)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_MAINT_RATE_TYPE_COMBO, &CMaintenance::OnCbnSelchangeMaintRateTypeCombo)
	ON_EN_SETFOCUS(IDC_MAINT_CALENDAR_EDIT, &CMaintenance::OnEnSetfocusMaintCalendarEdit)
	ON_EN_SETFOCUS(IDC_MAINT_RESTRUCTURING_EDIT, &CMaintenance::OnEnSetfocusMaintRestructuringEdit)
	ON_COMMAND(ID_MAINT_OR_FEE, &CMaintenance::OnMaintOrFee)
	ON_UPDATE_COMMAND_UI(ID_MAINT_OR_FEE, &CMaintenance::OnUpdateMaintOrFee)
	ON_COMMAND(ID_MAINT_IMPORT, &CMaintenance::OnMaintImport)
	ON_EN_KILLFOCUS(IDC_MAINT_ISSUE_DATE_EDIT, &CMaintenance::OnEnKillfocusMaintIssueDateEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintenance diagnostics
#ifdef _DEBUG
void CMaintenance::AssertValid() const
{
	CFormView::AssertValid();
}

void CMaintenance::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CMaintenance::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

void CMaintenance::EnableCtrls(CString &Class)
{
	BOOL bEnable = FALSE;
	CString Text;

	bEnable = (Class.Find("CDS SWAPS") >= 0 || Class.Find("EM SWAPS") >= 0 || Class.Find("DM SWAPS") >= 0) ? TRUE : FALSE;
	m_BDate.EnableWindow(bEnable);
	m_BDC.EnableWindow(bEnable);

	bEnable = Class.Find("CDS") >= 0 ? TRUE : FALSE;
	m_CDSFeeType.EnableWindow(bEnable);
	m_CDSRestructuring.EnableWindow(bEnable);

	Text = m_RateType.GetData();
	if (Class.Find("CDS SWAPS") >= 0 || ((Class.Find("EM SWAPS") >= 0 || Class.Find("DM SWAPS") >= 0 || Class.Find("G7 VAR SWAPS") >= 0) && Text == "FIXED"))
		bEnable = TRUE;
	
	m_Underline.EnableWindow(bEnable);
	m_SwapFactor.EnableWindow(bEnable);
	m_FRA.EnableWindow(bEnable);
	
	bEnable = Text == "FIXED" ? FALSE : TRUE;
	m_Index.EnableWindow(bEnable);

	if(Class == "CURRENCY FWDS" || Class == "CURRENCY OPTION")
	{
		m_Currency2.EnableWindow(TRUE);
		if(m_Currency2.GetCurSel() <0)
			m_Currency2.SetData(m_AssetDesc.GetData().Left(3));
	}
	else
	{
		m_Currency2.SetCurSel(-1);
		m_Currency2.EnableWindow(FALSE);
	}

	if(Class == "CURRENCY FWDS" || Class.Find("FUTURES") >= 0)
	{
		if(m_Method.GetCurSel() < 0)
			m_Method.SetData("FIFO");
	}
}

void CMaintenance::EnableInvCtrls()
{
	BOOL bEnabled;

	if(m_Asset.GetWindowTextLength() > 0)
		bEnabled = TRUE;
	else
		bEnabled = FALSE;

	m_FromDate.EnableWindow(bEnabled);
	m_ToDate.EnableWindow(bEnabled);
	m_SetDate.EnableWindow(bEnabled);
	m_EndDate.EnableWindow(bEnabled);
	m_Rate.EnableWindow(bEnabled);
	m_Amort.EnableWindow(bEnabled);
	m_Cap.EnableWindow(bEnabled);
	m_Plus.EnableWindow(bEnabled);
	m_Action.EnableWindow(bEnabled);
}

void CMaintenance::FindFieldItem(CCntlEdit &Edit, const char *Sql)
{
	CFindDlg FindDlg;

	FindDlg.m_OraLoader = GetData().GetOraLoader();
	FindDlg.m_OraLoader.GetSql() = Sql;
	
	FindDlg.m_Key = Edit.GetData();
	if(FindDlg.DoModal() == IDOK)
		Edit.SetData(FindDlg.m_Key);
}

void CMaintenance::InitControls()
{
	m_SS.SetVisibleCols(11);
	m_SS.SetVisibleRows(6);

	m_IntSS.SetVisibleCols(8);
	m_IntSS.SetVisibleRows(4);

	m_HoldingSS.SetVisibleCols(7);
	m_HoldingSS.SetVisibleRows(3);

	m_Bond.Setup(this, IDC_MAINT_BOND_CHECK, BONDTYPE, EQUITYTYPE);
	m_Accrue.Setup(this, IDC_MAINT_ACCRUE_CHECK, Y, N);
	m_PrePaid.Setup(this, IDC_MAINT_PPAY_CHECK, Y, N);
	m_UnListed.Setup(this, IDC_MAINT_UNLISTED_CHECK, Y, N);
	m_OTCClear.Setup(this, IDC_MAINT_OTCCLEAR_CHECK, Y);
	m_Forward.Setup(this, IDC_MAINT_FORWARD_CHECK, "F");
	m_Active.Setup(this, IDC_MAINT_ACTIVE_CHECK, N);
	m_1256.Setup(this, IDC_MAINT_1256_CHECK, Y, N);
	m_BelowRating.Setup(this, IDC_MAINT_BELOW_RATING_CHECK, Y);
	m_144A.Setup(this, IDC_MAINT_144A_CHECK, Y);
	m_Margin.Setup(this, IDC_MAINT_MARGIN_CHECK, Y);
	m_FRA.Setup(this, IDC_MAINT_FRA_CHECK, Y);

	m_Industry.Setup(this, IDC_MAINT_INDUSTRY_COMBO);
	m_Country.Setup(this, IDC_MAINT_COUNTRY_COMBO);
	m_ProfileCountry.Setup(this, IDC_MAINT_PROFILECOUNTRY_COMBO);
	m_Currency.Setup(this, IDC_MAINT_CURRENCY_COMBO);
	m_Currency2.Setup(this, IDC_MAINT_FWDCURRENCY_COMBO);
	m_Class.Setup(this, IDC_MAINT_ASSET_TYPE_COMBO);
	m_ProfileClass.Setup(this, IDC_MAINT_ASSET_PROFILECLASS_COMBO);
	m_RateBasis.Setup(this, IDC_MAINT_RATEBASIS_COMBO);
	m_RateType.Setup(this, IDC_MAINT_RATE_TYPE_COMBO, TRUE);
	m_Category.Setup(this, IDC_MAINT_STRATEGY_COMBO);
	m_Issuer.Setup(this, IDC_MAINT_ISSUER_COMBO);
	m_Action.Setup(this, IDC_MAINT_ACTION_COMBO, TRUE);
	m_Method.Setup(this, IDC_MAINT_METHOD_COMBO, TRUE);
	m_Liquidity.Setup(this, IDC_MAINT_LIQUIDITY_COMBO);
	m_PFClass.Setup(this, IDC_MAINT_PFCLASS_COMBO);
	m_PFSubClass.Setup(this, IDC_MAINT_PFSUBCLASS_COMBO, TRUE);
	m_CitiClass.Setup(this, IDC_MAINT_CITICLASS_COMBO, TRUE);
	m_BlmKey.Setup(this, IDC_MAINT_BLMKEY_COMBO, TRUE);
	m_Account.Setup(this, IDC_MAINT_ACCOUNT_COMBO, TRUE);
	m_FutureType.Setup(this, IDC_MAINT_FUTTYPE_COMBO, TRUE);

	m_Index.Setup(this, IDC_MAINT_FLOAT_INDEX_COMBO, 2, TRUE);
	m_PmntFreq.Setup(this, IDC_MAINT_PMNT_FRQ_COMBO, 2, TRUE);
	m_CmpdFreq.Setup(this, IDC_MAINT_CMPD_FRQ_COMBO, 2, TRUE);
	m_Bucket.Setup(this, IDC_MAINT_BUCKET_COMBO);
	m_Bucket2.Setup(this, IDC_MAINT_BUCKET2_COMBO, 2, TRUE);
	m_Bucket3.Setup(this, IDC_MAINT_BUCKET3_COMBO, 2, TRUE);
	m_Class2.Setup(this, IDC_MAINT_PCLASS2_COMBO, 2, TRUE);
	m_Level.Setup(this, IDC_MAINT_LEVEL_COMBO);
	m_CDSFeeType.Setup(this, IDC_MAINT_DF_FEE_TYPE_COMBO, 2, TRUE);
	m_BDate.Setup(this, IDC_MAINT_CALENDAR_COMBO, 2, TRUE);
	m_CDSRestructuring.Setup(this, IDC_MAINT_CDSRESTRUCTURING_COMBO, 2, TRUE);
	m_BDC.Setup(this, IDC_MAINT_BDC_COMBO, 2, TRUE);
	m_YieldCurve.Setup(this, IDC_MAINT_YC_COMBO, 2, TRUE);
	m_IRSTemplate.Setup(this, IDC_MAINT_TMPLATE_COMBO, 2, TRUE);
	m_Exchange.Setup(this, IDC_MAINT_EXCHANGE_COMBO, 2, TRUE);

	m_Asset.Setup(this, IDC_MAINT_ASSET_CODE);
	m_Asset.LimitText(10);
	m_AssetID.Setup(this, IDC_MAINT_ASSETID_EDIT);
	m_AssetID.EnableWindow(FALSE);
	m_AssetDesc.Setup(this, IDC_MAINT_ASSETDESC_EDIT);
	m_AssetDesc.LimitText(40);
	m_Common.Setup(this, IDC_MAINT_COMMON_EDIT);
	m_Common.LimitText(13);
	m_Cusip.Setup(this, IDC_MAINT_CUSIP_EDIT);
	m_Cusip.LimitText(10);
	m_Isin.Setup(this, IDC_MAINT_ISIN_EDIT);
	m_Isin.LimitText(12);
	m_RedCode.Setup(this, IDC_MAINT_REDCODE_EDIT);
	m_RedCode.LimitText(9);
	m_ImagineCode.Setup(this, IDC_MAINT_IMGCODE_EDIT);
	m_ImagineCode.LimitText(25);
	m_Sedol.Setup(this, IDC_MAINT_SEDOL_EDIT);
	m_Sedol.LimitText(10);
	m_PID.Setup(this, IDC_MAINT_PID_EDIT);
	m_PID.LimitText(25);
	m_IssueDate.Setup(this, IDC_MAINT_ISSUE_DATE_EDIT);
	m_CouponStartDate.Setup(this, IDC_MAINT_START_DATE_EDIT, TRUE, "Coupon Start Date must be greater or equal to Issue Date");
	m_RollDate.Setup(this, IDC_MAINT_ROLLDATE_EDIT, TRUE, "Dated Date must be greater or equal to Issue Date", &m_CouponStartDate);
	m_Maturity.Setup(this, IDC_MAINT_MATURITY_EDIT, TRUE, "Maturity Date must be greater or equal to Issue Date", &m_IssueDate);
	m_FxDate.Setup(this, IDC_MAINT_FXDATE_EDIT, TRUE, "FxFix Date must be greater or equal to Issue Date", &m_IssueDate);

	m_Formula.Setup(this, IDC_MAINT_FLOAT_FORM_EDIT);
	m_Formula.LimitText(20);
	m_Source.Setup(this, IDC_MAINT_SOURCE_EDIT);
	m_Source.LimitText(20);
	m_FixedRate.Setup(this, IDC_MAINT_FIXEDRATE_EDIT);
	m_Par.Setup(this, IDC_MAINT_PARVAL_EDIT);
	m_SwapFactor.Setup(this, IDC_MAINT_SWAPFACTOR_EDIT);
	m_Amount.Setup(this, IDC_MAINT_AMOUNT_EDIT);
	m_Underline.Setup(this, IDC_MAINT_UNDERLINE_EDIT);
	m_Underline.LimitText(10);
	m_UnderlineID.Setup(this, IDC_MAINT_UNDERLINEID_EDIT);
	m_UnderlineID.LimitText(10);
	m_AdminID.Setup(this, IDC_MAINT_ADMINID_EDIT);
	m_AdminID.LimitText(15);	
	
	m_Data.Add(&m_Asset, &m_Data.GetAssetRec().GetAsset());
	m_Data.Add(&m_AssetID, &m_Data.GetAssetRec().GetAssetID());
	m_Data.Add(&m_AssetDesc, &m_Data.GetAssetRec().GetDesc());
	m_Data.Add(&m_Common, &m_Data.GetAssetRec().GetCommon());
	m_Data.Add(&m_Cusip, &m_Data.GetAssetRec().GetCusip());
	m_Data.Add(&m_Isin, &m_Data.GetAssetRec().GetIsin());
	m_Data.Add(&m_Sedol, &m_Data.GetAssetRec().GetSedol());
	m_Data.Add(&m_RedCode, &m_Data.GetAssetRec().GetRedCode());
	m_Data.Add(&m_ImagineCode, &m_Data.GetAssetRec().GetImagineCode());
	m_Data.Add(&m_PID, &m_Data.GetAssetRec().GetPID());
	m_Data.Add(&m_IssueDate, &m_Data.GetAssetRec().GetIssueDate());
	m_Data.Add(&m_CouponStartDate, &m_Data.GetAssetRec().GetCouponStartDate());
	m_Data.Add(&m_RollDate, &m_Data.GetAssetRec().GetRollDate());
	m_Data.Add(&m_Maturity, &m_Data.GetAssetRec().GetMaturity());
	m_Data.Add(&m_FxDate, &m_Data.GetAssetRec().GetFxFixDate());
	m_Data.Add(&m_Index, &m_Data.GetAssetRec().GetIndex());
	m_Data.Add(&m_Formula, &m_Data.GetAssetRec().GetFormula());
	m_Data.Add(&m_Source, &m_Data.GetAssetRec().GetSource());
	m_Data.Add(&m_Industry, &m_Data.GetAssetRec().GetIndustry());
	m_Data.Add(&m_Country, &m_Data.GetAssetRec().GetCountry());
	m_Data.Add(&m_ProfileCountry, &m_Data.GetAssetRec().GetProfileCountry());
	m_Data.Add(&m_Currency, &m_Data.GetAssetRec().GetCurrency());
	m_Data.Add(&m_Currency2, &m_Data.GetAssetRec().GetCurrency2());
	m_Data.Add(&m_Class, &m_Data.GetAssetRec().GetClass());
	m_Data.Add(&m_Class2, &m_Data.GetAssetRec().GetClass2());
	m_Data.Add(&m_ProfileClass, &m_Data.GetAssetRec().GetProfileClass());
	m_Data.Add(&m_RateBasis, &m_Data.GetAssetRec().GetRateBasis());
	m_Data.Add(&m_RateType, &m_Data.GetAssetRec().GetRateType());
	m_Data.Add(&m_PmntFreq, &m_Data.GetAssetRec().GetPmntFreq());
	m_Data.Add(&m_CmpdFreq, &m_Data.GetAssetRec().GetCmpdFreq());
	m_Data.Add(&m_FixedRate, &m_Data.GetAssetRec().GetFixedRate());
	m_Data.Add(&m_Category, &m_Data.GetAssetRec().GetCategory());
	m_Data.Add(&m_Active, &m_Data.GetAssetRec().GetStatus());
	m_Data.Add(&m_Bucket, &m_Data.GetAssetRec().GetBucket());
	m_Data.Add(&m_Bucket2, &m_Data.GetAssetRec().GetBucket2());
	m_Data.Add(&m_Bucket3, &m_Data.GetAssetRec().GetBucket3());
	m_Data.Add(&m_Issuer, &m_Data.GetAssetRec().GetIssuer());
	m_Data.Add(&m_Method, &m_Data.GetAssetRec().GetMethod());
	m_Data.Add(&m_SwapFactor, &m_Data.GetAssetRec().GetSwapFactor());
	m_Data.Add(&m_Bond, &m_Data.GetAssetRec().GetBond());
	m_Data.Add(&m_Accrue, &m_Data.GetAssetRec().GetAccrue());
	m_Data.Add(&m_PrePaid, &m_Data.GetAssetRec().GetPrePaid());
	m_Data.Add(&m_UnListed, &m_Data.GetAssetRec().GetUnListed());
	m_Data.Add(&m_OTCClear, &m_Data.GetAssetRec().GetOTCClear());
	m_Data.Add(&m_Forward, &m_Data.GetAssetRec().GetForward());
	m_Data.Add(&m_1256, &m_Data.GetAssetRec().Get1256());
	m_Data.Add(&m_Par, &m_Data.GetAssetRec().GetPar());
	m_Data.Add(&m_CDSFeeType, &m_Data.GetAssetRec().GetCDSFeeType());
	m_Data.Add(&m_Underline, &m_Data.GetAssetRec().GetUnderlineAsset());
	m_Data.Add(&m_BelowRating, &m_Data.GetAssetRec().GetBelowRating());
	m_Data.Add(&m_144A, &m_Data.GetAssetRec().Get144A());
	m_Data.Add(&m_Level, &m_Data.GetAssetRec().GetLevel());
	m_Data.Add(&m_AdminID, &m_Data.GetAssetRec().GetAdminID());
	m_Data.Add(&m_Margin, &m_Data.GetAssetRec().GetMargin());
	m_Data.Add(&m_FRA, &m_Data.GetAssetRec().GetFRA());
	m_Data.Add(&m_Liquidity, &m_Data.GetAssetRec().GetLiquidity());
	m_Data.Add(&m_PFClass, &m_Data.GetAssetRec().GetPFClass());
	m_Data.Add(&m_PFSubClass, &m_Data.GetAssetRec().GetPFSubClass());
	m_Data.Add(&m_BDate, &m_Data.GetAssetRec().GetBDate());
	m_Data.Add(&m_CDSRestructuring, &m_Data.GetAssetRec().GetCDSRestructuring());
	m_Data.Add(&m_BDC, &m_Data.GetAssetRec().GetBDC());
	m_Data.Add(&m_CitiClass, &m_Data.GetAssetRec().GetCitiClass());
	m_Data.Add(&m_BlmKey, &m_Data.GetAssetRec().GetBlmKey());
	m_Data.Add(&m_YieldCurve, &m_Data.GetAssetRec().GetYieldCurve());
	m_Data.Add(&m_IRSTemplate, &m_Data.GetAssetRec().GetIRSTemplate());
	m_Data.Add(&m_Exchange, &m_Data.GetAssetRec().GetExchange());
	m_Data.Add(&m_Account, &m_Data.GetAssetRec().GetAccount());
	m_Data.Add(&m_FutureType, &m_Data.GetAssetRec().GetFutureType());
	m_Data.Add(&m_UnderlineID);
	m_Data.Add(&m_Amount);
	m_Data.Add(&m_Data.GetInvType());

	m_FromDate.Setup(this, IDC_MAINT_FROM_EDIT);
	m_ToDate.Setup(this, IDC_MAINT_TO_EDIT, TRUE, "To date must be greater or equal to from date.", &m_FromDate);
	m_SetDate.Setup(this, IDC_MAINT_SET_DATE_EDIT, TRUE, "Set date must be greater or equal to from date.", &m_FromDate);
	m_StartDate.Setup(this, IDC_MAINT_STARTDATE_EDIT);
	m_EndDate.Setup(this, IDC_MAINT_ENDDATE_EDIT, TRUE, "End date must be greater or equal to from date.", &m_FromDate);
	m_Rate.Setup(this, IDC_MAINT_RATE_EDIT);
	m_Amort.Setup(this, IDC_MAINT_AMORT_EDIT);
	m_Cap.Setup(this, IDC_MAINT_CAP_EDIT);
	m_Plus.Setup(this, IDC_MAINT_PLUS_AMT_EDIT);

	m_Data.GetSRowCtrl().Add(&m_FromDate, &m_Data.GetIntRec().GetFrom());
	m_Data.GetSRowCtrl().Add(&m_ToDate, &m_Data.GetIntRec().GetTo());
	m_Data.GetSRowCtrl().Add(&m_SetDate, &m_Data.GetIntRec().GetSetDate());
	m_Data.GetSRowCtrl().Add(&m_StartDate, &m_Data.GetIntRec().GetStartDate());
	m_Data.GetSRowCtrl().Add(&m_EndDate, &m_Data.GetIntRec().GetEndDate());
	m_Data.GetSRowCtrl().Add(&m_Rate, &m_Data.GetIntRec().GetRate());
	m_Data.GetSRowCtrl().Add(&m_Amort, &m_Data.GetIntRec().GetAmort());
	m_Data.GetSRowCtrl().Add(&m_Cap, &m_Data.GetIntRec().GetCap());
	m_Data.GetSRowCtrl().Add(&m_Plus, &m_Data.GetIntRec().GetPlus());
	m_Data.GetSRowCtrl().Add(&m_Action, &m_Data.GetIntRec().GetAction());
}

BOOL CMaintenance::Is1256(const CString Currency)
{
	COraLoader OraLoader;
	CQData QData;
	CString Data;

	if(m_Class.GetData() != "CURRENCY FWDS")
		return FALSE;

	OraLoader = GetData().GetOraLoader();
	Data = QData.GetQueryText(m_Currency.GetData());
	
	return OraLoader.GetCount("SELECT COUNT(*) FROM SEMAM.NW_CURRENCY WHERE CURRENCY = " + Data) > 0 ? TRUE : FALSE;
}

BOOL CMaintenance::IsAssetOK(BOOL bAdd)
{
	CQData QData;
	CString Asset, Class, Data, Text;
	COraLoader OraLoader;

	if(!IsUnique(m_Asset, "ASS_CODE", "ASSET CODE", bAdd))
		return FALSE;

	if(!IsUnique(m_AssetDesc, "ASS_DESC", "ASSET DESCRIPTION", bAdd))
		return FALSE;

	Class = m_Class.GetData();
	Asset = m_AssetDesc.GetData();
	if(m_AdminID.GetWindowTextLength() > 0)
	{
		if(!IsUnique(m_AdminID, "ASS_ADMIN_ID", "ADMINISTRATOR ID", bAdd))
			return FALSE;
	}

	OraLoader = GetData().GetOraLoader();
	Data = m_RateType.GetData();
	if(Class.Find("CDS") >= 0 || Class.Find("EM SWAPS") >= 0 || Class.Find("DM SWAPS") >= 0 || Class.Find("SWAPS") >= 0 || Class.Find("G7 VAR SWAPS") >= 0)
	{
		if(Data == "FIXED" && m_FixedRate.GetWindowTextLength() <= 0)
			Text = "No Rate entered";

		if(Class.Find("CDS") >= 0 || ((Class.Find("EM SWAPS") >= 0 || Class.Find("DM SWAPS") >= 0 || Class.Find("SWAPS") >= 0) && Data == "FIXED"))
		{
			if(m_Underline.GetWindowTextLength() <= 0)
				Text = "Underline Asset must be entered for CDS or IRS";
			else
			{
				OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASSETS "
										"WHERE ASS_CODE = %s ", QData.GetQueryText(m_Underline.GetData()));
				OraLoader.Open();
				if(OraLoader.GetCount() <= 0)
					Text = "Invalid Underline Asset";
			}
		}
	}

	if(Class.Find("EM SWAPS") >= 0 || Class.Find("DM SWAPS") >= 0)
	{
		if(Data.Find("FLOAT") >= 0)
		{
			Data = m_Source.GetData();
			if(m_Common.GetWindowTextLength() == 0 && m_Cusip.GetWindowTextLength() == 0 &&
				m_Isin.GetWindowTextLength() == 0 && m_Sedol.GetWindowTextLength() == 0 &&
				m_RedCode.GetWindowTextLength() == 0 && Data.Find(" BB") < 0 && Data.Find(" R") && 
				m_Index.GetWindowTextLength() == 0)
				Text = "IRS Float leg must have at least one reference ID such as index, cusip or other id";
			if(m_YieldCurve.GetCurSel() < 0)
				Text = "IRS Float leg must have a yield curve";
		}
	}
	
	// Because of the currency can be both deliverable or nondeliverable, disable this verication
/*	if(Class.Find("CURRENCY FWDS") >= 0)
	{
		m_Currency->GetSelString(Data);
		if(!Data.IsEmpty())
			OraLoader.GetSql() = "SELECT NDF_FIX FROM SEMAM.NW_CURRENCY "
								 "WHERE CURRENCY = '" + Data + "'";
		m_Currency2->GetSelString(Data);
		if(!Data.IsEmpty())
		{
			if(!OraLoader.GetSql().IsEmpty())
				OraLoader.GetSql() += "UNION ";
			OraLoader.GetSql() = "SELECT NDF_FIX FROM SEMAM.NW_CURRENCY "
								 "WHERE CURRENCY = '" + Data + "'";
		}
		
		OraLoader.Open();
		while(!OraLoader.IsEOF())
		{
			if(OraLoader.GetDBString(0) == Y && m_FxDate.GetData().IsEmpty())
				Text = "FX Fix Date must be entered";
			OraLoader.MoveNext();
		}
	} */

	if(m_Source.GetWindowTextLength() <= 0)
		Text = "Price Source must be entered";

	if(m_Country.GetCurSel() < 0)
		Text = "No Country Selected";

	if(m_Currency.GetCurSel() < 0)
		Text = "No Currency Selected";

	if(m_Class.GetCurSel() < 0)
		Text = "No Asset Class Selected";

	if(m_Category.GetCurSel() < 0)
		Text = "No Category Selected";
	
	if(m_Bucket.GetWindowTextLength() <= 0)	
		Text = "No Bucket Selected";

	if(m_Issuer.GetCurSel() < 0)
		Text = "No Issuer Selected";

	if(m_1256.GetCheck() == 0)
	{
		if(Class == "CURRENCY FWDS")
			if(Is1256(m_Currency.GetData()) || Is1256(m_Currency2.GetData()))
				Text = "1256 was not checked";
	}

	if(m_144A.GetCheck() == 1)
	{
		if(m_Cusip.GetWindowTextLength() <= 0)
			Text = "No cusip for 144A asset";
	}

	if(m_Level.GetCurSel() < 0)
		Text = "No Asset Level selected";

	if(m_Index.GetWindowTextLength() > 0)
	{
		OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_REF_IDX "
								"WHERE REF_IDX = %s ", QData.GetQueryText(m_Index.GetData()));
		OraLoader.Open();
		if(OraLoader.GetCount() <= 0)
			Text = "Invalid Reference Index";
	}

/*	if(m_ImagineCode.GetWindowTextLength() > 0)
	{
		Data = m_ImagineCode.GetData();
		Class = QData.GetQueryText(m_Category.GetData());

		OraLoader.Open("SELECT COUNT(*) FROM SEMAM.NW_ASSETS "
						"WHERE ASS_CATEGORY = " + Class + 
						" AND IMAGINE_CODE = " + QData.GetQueryText(Data) + 
						" AND ASS_INDUSTRY NOT IN ('EM CDS SWAPS', 'G7 CDS SWAPS') ");
		if(OraLoader.GetCount() > 1)
			Text = "Invalid Imagine Code";
	} */

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}

BOOL CMaintenance::IsInterestOK()
{
	CString Text, Data;
	
	if(m_Data.GetIntRec().GetInvType() == "D")
	{
		if(m_FromDate.GetData().IsEmpty())
			Text = "Must have Ex Date";
		if(m_Rate.GetWindowTextLength() <= 0)
			Text = "Must have Dividend";
	}
	else
	{
		if(m_Asset.GetWindowTextLength() <= 0)
			Text = "Asset must be created first";

		if(m_FromDate.GetData().IsEmpty() || m_ToDate.GetData().IsEmpty() || m_SetDate.GetData().IsEmpty())
			Text = "From Date, To Date and Set Date must be filled";

		if(m_Rate.GetWindowTextLength() <= 0 && m_Amort.GetWindowTextLength() <= 0 && m_Cap.GetWindowTextLength() <= 0)
			Text = "Must have Rate or Amort factor or Capitalization rate";
	}
	
	if(Text.IsEmpty())
		return TRUE;
	
	MessageBox(Text);
	return FALSE;
}

BOOL CMaintenance::IsUnique(CWnd &EditCtrl, LPCTSTR FieldName, LPCTSTR DescFieldName, BOOL bAdd)
{
	COraLoader OraLoader;
	CQData QData;
	CString Value, Text;

	OraLoader = GetData().GetOraLoader();

	Value = m_Asset.GetData();
	
	if(Value.IsEmpty())
	{
		Text = DescFieldName;
		Text += " must be entered";
	}
	else
	{
		if(bAdd)
		{
			OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASSETS WHERE %s = %s ", FieldName, QData.GetQueryText(Value));
			OraLoader.Open();
			if(OraLoader.GetCount() > 0)
			{
				Text = DescFieldName;
				Text += " must be unique";
			}
		}
	}

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}	

void CMaintenance::SetRateControls()
{
	BOOL bEnable = TRUE;
	
	if(m_OldInvType == m_Data.GetInvType())
		return;
	
	m_OldInvType = m_Data.GetInvType();

	if(m_Data.GetInvType() == "D")
	{
		bEnable = FALSE;

		GetDlgItem(IDC_MAINT_FROM_STATIC)->SetWindowText("Ex Date");
		GetDlgItem(IDC_MAINT_TO_STATIC)->SetWindowText(EMPTYSTRING);
		GetDlgItem(IDC_MAINT_SET_STATIC)->SetWindowText(EMPTYSTRING);
		GetDlgItem(IDC_MAINT_RATE_STATIC)->SetWindowText("Dividend");
		GetDlgItem(IDC_MAINT_AMORT_STATIC)->SetWindowText(EMPTYSTRING);
		GetDlgItem(IDC_MAINT_CAP_STATIC)->SetWindowText(EMPTYSTRING);
		GetDlgItem(IDC_MAIN_PLUS_AMT_STATIC)->SetWindowText(EMPTYSTRING);
		GetDlgItem(IDC_MAINT_ACTION_STATIC)->SetWindowText("Currency");
		GetData().GetCurrencyArr().CopyToComboBox(m_Action);
	}
	else
	{
		GetDlgItem(IDC_MAINT_FROM_STATIC)->SetWindowText("From Date");
		GetDlgItem(IDC_MAINT_TO_STATIC)->SetWindowText("To Date");
		GetDlgItem(IDC_MAINT_SET_STATIC)->SetWindowText("Set Date");
		GetDlgItem(IDC_MAINT_RATE_STATIC)->SetWindowText("Rate Date");
		GetDlgItem(IDC_MAINT_AMORT_STATIC)->SetWindowText("Amort Fact");
		GetDlgItem(IDC_MAINT_CAP_STATIC)->SetWindowText("Cap Rate");
		GetDlgItem(IDC_MAIN_PLUS_AMT_STATIC)->SetWindowText("Plus Amount");
		GetDlgItem(IDC_MAINT_ACTION_STATIC)->SetWindowText("Action");
		GetData().GetActionArr().CopyToComboBox(m_Action);
	}

	m_ToDate.EnableWindow(bEnable);
	m_SetDate.EnableWindow(bEnable);
	m_Amort.EnableWindow(bEnable);
	m_Cap.EnableWindow(bEnable);
	m_Plus.EnableWindow(bEnable);
}

BOOL CMaintenance::UpdateData(BOOL bSaveandValid)
{
	m_SS.SetFocus();
	if(bSaveandValid)
	{
		if(strlen(m_Data.GetKey()) == 0)
			m_Data.AddRow();
		else
		{
			m_Data.UpdateRow();	
			EnableInvCtrls();
			return TRUE;
		}
	}
	else
	{	
		BeginWaitCursor();

		m_Data.LoadDBData(0);

		if(!m_bLoaded)
		{
			GetData().LoadSupportData();
			GetData().LoadAssetData();

			GetData().GetIndustryArr().CopyToComboBox(m_Industry);
			GetData().GetCountryArr().CopyToComboBox(m_Country);
			GetData().GetProfileCountryArr().CopyToComboBox(m_ProfileCountry);
			GetData().GetCurrencyArr().CopyToComboBox(m_Currency);
			GetData().GetCurrencyArr().CopyToComboBox(m_Currency2);
			GetData().GetAssetTypeArr().CopyToComboBox(m_Class);
			GetData().GetAccountArr().CopyToComboBox(m_Account);

			GetData().GetOraLoader().Open("SELECT PROFILECLASS, PCLASS_DESC FROM SEMAM.NWB_PROFILECLASS ORDER BY 1");
			GetData().GetOraLoader().LoadMCComboBox(m_Class2);
			GetData().GetAssetProfileTypeArr().CopyToComboBox(m_ProfileClass);
			GetData().GetRateBasisArr().CopyToComboBox(m_RateBasis);
			GetData().GetCategoryArr().CopyToComboBox(m_Category);

			GetData().GetIssuerArr().CopyToComboBox(m_Issuer);
			GetData().GetActionArr().CopyToComboBox(m_Action);
			GetData().GetRateTypeArr().CopyToComboBox(m_RateType);
			GetData().GetMethodArr().CopyToComboBox(m_Method);
			GetData().GetFutureTypeArr().CopyToComboBox(m_FutureType);
			
			GetData().GetIndexArr().CopyToMCComboBox(m_Index);
			GetData().GetPmntFrqArr().CopyToMCComboBox(m_PmntFreq);
			GetData().GetPmntFrqArr().CopyToMCComboBox(m_CmpdFreq);
			GetData().GetLevelArr().CopyToMCComboBox(m_Level);
			GetData().GetBucketArr().CopyToMCComboBox(m_Bucket);
			GetData().GetOraLoader().Open("SELECT BUCKET, BUCKET_DESC FROM SEMAM.NW_BUCKETS2 ORDER BY 1 ");
			GetData().GetOraLoader().LoadMCComboBox(m_Bucket2);
			GetData().GetOraLoader().Open("SELECT BUCKET, BUCKET_DESC FROM SEMAM.NW_BUCKETS3 ORDER BY 1 ");
			GetData().GetOraLoader().LoadMCComboBox(m_Bucket3);
			GetData().GetCDSFeeTypeArr().CopyToMCComboBox(m_CDSFeeType);
			GetData().GetYieldCurveArr().CopyToMCComboBox(m_YieldCurve);
			GetData().GetOraLoader().Open("SELECT BDATE_ID, BDATE FROM SEMAM.NW_CDS_BDATE ORDER BY 1 ");
			GetData().GetOraLoader().LoadMCComboBox(m_BDate);
			GetData().GetOraLoader().Open("SELECT RESTRUCT_ID, RESTRUCT_CODE, RESTRUCT FROM SEMAM.NW_CDS_RESTRUCT ORDER BY 1 ");
			GetData().GetOraLoader().LoadMCComboBox(m_CDSRestructuring);
			GetData().GetOraLoader().Open("SELECT BDAY_ID, BDAY FROM SEMAM.NW_CDS_BDAY ORDER BY 1 ");
			GetData().GetOraLoader().LoadMCComboBox(m_BDC);
			GetData().GetOraLoader().Open("SELECT TEMPLATE, CURRENCY_PAIR, FIXED_BASIS, FIXED_CONVENTION, B.FREQ_DESC, FLOAT_INDEX, "
											"FLOAT_BASIS, FLOAT_CONVENTION, C.FREQ_DESC, FLOAT_TERM "
											"FROM SEMAM.NW_IRS_TEMPLATE A, SEMAM.NW_PAYMENT_FREQ B, "
											"SEMAM.NW_PAYMENT_FREQ C WHERE B.PAYMENT_FREQ = A.FIXED_FREQ "
											"AND C.PAYMENT_FREQ = A.FLOAT_FREQ ORDER BY 1 ");
			GetData().GetOraLoader().LoadMCComboBox(m_IRSTemplate);
			GetData().GetOraLoader().Open("SELECT EXCH_CODE, EXCHANGE, IMG_EXCH_CODE, MIC_CODE FROM SEMAM.NW_ASSET_EXCHANGE ORDER BY 1 ");
			GetData().GetOraLoader().LoadMCComboBox(m_Exchange);

			GetData().GetPFClassArr().CopyToComboBox(m_PFClass);
			GetData().GetPFSubClassArr().CopyToComboBox(m_PFSubClass);
			GetData().GetOraLoader().Open("SELECT DAYS FROM SEMAM.NW_LIQUIDITY ORDER BY 1 ");
			GetData().GetOraLoader().LoadComboBox(m_Liquidity);
			GetData().GetOraLoader().Open("SELECT CITI_CLASS FROM SEMAM.NW_CITI_CLASS ORDER BY 1 ");
			GetData().GetOraLoader().LoadComboBox(m_CitiClass);
			GetData().GetBlmKeyArr().CopyToComboBox(m_BlmKey);
			
			m_bLoaded = TRUE;
		}
		
		if(m_SS.GetSheetRows() == 1)
			OnDblClickMaintAssetList(1, 1);
		EndWaitCursor();

		return TRUE;
	}

	if(m_SS.GetSheetRows() > 0)
		m_SS.SetSheetActiveCell(1, m_SS.GetSheetRows());

	m_SS.SetSheetCurRow(-1);
	EnableInvCtrls();	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CMaintenance message handlers
void CMaintenance::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();

	InitControls();
	m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_IntSS, 0); // This is a must

	GetData().LoadDates();

	GetDocument()->SetViewTitle("MAINTENANCE");
	EnableInvCtrls();
	if(!GetData().GetTicketNumber().IsEmpty())
	{
		m_Data.LoadDBData(GetData().GetTicketNumber());
		GetData().SetTicketNumber(EMPTYSTRING);
		if(m_SS.GetSheetRows() == 1)
			OnDblClickMaintAssetList(1, 1);
	}

	m_Data.SetHeaders();

	CString Text;
	Text.Empty();
	EnableCtrls(Text);

	m_Data.Refresh();
	UpdateData(FALSE);
	EndWaitCursor();
}

BEGIN_EVENTSINK_MAP(CMaintenance, CFormView)
    //{{AFX_EVENTSINK_MAP(CMaintenance)
	ON_EVENT(CMaintenance, IDC_MAINT_ASSET_LIST, 2 /* BlockSelected */, OnBlockSelectedMaintAssetList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMaintenance, IDC_MAINT_ASSET_LIST, 11 /* DblClick */, OnDblClickMaintAssetList, VTS_I4 VTS_I4)
	ON_EVENT(CMaintenance, IDC_MAINT_INTEREST_LIST, 2 /* BlockSelected */, OnBlockSelectedMaintInterestList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMaintenance, IDC_MAINT_INTEREST_LIST, 11 /* DblClick */, OnDblClickMaintInterestList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMaintenance::OnChangeMaintAsset()
{
	EnableInvCtrls();
}

void CMaintenance::OnBlockSelectedMaintAssetList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickMaintAssetList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CMaintenance::OnDblClickMaintAssetList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		CString Asset;

		m_Data.SetCurrentRow(Row);
		Asset = m_Asset.GetData();
		m_Data.LoadHoldings(Asset, m_HoldingSS);
		SetRateControls();
		if(m_IntSS.GetSheetRows() > 0)
			OnDblClickMaintInterestList(1, 1);
	}

	OnSelchangeMaintAssetTypeCombo();
	EnableInvCtrls();
}

void CMaintenance::OnMaintClearScreen() 
{
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	EnableInvCtrls();
	m_AssetID.EnableWindow(TRUE);
}

void CMaintenance::OnUpdateMaintAdd(CCmdUI* pCmdUI) 
{
	if(GetData().IsPowerUser() || (GetData().IsOperation() && !GetData().IsGuest()))
		pCmdUI->Enable(m_Asset.GetWindowTextLength() > 0 && m_AssetDesc.GetWindowTextLength() > 0 ? TRUE : FALSE);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnUpdateMaintDelete(CCmdUI* pCmdUI) 
{
	if(GetData().IsPowerUser() || (GetData().IsOperation() && !GetData().IsGuest()))
		pCmdUI->Enable(m_SS.GetIsBlockSelected());
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnUpdateMaintUpdate(CCmdUI* pCmdUI) 
{
	if(GetData().IsPowerUser() || (GetData().IsOperation() && !GetData().IsGuest()))
		pCmdUI->Enable(m_SS.GetSheetCurRow() > 0);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnMaintAdd() 
{
	if(IsAssetOK())
	{
		CString Class, Text;

		Class = m_Class.GetData();
		Text = m_AssetDesc.GetData();

		m_Data.SetKey(EMPTYSTRING);
		UpdateData(TRUE);
	}
}

void CMaintenance::OnMaintDelete() 
{
	if(MessageBox("Are you sure you want to delete this asset ?", "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	
	BeginWaitCursor();
	m_Data.DeleteRow();
	EndWaitCursor();
}

void CMaintenance::OnMaintFind() 
{
	UpdateData(FALSE);
	m_AssetID.EnableWindow(FALSE);
}

void CMaintenance::OnMaintUpdate() 
{
	if(m_SS.GetSheetCurRow() <= 0)
		return;
		
	if(IsAssetOK(FALSE))
	{
		m_Data.SetKey();
		UpdateData(TRUE);
	}
}

void CMaintenance::OnMaintBondCheck() 
{
	int Bond;
	
	Bond = m_Bond.GetCheck();
	if(!Bond)
	{
		m_RateBasis.SetCurSel(-1);
		m_RateType.SetCurSel(-1);
	}
	
	m_Accrue.SetCheck(Bond);
	m_PrePaid.SetCheck(Bond);
}

void CMaintenance::OnBlockSelectedMaintInterestList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickMaintInterestList(m_IntSS.GetActiveCol(), m_IntSS.GetActiveRow());
}

void CMaintenance::OnDblClickMaintInterestList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_IntSS.GetSheetRows())
		m_Data.GetSRowCtrl().SetCurrentRow(Row);
	else
	{
		m_Data.GetSRowCtrl().SetCurrentRow(-1);
		m_Data.GetSRowCtrl().Refresh();
	}
}

void CMaintenance::OnMaintAddRate() 
{
	CString Text;

	Text = m_Class.GetData();
	if(Text == "ADR EQUITIES" || Text == "DOM EQUITIES" || Text == "DOM EQTY-NEW" || Text == "EXCH TRADE FUND")
		m_Data.GetIntRec().SetInvType("D");
	else
		m_Data.GetIntRec().SetInvType("R");

	if(IsInterestOK())
		m_Data.AddSRow();
}

void CMaintenance::OnMaintDeleteRate() 
{
	if(MessageBox("Are you sure you want to delete these interest ?", "Delete", MB_OKCANCEL) == IDOK)
		m_Data.DeleteSRow();
}

void CMaintenance::OnMaintUpdateRate() 
{
	if(IsInterestOK())
		m_Data.UpdateSRow();
}

void CMaintenance::OnUpdateMaintAddRate(CCmdUI* pCmdUI) 
{
	if(GetData().IsPowerUser() || (GetData().IsOperation() && !GetData().IsGuest()))
		pCmdUI->Enable(m_Rate.GetWindowTextLength() > 0);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnUpdateMaintDeleteRate(CCmdUI* pCmdUI) 
{
	if(GetData().IsPowerUser() || (GetData().IsOperation() && !GetData().IsGuest()))
		pCmdUI->Enable(m_IntSS.GetSheetCurRow() > 0 && m_IntSS.GetIsBlockSelected() ? TRUE : FALSE);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnUpdateMaintUpdateRate(CCmdUI* pCmdUI) 
{
	if(GetData().IsPowerUser() || (GetData().IsOperation() && !GetData().IsGuest()))
		pCmdUI->Enable(m_IntSS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnMaintFindItem()
{
	CString Sql, Text, Class;
	UINT CurrID;

	CurrID = m_CurrID;
	m_CurrID = 0;

	switch(CurrID)
	{
		case IDC_MAINT_UNDERLINE_EDIT: 
			Class = m_Class.GetData();
				
			if(Class.Find("CDS") >= 0)
			{
				CString Country;

				Country = m_Country.GetData();
				if(!Country.IsEmpty())
				{
					Sql = "SELECT ASS_CODE, ASS_DESC, "
							"NVL(ASS_MSTC_CODE, NVL(ASS_ISIN_CODE, NVL(ASS_SEDOL_NUM, ASS_COMMON_CODE))) "
							"\"ID\" FROM SEMAM.NW_ASSETS "
							"WHERE ASS_COUNTRY = '" + Country + "' ORDER BY 1";
				}
			}
			else // IRS or IRS Swaption
			{
				Sql = "SELECT ASS_CODE, ASS_DESC, NVL(ASS_MSTC_CODE, NVL(ASS_ISIN_CODE, NVL(ASS_SEDOL_NUM, ASS_COMMON_CODE))) \"ID\", ASS_CURRENCY "
						"FROM SEMAM.NW_ASSETS "
						"WHERE ASS_INT_TYPE = 'FLOAT' "
						"AND ASS_INDUSTRY IN ('EM SWAPS', 'DM SWAPS', 'SWAPS') "
						"AND (ASS_MATURITY_DATE IS NULL ";
					
				CString Maturity;
				
				Maturity = m_Maturity.GetData();

				if(!Maturity.IsEmpty())
				{
					Text.Format("OR ASS_MATURITY_DATE >= '" + Maturity + "' ");
					Sql += Text;
				}
					
				Sql += ") ORDER BY 1 ";
			}

			if(!Sql.IsEmpty())
				FindFieldItem(m_Underline, Sql);
			break;
		default:
			break;
	}
}

void CMaintenance::OnUpdateMaintFindItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_CurrID > 0);
}

void CMaintenance::OnMaintOther()
{
	COtherMaint OtherDlg;
	CString TableName;

	OtherDlg.m_OraLoader = GetData().GetOraLoader();
	if(OtherDlg.DoModal() != IDOK)
		return;

	TableName = OtherDlg.m_Table;

	switch(atoi(OtherDlg.m_Fields))
	{
		case 1:
			{
				COneField One;
				One.m_Data.Setup(GetData().GetOraLoader(), NULL, OtherDlg.m_TableSpace, TableName, OtherDlg.m_PreLoad, OtherDlg.m_Key);
				One.DoModal();
				if(One.m_bModified && OtherDlg.m_PreLoad == 'Y')
				{
					if(TableName == "NW_TRANS_TYPES")
						GetData().GetTransTypeArr().Modified();
					else
						if(TableName == "NW_RATE_BASES")
							GetData().GetRateBasisArr().Modified();
						else
							if(TableName == "NW_PMNT_TYPES")
								GetData().GetCashTypeArr().Modified();
							else
								if(TableName == "NW_METHODS")
									GetData().GetMethodArr().Modified();
				}
					
				return;
			}
			break;
		case 2:
			if(TableName == "NW_PAY_RESTRICTION")
			{
				CPaymentRestriction Dlg;
				Dlg.m_pDocData = &GetData();
				Dlg.DoModal();
			}
			else
			{
				CTwoField Two;

				Two.m_Data.Setup(GetData().GetOraLoader(), NULL, OtherDlg.m_TableSpace, TableName, OtherDlg.m_PreLoad, OtherDlg.m_Key, OtherDlg.m_Key2);
				Two.DoModal();
				if(Two.m_bModified && OtherDlg.m_PreLoad == 'Y')
				{
					if(TableName == "NW_PFU")
						GetData().GetPFUArr().Modified();
				}
			}
			break;
		case 3:
			if(TableName == "NW_AGENT")
			{
				CAgentDlg Dlg;
				Dlg.m_OraLoader = GetData().GetOraLoader();
				Dlg.DoModal();
			}
			else
				if(TableName == "NW_OPT_TICK_MAPS")
				{
					COptMap Dlg;
					Dlg.DoModal();
				}
				else
				{
					CThreeField Three;
					Three.m_Data.Setup(GetData().GetOraLoader(), NULL, OtherDlg.m_TableSpace, TableName, OtherDlg.m_PreLoad, OtherDlg.m_Key, OtherDlg.m_Key2);
					Three.DoModal();

					if(Three.m_bModified && OtherDlg.m_PreLoad == 'Y')
					{
						if(TableName == "NW_INDUSTRIES")
							GetData().GetAssetTypeArr().Modified();
						else
							if(TableName == "NW_BUCKETS")
								GetData().GetBucketArr().Modified();
					}
				}
			break;
		case 4:
			if(TableName == "NW_BANKS")
			{
				CBankDlg BankDlg;
				BankDlg.m_Data.Setup(GetData().GetOraLoader(), NULL, OtherDlg.m_TableSpace, TableName, OtherDlg.m_PreLoad, OtherDlg.m_Key, OtherDlg.m_Key2);
				BankDlg.DoModal();					
			}
			else
				if(TableName == "NW_CLEARING_FEE")
				{
					CClearingFeeDlg Dlg;
					Dlg.m_Data.SetOraLoader(GetData().GetOraLoader());
					Dlg.DoModal();
				}
				else
					if(TableName == "NW_CP_FUND_MARGIN")
					{
						CPrimeCPMargin Dlg;
						Dlg.m_OraLoader = GetData().GetOraLoader();
						Dlg.DoModal();
					}
					else
					{
						CFourField Four;
						Four.m_Data.Setup(GetData().GetOraLoader(), NULL, OtherDlg.m_TableSpace, TableName, OtherDlg.m_PreLoad, OtherDlg.m_Key, OtherDlg.m_Key2);
						Four.DoModal();
					
						if(Four.m_bModified && OtherDlg.m_PreLoad == 'Y')
						{
							if(TableName == "NW_INV_STRATEGIES")
								GetData().GetCategoryArr().Modified();
						}
					}
			break;
		default:
			if(TableName == "NW_CASH_ACCOUNTS")
			{
				CAccountDlg Dlg;

				Dlg.m_pData = &GetData();
				Dlg.DoModal();
			}
			else
				if(TableName == "NW_COUNTER_PARTY")
				{
					CCPDialog Dlg;
					
					Dlg.m_pData = &GetData();
					Dlg.DoModal();
				}
				else
					if(TableName == "NW_INVESTORS")
					{
						CInvestorDialog Dlg;
						
						Dlg.m_pData = &GetData();
						Dlg.DoModal();
					}
					else
						if(TableName == "NW_PORTFOLIOS")
						{
							CPortfolio Dlg;
							
							Dlg.m_pData = &GetData();
							Dlg.DoModal();
						}
						else
							if(TableName == "NW_PORTFOLIO_INFO")
							{
								CPortfolioInfo Dlg;
								
								Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
								Dlg.DoModal();
							}
							else
								if(TableName == "NW_PORTFOLIO_ACCOUNTS")
								{
									CPortfolioAccount Dlg;
									
									Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
									Dlg.DoModal();
								}
								else
									if(TableName == "NW_INDUSTRIES")
									{
										CAssetClassDlg Dlg;
								
										Dlg.m_bPowerUser = GetData().IsPowerUser();
										Dlg.m_OraLoader.Copy(GetData().GetOraLoader());
										Dlg.DoModal();
									}
									else
										if(TableName == "NW_ASS_SUPPLEMENTS")
										{
											CAssetSupplement Dlg;
											
											Dlg.m_Data.GetRec().GetAsset() = m_Asset.GetData();
											Dlg.m_pData = &GetData();
											Dlg.DoModal();
										}
										else
											if(TableName == "NW_ISSUERS")
											{
												CIssuerDlg Dlg;
												
												Dlg.DoModal();
											}
											else
												if(TableName == "NW_COUNTRIES")
												{
													CCountry Dlg;
													
													Dlg.m_pData = &GetData();
													Dlg.DoModal();
												}
												else
													if(TableName == "NW_CLAIM_CP")
													{
														CClaimCP Dlg;
												
														Dlg.m_pData = &GetData();
														Dlg.DoModal();
													}
													else
														if(TableName == "NW_PAY_TEMPLATE")
														{
															CPaymentTemplateDlg Dlg;
															
															Dlg.m_pData = &GetData();
															Dlg.DoModal();
														}
														else
															if(TableName == "NW_AA_ALLOC")
															{
																CAutoAllocationDlg Dlg;
																
																Dlg.m_pData = &GetData();
																Dlg.DoModal();
															}
															else
																if(TableName == "NW_CURRENCY")
																{
																	CCurrencyDlg Dlg;
																
																	Dlg.m_pData = &GetData();
																	Dlg.DoModal();
																}
																else
																	if(TableName == "NW_REF_IDX")
																	{
																		CRefIndexDlg Dlg;
																
																		Dlg.DoModal();
																	}
																	else
																		if(TableName == "NW_FUTURE_COMM_SCHEDULE")
																		{
																			CCPFutureCommDlg Dlg;
																	
																			Dlg.m_pData = &GetData();
																			Dlg.DoModal();
																		}
																		else
																			if(TableName == "NW_FUTURE_FEES")
																			{
																				CFutureFeesDlg Dlg;

																				Dlg.m_pData = &GetData();
																				Dlg.DoModal();
																			}
																			else
																				if(TableName == "NW_OTC_FEE_SCHEDULE")
																				{
																					CCPOTCFeeDlg Dlg;
																					Dlg.m_pData = &GetData();
																					Dlg.DoModal();
																				}
																				else
																					if(TableName == "NW_GIVEUP_UNIT_COST")
																					{
																						CGiveupFeeDlg Dlg;
	
																						Dlg.m_pData = &GetData();
																						Dlg.DoModal();
																					}
																					else
																						if(TableName == "NW_TRADERS")
																						{
																							CTraderDlg Dlg;
																				
																							if(Dlg.DoModal() == IDOK)
																								GetData().GetTraderArr().Modified();
																						}
																						else
																							if(TableName == "NW_OPERATIONS")
																							{
																								COPsDlg Dlg;
																								
																								Dlg.DoModal();
																							}
																							else
																								if(TableName == "NW_OPT_SETTLEMENT")
																								{
																									COptSettlementDlg Dlg;

																									Dlg.m_OraLoader = GetData().GetOraLoader();
																									Dlg.DoModal();
																								}
																								else
																									if(TableName == "NW_YIELD_CURVE")
																									{
																										CYieldCurveDlg Dlg;

																										Dlg.m_OraLoader = GetData().GetOraLoader();
																										Dlg.DoModal();
																									}
			break;
	}
}

void CMaintenance::OnMaintSecFee() 
{
	CSECFee Dlg;

	Dlg.m_OraLoader = GetData().GetOraLoader();
	Dlg.DoModal();
}

void CMaintenance::OnMaintOrFee()
{
	COrFeeDlg Dlg;

	Dlg.m_OraLoader = GetData().GetOraLoader();
	Dlg.m_Data = &GetData();

	Dlg.DoModal();
}

void CMaintenance::OnSelchangeMaintAssetTypeCombo() 
{
	CString Text;
	
	Text = m_Class.GetData();
	EnableCtrls(Text);
}

void CMaintenance::OnSetfocusMaintUnderlineEdit() 
{
	m_CurrID = IDC_MAINT_UNDERLINE_EDIT;
}

void CMaintenance::OnUpdateMaintOther(CCmdUI* pCmdUI) 
{
	if(GetData().IsPowerUser() || (GetData().IsOperation() && !GetData().IsGuest()))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnUpdateMaintFillRate(CCmdUI* pCmdUI) 
{
	CString AssetClass;

	AssetClass = m_Class.GetData();
	
	pCmdUI->Enable(m_Asset.GetWindowTextLength() > 0 && m_Maturity.GetWindowTextLength() > 0 && 
					(m_RateType.GetData() == "FIXED" || AssetClass.Find("CDS SWAPS") >= 0) && 
					!m_PmntFreq.GetData().IsEmpty() && m_IntSS.GetSheetRows() > 0);
}

void CMaintenance::OnMaintFillRate() 
{
	CString Asset, AssetClass, RateType, Freq, Maturity, IntToDate, Rate;

	Asset = m_Asset.GetData();
	AssetClass = m_Class.GetData();
	RateType = m_RateType.GetData();
	Freq = m_PmntFreq.GetData();
	Maturity = m_Maturity.GetData();
	IntToDate = m_ToDate.GetData();
	Rate = m_Rate.GetData();
	
	m_Data.GenerateFixedRates(Asset, AssetClass, RateType, Freq, Maturity, IntToDate, Rate);
}

void CMaintenance::OnUpdateMaintSecFee(CCmdUI* pCmdUI)
{
	if(GetData().IsPowerUser() && GetData().IsOperation())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnUpdateMaintOrFee(CCmdUI *pCmdUI)
{
	if(GetData().IsPowerUser() && GetData().IsOperation())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMaintenance::OnCbnSelchangeMaintRateTypeCombo()
{
	CString Class;

	Class = m_Class.GetData();
	EnableCtrls(Class);
}

void CMaintenance::OnEnSetfocusMaintCalendarEdit()
{
	m_CurrID = IDC_MAINT_CALENDAR_EDIT;
}

void CMaintenance::OnEnSetfocusMaintRestructuringEdit()
{
	m_CurrID = IDC_MAINT_RESTRUCTURING_EDIT;
}

void CMaintenance::OnMaintImport()
{
	m_Data.ImportImagineAsset();
}

void CMaintenance::OnEnKillfocusMaintIssueDateEdit()
{
	if(m_IssueDate.GetData().GetLength() == 10)
	{
		CString Date, Sql;

		Date = m_IssueDate.GetData();
		Sql = "SELECT TO_DATE('" + Date + "', 'MM/DD/YYYY') - TRUNC(SYSDATE) FROM DUAL ";

		if(m_Data.GetOraLoader().GetCount(Sql) > 0)
			if(MessageBox("Issue date is beyond today. Are you sure you want to continue?", "Maintenance", MB_YESNO) != IDYES)
			{
				m_Data.GetOraLoader().Today(Date);
				m_IssueDate.SetData(Date);
				m_IssueDate.SetFocus();
			}
	}
}
