#ifndef __CONFIRMDATA_H__
#define __CONFIRMDATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __ASSETVAL_H__
#include "assetval.h"
#endif

#ifndef __TICKETREC_H__
#include "ticketrec.h"
#endif

class CConfirmData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CConfirmData)
protected:
	CTicketRec m_Ticket;
	CInvRec m_Inv;
	CString m_RecCurrency;
	CString m_InvNum;
	CString m_Formula;
	BOOL m_bFindExact;
	double m_Accretion;
	double m_Cash;
	double m_PrePaid;
	BOOL m_bExact;

protected:
	void SetupAssetInfo(CAssetVal &Val);
	virtual BOOL AddSRow();
	virtual BOOL DeleteSRow(int CurrentRow = -1);
	virtual BOOL UpdateSRow();
	BOOL GetSelInvRec(int Index, CInvRec &InvRec);

public:
	CConfirmData() { }
	CConfirmData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User) 
				{ Setup(OraLoader, pSS, pSSS, User); }
	~CConfirmData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User, int KeyIndex = 4);

	void SetHeaders();
	
	void SetAccretion(double Accretion) { m_Accretion = Accretion; }
	void SetCash(double Cash) { m_Cash = Cash; }
	void SetPrePaid(double PrePaid) { m_PrePaid = PrePaid; }
	void SetFindExact(BOOL bFindExact) { m_bFindExact = bFindExact; }
	double GetAccretion() { return m_Accretion; }
	double GetCash() { return m_Cash; }
	double GetPrePaid() { return m_PrePaid; }
	LPCTSTR GetInvNum() { return (LPCTSTR ) m_InvNum; }
	void SetInvNum(LPCTSTR InvNum) { m_InvNum = InvNum; }
	CString &GetFormula() { return m_Formula; }
	LPCTSTR GetRecCurrency() { return (LPCTSTR ) m_RecCurrency; }

	BOOL IsReady();
	BOOL IsExact() { return m_bExact; }
	void SetExact(BOOL bExact) { m_bExact = bExact; }
	
	CTicketRec &GetTicket() { return m_Ticket; }
	CInvRec &GetInv() { return m_Inv; }

	virtual void LoadSheet();
	virtual int LoadTickets();

	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();

	virtual void Compute();
};
#endif