#include "stdafx.h"
#include "resource.h"
#include "assetrec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CAssetIntRec, CRecObj)
void CAssetIntRec::Copy(CAssetIntRec &AssetIntRec)
{
	SetID(AssetIntRec.GetID());
	SetAsset(AssetIntRec.GetAsset());
	SetFrom(AssetIntRec.GetFrom());
	SetTo(AssetIntRec.GetTo());
	SetSetDate(AssetIntRec.GetSetDate());
	SetEndDate(AssetIntRec.GetEndDate());
	SetRate(AssetIntRec.GetRate());
	SetAmort(AssetIntRec.GetAmort());
	SetCap(AssetIntRec.GetCap());
	SetPlus(AssetIntRec.GetPlus());
	SetAction(AssetIntRec.GetAction());
	SetInvType(AssetIntRec.GetInvType());
}

void CAssetIntRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	
	if(GetInvType() == "D")
	{
		Rec.Add(GetAsset());
		Rec.Add(GetFrom());
		Rec.Add(GetEndDate());
		Rec.Add(GetAction());
		Rec.Add(GetRate());
	}
	else
	{
		Rec.Add(GetAsset());
		Rec.Add(GetFrom());
		Rec.Add(GetTo());
		Rec.Add(GetSetDate());
		Rec.Add(GetEndDate());
		Rec.Add(GetRate());
		Rec.Add(GetAmort());
		Rec.Add(GetCap());
		Rec.Add(GetPlus());
		Rec.Add(GetAction());
	}
}

BOOL CAssetIntRec::AddRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty())
		return FALSE;

	if(!IsIntDateOK(OraLoader))
		return FALSE;

	CDBRec Rec;

	ToDBRec(Rec);

	if(GetInvType() == "D")
		OraLoader.GetSql() = "SELECT ASSET_CODE, EXD_DATE, PAY_DATE, CURRENCY, DIVIDEND FROM SEMAM.NW_DIVIDEND_SCHEDULE ";
	else
		OraLoader.GetSql() = "SELECT ASS_CODE, ASS_FROM, ASS_TO, INT_SET_DATE, END_DATE, RATE, AMORT_FACT, "
								"CAPIT_RATE, PLUS_AMOUNT, ACTION FROM SEMAM.NW_ASS_PERIODS ";
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAssetIntRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(!bByID || strlen(GetAsset()) == 0 || strlen(GetID()) == 0)
		return FALSE;

	CSqlStmt Sql;
	CQData QData;

	if(GetInvType() == "D")
		Sql = "SELECT ASSET_CODE, EXD_DATE, PAY_DATE, CURRENCY, DIVIDEND "
				"FROM SEMAM.NW_DIVIDEND_SCHEDULE WHERE ROWIDTOCHAR(ROWID) = %s ";
	else
		Sql = "SELECT ASS_CODE, ASS_FROM, ASS_TO, INT_SET_DATE, END_DATE, RATE, AMORT_FACT, "
				"CAPIT_RATE, PLUS_AMOUNT, ACTION FROM SEMAM.NW_ASS_PERIODS "
				"WHERE ROWIDTOCHAR(ROWID) = %s ";

	OraLoader.GetSql().Format((LPCTSTR) Sql, QData.GetQueryText(GetID()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CAssetIntRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;

	CQData QData;
	LPCTSTR P;

	if(bByID)
	{
		if(strlen(GetID()) == 0)
			return FALSE;
		
		P = QData.GetQueryText(GetID());
		if(GetInvType() == "D")
			OraLoader.GetSql().Format("DELETE SEMAM.NW_DIVIDEND_SCHEDULE WHERE ROWIDTOCHAR(ROWID) = %s", P);
		else
			OraLoader.GetSql().Format("DELETE SEMAM.NW_ASS_PERIODS WHERE ROWIDTOCHAR(ROWID) = %s", P);
	}
	else
	{
		P = QData.GetQueryText(GetAsset());
		if(GetInvType() == "D")
			OraLoader.GetSql().Format("DELETE SEMAM.NW_DIVIDEND_SCHEDULE WHERE ASSET_CODE = %s", P);
		else
			OraLoader.GetSql().Format("DELETE SEMAM.NW_ASS_PERIODS WHERE ASS_CODE = %s", P);
	}
	return OraLoader.ExecuteSql();
}

BOOL CAssetIntRec::IsIntDateOK(COraLoader &OraLoader)
{
	CString DateS;
	CQData QData;

	if(GetInvType() == "D")
		return TRUE;

	DateS = QData.GetQueryDate(GetFrom());
	OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASS_PERIODS "
							"WHERE ASS_CODE = %s "
							"AND ASS_FROM <= %s "
							"AND ASS_TO > %s ", QData.GetQueryText(GetAsset()), (LPCTSTR) DateS, (LPCTSTR) DateS);
	if(OraLoader.GetCount(OraLoader.GetSql()) > 0)
	{
		MessageBox(NULL, "Interest From Date is overlapped", NULL, MB_OK);
		return FALSE;
	}
	
	DateS = QData.GetQueryDate(GetTo());
	OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASS_PERIODS "
							"WHERE ASS_CODE = %s "
							"AND ASS_FROM <= %s "
							"AND ASS_TO > %s ", QData.GetQueryText(GetAsset()), (LPCTSTR) DateS, (LPCTSTR) DateS);
	
	if(OraLoader.GetCount(OraLoader.GetSql()) > 1)
	{
		MessageBox(NULL, "Interest To Date is overlapped", NULL, MB_OK);
		return FALSE;
	}
	
	return TRUE;
}




