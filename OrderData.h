#ifndef __ORDERDATA_H__
#define __ORDERDATA_H__

#ifndef __ROWCTRL_H__
#include "ROWCTRL.h"
#endif

#ifndef __ORDERREC_H__
#include "orderrec.h"
#endif

#ifndef __ORLOADER_H__
#include "oraloader.h"
#endif

class COrderData : public CRowCtrl
{
DECLARE_DYNAMIC(COrderData)
protected:
	COrderRec m_OrderRec;
	COraLoader m_OraLoader;
	CString m_Trader;

public:
	COrderData() { }
	COrderData(COraLoader &OraLoader, CDBSheet *pSS, const CString Trader) { Setup(OraLoader, pSS, Trader); }
	COrderData(COrderData &Data) { Setup(Data.GetOraLoader(), Data.GetSS(), Data.GetTrader()); }
	~COrderData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, const CString Trader);

	COrderRec &GetOrderRec() { return m_OrderRec; }
	
	COraLoader &GetOraLoader() { return m_OraLoader; }
	
	CString &GetTrader() { return m_Trader; }
	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL UpdateRow();
};
#endif