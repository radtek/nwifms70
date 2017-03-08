#ifndef __PORTFOLIOINFODATA_H__
#define __PORTFOLIOINFODATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CPortfolioInfoRec : public CRecObj
{
DECLARE_DYNAMIC(CPortfolioInfoRec)
protected:
	CString m_Portfolio;
	CString m_Custodian;
	CString m_CustodianAccount;
	CString m_Account;
	CString	m_Account2;
	CString m_Bank;
	CString m_EuroClear;
	CString m_Margin;
	CString m_Contact;
	CString m_Phone;
	CString m_Fax;

public:
	CPortfolioInfoRec() { }
	CPortfolioInfoRec(CPortfolioInfoRec &PortfolioInfoRec) { Copy(PortfolioInfoRec); }
	~CPortfolioInfoRec() {}

	CString &GetPortfolio() { return m_Portfolio; }
    CString &GetCustodian() { return m_Custodian; }
	CString &GetCustodianAccount() { return m_CustodianAccount; }
	CString &GetAccount() { return m_Account; }
	CString &GetAccount2() { return m_Account2; }
	CString &GetBank() { return m_Bank; }
	CString &GetEuroClear() { return m_EuroClear; }
	CString &GetMargin() { return m_Margin; }
	CString &GetContact() { return m_Contact; }
	CString &GetPhone() { return m_Phone; }
	CString &GetFax() { return m_Fax; }

	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetCustodian(LPCTSTR Custodian) { m_Custodian = Custodian; }
	void SetCustodianAccount(LPCTSTR CustodianAccount) { m_CustodianAccount = CustodianAccount; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetAccount2(LPCTSTR Account2) { m_Account2 = Account2; }
	void SetBank(LPCTSTR Bank) { m_Bank = Bank; }
	void SetEuroClear(LPCTSTR EuroClear) { m_EuroClear = EuroClear; }
	void SetMargin(LPCTSTR Margin) { m_Margin = Margin; }
	void SetContact(LPCTSTR Contact) { m_Contact = Contact; }
	void SetPhone(LPCTSTR Phone) { m_Phone = Phone; }
	void SetFax(LPCTSTR Fax) { m_Fax = Fax; }

	virtual void Copy(CPortfolioInfoRec &PortfolioInfoRec);
	const CPortfolioInfoRec &operator = (CPortfolioInfoRec &PortfolioInfoRec) { Copy(PortfolioInfoRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};


class CPortfolioInfoData : public CRowCtrl
{
DECLARE_DYNAMIC(CPortfolioInfoData)
protected:
	CPortfolioInfoRec m_Rec;
	COraLoader m_OraLoader;

public:
	CPortfolioInfoData() { }
	CPortfolioInfoData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	CPortfolioInfoData(CPortfolioInfoData &PortfolioInfoData) : CRowCtrl(PortfolioInfoData) 
				{ SetOraLoader(PortfolioInfoData.GetOraLoader()); }
	~CPortfolioInfoData() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { SetOraLoader(OraLoader), SetSS(pSS); }
	CPortfolioInfoRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif