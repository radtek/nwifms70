#ifndef __PORTFOLIOACCOUNTDATA_H__
#define __PORTFOLIOACCOUNTDATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CPortfolioAccountRec : public CRecObj
{
DECLARE_DYNAMIC(CPortfolioAccountRec)
protected:
	CString m_Portfolio;
	CString m_Custodian;
	CString m_CustodianAccount;
	CString m_AccountNum;
	CString m_Account;
	CString	m_Account2;
	CString m_Bank;
	CString m_EuroClear;
	CString m_CustodianCode;
	CString m_Margin;
	CString m_Contact;
	CString m_Phone;
	CString m_Fax;

public:
	CPortfolioAccountRec() { }
	CPortfolioAccountRec(CPortfolioAccountRec &PortfolioAccountRec) { Copy(PortfolioAccountRec); }
	~CPortfolioAccountRec() {}

	CString &GetPortfolio() { return m_Portfolio; }
    CString &GetCustodian() { return m_Custodian; }
	CString &GetCustodianAccount() { return m_CustodianAccount; }
	CString &GetAccountNum() { return m_AccountNum; }
	CString &GetAccount() { return m_Account; }
	CString &GetAccount2() { return m_Account2; }
	CString &GetBank() { return m_Bank; }
	CString &GetEuroClear() { return m_EuroClear; }
	CString &GetCustodianCode() { return m_CustodianCode; }
	CString &GetMargin() { return m_Margin; }
	CString &GetContact() { return m_Contact; }
	CString &GetPhone() { return m_Phone; }
	CString &GetFax() { return m_Fax; }

	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetCustodian(LPCTSTR Custodian) { m_Custodian = Custodian; }
	void SetCustodianAccount(LPCTSTR CustodianAccount) { m_CustodianAccount = CustodianAccount; }
	void SetAccountNum(LPCTSTR AccountNum) { m_AccountNum; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetAccount2(LPCTSTR Account2) { m_Account2 = Account2; }
	void SetBank(LPCTSTR Bank) { m_Bank = Bank; }
	void SetEuroClear(LPCTSTR EuroClear) { m_EuroClear = EuroClear; }
	void SetCustodianCode(LPCTSTR CustodianCode) { m_CustodianCode = CustodianCode; }
	void SetMargin(LPCTSTR Margin) { m_Margin = Margin; }
	void SetContact(LPCTSTR Contact) { m_Contact = Contact; }
	void SetPhone(LPCTSTR Phone) { m_Phone = Phone; }
	void SetFax(LPCTSTR Fax) { m_Fax = Fax; }

	virtual void Copy(CPortfolioAccountRec &PortfolioAccountRec);
	const CPortfolioAccountRec &operator = (CPortfolioAccountRec &PortfolioAccountRec) { Copy(PortfolioAccountRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};


class CPortfolioAccountData : public CRowCtrl
{
DECLARE_DYNAMIC(CPortfolioAccountData)
protected:
	CPortfolioAccountRec m_Rec;
	COraLoader m_OraLoader;

public:
	CPortfolioAccountData() { }
	CPortfolioAccountData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	CPortfolioAccountData(CPortfolioAccountData &PortfolioAccountData) : CRowCtrl(PortfolioAccountData) 
				{ SetOraLoader(PortfolioAccountData.GetOraLoader()); }
	~CPortfolioAccountData() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { SetOraLoader(OraLoader), SetSS(pSS); }
	CPortfolioAccountRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif