#ifndef __BLOCKDATA_H__
#define __BLOCKDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __RAWTICKETREC_H__
#include "rawticketrec.h"
#endif

class CBlockData : public CRowCtrl
{
DECLARE_DYNAMIC(CBlockData)
protected:
	COraLoader m_OraLoader;
	BOOL m_bBlotter;
	double m_BrokerFee;
	double m_SoftDollar;
	double m_OtherFee;
	double m_VAR;
	CRawTicketRec m_RawTicket;

protected:
	virtual BOOL DeleteDataRow();
	virtual BOOL UpdateDataRow();

public:
	CBlockData() { }
	CBlockData(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR DefTrader, LPCTSTR User) 
				{ Setup(OraLoader, pSS, DefTrader, User); }
	~CBlockData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR Trader, LPCTSTR User);

	COraLoader &GetOraLoader() { return m_OraLoader; }
	CRawTicketRec &GetRawTicket() { return m_RawTicket; }
	
	void SetBlotter(BOOL bBlotter = TRUE) { m_bBlotter = bBlotter; }
	BOOL GetBlotter() { return m_bBlotter; }

	double GetBrokerFee() { return m_BrokerFee; }
	double GetSoftDollar() { return m_SoftDollar; }
	double GetOtherFee() { return m_OtherFee; }
	double GetVAR() { return m_VAR; }
	BOOL GetMargin(LPCTSTR Asset);

	virtual int LoadTickets();
	int LoadVARs(CStringArray &Rec, CString Portfolio, CString Category, double VAR);

	virtual BOOL DeleteRow(int CurrentRow = -1);
	virtual BOOL UpdateRow(BOOL bFromSheet = TRUE);
};
#endif