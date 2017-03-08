#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "ideadata.h"
#include "orafield.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CIdeaData, CRowCtrl)
CIdeaData::CIdeaData(CIdeaData &IdeaData) 
{ 
	Setup(IdeaData.GetOraLoader(), IdeaData.GetSS()); 
}

void CIdeaData::Setup(COraLoader &OraLoader, CDBSheet *pSS)
{
	SetOraLoader(OraLoader);
	CRowCtrl::Setup(pSS);
}

int CIdeaData::FindRecords()
{
	CDBRec Rec;
	CString Sql;
	CStringArray QRec;

	UpdateData(TRUE);
	
	ToDBRec(Rec, TRUE);
	Rec.RemoveAt(Rec.GetSize() - 1); // Remove Class
	if(Rec.GetSize() <= 0)
		return -1;

	Sql = "SELECT IDEA_ID, INDATE, TIMETOACT, DIR, CURRENCY, AIM, "
			"COUNTERPARTY, TRADER_INI, CAPITAL, RISK, REWARD, REQUEST, "
			"RISK_NOTE, APROVED_CAPITAL, NOTE, DECISION FROM SEMAM.NW_TRADE_IDEAS ";

	GetSS()->ClearSheet();
	if(GetSS()->GetFieldArray().GetSize() <= 0)
	{
		GetOraLoader().GetSql() = Sql;
		GetOraLoader().GetSql() += " WHERE 1 = 2";
		GetOraLoader().Open();
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
	}

	GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), PERCENT);

	GetOraLoader().GetSql() = Sql;
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), TRUE);

	GetOraLoader().GetSql() += " ORDER BY 2,3,1 ";
	
	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);

	return -1;

}

int CIdeaData::LoadIdeaRecords(int nUserType, BOOL bNew)
{
	switch(nUserType)
	{
		case 1: // User is trader
			GetOraLoader().GetSql() = "SELECT IDEA_ID, INDATE, TIMETOACT, DIR, "
									"CURRENCY, AIM, COUNTERPARTY, TRADER_INI, CAPITAL, "
									"RISK, REWARD, REQUEST FROM SEMAM.NW_TRADE_IDEAS "
									"WHERE INDATE = TRUNC(SYSDATE) ORDER BY 2, 3, 1";
			break;
		case 2: // User is Risk Manager
			if(bNew)
				GetOraLoader().GetSql() = "SELECT IDEA_ID, INDATE, TIMETOACT, DIR, "
									"CURRENCY, AIM, COUNTERPARTY, TRADER_INI, CAPITAL, "
									"RISK, REWARD, REQUEST, RISK_NOTE FROM SEMAM.NW_TRADE_IDEAS "
									"WHERE RISK_NOTE IS NULL ORDER BY 2, 3, 1";
			else
				GetOraLoader().GetSql() = "SELECT IDEA_ID, INDATE, TIMETOACT, DIR, "
									"CURRENCY, AIM, COUNTERPARTY, TRADER_INI, CAPITAL, "
									"RISK, REWARD, REQUEST, RISK_NOTE FROM SEMAM.NW_TRADE_IDEAS "
									"WHERE INDATE = TRUNC(SYSDATE) ORDER BY 2, 3, 1";
			break;
		case 3: // User is CIO
			if(bNew)
				GetOraLoader().GetSql() = "SELECT IDEA_ID, INDATE, TIMETOACT, DIR, "
										"CURRENCY, AIM, COUNTERPARTY, TRADER_INI, CAPITAL, "
										"RISK, REWARD, REQUEST, RISK_NOTE, APROVED_CAPITAL, "
										"NOTE, DECISION FROM SEMAM.NW_TRADE_IDEAS "
										"WHERE MANAGER IS NULL ORDER BY 2, 3, 1 ";
			else
				GetOraLoader().GetSql() = "SELECT IDEA_ID, INDATE, TIMETOACT, DIR, "
										"CURRENCY, AIM, COUNTERPARTY, TRADER_INI, CAPITAL, "
										"RISK, REWARD, REQUEST, RISK_NOTE, APROVED_CAPITAL, "
										"NOTE, DECISION FROM SEMAM.NW_TRADE_IDEAS "
										"WHERE INDATE = TRUNC(SYSDATE) ORDER BY 2, 3, 1 ";
		default:
			return -1;
	}

	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this);

	return -1;
}

BOOL CIdeaData::AddRow()
{
	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetIdeaRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		AddSheetRow();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CIdeaData::DeleteRow()
{
	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetIdeaRec().DeleteRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		DeleteSheetRow(GetCurrentRow());
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CIdeaData::UpdateRow()
{
	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetIdeaRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		UpdateSheetRow(FALSE);
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CIdeaData::RiskApprove(const CString ID, const CString Note)
{
	GetOraLoader().BeginTransaction();

	if(GetIdeaRec().RiskApprove(GetOraLoader(), ID, Note))
	{
		GetOraLoader().Commit();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CIdeaData::ApproveIdea(const CString ID, const CString Decision, 
							const CString Capital, const CString Note)
{
	GetOraLoader().BeginTransaction();

	if(GetIdeaRec().Approve(GetOraLoader(), ID, Decision, Capital, Note))
	{
		GetOraLoader().Commit();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}
