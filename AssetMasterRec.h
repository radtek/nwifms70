#ifndef __ASSETMASTERREC_H__
#define __ASSETMASTERREC_H__

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CAssetMasterClassRec : public CRecObj
{
DECLARE_DYNAMIC(CAssetMasterClassRec)
protected:
	CString m_AssetCode;
	CString m_AssetDesc;
	CString m_Asset;
	CString m_Category;
	CString m_Bucket;
	CString m_Class;

public:
	CAssetMasterClassRec() {}
	CAssetMasterClassRec(CAssetMasterClassRec &AssetClassRec) { Copy(AssetClassRec); }

	CString &GetAssetCode() { return m_AssetCode; }
	CString &GetAssetDesc() { return m_AssetDesc; }
	CString &GetAsset() { return m_Asset; }
	CString &GetCategory() { return m_Category; }
	CString &GetBucket() { return m_Bucket; }
	CString &GetClass() { return m_Class; }
	
	void SetAssetCode(LPCTSTR AssetCode) { m_AssetCode = AssetCode; }
	void SetAssetDesc(LPCTSTR AssetDesc) { m_AssetDesc = AssetDesc; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetCategory(LPCTSTR Category) { m_Category = Category; }
	void SetBucket(LPCTSTR Bucket) { m_Bucket = Bucket; }
	void SetClass(LPCTSTR Class) { m_Class = Class; }

	virtual void Copy(CAssetMasterClassRec &AssetClassRec);
	const CAssetMasterClassRec &operator = (CAssetMasterClassRec &AssetClassRec) { Copy(AssetClassRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};

class CAssetMasterIntRec : public CRecObj
{
DECLARE_DYNAMIC(CAssetMasterIntRec)
protected:
	CString m_Asset;
	CString m_AssetFrom;
	CString m_AssetTo;
	CString m_From;
	CString m_To;
	CString m_SetDate;
	CString m_Rate;
	CString m_Amort;
	CString m_Cap;
	CString m_Plus;
	CString m_Inclusive;

public:
	CAssetMasterIntRec() {}
	CAssetMasterIntRec(CAssetMasterIntRec &AssetIntRec) { Copy(AssetIntRec); }

	CString &GetAsset() { return m_Asset; }
	CString &GetAssetFrom() { return m_AssetFrom; }
	CString &GetAssetTo() { return m_AssetTo; }
	CString &GetFrom() { return m_From; }
	CString &GetTo() { return m_To; }
	CString &GetSetDate() { return m_SetDate; }
	CString &GetRate() { return m_Rate; }
	CString &GetAmort() { return m_Amort; }
	CString &GetCap() { return m_Cap; }
	CString &GetPlus() { return m_Plus; }
	CString &GetInclusive() { return m_Inclusive; }
	
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetAssetFrom(LPCTSTR AssetFrom) { m_AssetFrom = AssetFrom; }
	void SetAssetTo(LPCTSTR AssetTo) { m_AssetTo = AssetTo; }
	void SetFrom(LPCTSTR From) { m_From = From; }
	void SetTo(LPCTSTR To) { m_To = To; }
	void SetSetDate(LPCTSTR SetDate) { m_SetDate = SetDate; }
	void SetRate(LPCTSTR Rate) { m_Rate = Rate; }
	void SetAmort(LPCTSTR Amort) { m_Amort = Amort; }
	void SetCap(LPCTSTR Cap) { m_Cap = Cap; }
	void SetPlus(LPCTSTR Plus) { m_Plus = Plus; }
	void SetInclusive(LPCTSTR Inclusive) { m_Inclusive = Inclusive; }

	virtual void Copy(CAssetMasterIntRec &AssetIntRec);
	const CAssetMasterIntRec &operator = (CAssetMasterIntRec &AssetIntRec) { Copy(AssetIntRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};

class CAssetMasterRec : public CRecObj
{
DECLARE_DYNAMIC(CAssetMasterRec)
protected:
	CString m_Asset;
	CString m_Desc;
	CString m_Par;
	CString m_Underline;
	CString m_Common;
	CString m_Cusip;
	CString m_Sedol;
	CString m_Isin;
	CString m_PID;
	CString m_IssueDate;
	CString m_CDSFeeType;
	CString m_Maturity;
	CString m_RollDate;
	CString m_RefIndex;
	CString m_Formula;
	CString m_Source;
	CString m_Country;
	CString m_Currency;
	CString m_Issuer;
	CString m_Industry;
	CString m_RateType;
	CString m_RateBasis;
	CString m_PmntFrq;
	CString m_AdjFactor;
	CString m_Factor;
	CString m_Accrue;
	CString m_PrePaid;
	CString m_Future;
	CString m_1256;
	CString m_BelowRating;
	CString m_144A;
	CString m_Method;
	CString m_Unlisted;
	CString m_Inactive;

public:
	CAssetMasterRec() {}
	CAssetMasterRec(LPCTSTR Asset, LPCTSTR Desc, LPCTSTR Currency) { Setup(Asset, Desc, Currency); }
	CAssetMasterRec(CAssetMasterRec &AssetRec) { Copy(AssetRec); }

	void Setup(LPCTSTR Asset, LPCTSTR Desc, LPCTSTR Currency) 
				{ SetAsset(Asset); SetDesc(Desc); SetCurrency(Currency); }

	CString &GetAsset() { return m_Asset; }
	CString &GetDesc() { return m_Desc; }
	CString &GetPar() { return m_Par; }
	CString &GetUnderline() { return m_Underline; }
	CString &GetCommon() { return m_Common; }
	CString &GetCusip() { return m_Cusip; }
	CString &GetSedol() { return m_Sedol; }
	CString &GetIsin() { return m_Isin; }
	CString &GetPID() { return m_PID; }
	CString &GetIssueDate() { return m_IssueDate; }
	CString &GetCDSFeeType() { return m_CDSFeeType; }
	CString &GetMaturity() { return m_Maturity; }
	CString &GetRollDate() { return m_RollDate; }
	CString &GetRefIndex() { return m_RefIndex; }
	CString &GetFormula() { return m_Formula; }
	CString &GetSource() { return m_Source; }
	CString &GetCountry() { return m_Country; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetIssuer() { return m_Issuer; }
	CString &GetIndustry() { return m_Industry; }
	CString &GetRateType() { return m_RateType; }
	CString &GetRateBasis() { return m_RateBasis; }
	CString &GetPmntFrq() { return m_PmntFrq; }
	CString &GetAdjFactor() { return m_AdjFactor; }
	CString &GetFactor() { return m_Factor; }
	CString &GetAccrue() { return m_Accrue; }
	CString &GetPrePaid() { return m_PrePaid; }
	CString &GetFuture() { return m_Future; }
	CString &Get1256() { return m_1256; }
	CString &GetBelowRating() { return m_BelowRating; }
	CString &Get144A() { return m_144A; }
	CString &GetMethod() { return m_Method; }
	CString &GetUnlisted() { return m_Unlisted; }
	CString &GetInactive() { return m_Inactive; }

	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetDesc(LPCTSTR Desc) { m_Desc = Desc; }
	void SetPar(LPCTSTR Par) { m_Par = Par; }
	void SetUnderline(LPCTSTR Underline) { m_Underline = Underline; }
	void SetCommon(LPCTSTR Common) { m_Common = Common; }
	void SetCusip(LPCTSTR Cusip) { m_Cusip = Cusip; }
	void SetSedol(LPCTSTR Sedol) { m_Sedol = Sedol; }
	void SetIsin(LPCTSTR Isin) { m_Isin = Isin; }
	void SetPID(LPCTSTR PID) { m_PID = PID; }
	void SetIssueDate(LPCTSTR IssueDate) { m_IssueDate = IssueDate; }
	void SetCDSFeeType(LPCTSTR CDSFeeType) { m_CDSFeeType = CDSFeeType; }
	void SetMaturity(LPCTSTR Maturity) { m_Maturity = Maturity; }
	void SetRollDate(LPCTSTR RollDate) { m_RollDate = RollDate; }
	void SetRefIndex(LPCTSTR RefIndex) { m_RefIndex = RefIndex; }
	void SetFormula(LPCTSTR Formula) { m_Formula = Formula; }
	void SetSource(LPCTSTR Source) { m_Source = Source; }
	void SetCountry(LPCTSTR Country) { m_Country = Country; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetIssuer(LPCTSTR Issuer) { m_Issuer = Issuer; }
	void SetIndustry(LPCTSTR Industry) { m_Industry = Industry; }
	void SetRateType(LPCTSTR RateType) { m_RateType = RateType; }
	void SetRateBasis(LPCTSTR RateBasis) { m_RateBasis = RateBasis; }
	void SetPmntFrq(LPCTSTR PmntFrq) { m_PmntFrq = PmntFrq; }
	void SetAdjFactor(LPCTSTR AdjFactor) { m_AdjFactor = AdjFactor; }
	void SetFactor(LPCTSTR Factor) { m_Factor = Factor; }
	void SetAccrue(LPCTSTR Accrue) { m_Accrue = Accrue; }
	void SetPrePaid(LPCTSTR PrePaid) { m_PrePaid = PrePaid; }
	void SetFuture(LPCTSTR Future) { m_Future = Future; }
	void Set1256(LPCTSTR S1256) { m_1256 = S1256; }
	void SetBelowRating(LPCTSTR BelowRating) { m_BelowRating = BelowRating; }
	void Set144A(LPCTSTR S144A) { m_144A = S144A; }
	void SetMethod(LPCTSTR Method) { m_Method = Method; }
	void SetUnlisted(LPCTSTR Unlisted) { m_Unlisted = Unlisted; }
	void SetInactive(LPCTSTR Inactive) { m_Inactive = Inactive; }

	virtual void Copy(CAssetMasterRec &AssetRec);
	const CAssetMasterRec &operator = (CAssetMasterRec &AssetRec) { Copy(AssetRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};
#endif 