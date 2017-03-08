#include "stdafx.h"
#include "resource.h"
#include "investorrec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(CInvestorAssetRec, CRecObj)
void CInvestorAssetRec::Copy(CInvestorAssetRec &InvestorAssetRec)
{
	SetID(InvestorAssetRec.GetID());

	SetInvCode(InvestorAssetRec.GetInvCode());
	SetAsset(InvestorAssetRec.GetAsset());
	SetTaxID(InvestorAssetRec.GetTaxID());
	SetOnShore(InvestorAssetRec.GetOnShore());
	SetErisa(InvestorAssetRec.GetErisa());
	SetBhca(InvestorAssetRec.GetBhca());
	SetIPO(InvestorAssetRec.GetIPO());
	SetSide(InvestorAssetRec.GetSide());
}

void CInvestorAssetRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetInvCode());
	Rec.Add(GetAsset());
	Rec.Add(GetTaxID());
	Rec.Add(GetOnShore());
	Rec.Add(GetErisa());
	Rec.Add(GetBhca());
	Rec.Add(GetIPO());
	Rec.Add(GetSide());
}

BOOL CInvestorAssetRec::AddRec(COraLoader &OraLoader)
{
	if(GetInvCode().IsEmpty() || GetAsset().IsEmpty())
		return FALSE;
	
	if(!OraLoader.Open("SELECT INV_CODE, ASSET, TAX_ID, ONSHORE, ERISA, BHCA, IPO, SIDE FROM SEMAM.NW_INVESTOR_ASSET ", 
						ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CInvestorAssetRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetInvCode().IsEmpty() || GetAsset().IsEmpty())
		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT INV_CODE, ASSET, TAX_ID, ONSHORE, ERISA, BHCA, IPO, SIDE "
						"FROM SEMAM.NW_INVESTOR_ASSET WHERE ROWIDTOCHAR(ROWID) = %s ", QData.GetQueryText(GetID()));
							
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CInvestorAssetRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	CQData QData;

	if(bByID)
	{
		if(strlen(GetID()) == 0)
			return FALSE;
		OraLoader.GetSql().Format("DELETE SEMAM.NW_INVESTOR_ASSET WHERE ROWIDTOCHAR(ROWID) = %s ", QData.GetQueryText(GetID()));
	}
	else
	{
		if(GetInvCode().IsEmpty())
			return FALSE;
		OraLoader.GetSql().Format("DELETE SEMAM.NW_INVESTOR_ASSET WHERE INV_CODE = %s ", QData.GetQueryText(GetInvCode()));
	}

	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CInvestorRec, CRecObj)
void CInvestorRec::Copy(CInvestorRec &InvestorRec)
{
	SetID(GetID());
	SetInvCode(InvestorRec.GetInvCode());
    SetInvestor(InvestorRec.GetInvestor());
	SetAddress(InvestorRec.GetAddress());
	SetCity(InvestorRec.GetCity());
	SetCountry(InvestorRec.GetCountry());
	SetAgent(InvestorRec.GetAgent());
	SetInvClass(InvestorRec.GetInvClass());
	SetMgmtFee(InvestorRec.GetMgmtFee());
	SetPerformance(InvestorRec.GetPerformance());
	SetStatus(InvestorRec.GetStatus());
}

void CInvestorRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetInvCode());
    Rec.Add(GetInvestor());
	Rec.Add(GetAddress());
	Rec.Add(GetCity());
	Rec.Add(GetCountry());
	Rec.Add(GetAgent());
	Rec.Add(GetInvClass());
	Rec.Add(GetMgmtFee());
	Rec.Add(GetPerformance());
	Rec.Add(GetStatus());
}

BOOL CInvestorRec::AddRec(COraLoader &OraLoader)
{
	if(GetInvCode().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT INV_CODE, INVESTOR, ADDRESS, CITY_ADDRESS, COUNTRY, AGENT, INV_CLASS, "
						"MGMTFEE, PERFORMANCE, STATUS FROM SEMAM.NW_INVESTORS ", ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CInvestorRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetInvCode().IsEmpty())
		return FALSE;
	
	CQData QData;
	CDBRec Rec;	

	OraLoader.GetSql().Format("SELECT INV_CODE, INVESTOR, ADDRESS, CITY_ADDRESS, COUNTRY, AGENT, INV_CLASS, "
							"MGMTFEE, PERFORMANCE, STATUS FROM SEMAM.NW_INVESTORS WHERE INV_CODE = %s ", 
							QData.GetQueryText(GetInvCode()));

	ToDBRec(Rec);
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	return OraLoader.UpdateRecord(Rec);
}

BOOL CInvestorRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetInvCode().IsEmpty())
		return FALSE;

	CInvestorAssetRec InvestorAssetRec;
	CQData QData;

	InvestorAssetRec.SetInvCode(GetInvCode());
	InvestorAssetRec.DeleteRec(OraLoader, FALSE);

	OraLoader.GetSql().Format("DELETE SEMAM.NW_INVESTORS WHERE INV_CODE = %s ", QData.GetQueryText(GetInvCode()));

	return OraLoader.ExecuteSql();
}
