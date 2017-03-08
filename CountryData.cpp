#include "stdafx.h"
#include "resource.h"
#include "countrydata.h"
#include "qdata.h"
#include "nwivar.h"



IMPLEMENT_DYNAMIC(CCountryRec, CRecObj)
void CCountryRec::Copy(CCountryRec &Rec)
{
	SetCountry(Rec.GetCountry());
	SetCode(Rec.GetCode());
	SetCode2(Rec.GetCode2());
	SetOldCountry(Rec.GetOldCountry());
	SetLimit(Rec.GetLimit());
	SetGeoArea(Rec.GetGeoArea());
	SetPFRegion(Rec.GetPFRegion());
	SetProfileRegion(Rec.GetProfileRegion());
	SetPru(Rec.GetPru());
	SetCurrency(Rec.GetCurrency());
	SetNewCurrency(Rec.GetNewCurrency());
	SetConversion(Rec.GetConversion());
	SetConvDate(Rec.GetConvDate());
	SetSovFlag(Rec.GetSovFlag());
	SetSecFlag(Rec.GetSecFlag());
	SetDMFlag(Rec.GetDMFlag());
}

void CCountryRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetCountry());
	Rec.Add(GetCode());
	Rec.Add(GetCode2());
	Rec.Add(GetOldCountry());
	Rec.Add(EMPTYSTRING);
	Rec.Add(GetGeoArea());
	Rec.Add(GetPFRegion());
	Rec.Add(GetProfileRegion());
	Rec.Add(GetPru());
	Rec.Add(GetCurrency());
	Rec.Add(GetNewCurrency());
	Rec.Add(GetConversion());
	Rec.Add(GetConvDate());
	Rec.Add(GetSovFlag());
	Rec.Add(GetSecFlag());
	Rec.Add(GetDMFlag());
}

BOOL CCountryRec::AddRec(COraLoader &OraLoader)
{
	if(GetCountry().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT COUNTRY, C_CODE, C_CODE2, OLD_COUNTRY, C_LIMIT, "
					"GEO_AREA, PF_REGION, PROFILE_REGION, PRU_TIER, CURRENCY, "
					"NEW_CURRENCY, CONVERSION, CONV_DATE, SOV_FLAG, SEC_FLAG, DM_FLAG "
					"FROM SEMAM.NW_COUNTRIES ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CCountryRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCountry().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("SELECT COUNTRY, C_CODE, C_CODE2, OLD_COUNTRY, C_LIMIT, "
								"GEO_AREA, PF_REGION, PROFILE_REGION, PRU_TIER, CURRENCY, "
								"NEW_CURRENCY, CONVERSION, CONV_DATE, SOV_FLAG, SEC_FLAG, "
								"DM_FLAG FROM SEMAM.NW_COUNTRIES "
								"WHERE COUNTRY = %s", QData.GetQueryText(GetCountry()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CCountryRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCountry().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("DELETE SEMAM.NW_COUNTRIES WHERE COUNTRY = %s ", 
								QData.GetQueryText(GetCountry()));
	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CCountryData, CRowCtrl)
BOOL CCountryData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CCountryData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().GetSql() = "SELECT COUNTRY, C_CODE, C_CODE2, OLD_COUNTRY, C_LIMIT, "
							"GEO_AREA, PF_REGION, PROFILE_REGION, SOV_FLAG, SEC_FLAG, "
							"DM_FLAG, PRU_TIER, CURRENCY, NEW_CURRENCY, CONVERSION, "
							"CONV_DATE FROM SEMAM.NW_COUNTRIES ";
	GetOraLoader().GetSql() += " ORDER BY 1 ";
	
	if(!GetOraLoader().Open())
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CCountryData::AddRow()
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

BOOL CCountryData::DeleteRow()
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

BOOL CCountryData::UpdateRow()
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

