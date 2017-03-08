#ifndef __ACCOUNTDATA_H__
#define __ACCOUNTDATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CAccountRec : public CRecObj
{
DECLARE_DYNAMIC(CAccountRec)
protected:
	CString m_CP;
	CString m_Account;
	CString m_Desc;
	CString m_Group;
	CString m_Collateral;
	CString m_Future;
	CString m_Clearing;
	CString m_Active;
	
public:
	CAccountRec() {}
	CAccountRec(CAccountRec &CAccountRec) { Copy(CAccountRec); }
	~CAccountRec() { }

	CString &GetCP() { return m_CP; }
	CString &GetAccount() { return m_Account; }
	CString &GetDesc() { return m_Desc; }
	CString &GetGroup() { return m_Group; }
	CString &GetCollateral() { return m_Collateral; }
	CString &GetFuture() { return m_Future; }
	CString &GetClearing() { return m_Clearing; }
	CString &GetActive() { return m_Active; }

	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetDesc(LPCTSTR Desc) { m_Desc = Desc; }
	void SetGroup(LPCTSTR Group) { m_Group = Group; }
	void SetCollateral(LPCTSTR Collateral) { m_Collateral = Collateral; }
	void SetFuture(LPCTSTR Future) { m_Future = Future; }
	void SetClearing(LPCTSTR Clearing) { m_Clearing = Clearing; }
	void SetActive(LPCTSTR Active) { m_Active = Active; }

	virtual void Copy(CAccountRec &CAccountRec);
	const CAccountRec &operator = (CAccountRec &AccountRec) { Copy(AccountRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = TRUE);
};

class CAccountData : public CRowCtrl
{
DECLARE_DYNAMIC(CAccountData)
protected:
	CAccountRec m_Rec;
	COraLoader m_OraLoader;

public:
	CAccountData() { }
	CAccountData(COraLoader &OraLoader, CDBSheet *pSS) : CRowCtrl(pSS) { SetOraLoader(OraLoader); }
	CAccountData(CAccountData &Data) : CRowCtrl(Data) { SetOraLoader(Data.GetOraLoader()); }
	~CAccountData() { }

	CAccountRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif