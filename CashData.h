#ifndef __CASHDATA_H__
#define __CASHDATA_H__

#ifndef __CASHREC_H__
#include "cashrec.h"
#endif

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif


class CCashData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CCashData)
protected:
	CCashRec m_Rec;
	CCashInvRec m_InvRec;
	BOOL m_bInit;
	BOOL m_bCoupon;
	BOOL m_bLoaded;
	BOOL m_bFindExact;
	int m_nCashStatus;
	CString m_TMinus1Date;
	CRowCtrl m_SubData;

protected:
	virtual BOOL AddSRow();
	virtual BOOL UpdateSRow();
	BOOL AddSRows();
	BOOL DeleteSRows();
	BOOL UpdateInvData();

public:
	CCashData() { }
	CCashData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex) { Setup(OraLoader, pSS, pSSS, KeyIndex); }
	CCashData(CCashData &Data) : CMSRowCtrl(Data) { }
	~CCashData() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex);
	CCashRec &GetRec() { return m_Rec; }
	CCashInvRec &GetInvRec() { return m_InvRec; }

	CRowCtrl &GetCashSubData() { return m_SubData; }
	void SetCashStatus(int CashStatus) { m_nCashStatus = CashStatus; } // 0. nothing 1. OpenTrade 2. PaymentInstruction
	int GetCashStatus() { return m_nCashStatus; }
	void SetFindExact(BOOL bFindExact) { m_bFindExact = bFindExact; }

	BOOL LoadOpenTrade(CString &Portfolio, CString &Account, CString &Currency, CString &PostDate, CString &Asset);
	BOOL LoadCash(CString &Portfolio, CString &Account, CString &Currency, CString &PostDate);
	BOOL LoadCashDetail();

	BOOL GetInit() { return m_bInit; }
	void SetInit(BOOL bInit) { m_bInit = bInit; }
	BOOL FindExact() { return m_bFindExact; }

	BOOL IsCoupon() { return m_bCoupon; }
	void SetCoupon(BOOL bCoupon) { m_bCoupon = bCoupon; }

	BOOL IsLoaded() { return m_bLoaded; }
	void SetLoaded(BOOL bLoaded) { m_bLoaded = bLoaded; }

	virtual void LoadSheet();
	virtual int LoadDBData();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
	virtual BOOL DeleteSRow(int CurrentRow = -1);
};
#endif