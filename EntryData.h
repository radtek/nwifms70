#ifndef __ENTRYDATA_H__
#define __ENTRYDATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __RAWTICKETREC_H__
#include "rawticketrec.h"
#endif

#ifndef __RISKREC_H__
#include "RISKREC.h"
#endif

class CEntryData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CEntryData)
protected:
	BOOL m_bBlotter;
	BOOL m_bBlock;
	BOOL m_bFxEntry;
	double m_AllocAmount;
	double m_BrokerFee;
	double m_SoftDollar;
	double m_OtherFee;
	double m_VAR;
	CRawTicketRec m_RawTicket;
	CRawInvRec m_RawInv;
	CKeyDBList m_AllocList;

protected:
	virtual BOOL DeleteDataRow();
	virtual BOOL UpdateDataRow();
	void AddAllocListItem();
	void DeleteAllocListItem();
	virtual BOOL GetSelRawInv(int Index, CRawInvRec &RecInvRec);

public:
	CEntryData() { GetFXEntry() = FALSE; }
	CEntryData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, 
				LPCTSTR DefTrader, LPCTSTR User) 
				{ Setup(OraLoader, pSS, pSSS, DefTrader, User); }
	~CEntryData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR Trader, LPCTSTR User, int KeyIndex = -1);

	CRawTicketRec &GetRawTicket() { return m_RawTicket; }
	CRawInvRec &GetRawInv() { return m_RawInv; }
	
/*	double ComputeAllocAmount(CString Portfolio = NULL, double CurrentAmount = 0, 
				double BrokerFee = 0, double CurrentSoftDollar = 0, 
				double CurrentOtherFee = 0, double CurrentVAR = 0); */
	double ComputeAllocAmount(CString Portfolio = NULL, double CurrentAmount = 0);
	int AllocCheckSum(CString Portfolio = NULL, double CurrentAmount = 0);
	BOOL FindPortfolio(LPCTSTR Portfolio);
	const CString GetAutoAllocationSql(const CString AAMethod, const CString NomAmount, const CString BrokerFee, 
						const CString SoftDollar, const CString OtherFee, const CString VAR, const CString Ticket);
	const CString GetAutoAllocationSql(const CString AAMethod, const CString NomAmount, const CString Ticket);

	CKeyDBList &GetAllocList() { return m_AllocList; }
	
	void SetBlotter(BOOL bBlotter = TRUE) { m_bBlotter = bBlotter; }
	BOOL GetBlotter() { return m_bBlotter; }

	void SetBlock(BOOL bBlock = TRUE) { m_bBlock = bBlock; }
	BOOL GetBlock() { return m_bBlock; }

	double GetAllocAmount() { return m_AllocAmount; }

	BOOL &GetFXEntry() { return m_bFxEntry; }
	double GetBrokerFee() { return m_BrokerFee; }
	double GetSoftDollar() { return m_SoftDollar; }
	double GetOtherFee() { return m_OtherFee; }
	double GetVAR() { return m_VAR; }
	int GetUnSignTicketCount();
	BOOL GetMargin(LPCTSTR Asset);

	virtual int LoadAllocList();
	virtual int LoadTickets();
	int LoadVARs(CStringArray &Rec, CString Portfolio, CString Category, double VAR);

	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif