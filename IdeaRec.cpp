#include "stdafx.h"
#include "resource.h"
#include "idearec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CIdeaRec, CRecObj)
void CIdeaRec::Copy(CIdeaRec &IdeaRec)
{
	SetIdeaID(IdeaRec.GetIdeaID());
	SetDate(IdeaRec.GetDate());
	SetAction(IdeaRec.GetAction());
	SetAim(IdeaRec.GetAim());
	SetDir(IdeaRec.GetDir());
	SetCurrency(IdeaRec.GetCurrency());
	SetCounterparty(IdeaRec.GetCounterparty());
	SetCapital(IdeaRec.GetCapital());
	SetRisk(IdeaRec.GetRisk());
	SetReward(IdeaRec.GetReward());
	SetIdea(IdeaRec.GetIdea());
	SetTrader(IdeaRec.GetTrader());
	SetRiskNote(IdeaRec.GetRiskNote());
	SetDecision(IdeaRec.GetDecision());
	SetApprovedAmt(IdeaRec.GetApprovedAmt());
	SetManagerNote(IdeaRec.GetManagerNote());
}

void CIdeaRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	Rec.Add(GetIdeaID());
	Rec.Add(GetDate());
	Rec.Add(GetAction());
	Rec.Add(GetAim());
	Rec.Add(GetDir());
	Rec.Add(GetCurrency());
	Rec.Add(GetCounterparty());
	Rec.Add(GetCapital());
	Rec.Add(GetRisk());
	Rec.Add(GetReward());
	Rec.Add(GetIdea());
	Rec.Add(GetTrader());
	Rec.Add(GetRiskNote());
	Rec.Add(GetDecision());
	Rec.Add(GetApprovedAmt());
	Rec.Add(GetManagerNote());
}

BOOL CIdeaRec::AddRec(COraLoader &OraLoader)
{
	if(GetIdeaID().IsEmpty())
		return FALSE;
	
	if(!OraLoader.Open("SELECT IDEA_ID, INDATE, TIMETOACT, AIM, DIR, CURRENCY, "
						"COUNTERPARTY, RISK, REWARD, CAPITAL, REQUEST, TRADER_INI, "
						"RISK_NOTE, DECISION, APPROVED_CAPITAL, NOTE "
						"FROM SEMAM.NW_TRADE_IDEAS ", ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CIdeaRec::UpdateRec(COraLoader &OraLoader)
{
	if(GetIdeaID().IsEmpty())
		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT IDEA_ID, INDATE, TIMETOACT, AIM, DIR, CURRENCY, "
								"COUNTERPARTY, RISK, REWARD, CAPITAL, REQUEST, TRADER_INI, "
								"RISK_NOTE, DECISION, APPROVED_CAPITAL, NOTE "
								"FROM SEMAM.NW_TRADE_IDEAS "
								"FROM SEMAM.NW_TRADE_IDEAS WHERE IDEA_ID = %s ", 
								QData.GetQueryNumber(GetIdeaID()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CIdeaRec::DeleteRec(COraLoader &OraLoader)
{
	if(GetIdeaID().IsEmpty())
		return FALSE;

	CQData QData;

	OraLoader.GetSql().Format("DELETE SEMAM.NW_TRADE_IDEAS WHERE IDEA_ID = %s ", 
								QData.GetQueryNumber(GetIdeaID()));
	return OraLoader.ExecuteSql();
}

BOOL CIdeaRec::RiskApprove(COraLoader &OraLoader, const CString IdeaId, 
						   const CString RiskNote)
{
	CQData QData;
	CString strID, strNote;

	strID = QData.GetQueryNumber(IdeaId);
	strNote = QData.GetQueryText(RiskNote);

	return OraLoader.ExecuteSql("UPDATE SEMAM.NW_TRADE_IDEAS A SET RISK_NOTE = " + 
								strNote + "WHERE IDEA_ID = " + strID);
}

BOOL CIdeaRec::Approve(COraLoader &OraLoader, const CString IdeaId, const CString Decision, 
					   const CString ApprovedCapital, const CString Note)
{
	CQData QData;
	CString strID, strDecision, strAppCapital, strNote;

	strDecision = QData.GetQueryText(Decision);
	strAppCapital = QData.GetQueryNumber(ApprovedCapital);
	strNote = QData.GetQueryText(Note);
	strID = QData.GetQueryNumber(IdeaId);
	
	return OraLoader.ExecuteSql("UPDATE SEMAM.NW_TRADE_IDEAS A SET (MANAGER_OPINION, "
								"APPROVED_CAPITAL, NOTE) = (" + strDecision + ", " + 
								strAppCapital + ", " + strNote + ") "
								"WHERE IDEA_ID = " + strID);
}
