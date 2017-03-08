#ifndef __FLOATREPOREC_H__
#define __FLOATREPOREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CFloatRepoInvRec : public CRecObj
{
DECLARE_DYNAMIC(CFloatRepoInvRec)
protected:
	CString m_TransNum;
	CString m_Portfolio;
	CString m_FromDate;
	CString m_ToDate;
	CString m_Rate;
	
	CString m_Days;
	CString m_Interest;

public:
	CFloatRepoInvRec() {}
	CFloatRepoInvRec(CFloatRepoInvRec &RepoInvRec) { Copy(RepoInvRec); }

	CString &GetTransNum() { return m_TransNum; }
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetFromDate() { return m_FromDate; }
	CString &GetToDate() { return m_ToDate; }
	CString &GetRate() { return m_Rate; }
	CString &GetDays() { return m_Days; }
	CString &GetInterest() { return m_Interest; }

	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetFromDate(LPCTSTR FromDate) { m_FromDate = FromDate; }
	void SetToDate(LPCTSTR ToDate) { m_ToDate = ToDate; }
	void SetRate(LPCTSTR Rate) { m_Rate = Rate; }
	
	void SetDays(LPCTSTR Days) { m_Days = Days; }
	void SetInterest(LPCTSTR Interest) { m_Interest = Interest; }

	virtual void Copy(CFloatRepoInvRec &RepoInvRec);
	const CFloatRepoInvRec &operator = (CFloatRepoInvRec &RepoInvRec) { Copy(RepoInvRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};


class CFloatRepoRec : public CRecObj
{
DECLARE_DYNAMIC(CFloatRepoRec)
protected:
	double m_AmountVal;

	CString m_TransNum;
	CString m_Portfolio;
	CString m_NomAmount;
	CString m_Currency;
	CString m_Price;
	CString m_RateBasis;
	CString m_ValueDate;
	CString m_Maturity;
	CString m_AssetCode;
	CString m_Asset;
	CString m_AssFrom;
	CString m_AssRateBasis;
	CString m_AssetType;
	CString m_TransType;
	CString m_IntRate;
	CString m_Amort;
	CString m_CapRate;
	CString m_Plus;
	CString m_AADays;
	CString m_Amount;
	CString m_Formula;

public:
	CFloatRepoRec() {}
	CFloatRepoRec(CFloatRepoRec &RepoRec) { Copy(RepoRec); }

	CString &GetTransNum() { return m_TransNum; }
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetNomAmount() { return m_NomAmount; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetPrice() { return m_Price; }
	CString &GetRateBasis() { return m_RateBasis; }
	CString &GetValueDate() { return m_ValueDate; }
	CString &GetMaturity() { return m_Maturity; }
	CString &GetAssetCode() { return m_AssetCode; }
	CString &GetAsset() { return m_Asset; }
	CString &GetAssFrom() { return m_AssFrom; }
	CString &GetAssRateBasis() { return m_AssRateBasis; }
	CString &GetAssetType() { return m_AssetType; }
	CString &GetTransType() { return m_TransType; }
	CString &GetIntRate() { return m_IntRate; }
	CString &GetAmort() { return m_Amort; }
	CString &GetCapRate() { return m_CapRate; }
	CString &GetPlus() { return m_Plus; }
	CString &GetAADays() { return m_AADays; }
	CString &GetAmount() { return m_Amount; }
	CString &GetFormula() { return m_Formula; }
	double GetAmountVal() { return m_AmountVal; }

	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetNomAmount(LPCTSTR NomAmount) { m_NomAmount = NomAmount; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetPrice(LPCTSTR Price) { m_Price = Price; }
	void SetRateBasis(LPCTSTR RateBasis) { m_RateBasis = RateBasis; }
	void SetValueDate(LPCTSTR ValueDate) { m_ValueDate = ValueDate; }
	void SetMaturity(LPCTSTR Maturity) { m_Maturity = Maturity; }
	void SetAssetCode(LPCTSTR AssetCode) { m_AssetCode = AssetCode; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetAssFrom(LPCTSTR AssFrom) { m_AssFrom = AssFrom; }
	void SetAssRateBasis(LPCTSTR AssRateBasis) { m_AssRateBasis = AssRateBasis; }
	void SetAssetType(LPCTSTR AssetType) { m_AssetType = AssetType; }
	void SetTransType(LPCTSTR TransType) { m_TransType = TransType; }
	void SetIntRate(LPCTSTR IntRate) { m_IntRate = IntRate; }
	void SetAmort(LPCTSTR Amort) { m_Amort = Amort; }
	void SetCapRate(LPCTSTR CapRate) { m_CapRate = CapRate; }
	void SetPlus(LPCTSTR Plus) { m_Plus = Plus; }
	void SetAADays(LPCTSTR AADays) { m_AADays = AADays; }
	void SetFormula(LPCTSTR Formula) { m_Formula = Formula; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; SetAmountVal(Amount); }
	void SetAmountVal(double AmountVal) { m_AmountVal = AmountVal; }
	void SetAmountVal(LPCTSTR AmountVal);

	virtual void Copy(CFloatRepoRec &RepoRec);
	const CFloatRepoRec &operator = (CFloatRepoRec &RepoRec) { Copy(RepoRec); return *this; }
};
#endif __FLOATREPOREC_H__
