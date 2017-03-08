#ifndef __PORTFOLIODATA_H__
#define __PORTFOLIODATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CPortfolioRec : public CRecObj
{
DECLARE_DYNAMIC(CPortfolioRec)
protected:
	CString m_Portfolio;
	CString m_FullName;
	CString m_Amount;
	CString m_CtrlFactor;
	CString	m_WtFactor;
	CString m_TableSpace;
	CString m_Custodian;
	CString m_Account;
	CString m_Currency;
	CString m_Accountant;
	CString m_Status;

public:
	CPortfolioRec() { }
	CPortfolioRec(CPortfolioRec &PortfolioRec) { Copy(PortfolioRec); }
	~CPortfolioRec() {}

	CString &GetPortfolio() { return m_Portfolio; }
    CString &GetFullName() { return m_FullName; }
	CString &GetAmount() { return m_Amount; }
	CString &GetCtrlFactor() { return m_CtrlFactor; }
	CString &GetWtFactor() { return m_WtFactor; }
	CString &GetTableSpace() { return m_TableSpace; }
	CString &GetCustodian() { return m_Custodian; }
	CString &GetAccount() { return m_Account; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetAccountant() { return m_Accountant; }
	CString &GetStatus() { return m_Status; }

	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetFullName(LPCTSTR FullName) { m_FullName = FullName; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; }
	void SetCtrlFactor(LPCTSTR CtrlFactor) { m_CtrlFactor = CtrlFactor; }
	void SetWtFactor(LPCTSTR WtFactor) { m_WtFactor = WtFactor; }
	void SetTableSpace(LPCTSTR TableSpace) { m_TableSpace = TableSpace; }
	void SetCustodian(LPCTSTR Custodian) { m_Custodian = Custodian; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetAccountant(LPCTSTR Accountant) { m_Accountant = Accountant; }
	void SetStatus(LPCTSTR Status) { m_Status = Status; }

	virtual void Copy(CPortfolioRec &PortfolioRec);
	const CPortfolioRec &operator = (CPortfolioRec &PortfolioRec) { Copy(PortfolioRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};


class CPortfolioData : public CRowCtrl
{
DECLARE_DYNAMIC(CPortfolioData)
protected:
	CPortfolioRec m_Rec;
	COraLoader m_OraLoader;

public:
	CPortfolioData() { }
	CPortfolioData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	CPortfolioData(CPortfolioData &PortfolioData) : CRowCtrl(PortfolioData) 
				{ SetOraLoader(PortfolioData.GetOraLoader()); }
	~CPortfolioData() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { SetOraLoader(OraLoader), SetSS(pSS); }
	CPortfolioRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};

#endif