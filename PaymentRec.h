#ifndef __PAYMENTREC_H__
#define __PAYMENTREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CPayTemplateRec : public CRecObj
{
DECLARE_DYNAMIC(CPayTemplateRec)
protected:
	CString m_RefID;
	CString m_CP;
	CString m_Currency;
	CString m_Bank;
	CString m_Account;
	CString m_CPRef;
	CString m_Beneficiary;
	CString m_BeneficiaryID;
	CString m_BeneficiaryBIC;
	CString m_BeneficiaryAccount;
	CString m_FFC;
	CString m_FFCID;
	CString m_FFCBIC;
	CString m_FFCAccount;
	CString m_Active;
	CString m_BankID;
	CString m_BankBIC;

public:
	CPayTemplateRec() {}
	CPayTemplateRec(CPayTemplateRec &PayTemplateRec) { Copy(PayTemplateRec); }

	CString &GetRefID() { return m_RefID; }
	CString &GetCP() { return m_CP; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetBank() { return m_Bank; }
	CString &GetAccount() { return m_Account; }
	CString &GetCPRef() { return m_CPRef; }
	CString &GetBeneficiary() { return m_Beneficiary; }
	CString &GetBeneficiaryID() { return m_BeneficiaryID; }
	CString &GetBeneficiaryBIC() { return m_BeneficiaryBIC; }
	CString &GetBeneficiaryAccount() { return m_BeneficiaryAccount; }
	CString &GetFFC() { return m_FFC; }
	CString &GetFFCID() { return m_FFCID; }
	CString &GetFFCBIC() { return m_FFCBIC; }
	CString &GetFFCAccount() { return m_FFCAccount; }
	CString &GetActive() { return m_Active; }
	
	CString &GetBankID() { return m_BankID; }
	CString &GetBankBIC() { return m_BankBIC; }

	void SetRefID(LPCTSTR RefID) { m_RefID = RefID; }
	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetBank(LPCTSTR Bank) { m_Bank = Bank; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetCPRef(LPCTSTR CPRef) { m_CPRef = CPRef; }
	void SetBeneficiary(LPCTSTR Beneficiary) { m_Beneficiary = Beneficiary; }
	void SetBeneficiaryID(LPCTSTR BeneficiaryID) { m_BeneficiaryID = BeneficiaryID; }
	void SetBeneficiaryBIC(LPCTSTR BeneficiaryBIC) { m_BeneficiaryBIC = BeneficiaryBIC; }
	void SetBeneficiaryAccount(LPCTSTR BeneficiaryAccount) { m_BeneficiaryAccount = BeneficiaryAccount; }
	void SetFFC(LPCTSTR FFC) { m_FFC = FFC; }
	void SetFFCID(LPCTSTR FFCID) { m_FFC = FFCID; }
	void SetFFCBIC(LPCTSTR FFCBIC) { m_FFC = FFCBIC; }
	void SetFFCAccount(LPCTSTR FFCAccount) { m_FFCAccount = FFCAccount; }
	void SetActive(LPCTSTR Active) { m_Active = Active; }

	void SetBankID(LPCTSTR BankID) { m_BankID = BankID; }
	void SetBankBIC(LPCTSTR BankBIC) { m_BankBIC = BankBIC; }

	virtual void Copy(CPayTemplateRec &PayTemplateRec);
	const CPayTemplateRec &operator = (CPayTemplateRec &PayTemplateRec) 
			{ Copy(PayTemplateRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};

class CPayInvRec : public CRecObj
{
DECLARE_DYNAMIC(CPayInvRec)
protected:
	CString m_PayID;
	CString m_TransNum;
	CString m_Amount;

public:
	CPayInvRec() { }
	CPayInvRec(CPayInvRec &PayInvRec) { Copy(PayInvRec); }

	CString &GetPayID() { return m_PayID; }
	CString &GetTransNum() { return m_TransNum; }
	CString &GetAmount() { return m_Amount; }

	void SetPayID(LPCTSTR PayID) { m_PayID = PayID; }
	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; }

	virtual void Copy(CPayInvRec &PayInvRec);
	const CPayInvRec &operator = (CPayInvRec &PayInvRec) { Copy(PayInvRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};

class CPayInvCouponRec : public CRecObj
{
DECLARE_DYNAMIC(CPayInvCouponRec)
protected:
	CString m_PayID;
	CString m_Portfolio;
	CString m_Asset;
	CString m_TransNum;
	CString m_InvNum;
	CString m_Date;
	CString m_Amount;
	CString m_Fxrate;
	CString m_Remaining;
	CString m_CouponPaid;

public:
	CPayInvCouponRec() {}
	CPayInvCouponRec(CPayInvCouponRec &PayInvCouponRec) { Copy(PayInvCouponRec); }

	CString &GetPayID() { return m_PayID; }
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetAsset() { return m_Asset; }
	CString &GetTransNum() { return m_TransNum; }
	CString &GetInvNum() { return m_InvNum; }
	CString &GetDate() { return m_Date; }
	CString &GetAmount() { return m_Amount; }
	CString &GetFxrate() { return m_Fxrate; }
	CString &GetRemaining() { return m_Remaining; }
	CString &GetCouponPaid() { return m_CouponPaid; }

	void SetPayID(LPCTSTR PayID) { m_PayID = PayID; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	void SetInvNum(LPCTSTR InvNum) { m_InvNum = InvNum; }
	void SetDate(LPCTSTR Date) { m_Date = Date; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; }
	void SetFxrate(LPCTSTR Fxrate) { m_Fxrate = Fxrate; }
	void SetRemaining(LPCTSTR Remaining) { m_Remaining = Remaining; }
	void SetCouponPaid(LPCTSTR CouponPaid) { m_CouponPaid = CouponPaid; }

	virtual void Copy(CPayInvCouponRec &PayInvCouponRec);
	const CPayInvCouponRec &operator = (CPayInvCouponRec &PayInvCouponRec) { Copy(PayInvCouponRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};

class CPaymentRec : public CRecObj
{
DECLARE_DYNAMIC(CPaymentRec)
protected:
	CString m_PayID;
	CString m_RefID;
	CString m_Portfolio;
	CString m_Account;
	CString m_CP;
	CString m_Dir;
	CString m_Date;
	CString m_Currency;
	CString m_Amount;
	CString m_Adjust;
	CString m_Note;
	CString m_ValueDate;
	CString m_CashType;
	CString m_AssetCode;
	CString m_CPRef;
	CString m_CloseFlag;
	CString m_ToAccount;
	CString m_Fxrate;

public:
	CPaymentRec() { m_CloseFlag = EMPTYSTRING; }
	CPaymentRec(CPaymentRec &PaymentRec) { Copy(PaymentRec); }

	CString &GetPayID() { return m_PayID; }
	CString &GetRefID() { return m_RefID; }
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetAccount() { return m_Account; }
	CString &GetCP() { return m_CP; }
	CString &GetDir() { return m_Dir; }
	CString &GetDate() { return m_Date; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetAmount() { return m_Amount; }
	CString &GetAdjust() { return m_Adjust; }
	CString &GetNote() { return m_Note; }
	CString &GetValueDate() { return m_ValueDate; }
	CString &GetCashType() { return m_CashType; }
	CString &GetAssetCode() { return m_AssetCode; }
	CString &GetCPRef() { return m_CPRef; }
	CString &GetCloseFlag() { return m_CloseFlag; }
	CString &GetToAccount() { return m_ToAccount; }
	CString &GetFxrate() { return m_Fxrate; }

	void SetPayID(LPCTSTR PayID) { m_PayID = PayID; }
	void SetRefID(LPCTSTR RefID) { m_RefID = RefID; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetDir(LPCTSTR Dir) { m_Dir = Dir; }
	void SetDate(LPCTSTR Date) { m_Date = Date; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; }
	void SetAdjust(LPCTSTR Adjust) { m_Adjust = Adjust; }
	void SetNote(LPCTSTR Note) { m_Note = Note; }
	void SetValueDate(LPCTSTR ValueDate) { m_ValueDate; }
	void SetCashType(LPCTSTR CashType) { m_CashType = CashType; }
	void SetAssetCode(LPCTSTR AssetCode) { m_AssetCode = AssetCode; }
	void SetCPRef(LPCTSTR CPRef) { m_CPRef = CPRef; }
	void SetCloseFlag(LPCTSTR CloseFlag = "Y") { m_CloseFlag = CloseFlag; }
	void SetToAccount(LPCTSTR ToAccount) { m_ToAccount = ToAccount; }
	void SetFxrate(LPCTSTR Fxrate) { m_Fxrate = Fxrate; }

	virtual void Copy(CPaymentRec &PaymentRec);
	const CPaymentRec &operator = (CPaymentRec &PaymentRec) { Copy(PaymentRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	BOOL Closing(COraLoader &OraLoader, BOOL bClose = TRUE);
};
#endif