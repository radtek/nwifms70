#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "qdata.h"
#include "sqlstmt.h"
#include "investordata.h"


IMPLEMENT_DYNAMIC(CInvestorData, CMSRowCtrl)
void CInvestorData::SetHeaders()
{
	if(!IsReady())
		return;

	if(GetOraLoader().Open("SELECT INV_CODE, INVESTOR, ADDRESS, CITY_ADDRESS, COUNTRY, AGENT, INV_CLASS, "
							"MGMTFEE, PERFORMANCE, STATUS FROM SEMAM.NW_INVESTORS WHERE 1 = 2 "))
	{
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
		GetSS()->SetSheetColHeaders();
	}

	if(GetOraLoader().Open("SELECT INV_CODE, ASSET, TAX_ID, ONSHORE, ERISA, BHCA, IPO, SIDE "
							"FROM SEMAM.NW_INVESTOR_ASSET WHERE 1 = 2 "))
	{
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
	}
}

int CInvestorData::LoadDBData()
{
	CDBRec Rec;
	CStringArray QRec;

	if(!IsReady())
		return FALSE;

	ToDBRec(Rec, TRUE);
	if(Rec.GetSize() <= 0)
		return -1;

	GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), "%");
	GetOraLoader().GetSql() = "SELECT INV_CODE, INVESTOR, ADDRESS, CITY_ADDRESS, COUNTRY, AGENT, INV_CLASS, "
								"MGMTFEE, PERFORMANCE, STATUS FROM SEMAM.NW_INVESTORS WHERE ";
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), FALSE);
	GetOraLoader().GetSql() += " ORDER BY 1 ";
	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
	
	return FALSE;
}

BOOL CInvestorData::AddRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetInvestorRec(), &GetInvestorAssetRec());
	if(GetInvestorRec().GetInvCode().IsEmpty())
		return FALSE;

	if(GetModify(FALSE))
	{
		GetOraLoader().BeginTransaction();
		if(GetInvestorRec().AddRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			AddSheetRow();
			return TRUE;
		}	

		GetOraLoader().Rollback();
		return FALSE;
	}
	else
		return AddSRow(FALSE);
}

BOOL CInvestorData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetInvestorRec(), &GetInvestorAssetRec());
	if(strlen(GetKey()) == 0)
		return FALSE;
	
	if(GetSS()->GetIsBlockSelected())
	{
		GetOraLoader().BeginTransaction();
		if(GetInvestorRec().DeleteRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			DeleteSheetRow(GetCurrentRow());
			LoadSheet();
			Refresh();
			return TRUE;
		}
		
		GetOraLoader().Rollback();
		return FALSE;
	}
	else
		return DeleteSRow();
}

BOOL CInvestorData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetInvestorRec(), &GetInvestorAssetRec());

	if(strlen(GetKey()) == 0)
		return FALSE;

	if(GetModify(FALSE))
	{
		GetOraLoader().BeginTransaction();
		if(GetInvestorRec().UpdateRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			UpdateSheetRow(FALSE);
			return TRUE;
		}	

		GetOraLoader().Rollback();
		return FALSE;
	}
	
	return UpdateSRow(FALSE);
}

BOOL CInvestorData::AddSRow(BOOL bDirectly)
{
	if(!IsReady())
		return FALSE;

	if(bDirectly)
		GetSRowCtrl().UpdateData();
	if(!GetSRowCtrl().GetModify(FALSE))
		return TRUE;
	
	GetInvestorAssetRec().SetInvCode(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetInvestorAssetRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		LoadSheet();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CInvestorData::DeleteSRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	if(GetSRowCtrl().GetSS()->GetIsBlockSelected())
	{
		GetInvestorAssetRec().SetID(GetSRowCtrl().GetID());
		GetInvestorAssetRec().SetInvCode(GetKey());
		GetOraLoader().BeginTransaction();
		if(GetInvestorAssetRec().DeleteRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			GetSRowCtrl().DeleteSheetRow(CurrentRow);
			LoadSheet();
			GetSRowCtrl().SetCurrentRow(-1);
			GetSRowCtrl().Refresh();
			return TRUE;
		}
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CInvestorData::UpdateSRow(BOOL bDirectly)
{
	if(!IsReady())
		return FALSE;

	if(bDirectly)
		GetSRowCtrl().UpdateData();
	
	if(!GetSRowCtrl().GetModify(FALSE))
		return TRUE;

	GetInvestorAssetRec().SetID(GetSRowCtrl().GetID());
	GetInvestorAssetRec().SetInvCode(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetInvestorAssetRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		GetSRowCtrl().UpdateSheetRow(FALSE);
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

void CInvestorData::LoadSheet()
{
	if(!IsReady())
		return;

	CQData QData;

	SetKey();
	GetOraLoader().GetSql().Format("SELECT ROWIDTOCHAR(ROWID), ASSET, TAX_ID, ONSHORE, ERISA, BHCA, IPO, SIDE "
								"FROM SEMAM.NW_INVESTOR_ASSET WHERE INV_CODE = %s ", QData.GetQueryText(GetKey()));
	if(GetOraLoader().Open())
		GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE);
}