IMPLEMENT_DYNAMIC(CAssetRec, CRecObj)
void CAssetRec::Copy(CAssetRec &Rec)
{
	Setup(Rec.GetAsset(), Rec.GetDesc(), Rec.GetCurrency(), Rec.GetOptTicker());
	SetAssetID(Rec.GetAssetID());
	SetDesc2(Rec.GetDesc2());
	SetCountry(Rec.GetCountry());
	SetProfileCountry(Rec.GetProfileCountry());
	SetCurrency2(Rec.GetCurrency2());
	SetCommon(Rec.GetCommon());
	SetCusip(Rec.GetCusip());
	SetIsin(Rec.GetIsin());
	SetSedol(Rec.GetSedol());
	SetRedCode(Rec.GetRedCode());
	SetImagineCode(Rec.GetImagineCode());
	SetPID(Rec.GetPID());
	SetClass(Rec.GetClass());
	SetClass2(Rec.GetClass2());
	SetProfileClass(Rec.GetProfileClass());
	SetIndustry(Rec.GetIndustry());
	SetCategory(Rec.GetCategory());
	SetBucket(Rec.GetBucket());
	SetBucket2(Rec.GetBucket2());
	SetBucket3(Rec.GetBucket3());
	SetSource(Rec.GetSource());
	SetCoupon(Rec.GetCoupon());

	SetIssueDate(Rec.GetIssueDate());
	SetCouponStartDate(Rec.GetCouponStartDate());
	SetRollDate(Rec.GetRollDate());
	SetMaturity(Rec.GetMaturity());
	SetFxFixDate(Rec.GetFxFixDate());
	SetIndex(Rec.GetIndex());
	SetFormula(Rec.GetFormula());
	SetRateBasis(Rec.GetRateBasis());
	SetRateType(Rec.GetRateType());
	SetPmntFreq(Rec.GetPmntFreq());
	SetCmpdFreq(Rec.GetCmpdFreq());
	SetIssuer(Rec.GetIssuer());
	SetMethod(Rec.GetMethod());
	SetSwapFactor(Rec.GetSwapFactor());
	SetAdjF(Rec.GetAdjF());
	SetStatus(Rec.GetStatus());
	SetBond(Rec.GetBond());
	SetAccrue(Rec.GetAccrue());
	SetPrePaid(Rec.GetPrePaid());
	SetUnListed(Rec.GetUnListed());
	SetOTCClear(Rec.GetOTCClear());
	SetForward(Rec.GetForward());
	Set1256(Rec.Get1256());
	SetPar(Rec.GetPar());
	SetCDSFeeType(Rec.GetCDSFeeType());
	SetUnderlineAsset(Rec.GetUnderlineAsset());
	SetBelowRating(Rec.GetBelowRating());
	Set144A(Rec.Get144A());
	SetLevel(Rec.GetLevel());
	SetAdminID(Rec.GetAdminID());
	SetMargin(Rec.GetMargin());
	SetFRA(Rec.GetFRA());
	SetLiquidity(Rec.GetLiquidity());
	SetPFClass(Rec.GetPFClass());
	SetPFSubClass(Rec.GetPFSubClass());
	SetBDate(Rec.GetBDate());
	SetCDSRestructuring(Rec.GetCDSRestructuring());
	SetBDC(Rec.GetBDC());
	SetCitiClass(Rec.GetCitiClass());
	SetBlmKey(Rec.GetBlmKey());
	SetYieldCurve(Rec.GetYieldCurve());
	SetIRSTemplate(Rec.GetIRSTemplate());
	SetExchange(Rec.GetExchange());
	SetAccount(Rec.GetAccount());
	SetClearFee(Rec.GetClearFee());
}

void CAssetRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetAsset());
	Rec.Add(GetAssetID());
	Rec.Add(GetDesc());
	Rec.Add(GetCommon());
	Rec.Add(GetCusip());
	Rec.Add(GetIsin());
	Rec.Add(GetSedol());
	Rec.Add(GetRedCode());
	Rec.Add(GetImagineCode());
	Rec.Add(GetPID());
	Rec.Add(GetIssueDate());
	Rec.Add(GetCouponStartDate());
	Rec.Add(GetRollDate());
	Rec.Add(GetMaturity());
	Rec.Add(GetFxFixDate());
	Rec.Add(GetIndex());
	Rec.Add(GetFormula());
	Rec.Add(GetSource());
	Rec.Add(GetIndustry());
	Rec.Add(GetCountry());
	Rec.Add(GetProfileCountry());
	Rec.Add(GetCurrency());
	Rec.Add(GetCurrency2());
	Rec.Add(GetClass());
	Rec.Add(GetClass2());
	Rec.Add(GetProfileClass());
	Rec.Add(GetRateBasis());
	Rec.Add(GetRateType());
	Rec.Add(GetPmntFreq());
	Rec.Add(GetCmpdFreq());
	Rec.Add(GetCategory());
	if(GetStatus() == N)
		Rec.Add(GetStatus());
	else
		Rec.Add(EMPTYSTRING);
	Rec.Add(GetBucket());
	Rec.Add(GetBucket2());
	Rec.Add(GetBucket3());
	Rec.Add(GetIssuer());

	Rec.Add(GetMethod());
	Rec.Add(GetSwapFactor());
	Rec.Add(GetAdjF());
	Rec.Add(GetBond());
	Rec.Add(GetAccrue());
	Rec.Add(GetPrePaid());
	Rec.Add(GetUnListed());
	Rec.Add(GetOTCClear());
	Rec.Add(GetForward());
	Rec.Add(Get1256());
	Rec.Add(GetPar());
	Rec.Add(GetCDSFeeType());
	Rec.Add(GetUnderlineAsset());
	Rec.Add(GetBelowRating());
	Rec.Add(Get144A());
	Rec.Add(GetLevel());
	Rec.Add(GetAdminID());
	Rec.Add(GetMargin());
	Rec.Add(GetFRA());
	Rec.Add(GetLiquidity());
	Rec.Add(GetPFClass());
	Rec.Add(GetPFSubClass());
	Rec.Add(GetBDate());
	Rec.Add(GetCDSRestructuring());
	Rec.Add(GetBDC());
	Rec.Add(GetCitiClass());
	Rec.Add(GetBlmKey());
	Rec.Add(GetYieldCurve());
	Rec.Add(GetIRSTemplate());
	Rec.Add(GetExchange());
	Rec.Add(GetAccount());
	Rec.Add(GetClearFee());
}

