#ifndef __IDEAREC_H__
#define __IDEAREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CIdeaRec : public CRecObj
{
DECLARE_DYNAMIC(CIdeaRec)
protected:
	CString m_IdeaID;
	CString m_Date;
	CString m_Action;
	CString m_Aim;
	CString m_Dir;
	CString m_Currency;
	CString m_Counterparty;
	CString m_Capital;
	CString m_Risk;
	CString m_Reward;
	CString m_Idea;
	CString m_Trader;
	CString m_RiskNote;
	CString m_Decision;
	CString m_ApprovedAmt;
	CString m_ManagerNote;
	
public:	
	CIdeaRec(CIdeaRec &IdeaRec) { Copy(IdeaRec); }
	CIdeaRec() { }

	void SetIdeaID(LPCTSTR IdeaID) { m_IdeaID = IdeaID; }
	CString &GetIdeaID() { return m_IdeaID; }

	void SetDate(LPCTSTR Date) { m_Date = Date; }
	CString &GetDate() { return m_Date; }

	void SetAction(LPCTSTR Action) { m_Action = Action; }
	CString &GetAction() { return m_Action; }

	void SetAim(LPCTSTR Aim) { m_Aim = Aim; }
	CString &GetAim() { return m_Aim; }

	void SetDir(LPCTSTR Dir) { m_Dir = Dir; }
	CString &GetDir() { return m_Dir; }

	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	CString &GetCurrency() { return m_Currency; }

	void SetCounterparty(LPCTSTR Counterparty) { m_Counterparty = Counterparty; }
	CString &GetCounterparty() { return m_Counterparty; }

	void SetCapital(LPCTSTR Capital) { m_Capital = Capital; }
	CString &GetCapital() { return m_Capital; }

	void SetRisk(LPCTSTR Risk) { m_Risk = Risk; }
	CString &GetRisk() { return m_Risk; }

	void SetReward(LPCTSTR Reward) { m_Reward = Reward; }
	CString &GetReward() { return m_Reward; }

	void SetIdea(LPCTSTR Idea) { m_Idea = Idea; }
	CString &GetIdea() { return m_Idea; }

	void SetTrader(LPCTSTR Trader) { m_Trader = Trader; }
	CString &GetTrader() { return m_Trader; }

	void SetRiskNote(LPCTSTR RiskNote) { m_RiskNote = RiskNote; }
	CString &GetRiskNote() { return m_RiskNote; }

	void SetDecision(LPCTSTR Decision) { m_Decision = Decision; }
	CString &GetDecision() { return m_Decision; }

	void SetApprovedAmt(LPCTSTR ApprovedAmt) { m_ApprovedAmt = ApprovedAmt; }
	CString &GetApprovedAmt() { return m_ApprovedAmt; }

	void SetManagerNote(LPCTSTR ManagerNote) { m_ManagerNote = ManagerNote; }
	CString &GetManagerNote() { return m_ManagerNote; }

	virtual void Copy(CIdeaRec &IdeaRec);
	const CIdeaRec &operator = (CIdeaRec &IdeaRec) { Copy(IdeaRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader);
	virtual BOOL DeleteRec(COraLoader &OraLoader);

	BOOL RiskApprove(COraLoader &OraLoader, const CString IdeaId, const CString RiskNote);

	BOOL Approve(COraLoader &OraLoader, const CString IdeaId, const CString Decision, 
				const CString ApprovedCapital, const CString Note);
};
#endif
