#ifndef __ASSETREC_H__
#define __ASSETREC_H__

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CAssetIntRec : public CRecObj
{
DECLARE_DYNAMIC(CAssetIntRec)
protected:
	CString m_Asset;
	CString m_From;
	CString m_To;
	CString m_SetDate;
	CString m_StartDate;
	CString m_EndDate;
	CString m_Rate;
	CString m_Amort;
	CString m_Cap;
	CString m_Plus;
	CString m_Action;
	CString m_InvType;

public:
	CAssetIntRec() {}
	CAssetIntRec(CAssetIntRec &AssetIntRec) { Copy(AssetIntRec); }

	CString &GetAsset() { return m_Asset; }
	CString &GetFrom() { return m_From; }
	CString &GetTo() { return m_To; }
	CString &GetSetDate() { return m_SetDate; }
	CString &GetStartDate() { return m_StartDate; }
	CString &GetEndDate() { return m_EndDate; }
	CString &GetRate() { return m_Rate; }
	CString &GetAmort() { return m_Amort; }
	CString &GetCap() { return m_Cap; }
	CString &GetPlus() { return m_Plus; }
	CString &GetAction() { return m_Action; }
	CString &GetInvType() { return m_InvType; }
	
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetFrom(LPCTSTR From) { m_From = From; }
	void SetTo(LPCTSTR To) { m_To = To; }
	void SetSetDate(LPCTSTR SetDate) { m_SetDate = SetDate; }
	void SetStartDate(LPCTSTR StartDate) { m_StartDate = StartDate; }
	void SetEndDate(LPCTSTR EndDate) { m_EndDate = EndDate; }
	void SetRate(LPCTSTR Rate) { m_Rate = Rate; }
	void SetAmort(LPCTSTR Amort) { m_Amort = Amort; }
	void SetCap(LPCTSTR Cap) { m_Cap = Cap; }
	void SetPlus(LPCTSTR Plus) { m_Plus = Plus; }
	void SetAction(LPCTSTR Action) { m_Action = Action; }
	void SetInvType(LPCTSTR InvType) { m_InvType = InvType; }

	virtual void Copy(CAssetIntRec &AssetIntRec);
	const CAssetIntRec &operator = (CAssetIntRec &AssetIntRec) { Copy(AssetIntRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);

	BOOL IsIntDateOK(COraLoader &OraLoader);
};

class CAssetRec : public CRecObj
{
DECLARE_DYNAMIC(CAssetRec)
protected:
	CString m_Asset;
	CString m_AssetID;
	CString m_Desc;
	CString m_Desc2;
	CString m_Currency;
	CString m_Currency2;
	CString m_OptTicker;
	CString m_Country;
	CString m_ProfileCountry;
	CString m_Common;
	CString m_Cusip;
	CString m_Isin;
	CString m_Sedol;
	CString m_RedCode;
	CString m_ImagineCode;
	CString m_PID;
	CString m_Class;
	CString m_Class2;
	CString m_ProfileClass;
	CString m_Industry;
	CString m_Category;
	CString m_Bucket;
	CString m_Bucket2;
	CString m_Bucket3;
	CString m_Source;
	CString m_Coupon;

	CString m_IssueDate;
	CString m_CouponStartDate;
	CString m_RollDate;
	CString m_Maturity;
	CString m_FxFixDate;
	CString m_Index;
	CString m_Formula;
	CString m_RateBasis;
	CString m_RateType;
	CString m_PmntFreq;
	CString m_CmpdFreq;
	CString m_FixedRate;
	CString m_Issuer;
	CString m_Method;
	CString m_SwapFactor;
	CString m_Status;
	CString m_Bond;
	CString m_Accrue;
	CString m_PrePaid;
	CString m_UnListed;
	CString m_OTCClear;
	CString m_Forward;
	CString m_1256;
	CString m_Par;
	CString m_CDSFeeType;
	CString m_UnderlineAsset;
	CString m_BelowRating;
	CString m_144A;
	CString m_Level;
	CString m_AdminID;
	CString m_Margin;
	CString m_FRA;
	CString m_Liquidity;
	CString m_PFClass;
	CString m_PFSubClass;
	CString m_BDate;
	CString m_CDSRestructuring;
	CString m_BDC;
	CString m_CitiClass;
	CString m_BlmKey;
	CString m_YieldCurve;
	CString m_IRSTemplate;
	CString m_Exchange;
	CString m_Account;
	CString m_FutureType;

public:
	CAssetRec() {}
	CAssetRec(LPCTSTR Asset, LPCTSTR Desc, LPCTSTR Currency, LPCTSTR OptTicker)
			{ Setup(Asset, Desc, Currency, OptTicker); }
	CAssetRec(CAssetRec &AssetRec) { Copy(AssetRec); }

	void Setup(LPCTSTR Asset, LPCTSTR Desc, LPCTSTR Currency, LPCTSTR OptTicker)
				{ SetAsset(Asset); SetDesc(Desc); SetCurrency(Currency); SetOptTicker(OptTicker); }

	CString &GetAsset() { return m_Asset; }
	CString &GetAssetID() { return m_AssetID; }
	CString &GetDesc() { return m_Desc; }
	CString &GetDesc2() { return m_Desc2; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetCurrency2() { return m_Currency2; }
	CString &GetOptTicker() { return m_OptTicker; }
	CString &GetCountry() { return m_Country; }
	CString &GetProfileCountry() { return m_ProfileCountry; }
	CString &GetCommon() { return m_Common; }
	CString &GetCusip() { return m_Cusip; }
	CString &GetIsin() { return m_Isin; }
	CString &GetSedol() { return m_Sedol; }
	CString &GetRedCode() { return m_RedCode; }
	CString &GetImagineCode() { return m_ImagineCode; }
	CString &GetPID() { return m_PID; }
	CString &GetClass() { return m_Class; }
	CString &GetClass2() { return m_Class2; }
	CString &GetProfileClass() { return m_ProfileClass; }
	CString &GetIndustry() { return m_Industry; }
	CString &GetCategory() { return m_Category; }
	CString &GetBucket() { return m_Bucket; }
	CString &GetBucket2() { return m_Bucket2; }
	CString &GetBucket3() { return m_Bucket3; }
	CString &GetSource() { return m_Source; }
	CString &GetCoupon() { return m_Coupon; }	
	CString &GetIssueDate() { return m_IssueDate; }
	CString &GetCouponStartDate() { return m_CouponStartDate; }
	CString &GetRollDate() { return m_RollDate; }
	CString &GetMaturity() { return m_Maturity; }
	CString &GetFxFixDate() { return m_FxFixDate; }
	CString &GetIndex() { return m_Index; }
	CString &GetFormula() { return m_Formula; }
	CString &GetRateBasis() { return m_RateBasis; }
	CString &GetRateType() { return m_RateType; }
	CString &GetPmntFreq() { return m_PmntFreq; }
	CString &GetCmpdFreq() { return m_CmpdFreq; }
	CString &GetFixedRate() { return m_FixedRate; }
	CString &GetIssuer() { return m_Issuer; }
	CString &GetMethod() { return m_Method; }
	CString &GetSwapFactor() { return m_SwapFactor; }
	CString &GetStatus() { return m_Status; }
	CString &GetBond() { return m_Bond; }
	CString &GetAccrue() { return m_Accrue; }
	CString &GetPrePaid() { return m_PrePaid; }
	CString &GetUnListed() { return m_UnListed; }
	CString &GetOTCClear() { return m_OTCClear; }
	CString &GetForward() { return m_Forward; }
	CString &Get1256() { return m_1256; }
	CString &GetPar() { return m_Par; }
	CString &GetCDSFeeType() { return m_CDSFeeType; }
	CString &GetUnderlineAsset() { return m_UnderlineAsset; }
	CString &GetBelowRating() { return m_BelowRating; }
	CString &Get144A() { return m_144A; }
	CString &GetLevel() { return m_Level; }
	CString &GetAdminID() { return m_AdminID; }
	CString &GetMargin() { return m_Margin; }
	CString &GetFRA() { return m_FRA; }
	CString &GetLiquidity() { return m_Liquidity; }
	CString &GetPFClass() { return m_PFClass; }
	CString &GetPFSubClass() { return m_PFSubClass; }
	CString &GetBDate() { return m_BDate; }
	CString &GetCDSRestructuring() { return m_CDSRestructuring; }
	CString &GetBDC() { return m_BDC; }
	CString &GetCitiClass() { return m_CitiClass; }
	CString &GetBlmKey() { return m_BlmKey; }
	CString &GetYieldCurve() { return m_YieldCurve; }
	CString &GetIRSTemplate() { return m_IRSTemplate; }
	CString &GetExchange() { return m_Exchange; }
	CString &GetAccount() { return m_Account; }
	CString &GetFutureType() { return m_FutureType; }

	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetAssetID(LPCTSTR AssetID) { m_AssetID = AssetID; }
	void SetDesc(LPCTSTR Desc) { m_Desc = Desc; }
	void SetDesc2(LPCTSTR Desc2) { m_Desc2 = Desc2; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetCurrency2(LPCTSTR Currency2) { m_Currency2 = Currency2; }
	void SetOptTicker(LPCTSTR OptTicker) { m_OptTicker = OptTicker; }
	void SetCountry(LPCTSTR Country) { m_Country = Country; }
	void SetProfileCountry(LPCTSTR ProfileCountry) { m_ProfileCountry = ProfileCountry; }
	void SetCommon(LPCTSTR Common) { m_Common = Common; }
	void SetCusip(LPCTSTR Cusip) { m_Cusip = Cusip; }
	void SetIsin(LPCTSTR Isin) { m_Isin = Isin; }
	void SetSedol(LPCTSTR Sedol) { m_Sedol = Sedol; }
	void SetRedCode(LPCTSTR RedCode) { m_RedCode = RedCode; }
	void SetImagineCode(LPCTSTR ImagineCode) { m_ImagineCode = ImagineCode; }
	void SetPID(LPCTSTR PID) { m_PID = PID; }
	void SetClass(LPCTSTR Class) { m_Class = Class; }
	void SetClass2(LPCTSTR Class2) { m_Class2 = Class2; }
	void SetProfileClass(LPCTSTR ProfileClass) { m_ProfileClass; }
	void SetIndustry(LPCTSTR Industry) { m_Industry = Industry; }
	void SetCategory(LPCTSTR Category) { m_Category = Category; }
	void SetBucket(LPCTSTR Bucket) { m_Bucket = Bucket; }
	void SetBucket2(LPCTSTR Bucket2) { m_Bucket2 = Bucket2; }
	void SetBucket3(LPCTSTR Bucket3) { m_Bucket2 = Bucket3; }
	void SetSource(LPCTSTR Source) { m_Source = Source; }
	void SetCoupon(LPCTSTR Coupon) { m_Coupon = Coupon; }
	void SetIssueDate(LPCTSTR IssueDate) { m_IssueDate = IssueDate; }
	void SetCouponStartDate(LPCTSTR CouponStartDate) { m_CouponStartDate = CouponStartDate; }
	void SetRollDate(LPCTSTR RollDate) { m_RollDate = RollDate; }
	void SetMaturity(LPCTSTR Maturity) { m_Maturity = Maturity; }
	void SetFxFixDate(LPCTSTR FxFixDate) { m_FxFixDate = FxFixDate; }
	void SetIndex(LPCTSTR Index) { m_Index = Index; }
	void SetFormula(LPCTSTR Formula) { m_Formula = Formula; }
	void SetRateBasis(LPCTSTR RateBasis) { m_RateBasis = RateBasis; }
	void SetRateType(LPCTSTR RateType) { m_RateType = RateType; }
	void SetPmntFreq(LPCTSTR PmntFreq) { m_PmntFreq = PmntFreq; }
	void SetCmpdFreq(LPCTSTR CmpdFreq) { m_CmpdFreq = CmpdFreq; }
	void SetFixedRate(LPCTSTR FixedRate) { m_FixedRate = FixedRate; }
	void SetIssuer(LPCTSTR Issuer) { m_Issuer = Issuer; }
	void SetMethod(LPCTSTR Method) { m_Method = Method; }
	void SetSwapFactor(LPCTSTR SwapFactor) { m_SwapFactor = SwapFactor; }
	void SetStatus(LPCTSTR Status) { m_Status = Status; }
	void SetBond(LPCTSTR Bond) { m_Bond = Bond; }
	void SetAccrue(LPCTSTR Accrue) { m_Accrue = Accrue; }
	void SetPrePaid(LPCTSTR PrePaid) { m_PrePaid = PrePaid; }
	void SetUnListed(LPCTSTR UnListed) { m_UnListed = UnListed; }
	void SetOTCClear(LPCTSTR OTCClear) { m_OTCClear = OTCClear; }
	void SetForward(LPCTSTR Forward) { m_Forward = Forward; }
	void Set1256(LPCTSTR S1256) { m_1256 = S1256; }
	void SetPar(LPCTSTR Par) { m_Par = Par; }
	void SetCDSFeeType(LPCTSTR CDSFeeType) { m_CDSFeeType = CDSFeeType; }
	void SetUnderlineAsset(LPCTSTR UnderlineAsset) { m_UnderlineAsset = UnderlineAsset; }
	void SetBelowRating(LPCTSTR BelowRating) { m_BelowRating = BelowRating; }
	void Set144A(LPCTSTR S144A) { m_144A = S144A; }
	void SetLevel(LPCTSTR Level) { m_Level = Level; }
	void SetAdminID(LPCTSTR AdminID) { m_AdminID = AdminID; }
	void SetMargin(LPCTSTR Margin) { m_Margin = Margin; }
	void SetFRA(LPCTSTR FRA) { m_FRA = FRA; }
	void SetLiquidity(LPCSTR Liquidity) { m_Liquidity = Liquidity; }
	void SetPFClass(LPCTSTR PFClass) { m_PFClass = PFClass; }
	void SetPFSubClass(LPCTSTR PFSubClass) { m_PFSubClass = PFSubClass; }
	void SetBDate(LPCTSTR BDate) { m_BDate = BDate; }
	void SetCDSRestructuring(LPCTSTR CDSRestructuring) { m_CDSRestructuring = CDSRestructuring; } 
	void SetBDC(LPCTSTR BDC) { m_BDC = BDC; }
	void SetCitiClass(LPCTSTR CitiClass) { m_CitiClass = CitiClass; }
	void SetBlmKey(LPCTSTR BlmKey) { m_BlmKey = BlmKey; }
	void SetYieldCurve(LPCTSTR YieldCurve) { m_YieldCurve = YieldCurve; }
	void SetIRSTemplate(LPCTSTR IRSTemplate) { m_IRSTemplate = IRSTemplate; }
	void SetExchange(LPCTSTR Exchange) { m_Exchange = Exchange; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetFutureType(LPCTSTR FutureType) { m_FutureType = FutureType; }

	virtual void Copy(CAssetRec &AssetRec);
	const CAssetRec &operator = (CAssetRec &AssetRec) { Copy(AssetRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};
#endif 