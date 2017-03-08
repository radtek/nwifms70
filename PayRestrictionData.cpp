#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include "resource.h"
#include "payrestrictiondata.h"
#include "sqlstmt.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(CPayRestrictionRec, CRecObj)
void CPayRestrictionRec::Copy(CPayRestrictionRec &PayRestrictionRec)
{
	SetID(PayRestrictionRec.GetID());
	SetPortfolio(PayRestrictionRec.GetPortfolio());
	SetRefID(PayRestrictionRec.GetRefID());
}

void CPayRestrictionRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	Rec.Add(GetPortfolio());
	Rec.Add(GetRefID());
}

BOOL CPayRestrictionRec::AddRec(COraLoader &OraLoader)
{
	if(GetPortfolio().IsEmpty())
		return FALSE;
	
	if(!OraLoader.Open("SELECT PORTFOLIO, REF_ID FROM SEMAM.NW_PAY_RESTRICTION ", ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPayRestrictionRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty() || strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT PORTFOLIO, REF_ID FROM SEMAM.NW_PAY_RESTRICTION "
						"WHERE ROWIDTOCHAR(ROWID) = %s ", QData.GetQueryText(GetID())); // Rowid 

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CPayRestrictionRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty() || strlen(GetID()) == 0)
		return FALSE;

	CQData QData;

	OraLoader.GetSql().Format("DELETE SEMAM.NW_PAY_RESTRICTION WHERE ROWIDTOCHAR(ROWID) = %s ",
								QData.GetQueryText(GetID()));

	return OraLoader.ExecuteSql();
}


IMPLEMENT_DYNAMIC(CPayRestrictionData, CRowCtrl)
BOOL CPayRestrictionData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CPayRestrictionData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	if(!GetOraLoader().Open("SELECT ROWIDTOCHAR(ROWID), PORTFOLIO, REF_ID "
							"FROM SEMAM.NW_PAY_RESTRICTION ORDER BY 1 "))
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE);
}

BOOL CPayRestrictionData::LoadRefData(CDBSheet &SS)
{
	if(!IsReady())
		return FALSE;

	if(!GetOraLoader().Open("SELECT REF_ID, COUNTERPARTY, CURRENCY, BANK, ACCOUNT, CP_REF, "
						"BENEFICIARY, BENEFICIARY_ID, BENEFICIARY_BIC, BENEFICIARY_ACCOUNT, "
						"FFC, FFC_ID, FFC_BIC, FFC_ACCOUNT, ACTIVE FROM SEMAM.NW_PAY_TEMPLATE "
						"ORDER BY 1 "))
		return FALSE;
	
	return GetOraLoader().LoadDBSheet(SS, TRUE);
}

BOOL CPayRestrictionData::AddRow()
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

BOOL CPayRestrictionData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetRec().SetID(GetID());
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

BOOL CPayRestrictionData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetRec().SetID(GetID());
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