#include "stdafx.h"
#include "resource.h"
#include "assetmasterrec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(CAssetMasterClassRec, CRecObj)
void CAssetMasterClassRec::Copy(CAssetMasterClassRec &AssetClassRec)
{
	SetID(AssetClassRec.GetID());
	SetAssetCode(AssetClassRec.GetAssetCode());
	SetAssetDesc(AssetClassRec.GetAssetDesc());
	SetAsset(AssetClassRec.GetAsset());
	SetCategory(AssetClassRec.GetCategory());
	SetBucket(AssetClassRec.GetBucket());
	SetClass(AssetClassRec.GetClass());
}

void CAssetMasterClassRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	Rec.Add(GetAssetCode());
	Rec.Add(GetAssetDesc());
	Rec.Add(GetAsset());
	Rec.Add(GetCategory());
	Rec.Add(GetBucket());
	Rec.Add(GetClass());
}

BOOL CAssetMasterClassRec::AddRec(COraLoader &OraLoader)
{
	if(GetAssetCode().IsEmpty())
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	if(!OraLoader.Open("SELECT ASSET_CODE, ASSET_DESC, ASS_CODE, CATEGORY, BUCKET, CLASS FROM SEMAM.NW_ASSET_CLASS ", 
						ODYNASET_DEFAULT))
		return FALSE;

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAssetMasterClassRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{

	if(!bByID || strlen(GetAssetCode()) == 0 || strlen(GetID()) == 0)
		return FALSE;
	
	CString Sql;
	CQData QData;

	Sql.Format("SELECT ASSET_CODE, ASSET_DESC, ASS_CODE, CATEGORY, BUCKET, CLASS FROM SEMAM.NW_ASSET_CLASS "
				"WHERE ROWIDTOCHAR(ROWID) = %s ", QData.GetQueryText(GetID())); 

	if(!OraLoader.Open(Sql, ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CAssetMasterClassRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAssetCode().IsEmpty())
		return FALSE;

	CQData QData;
	if(bByID)
	{
		if(strlen(GetID()) == 0)
			return FALSE;
		OraLoader.GetSql().Format("DELETE SEMAM.NW_ASSET_CLASS WHERE ROWIDTOCHAR(ROWID) = %s", QData.GetQueryText(GetID()));
	}
	else
		OraLoader.GetSql().Format("DELETE SEMAM.NW_ASSET_CLASS WHERE ASS_CODE = %s", QData.GetQueryText(GetAsset()));

	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CAssetMasterIntRec, CRecObj)
void CAssetMasterIntRec::Copy(CAssetMasterIntRec &AssetIntRec)
{
	SetID(AssetIntRec.GetID());
	SetAsset(AssetIntRec.GetAsset());
	SetAssetFrom(AssetIntRec.GetAssetFrom());
	SetAssetTo(AssetIntRec.GetAssetTo());
	SetFrom(AssetIntRec.GetFrom());
	SetTo(AssetIntRec.GetTo());
	SetSetDate(AssetIntRec.GetSetDate());
	SetRate(AssetIntRec.GetRate());
	SetAmort(AssetIntRec.GetAmort());
	SetCap(AssetIntRec.GetCap());
	SetPlus(AssetIntRec.GetPlus());
	SetInclusive(AssetIntRec.GetInclusive());
}

void CAssetMasterIntRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	Rec.Add(GetAsset());
	Rec.Add(GetAssetFrom());
	Rec.Add(GetAssetTo());
	Rec.Add(GetFrom());
	Rec.Add(GetTo());
	Rec.Add(GetSetDate());
	Rec.Add(GetRate());
	Rec.Add(GetAmort());
	Rec.Add(GetCap());
	Rec.Add(GetPlus());
	Rec.Add(GetInclusive());
}

BOOL CAssetMasterIntRec::AddRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty())
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	if(!OraLoader.Open("SELECT ASS_CODE, FROMDATE, TODATE, INT_FROM, INT_TO, INT_SET_DATE, RATE, AMORT_FACT, "
						"CAPIT_RATE, PLUS_AMOUNT, INCLUSIVE FROM SEMAM.NW_ASSET_RATE ", ODYNASET_DEFAULT))
		return FALSE;

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAssetMasterIntRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(!bByID || strlen(GetAsset()) == 0 || strlen(GetID()) == 0)
		return FALSE;
	CSqlStmt Sql;
	CQData QData;

	Sql = "SELECT ASS_CODE, FROMDATE, TODATE, INT_FROM, INT_TO, INT_SET_DATE, RATE, AMORT_FACT, CAPIT_RATE, "
			"PLUS_AMOUNT, INCLUSIVE FROM SEMAM.NW_ASSET_RATE WHERE ROWIDTOCHAR(ROWID) = %s ";

	OraLoader.GetSql().Format((LPCTSTR) Sql, QData.GetQueryText(GetID()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CAssetMasterIntRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;

	CQData QData;
	if(bByID)
	{
		if(strlen(GetID()) == 0)
			return FALSE;
		OraLoader.GetSql().Format("DELETE SEMAM.NW_ASSET_RATE WHERE ROWIDTOCHAR(ROWID) = %s", QData.GetQueryText(GetID()));
	}
	else
		OraLoader.GetSql().Format("DELETE SEMAM.NW_ASSET_RATE WHERE ASS_CODE = %s", QData.GetQueryText(GetAsset()));

	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CAssetMasterRec, CRecObj)
void CAssetMasterRec::Copy(CAssetMasterRec &Rec)
{
	Setup(Rec.GetAsset(), Rec.GetDesc(), Rec.GetCurrency());
	SetPar(Rec.GetPar());
	SetUnderline(Rec.GetUnderline());
	SetCommon(Rec.GetCommon());
	SetCusip(Rec.GetCusip());
	SetSedol(Rec.GetSedol());
	SetIsin(Rec.GetIsin());
	SetPID(Rec.GetPID());
	SetIssueDate(Rec.GetIssueDate());
	SetCDSFeeType(Rec.GetCDSFeeType());
	SetMaturity(Rec.GetMaturity());
	SetRollDate(Rec.GetRollDate());
	SetRefIndex(Rec.GetRefIndex());
	SetFormula(Rec.GetFormula());
	SetSource(Rec.GetSource());
	SetCountry(Rec.GetCountry());
	SetIssuer(Rec.GetIssuer());
	SetIndustry(Rec.GetIndustry());
	SetRateType(Rec.GetRateType());
	SetRateBasis(Rec.GetRateBasis());
	SetPmntFrq(Rec.GetPmntFrq());
	SetAdjFactor(Rec.GetAdjFactor());
	SetFactor(Rec.GetFactor());
	SetAccrue(Rec.GetAccrue());
	SetPrePaid(Rec.GetPrePaid());
	SetFuture(Rec.GetFuture());
	SetBelowRating(Rec.GetBelowRating());
	Set144A(Rec.Get144A());
	Set1256(Rec.Get1256());
	SetMethod(Rec.GetMethod());
	SetUnlisted(Rec.GetUnlisted());
	SetInactive(Rec.GetInactive());
}

void CAssetMasterRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetAsset());
	Rec.Add(GetDesc());
	Rec.Add(GetPar());
	Rec.Add(GetUnderline());
	Rec.Add(GetCommon());
	Rec.Add(GetCusip());
	Rec.Add(GetSedol());
	Rec.Add(GetIsin());
	Rec.Add(GetPID());
	Rec.Add(GetIssueDate());
	Rec.Add(GetCDSFeeType());
	Rec.Add(GetMaturity());
	Rec.Add(GetRollDate());
	Rec.Add(GetRefIndex());
	Rec.Add(GetFormula());
	Rec.Add(GetSource());
	Rec.Add(GetCountry());
	Rec.Add(GetCurrency());
	Rec.Add(GetIssuer());
	Rec.Add(GetIndustry());
	Rec.Add(GetRateType());
	Rec.Add(GetRateBasis());
	Rec.Add(GetPmntFrq());
	Rec.Add(GetAdjFactor());
	Rec.Add(GetFactor());
	Rec.Add(GetAccrue());
	Rec.Add(GetPrePaid());
	Rec.Add(GetFuture());
	Rec.Add(Get1256());
	Rec.Add(GetBelowRating());
	Rec.Add(Get144A());
	Rec.Add(GetMethod());
	Rec.Add(GetUnlisted());
	Rec.Add(GetInactive());
}

