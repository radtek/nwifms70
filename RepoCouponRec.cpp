#include "stdafx.h"
#include "resource.h"
#include "repocouponrec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(CRepoCouponRec, CRecObj)
void CRepoCouponRec::Copy(CRepoCouponRec &RepoCouponRec)
{
	SetID(GetID());
	SetCP(RepoCouponRec.GetCP());
	SetClaimCode(RepoCouponRec.GetClaimCode());
    SetPortfolio(RepoCouponRec.GetPortfolio());
	SetTransNum(RepoCouponRec.GetTransNum());
	SetAsset(RepoCouponRec.GetAsset());
	SetPayDate(RepoCouponRec.GetPayDate());
	SetClaimDate(RepoCouponRec.GetClaimDate());
	SetNomAmount(RepoCouponRec.GetNomAmount());
	SetCurrency(RepoCouponRec.GetCurrency());
	SetInterest(RepoCouponRec.GetInterest());
	SetFxrate(RepoCouponRec.GetFxrate());
	SetUSDInterest(RepoCouponRec.GetUSDInterest());
	SetClaimed(RepoCouponRec.GetClaimed());
	SetStatus(RepoCouponRec.GetStatus());
}

void CRepoCouponRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetCP());
	Rec.Add(GetClaimCode());
    Rec.Add(GetPortfolio());
	Rec.Add(GetTransNum());
	Rec.Add(GetAsset());
	Rec.Add(GetPayDate());
	Rec.Add(GetClaimDate());
	Rec.Add(GetNomAmount());
	Rec.Add(GetCurrency());
	Rec.Add(GetInterest());
	Rec.Add(GetFxrate());
	Rec.Add(GetUSDInterest());
	Rec.Add(GetClaimed());
	Rec.Add(GetStatus());
}

BOOL CRepoCouponRec::AddRec(COraLoader &OraLoader)
{
	OraLoader.GetSql() = "SELECT COUNTERPARTY, CLAIM_CODE, PORTFOLIO, TRANS_NUM, "
						"PAYDATE, CLAIM_DATE, NOM_AMOUNT, CURRENCY, INTEREST, FXRATE, "
						"USD_INTEREST, CLAIMED, STATUS FROM SEMAM.NW_REPO_COUPONS ";

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CRepoCouponRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	CDBRec Rec;	
	CQData QData;

	OraLoader.GetSql().Format("SELECT COUNTERPARTY, CLAIM_CODE, PORTFOLIO, "
					"TRANS_NUM, ASSET_CODE, PAY_DATE, CLAIM_DATE, NOM_AMOUNT, "
					"CURRENCY, INTEREST, FXRATE, USD_INTEREST, CLAIMED, STATUS "
					"FROM SEMAM.NW_REPO_COUPONS WHERE ROWIDTOCHAR(ROWID) = %s ",
					QData.GetQueryText(GetID()));
	ToDBRec(Rec);

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	return OraLoader.UpdateRecord(Rec);
}

BOOL CRepoCouponRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	CQData QData;

	OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_REPO_COUPONS "
								"WHERE ROWIDTOCHAR(ROWID) = %s ", 
								QData.GetQueryText(GetID()));

	return OraLoader.ExecuteSql();
}
