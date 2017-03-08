#include "stdafx.h"
#include "resource.h"
#include "assetsupplementdata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CAssetSupplementRec, CRecObj)
void CAssetSupplementRec::Copy(CAssetSupplementRec &Rec)
{
	SetAsset(Rec.GetAsset());
	SetCode(Rec.GetCode());
	SetDesc(Rec.GetDesc());
	SetCommon(Rec.GetCommon());
	SetIsin(Rec.GetIsin());
	SetAmountOf(Rec.GetAmountOf());
	SetPerAmount(Rec.GetPerAmount());
	SetRoundingFactor(Rec.GetRoundingFactor());

	SetUser(Rec.GetUser());
}

void CAssetSupplementRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetAsset());
	Rec.Add(GetCode());
	Rec.Add(GetDesc());
	Rec.Add(GetCommon());
	Rec.Add(GetIsin());
	Rec.Add(GetAmountOf());
	Rec.Add(GetPerAmount());
	Rec.Add(GetRoundingFactor());
	Rec.Add(GetUser());
	Rec.Add(DBSYSDATE);
}

BOOL CAssetSupplementRec::AddRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty())
		return FALSE;
	
	if(!OraLoader.Open("SELECT ASSET, SUP_CODE, SUP_DESC,  COMMON_CODE, ISIN_CODE, AMOUNT_OF, "
						"PER_AMOUNT, ROUNDING_FACT, USER_STAMP, DATE_STAMP "
						"FROM SEMAM.NW_ASS_SUPPLEMENTS", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAssetSupplementRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("SELECT ASSET, SUP_CODE, SUP_DESC, COMMON_CODE, ISIN_CODE, AMOUNT_OF, "
								"PER_AMOUNT, ROUNDING_FACT, USER_STAMP, DATE_STAMP "
								"FROM SEMAM.NW_ASS_SUPPLEMENTS WHERE ASSET = %s", 
								QData.GetQueryText(GetAsset()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CAssetSupplementRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAsset().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("DELETE SEMAM.NW_ASS_SUPPLEMENTS WHERE ASSET = %s", 
							QData.GetQueryText(GetAsset()));
	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CAssetSupplementData, CRowCtrl)
BOOL CAssetSupplementData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CAssetSupplementData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	if(!GetOraLoader().Open("SELECT ASSET, SUP_CODE, SUP_DESC, COMMON_CODE, ISIN_CODE, AMOUNT_OF, "
							"PER_AMOUNT, ROUNDING_FACT FROM SEMAM.NW_ASS_SUPPLEMENTS ORDER BY 1, 3"))
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CAssetSupplementData::AddRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		AddSheetRow();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CAssetSupplementData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetRec().DeleteRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		DeleteSheetRow(GetCurrentRow());
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CAssetSupplementData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		UpdateSheetRow(FALSE);
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

