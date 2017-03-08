#include "stdafx.h"
#include "resource.h"
#include "clearingfeedata.h"
#include "qdata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CClearingFeeRec, CRecObj)
void CClearingFeeRec::Copy(CClearingFeeRec &ClearingFeeRec)
{
	SetAccount(ClearingFeeRec.GetAccount());
	SetFromDate(ClearingFeeRec.GetFromDate());
	SetToDate(ClearingFeeRec.GetToDate());
	SetClearingFee(ClearingFeeRec.GetClearingFee());
}

void CClearingFeeRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetAccount());
	Rec.Add(GetFromDate());
	Rec.Add(GetToDate());
	Rec.Add(GetClearingFee());
}

BOOL CClearingFeeRec::AddRec(COraLoader &OraLoader)
{
	if(GetAccount().IsEmpty())
		return FALSE;

	OraLoader.GetSql() = "SELECT ACC_CODE, FROM_DATE, TO_DATE, CLEARING_FEE "
						"FROM SEMAM.NW_CLEARING_FEE ";

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CClearingFeeRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	CQData QData;

	OraLoader.GetSql().Format("SELECT ACC_CODE, FROM_DATE, TO_DATE, CLEARING_FEE "
							"FROM SEMAM.NW_CLEARING_FEE WHERE ROWIDTOCHAR(ROWID) = %s ", 
							QData.GetQueryText(GetID()));

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CClearingFeeRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetAccount().IsEmpty())
		return FALSE;

	CQData QData;

	OraLoader.GetSql().Format("DELETE SEMAM.NW_CLEARING_FEE WHERE ROWIDTOCHAR(ROWID) = %s ", 
							QData.GetQueryText(GetID()));

	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CClearingFeeData, CRowCtrl)
BOOL CClearingFeeData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CClearingFeeData::LoadAccounts(COptComboBox *Account)
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().GetSql() = "SELECT ACC_CODE FROM SEMAM.NW_CASH_ACCOUNTS "
								"WHERE CLEARING = 'Y' ORDER BY 1 ";	
	if(!GetOraLoader().Open())
		return FALSE;

	return GetOraLoader().LoadComboBox(*Account);
}

BOOL CClearingFeeData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().GetSql() = "SELECT ROWIDTOCHAR(ROWID), ACC_CODE, FROM_DATE, TO_DATE, "
							"CLEARING_FEE FROM SEMAM.NW_CLEARING_FEE ORDER BY 2, 3 ";	
	if(!GetOraLoader().Open())
		return FALSE;

	return GetOraLoader().LoadRowCtrl(*this, TRUE);
}

BOOL CClearingFeeData::AddRow()
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

BOOL CClearingFeeData::DeleteRow()
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

BOOL CClearingFeeData::UpdateRow()
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
