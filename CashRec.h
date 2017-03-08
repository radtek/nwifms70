#ifndef __CASHREC_H__
#define __CASHREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CCashInvRec : public CRecObj
{
DECLARE_DYNAMIC(CCashInvRec)
protected:
	CString m_CashNum;
	CString m_TransNum;
	CString m_Amount;
	
	CString m_CloseDate;
	CString m_PostDate;

public:
	CCashInvRec(CCashInvRec &CashInvRec) { Copy(CashInvRec); }
	CCashInvRec(LPCTSTR ID = NULL) { SetID(ID); }
	
	void SetCashNum(LPCTSTR CashNum) { m_CashNum = CashNum; }
	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; }
	void SetCloseDate(LPCTSTR CloseDate) { m_CloseDate = CloseDate; }
	void SetPostDate(LPCTSTR PostDate) { m_PostDate = PostDate; }
	
	CString &GetCashNum() { return m_CashNum; }
	CString &GetTransNum() { return m_TransNum; }
	CString &GetAmount() { return m_Amount; }
	CString &GetCloseDate() { return m_CloseDate; }
	CString &GetPostDate() { return m_PostDate; }

	virtual void Copy(CCashInvRec &CashInvRec);
	const CCashInvRec &operator = (CCashInvRec &CashInvRec) { Copy(CashInvRec); }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = TRUE);
};

class CCashRec : public CRecObj
{
DECLARE_DYNAMIC(CCashRec)
protected:
	CString m_CashNum;
	CString m_Portfolio;
	CString m_Account;
	CString m_Ref;
	CString m_Currency;
	CString	m_CloseDate;
	CString	m_PostDate;
	CString m_ToDate;
	CString m_PayType;
	CString	m_Amount;
	CString	m_FxRate;
	CString m_Remain;
	CString	m_DiffReason;
	CString m_Asset;
	CString m_PayID;
	CString m_ToAccount;
	CString m_ToPortfolio;
	CString m_Link;
	CString m_IRSID;
	CString m_Comment;
	CString m_CouponPaid;
	CString m_BookDate;

public:
	CCashRec() { }
	CCashRec(LPCTSTR ID) { SetID(ID); }
	CCashRec(CCashRec &CashRec) { Copy(CashRec); }
	~CCashRec() {}

	CString &GetCashNum() { return m_CashNum; }
    CString &GetPortfolio() { return m_Portfolio; }
	CString &GetAccount() { return m_Account; }
	CString &GetRef() { return m_Ref; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetCloseDate() { return m_CloseDate; }
	CString &GetPostDate() { return m_PostDate; }
	CString &GetToDate() { return m_ToDate; }
	CString &GetPayType() { return m_PayType; }
	CString &GetAmount() { return m_Amount; }
	CString &GetFxRate() { return m_FxRate; }
	CString &GetRemain() { return m_Remain; }
	CString &GetDiffReason() { return m_DiffReason; }
	CString &GetAsset() { return m_Asset; }
	CString &GetPayID() { return m_PayID; }
	CString &GetToAccount() { return m_ToAccount; }
	CString &GetToPortfolio() { return m_ToPortfolio; }
	CString &GetLink() { return m_Link; }
	CString &GetIRSID() { return m_IRSID; }
	CString &GetComment() { return m_Comment; }
	CString &GetCouponPaid() { return m_CouponPaid; }
	CString &GetBookDate() { return m_BookDate; }

	void SetCashNum(LPCTSTR CashNum) { m_CashNum = CashNum; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetRef(LPCTSTR Ref) { m_Ref = Ref; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetCloseDate(LPCTSTR CloseDate) { m_CloseDate = CloseDate; }
	void SetPostDate(LPCTSTR PostDate) { m_PostDate = PostDate; }
	void SetToDate(LPCTSTR ToDate) { m_ToDate = ToDate; }
	void SetPayType(LPCTSTR PayType) { m_PayType = PayType; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; }
	void SetFxRate(LPCTSTR FxRate) { m_FxRate = FxRate; }
	void SetRemain(LPCTSTR Remain) { m_Remain = Remain; }
	void SetDiffReason(LPCTSTR DiffReason) { m_DiffReason = DiffReason; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetPayID(LPCTSTR PayID) { m_PayID = PayID; }
	void SetToAccount(LPCTSTR ToAccount) { m_ToAccount = ToAccount; }
	void SetToPortfolio(LPCTSTR ToPortfolio) { m_ToPortfolio = ToPortfolio; }
	void SetLink(LPCTSTR Link) { m_Link = Link; }
	void SetIRSID(LPCTSTR IRSID) { m_IRSID = IRSID; }
	void SetComment(LPCTSTR Comment) { m_Comment = Comment; }
	void SetCouponPaid(LPCTSTR CouponPaid) { m_CouponPaid = CouponPaid; }
	void SetBookDate(LPCTSTR BookDate) { m_BookDate = BookDate; }

	void Copy(CCashRec &CashRec);
	const CCashRec &operator = (CCashRec &CashRec) { Copy(CashRec); return *this; }

	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};
#endif
