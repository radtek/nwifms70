#ifndef __ORDERREC_H__
#define __ORDERREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif


class COrderRec : public CRecObj
{
DECLARE_DYNAMIC(COrderRec)
protected:
	double m_AvailAmount;

	CString m_OrderID;
	CString m_OrderDate;
	CString m_TransType;
	CString m_Dir;
	CString m_Asset;
	CString m_Strike;
	CString m_Rate;
	CString m_Maturity;
	CString m_Currency;
	CString m_Fxrate;
	CString m_NomAmount;
	CString m_Price;
	CString m_CP;
	CString m_RefTicket;
	CString m_Trader;
	CString m_AAMethod;
	CString m_BestExecution;
//	CString m_Reason;
	CString m_Note;
	CString m_TRS;
	CString m_IPO;
	CString m_MarketOrder;
	CString m_SameDay;
	CString m_PartialFill;
	CString m_Discretionary;
	CString m_Expired;
	CString m_Cancelled;

public:
	COrderRec() { }
	COrderRec(COrderRec &OrderRec) { Copy(OrderRec); }
	~COrderRec() {}

	CString &GetOrderID() { return m_OrderID; }
	CString &GetOrderDate() { return m_OrderDate; }
	CString &GetTransType() { return m_TransType; }
	CString &GetDir() { return m_Dir; }
	CString &GetAsset() { return m_Asset; }
	CString &GetStrike() { return m_Strike; }
	CString &GetRate() { return m_Rate; }
	CString &GetMaturity() { return m_Maturity; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetFxrate() { return m_Fxrate; }
	CString &GetNomAmount() { return m_NomAmount; }
	CString &GetPrice() { return m_Price; }
	CString &GetCP() { return m_CP; }
	CString &GetRefTicket() { return m_RefTicket; }
	CString &GetTrader() { return m_Trader; }
	CString &GetAAMethod() { return m_AAMethod; }
	CString &GetBestExecution() { return m_BestExecution; }
//	CString &GetReason() { return m_Reason; }
	CString &GetNote() { return m_Note; }
	CString &GetTRS() { return m_TRS; }
	CString &GetIPO() { return m_IPO; }
	CString &GetMarketOrder() { return m_MarketOrder; }
	CString &GetSameDay() { return m_SameDay; }
	CString &GetPartialFill() { return m_PartialFill; }
	CString &GetDiscretionary() { return m_Discretionary; }
	CString &GetExpired() { return m_Expired; }
	CString &GetCancelled() { return m_Cancelled; }

	virtual void Copy(COrderRec &OrderRec);
	const COrderRec &operator = (COrderRec &OrderRec) { Copy(OrderRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader);
	virtual BOOL DeleteRec(COraLoader &OraLoader);	
};
#endif
