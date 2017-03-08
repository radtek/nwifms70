#ifndef __INVESTORREC_H__
#define __INVESTORREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif


class CInvestorAssetRec : public CRecObj
{
DECLARE_DYNAMIC(CInvestorAssetRec)
protected:
	CString m_InvCode;
	CString m_Asset;
	CString m_TaxID;
	CString m_OnShore;
	CString m_Erisa;
	CString m_Bhca;
	CString m_IPO;
	CString m_Side;

public:
	CInvestorAssetRec() {}
	CInvestorAssetRec(CInvestorAssetRec &InvestorAssetRec) { Copy(InvestorAssetRec); }
	CInvestorAssetRec(LPCTSTR ID) : CRecObj(ID) { }
	~CInvestorAssetRec() {}

	CString &GetInvCode() { return m_InvCode; }
	CString &GetAsset() { return m_Asset; }
	CString &GetTaxID() { return m_TaxID; }
	CString &GetOnShore() { return m_OnShore; }
	CString &GetErisa() { return m_Erisa; }
	CString &GetBhca() { return m_Bhca; }
	CString &GetIPO() { return m_IPO; }
	CString &GetSide() { return m_Side; }

	void SetInvCode(LPCTSTR InvCode) { m_InvCode = InvCode; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetTaxID(LPCTSTR TaxID) { m_TaxID = TaxID; }
	void SetOnShore(LPCTSTR OnShare) { m_OnShore = OnShare; }
	void SetErisa(LPCTSTR Erisa) { m_Erisa = Erisa; }
	void SetBhca(LPCTSTR Bhca) { m_Bhca = Bhca; }
	void SetIPO(LPCTSTR IPO) { m_IPO = IPO; }
	void SetSide(LPCTSTR Side) { m_Side = Side; }
	
	virtual void Copy(CInvestorAssetRec &InvestorAssetRec);
	const CInvestorAssetRec &operator = (CInvestorAssetRec &InvestorAssetRec) { Copy(InvestorAssetRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = TRUE);
};

class CInvestorRec : public CRecObj
{
DECLARE_DYNAMIC(CInvestorRec)
protected:
	CString m_InvCode;
	CString m_Investor;
	CString m_Address;
	CString	m_City;
	CString m_Country;
	CString m_Agent;
	CString m_InvClass;      
	CString m_MgmtFee;
	CString m_Performance;
	CString m_Status;

public:
	CInvestorRec() { }
	CInvestorRec(LPCTSTR ID) : CRecObj(ID) { }
	CInvestorRec(CInvestorRec &InvestorRec) { Copy(InvestorRec); }
	~CInvestorRec() {}

	CString &GetInvCode() { return m_InvCode; }
    CString &GetInvestor() { return m_Investor; }
	CString &GetAddress() { return m_Address; }
	CString &GetCity() { return m_City; }
	CString &GetCountry() { return m_Country; }
	CString &GetAgent() { return m_Agent; }
	CString &GetInvClass() { return m_InvClass; }
	CString &GetMgmtFee() { return m_MgmtFee; }
	CString &GetPerformance() { return m_Performance; }
	CString &GetStatus() { return m_Status; }

	void SetInvCode(LPCTSTR InvCode) { m_InvCode = InvCode; }
	void SetInvestor(LPCTSTR Investor) { m_Investor = Investor; }
	void SetAddress(LPCTSTR Address) { m_Address = Address; }
	void SetCity(LPCTSTR City) { m_City = City; }
	void SetCountry(LPCTSTR Country) { m_Country = Country; }
	void SetAgent(LPCTSTR Agent) { m_Agent = Agent; }
	void SetInvClass(LPCTSTR InvClass) { m_InvClass = InvClass; }
	void SetMgmtFee(LPCTSTR MgmtFee) { m_MgmtFee = MgmtFee; }
	void SetPerformance(LPCTSTR Performance) { m_Performance = Performance; }
	void SetStatus(LPCTSTR Status) { m_Status = Status; }

	virtual void Copy(CInvestorRec &InvestorRec);
	const CInvestorRec &operator = (CInvestorRec &InvestorRec) { Copy(InvestorRec); }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};
#endif
