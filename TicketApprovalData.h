#ifndef __TICKETAPPROVALDATA_H__
#define __TICKETAPPROVALDATA_H__

#ifndef __MSROWCTRL_H__
#include "MSROWCTRL.h"
#endif

#ifndef __RAWTICKETREC_H__
#include "rawticketrec.h"
#endif

class CTicketApprovalData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CTicketApprovalData)
protected:
	CRawTicketRec m_Ticket;
	CKeyDBList m_AllocList;
	BOOL m_bTrader;
	BOOL m_bBlotter;
	BOOL m_bSign;

public:
	CTicketApprovalData() { m_bBlotter = FALSE; m_bTrader = TRUE; m_bSign = FALSE; }
	CTicketApprovalData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, BOOL bBlotter, BOOL bTrader)
		: CMSRowCtrl(OraLoader, pSS, pSSS)	{ SetBlotter(bBlotter); SetTrader(bTrader); }
	CTicketApprovalData(CTicketApprovalData &TicketData);
	~CTicketApprovalData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, BOOL m_bBlotter, BOOL m_bTrader, BOOL m_bSign, int KeyIndex);

	CKeyDBList &GetAllocList() { return m_AllocList; }

	void SetTrader(int bTrader) { m_bTrader = bTrader; }
	BOOL IsTrader() { return m_bTrader; }

	void SetBlotter(BOOL bBlotter = TRUE) { m_bBlotter = bBlotter; }
	BOOL GetBlotter() { return m_bBlotter; }

	void SetSign(BOOL bSign = TRUE) { m_bSign = bSign; }
	BOOL GetSign() { return m_bSign; }

	CRawTicketRec &GetTicket() { return m_Ticket; }

	int LoadTickets();
	int LoadAllocList();
	int LoadPositionList(CString Asset, CDBSheet &DBSheet);
	int LoadVARs(CStringArray &Rec, CString Portfolio, CString Category, double VAR);

	BOOL ApprovalTicket(const CString SNote);
	BOOL StraightThoughTrade();

protected:
	void GetCTCode(const CString Counterparty, CString &CTCode);
};
#endif