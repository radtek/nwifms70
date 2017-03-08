#include "stdafx.h"
#include "resource.h"
#include "accountdata.h"
#include "qdata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CAccountRec, CRecObj)
void CAccountRec::Copy(CAccountRec &AccountRec)
{
	SetID(AccountRec.GetID());
	SetCP(AccountRec.GetCP());
	SetAccount(AccountRec.GetAccount());
	SetDesc(AccountRec.GetDesc());
	SetGroup(AccountRec.GetGroup());
	SetCollateral(AccountRec.GetCollateral());
	SetFuture(AccountRec.GetFuture());
	SetClearing(AccountRec.GetClearing());
	SetActive(AccountRec.GetActive());
}

void CAccountRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetAccount());
	Rec.Add(GetDesc());
	Rec.Add(GetCP());
	Rec.Add(GetGroup());
	Rec.Add(GetCollateral());
	Rec.Add(GetFuture());
	Rec.Add(GetClearing());
	Rec.Add(GetActive());
}

BOOL CAccountRec::AddRec(COraLoader &OraLoader)
{
//	if(GetCP().IsEmpty())
//		return FALSE;

	OraLoader.GetSql() = "SELECT ACC_CODE, ACC_DESC, COUNTERPARTY, GROUP_ID, COLLATERAL, "
						"FUTURE, CLEARING, ACTIVE "
						"FROM SEMAM.NW_CASH_ACCOUNTS ";

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAccountRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
//	if(strlen(GetCP()) == 0)
//		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT ACC_CODE, ACC_DESC, COUNTERPARTY, GROUP_ID, COLLATERAL, "
							"FUTURE, CLEARING, ACTIVE "
							"FROM SEMAM.NW_CASH_ACCOUNTS WHERE ROWIDTOCHAR(ROWID) = %s ", 
							QData.GetQueryText(GetID()));

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CAccountRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCP().IsEmpty())
		return FALSE;

	CQData QData;

	OraLoader.GetSql().Format("DELETE SEMAM.NW_CASH_ACCOUNTS WHERE ROWIDTOCHAR(ROWID) = %s ", 
							QData.GetQueryText(GetID()));

	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CAccountData, CRowCtrl)
BOOL CAccountData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CAccountData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().GetSql() = "SELECT ROWIDTOCHAR(ROWID), ACC_CODE, ACC_DESC, COUNTERPARTY, "
							"GROUP_ID, COLLATERAL, FUTURE, CLEARING, ACTIVE "
							"FROM SEMAM.NW_CASH_ACCOUNTS ORDER BY 2";	
	if(!GetOraLoader().Open())
		return FALSE;

	return GetOraLoader().LoadRowCtrl(*this, TRUE);
}

BOOL CAccountData::AddRow()
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

BOOL CAccountData::DeleteRow()
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

BOOL CAccountData::UpdateRow()
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