BOOL CAssetRec::AddRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty())
		return FALSE;
	
	if(!OraLoader.Open("SELECT ASS_CODE, ASSET_NUM, ASS_DESC, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, "
						"ASS_SEDOL_NUM, RED_CODE, IMAGINE_CODE, ASS_PID, ASS_ISSUE_DATE, ASS_COUPON_START_DATE, "
						"ASS_BEGIN_ROLL_DATE, ASS_MATURITY_DATE, ASS_FX_DATE, ASS_FLOAT_INDEX, ASS_FLOAT_FORMULA, "
						"SOURCE, ASS_IND_AREA, ASS_COUNTRY, ASS_PROFILE_COUNTRY, ASS_CURRENCY, ASS_CURRENCY2, "
						"ASS_INDUSTRY, ASS_PCLASS2, ASS_PROFILE_CLASS, ASS_RATE_BASIS, ASS_INT_TYPE, "
						"ASS_PAYMENT_FREQUENCY, ASS_CMPD_FREQ, ASS_CATEGORY, STATUS, BUCKET, BUCKET2, BUCKET3, "
						"ASS_ISSUER, ASS_METHOD, ASS_SWAP_FACTOR, ASS_INT_RATE, ASS_TYPE, ASS_ACCRUABLE, "
						"ASS_PPAID_INT, UNLISTED_SECURITY, ASS_OTC_CLEAR, ASS_CONV_TYPE, ASS_1256, ASS_PAR_VALUE, "
						"ASS_DF_FEE_TYPE, UNDERLINE_ASSET, BELOW_RATING, ASS_144A, ID_144A, ASS_ADMIN_ID, ASS_MARGIN, "
						"ASS_FRA, ASS_LIQUIDITY, ASS_PF_CLASS, ASS_PF_SUBCLASS, ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, "
						"ASS_CITI_CLASS, BLM_KEY, ASS_YC_NAME, ASS_IRS_TMPLATE, ASS_EXCHANGE, ASS_ACCOUNT, ASS_CLEAR_FEE "
						"FROM SEMAM.NW_ASSETS ", 
					ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAssetRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;
	
	CSqlStmt Sql;
	CQData QData;

	Sql = ("SELECT ASS_CODE, ASSET_NUM, ASS_DESC, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, "
			"RED_CODE, IMAGINE_CODE, ASS_PID, ASS_ISSUE_DATE, ASS_COUPON_START_DATE, ASS_BEGIN_ROLL_DATE, "
			"ASS_MATURITY_DATE, ASS_FX_DATE, ASS_FLOAT_INDEX, ASS_FLOAT_FORMULA, SOURCE, ASS_IND_AREA, ASS_COUNTRY, "
			"ASS_PROFILE_COUNTRY, ASS_CURRENCY, ASS_CURRENCY2, ASS_INDUSTRY, ASS_PCLASS2, ASS_PROFILE_CLASS, "
			"ASS_RATE_BASIS, ASS_INT_TYPE, ASS_PAYMENT_FREQUENCY, ASS_CMPD_FREQ, ASS_CATEGORY, STATUS, BUCKET, "
			"BUCKET2, BUCKET3, ASS_ISSUER, ASS_METHOD, ASS_SWAP_FACTOR, ASS_INT_RATE, ASS_TYPE, ASS_ACCRUABLE, "
			"ASS_PPAID_INT, UNLISTED_SECURITY, ASS_OTC_CLEAR, ASS_CONV_TYPE, ASS_1256, ASS_PAR_VALUE, ASS_DF_FEE_TYPE, "
			"UNDERLINE_ASSET, BELOW_RATING, ASS_144A, ID_144A, ASS_ADMIN_ID, ASS_MARGIN, ASS_FRA, ASS_LIQUIDITY, "
			"ASS_PF_CLASS, ASS_PF_SUBCLASS, ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, ASS_CITI_CLASS, BLM_KEY, ASS_YC_NAME, "
			"ASS_IRS_TMPLATE, ASS_EXCHANGE, ASS_ACCOUNT, ASS_CLEAR_FEE FROM SEMAM.NW_ASSETS "
			"WHERE ASS_CODE = %s ");
	OraLoader.GetSql().Format((LPCTSTR ) Sql, QData.GetQueryText(GetAsset()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CAssetRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;

	CQData QData;
	CAssetIntRec IntRec;

	IntRec.SetAsset(GetAsset());

	if(!IntRec.DeleteRec(OraLoader))
		return FALSE;
	OraLoader.GetSql().Format("DELETE SEMAM.NW_ASSETS WHERE ASS_CODE = %s", 
								QData.GetQueryText(GetAsset()));
	return OraLoader.ExecuteSql();
}
