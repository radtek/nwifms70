#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "qdata.h"
#include "sqlstmt.h"
#include "cpdata.h"


IMPLEMENT_DYNAMIC(CCPData, CMSRowCtrl)
void CCPData::SetHeaders()
{
	if(!IsReady())
		return;

	GetOraLoader().GetSql() = "SELECT CP_CODE, CP_MAIN_NAME, IMG_CP_CODE, CP_LIMIT, "
								"CP_SET_LIMIT, PRU, ISVALID "
								"FROM SEMAM.NW_COUNTER_PARTY WHERE ";
	GetOraLoader().GetSql() += " 1 = 2 ";
	if(GetOraLoader().Open())
	{
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
		GetSS()->SetSheetColHeaders();
	}

	CQData QData;
	GetOraLoader().GetSql().Format("SELECT CT_CODE, CT_NAME, CT_CONTACT, CT_ADDRESS1, CT_ADDRESS2, "
							"CT_PHONE, CT_FAX, CT_CEDEL_NUM, CT_EURO_NUM, CT_MSTC_CODE, CT_FED_ID, "
							"CT_LIMIT, CT_ISIN_NUM, CT_BIC, CT_DTC_NUM, CT_GROUP, CT_SEC_ID, "
							"CT_FSA_ID, CT_INTL, CT_FX, ISVALID "
							"FROM SEMAM.NW_COUNTER_CONTACTS WHERE CP_CODE = %s", 
							QData.GetQueryText("MORGAN STANLEY"));
	if(GetOraLoader().Open())
	{
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
	}
}

int CCPData::LoadDBData()
{
	CDBRec Rec;
	CStringArray QRec;

	if(!IsReady())
		return FALSE;

	ToDBRec(Rec, TRUE);
	if(Rec.GetSize() <= 0)
		return -1;

	GetSRowCtrl().GetSS()->ClearSheet();
	GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), "%");
	GetOraLoader().GetSql() = "SELECT CP_CODE, CP_MAIN_NAME, IMG_CP_CODE, CP_LIMIT, "
								"CP_SET_LIMIT, PRU, ISVALID "
								"FROM SEMAM.NW_COUNTER_PARTY WHERE ";
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), FALSE);
	GetOraLoader().GetSql() += " ORDER BY 1 ";
	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
	return FALSE;
}

BOOL CCPData::AddRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetCPRec(), &GetCTRec());
	if(GetCPRec().GetCPCode().IsEmpty())
		return FALSE;

	if(GetModify(FALSE))
	{
		GetOraLoader().BeginTransaction();
		if(GetCPRec().AddRec(GetOraLoader()))
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

BOOL CCPData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetCPRec(), &GetCTRec());
	if(strlen(GetKey()) == 0)
		return FALSE;
	
	if(GetSS()->GetIsBlockSelected())
	{
		GetOraLoader().BeginTransaction();
		if(GetCPRec().DeleteRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			DeleteSheetRow(GetCurrentRow());
			Refresh();
			return TRUE;
		}
		
		GetOraLoader().Rollback();
		return FALSE;
	}
	else
		return DeleteSRow();
}

BOOL CCPData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetCPRec(), &GetCTRec());

	if(strlen(GetKey()) == 0)
		return FALSE;

	if(GetModify(FALSE))
	{
		GetOraLoader().BeginTransaction();
		if(GetCPRec().UpdateRec(GetOraLoader()))
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

BOOL CCPData::AddSRow(BOOL bDirectly)
{
	if(!IsReady())
		return FALSE;

	if(bDirectly)
		GetSRowCtrl().UpdateData();
	if(!GetSRowCtrl().GetModify(FALSE))
		return TRUE;
	
	GetCTRec().SetCP(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetCTRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		LoadSheet();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CCPData::DeleteSRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	if(GetSRowCtrl().GetSS()->GetIsBlockSelected())
	{
		GetCTRec().SetID(GetSRowCtrl().GetID());
		GetCTRec().SetCP(GetKey());
		GetOraLoader().BeginTransaction();
		if(GetCTRec().DeleteRec(GetOraLoader(), TRUE))
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

BOOL CCPData::UpdateSRow(BOOL bDirectly)
{
	if(!IsReady())
		return FALSE;

	if(bDirectly)
		GetSRowCtrl().UpdateData();
	
	if(!GetSRowCtrl().GetModify(FALSE))
		return TRUE;

	GetCTRec().SetID(GetSRowCtrl().GetID());
	GetCTRec().SetCP(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetCTRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		GetSRowCtrl().UpdateSheetRow(FALSE);
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

void CCPData::LoadSheet()
{
	if(!IsReady())
		return;

	CQData QData;

	SetKey();
	GetOraLoader().GetSql().Format("SELECT ROWIDTOCHAR(ROWID) \"ID\", CT_CODE, CT_NAME, "
						"CT_CONTACT, CT_ADDRESS1, CT_ADDRESS2, CT_PHONE, CT_FAX, CT_CEDEL_NUM, "
						"CT_EURO_NUM, CT_MSTC_CODE, CT_FED_ID, CT_LIMIT, CT_ISIN_NUM, CT_BIC, "
						"CT_DTC_NUM, CT_GROUP, CT_SEC_ID, CT_FSA_ID, CT_INTL, CT_FX, ISVALID "
						"FROM SEMAM.NW_COUNTER_CONTACTS WHERE CP_CODE = %s "
						"ORDER BY TO_NUMBER(CT_CODE)", QData.GetQueryText(GetKey()));

	if(GetOraLoader().Open())
		GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE);
}

