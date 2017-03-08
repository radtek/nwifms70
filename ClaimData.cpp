#include "stdafx.h"
#include "resource.h"
#include "claimdata.h"
#include "qdata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CClaimData, CRowCtrl)
BOOL CClaimData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CClaimData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().GetSql() = "SELECT ROWIDTOCHAR(ROWID), CP_CODE, NVL(CLAIM_CODE, '1') \"CLAIM_CODE\", "
								"NAME, PHONE, FAX, EMAIL FROM SEMAM.NW_CLAIM_CP ORDER BY 2, 3 ";	
	if(!GetOraLoader().Open())
		return FALSE;

	return GetOraLoader().LoadRowCtrl(*this, TRUE);
}

BOOL CClaimData::AddRow()
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

BOOL CClaimData::DeleteRow()
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

BOOL CClaimData::UpdateRow()
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