BOOL CAssetMasterRec::AddRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty())
		return FALSE;
	
	if(!OraLoader.Open("SELECT ASS_CODE, ASSET, PAR_VALUE, ASSET2, COMMON, CUSIP, SEDOL, ISIN, "
					"PID, ISSUE_DATE, CDS_FEE_TYPE, MATURITY, ROLL_DATE, FLOAT_INDEX, "
					"FLOAT_FORMULA, SOURCE, COUNTRY, CURRENCY, ISSUER, INDUSTRY, INT_TYPE, "
					"RATE_BASIS, FREQ, ADJUST_FACTOR, FACTOR, ACCRUABLE, PPAID_INT, FUTURE, "
					"T1256, BELOW_RATING, ID_144A, METHOD, UNLISTED, STATUS "
					"FROM SEMAM.NW_ASSET_MASTER ", 
					ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAssetMasterRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;

	CQData QData;

	OraLoader.GetSql().Format("SELECT ASS_CODE, ASSET, PAR_VALUE, ASSET2, COMMON, CUSIP, SEDOL, ISIN, "
			"PID, ISSUE_DATE, CDS_FEE_TYPE, MATURITY, ROLL_DATE, FLOAT_INDEX, "
			"FLOAT_FORMULA, SOURCE, COUNTRY, CURRENCY, ISSUER, INDUSTRY, INT_TYPE, RATE_BASIS, FREQ, "
			"ADJUST_FACTOR, FACTOR, ACCRUABLE, PPAID_INT, FUTURE, T1256, BELOW_RATING, ID_144A, METHOD, "
			"UNLISTED, STATUS "
			"FROM SEMAM.NW_ASSET_MASTER WHERE ASS_CODE = %s ", QData.GetQueryText(GetAsset()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CAssetMasterRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;

	CQData QData;
	CAssetMasterIntRec IntRec;

	IntRec.SetAsset(GetAsset());
	if(!IntRec.DeleteRec(OraLoader))
		return FALSE;

	OraLoader.GetSql().Format("DELETE SEMAM.NW_ASSET_MASTER WHERE ASS_CODE = %s ", 
		                       QData.GetQueryText(GetAsset()));
	return OraLoader.ExecuteSql();
}