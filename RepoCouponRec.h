#ifndef __REPOCOUPONREC_H__
#define __REPOCOUPONREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif


class CRepoCouponRec : public CRecObj
{
DECLARE_DYNAMIC(CRepoCouponRec)
protected:
	CString m_CP;
	CString m_ClaimCode;
	CString m_Portfolio;
	CString m_TransNum;
	CString m_Asset;
	CString m_PayDate;
	CString m_ClaimDate;
	CString m_NomAmount;
	CString m_Currency;
	CString m_Interest;
	CString m_Fxrate;
	CString m_USDInterest;
	CString	m_Claimed;
	CString m_Status;

public:
	CRepoCouponRec() { }
	CRepoCouponRec(LPCTSTR ID) { SetID(ID); }
	CRepoCouponRec(CRepoCouponRec &CRepoCouponRec) { Copy(CRepoCouponRec); }
	~CRepoCouponRec() {}

	CString &GetCP() { return m_CP; }
	CString &GetClaimCode() { return m_ClaimCode; }
    CString &GetPortfolio() { return m_Portfolio; }
	CString &GetTransNum() { return m_TransNum; }
	CString &GetAsset() { return m_Asset; }
	CString &GetPayDate() { return m_PayDate; }
	CString &GetClaimDate() { return m_ClaimDate; }
	CString &GetNomAmount() { return m_NomAmount; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetInterest() { return m_Interest; }
	CString &GetFxrate() { return m_Fxrate; }
	CString &GetUSDInterest() { return m_USDInterest; }
	CString &GetClaimed() { return m_Claimed; }
	CString &GetStatus() { return m_Status; }

	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetClaimCode(LPCTSTR ClaimCode) { m_ClaimCode = ClaimCode; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetPayDate(LPCTSTR PayDate) { m_PayDate = PayDate; }
	void SetClaimDate(LPCTSTR ClaimDate) { m_ClaimDate = ClaimDate; }
	void SetNomAmount(LPCTSTR NomAmount) { m_NomAmount = NomAmount; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetInterest(LPCTSTR Interest) { m_Interest = Interest; }
	void SetFxrate(LPCTSTR Fxrate) { m_Fxrate = Fxrate; }
	void SetUSDInterest(LPCTSTR USDInterest) { m_USDInterest = USDInterest; }
	void SetClaimed(LPCTSTR Claimed) { m_Claimed = Claimed; }
	void SetStatus(LPCTSTR Status) { m_Status = Status; }

	virtual void Copy(CRepoCouponRec &RepoCouponRec);
	const CRepoCouponRec &operator = (CRepoCouponRec &RepoCouponRec) { Copy(RepoCouponRec); }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};
#endif
