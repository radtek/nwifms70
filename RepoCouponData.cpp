#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "qdata.h"
#include "sqlstmt.h"
#include "repocoupondata.h"

IMPLEMENT_DYNAMIC(CRepoCouponData, CRowCtrl)
int CRepoCouponData::LoadDBData(BOOL bInitial)
{
	if(!IsReady())
		return -1;

	GetOraLoader().GetSql() = "SELECT ROWIDTOCHAR(ROWID), COUNTERPARTY, "
				"NVL(CLAIM_CODE, '1') \"CLAIM_CODE\", PORTFOLIO, TRANS_NUM, "
				"ASSET_CODE, PAY_DATE, CLAIM_DATE, NOM_AMOUNT, CURRENCY, "
				"INTEREST, FXRATE, USD_INTEREST, CLAIMED, STATUS "
				"FROM SEMAM.NW_REPO_COUPONS WHERE ";
	
	if(bInitial)
		GetOraLoader().GetSql() += "CLAIMED IS NULL ";
	else
	{
		CDBRec Rec;
		CStringArray QRec;

		ToDBRec(Rec, TRUE);
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), "%");
		GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, 
							&GetSS()->GetFieldArray(), FALSE);
	}
	
	GetOraLoader().Open();
	return GetOraLoader().LoadRowCtrl(*this, TRUE);
}

BOOL CRepoCouponData::DeleteRow()
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

BOOL CRepoCouponData::UpdateRow()
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
