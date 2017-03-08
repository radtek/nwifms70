#ifndef __CLEARFEEDATA_H__
#define __CLEARFEEDATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CClearingFeeRec : public CRecObj
{
DECLARE_DYNAMIC(CClearingFeeRec)
protected:
	CString m_Account;
	CString m_FromDate;
	CString m_ToDate;
	CString m_ClearingFee;
	
public:
	CClearingFeeRec() {}
	CClearingFeeRec(CClearingFeeRec &ClearingFeeRec) { Copy(ClearingFeeRec); }
	~CClearingFeeRec() { }

	CString &GetAccount() { return m_Account; }
	CString &GetFromDate() { return m_FromDate; }
	CString &GetToDate() { return m_ToDate; }
	CString &GetClearingFee() { return m_ClearingFee; }

	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetFromDate(LPCTSTR FromDate) { m_FromDate = FromDate; }
	void SetToDate(LPCTSTR ToDate) { m_ToDate = ToDate; }
	void SetClearingFee(LPCTSTR ClearingFee) { m_ClearingFee = ClearingFee; }

	virtual void Copy(CClearingFeeRec &CClearingFeeRec);
	const CClearingFeeRec &operator = (CClearingFeeRec &ClearingFeeRec) { Copy(ClearingFeeRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = TRUE);
};

class CClearingFeeData : public CRowCtrl
{
DECLARE_DYNAMIC(CClearingFeeData)
protected:
	CClearingFeeRec m_Rec;
	COraLoader m_OraLoader;

public:
	CClearingFeeData() { }
	CClearingFeeData(COraLoader &OraLoader, CDBSheet *pSS) : CRowCtrl(pSS) { SetOraLoader(OraLoader); }
	CClearingFeeData(CClearingFeeData &Data) : CRowCtrl(Data) { SetOraLoader(Data.GetOraLoader()); }
	~CClearingFeeData() { }

	CClearingFeeRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadAccounts(COptComboBox *Account);
	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif