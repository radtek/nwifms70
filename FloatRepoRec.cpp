#include "stdafx.h"
#include "resource.h"
#include "floatreporec.h"
#include "qdata.h"



IMPLEMENT_DYNAMIC(CFloatRepoInvRec, CRecObj)	
void CFloatRepoInvRec::Copy(CFloatRepoInvRec &RepoInvRec)
{
	SetTransNum(RepoInvRec.GetTransNum());
	SetPortfolio(RepoInvRec.GetPortfolio());
	SetFromDate(RepoInvRec.GetFromDate());
	SetToDate(RepoInvRec.GetToDate());
	SetRate(RepoInvRec.GetRate());
	SetDays(RepoInvRec.GetDays());
	SetInterest(RepoInvRec.GetInterest());
}

void CFloatRepoInvRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetFromDate());
	Rec.Add(GetToDate());
	Rec.Add(GetRate());
	Rec.Add(GetPortfolio());
	Rec.Add(GetTransNum());
}

BOOL CFloatRepoInvRec::AddRec(COraLoader &OraLoader)
{
	if(GetTransNum().IsEmpty())
		return FALSE;
		
	OraLoader.GetSql() = "SELECT INT_FROM, INT_TO, RATE, PORTFOLIO, TRANS_NUM "
							"FROM SEMAM.NW_REPO_RATES ";
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CFloatRepoInvRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("SELECT INT_FROM, INT_TO, RATE, PORTFOLIO, TRANS_NUM "
						"FROM SEMAM.NW_REPO_RATES WHERE ROWIDTOCHAR(ROWID) = %s", 
						QData.GetQueryText(GetID()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CFloatRepoInvRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("DELETE SEMAM.NW_REPO_RATES WHERE ROWIDTOCHAR(ROWID) = %s", 
								QData.GetQueryText(GetID()));
	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CFloatRepoRec, CRecObj)
void CFloatRepoRec::Copy(CFloatRepoRec &RepoRec)
{
	SetTransNum(RepoRec.GetTransNum());
	SetPortfolio(RepoRec.GetPortfolio());
	SetNomAmount(RepoRec.GetNomAmount());
	SetCurrency(RepoRec.GetCurrency());
	SetPrice(RepoRec.GetPrice());
	SetRateBasis(RepoRec.GetRateBasis());
	SetValueDate(RepoRec.GetValueDate());
	SetMaturity(RepoRec.GetMaturity());
	SetAssetCode(RepoRec.GetAssetCode());
	SetAsset(RepoRec.GetAsset());
	SetAssFrom(RepoRec.GetAssFrom());
	SetAssRateBasis(RepoRec.GetAssRateBasis());
	SetAssetType(RepoRec.GetAssetType());
	SetTransType(RepoRec.GetTransType());
	SetIntRate(RepoRec.GetIntRate());
	SetAmort(RepoRec.GetAmort());
	SetCapRate(RepoRec.GetCapRate());
	SetPlus(RepoRec.GetPlus());
	SetAADays(RepoRec.GetAADays());
	SetAmount(RepoRec.GetAmount());
	SetFormula(RepoRec.GetFormula());
}

void CFloatRepoRec::SetAmountVal(LPCTSTR AmountVal)
{
	CQData QData;

	SetAmountVal(atof(QData.RemoveComma(AmountVal)));
}
