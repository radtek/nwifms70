#ifndef __SHORTCASHDATA_H__
#define __SHORTCASHDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class CShortCashData : public CRowCtrl
{
DECLARE_DYNAMIC(CShortCashData)
protected:
	COraLoader m_OraLoader;
	CString m_Date;
	CString m_Portfolio;
	CString m_Account;
	CString m_Currency;
	CString m_Amount;		

public:
	CShortCashData() {}
	CShortCashData(COraLoader &OraLoader, CDBSheet *pSS) 
		: CRowCtrl(pSS) { SetOraLoader(OraLoader); }
	~CShortCashData() {}
	
	BOOL IsRowOK();
	
	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }
	void Setup(COraLoader &OraLoader, CDBSheet *pSS) 
			{ CRowCtrl::Setup(pSS), SetOraLoader(OraLoader); }	
	
	CString &GetDate() { return m_Date; }
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetAccount() { return m_Account; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetAmount() { return m_Amount; }

	int LoadDBData();
	int SaveData(BOOL bAdd = FALSE);
	int DeleteData();
};
#endif // SHORTCASHDATA_H