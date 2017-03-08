#include "stdafx.h"
#include "resource.h"
#include "currencydata.h"
#include "qdata.h"
#include "nwivar.h"



IMPLEMENT_DYNAMIC(CCurrencyRec, CRecObj)
void CCurrencyRec::Copy(CCurrencyRec &Rec)
{
	SetCurrency(Rec.GetCurrency());
	SetNewCurrency(Rec.GetNewCurrency());
	SetConversion(Rec.GetConversion());
	SetConvDate(Rec.GetConvDate());
	SetDBPBTier(Rec.GetDBPBTier());
	SetHSBCTier(Rec.GetHSBCTier());
	SetInverted(Rec.GetInverted());
	SetNDFFix(Rec.GetNDFFix());
	Set1256(Rec.Get1256());
}

void CCurrencyRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetCurrency());
	Rec.Add(GetNewCurrency());
	Rec.Add(GetConversion());
	Rec.Add(GetConvDate());
	Rec.Add(GetDBPBTier());
	Rec.Add(GetHSBCTier());
	Rec.Add(GetInverted());
	Rec.Add(GetNDFFix());
	Rec.Add(Get1256());
}

BOOL CCurrencyRec::AddRec(COraLoader &OraLoader)
{
	if(GetCurrency().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT CURRENCY, NEW_CURRENCY, CONVERSION, CONV_DATE, DB_TIER, "
					"HSBC_TIER, REV, NDF_FIX, SEC_1256 FROM SEMAM.NW_CURRENCY ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CCurrencyRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCurrency().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("SELECT CURRENCY, NEW_CURRENCY, CONVERSION, CONV_DATE, DB_TIER, "
								"HSBC_TIER, REV, NDF_FIX, SEC_1256 "
								"FROM SEMAM.NW_CURRENCY WHERE CURRENCY = %s", 
								QData.GetQueryText(GetCurrency()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CCurrencyRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCurrency().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("DELETE SEMAM.NW_CURRENCY WHERE CURRENCY = %s ", 
								QData.GetQueryText(GetCurrency()));
	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CCurrencyData, CRowCtrl)
BOOL CCurrencyData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CCurrencyData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().GetSql() = "SELECT CURRENCY, NEW_CURRENCY, CONVERSION, CONV_DATE, DB_TIER, "
								"HSBC_TIER, REV, NDF_FIX, SEC_1256 FROM SEMAM.NW_CURRENCY ";
	GetOraLoader().GetSql() += " ORDER BY 1 ";
	
	if(!GetOraLoader().Open())
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CCurrencyData::AddRow()
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

BOOL CCurrencyData::DeleteRow()
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

BOOL CCurrencyData::UpdateRow()
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

