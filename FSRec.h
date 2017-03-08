#ifndef __FSREC_H__
#define __FSREC_H__

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CFSRec : public CRecObj
{
DECLARE_DYNAMIC(CFSRec)
protected:
	CString m_Indate;
	CString m_Portfolio;
	CString m_Asset;
	CString m_Bucket;
	CString m_TransType;
	CString m_Strike;
	CString m_Rate;
	CString m_Maturity;
	CString m_TransNum;
	CString m_Nominal;
	CString m_Amount;
	CString m_DailyChange;
	CString m_Reason;
	CString m_Note;

public:
	CFSRec() {}
	CFSRec(CFSRec &FSRec) { Copy(FSRec); }

	CString &GetIndate() { return m_Indate; }
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetAsset() { return m_Asset; }
	CString &GetBucket() { return m_Bucket; }
	CString &GetTransType() { return m_TransType; }
	CString &GetStrike() { return m_Strike; }
	CString &GetRate() { return m_Rate; }
	CString &GetMaturity() { return m_Maturity; }
	CString &GetTransNum() { return m_TransNum; }
	CString &GetNominal() { return m_Nominal; }
	CString &GetAmount() { return m_Amount; }
	CString &GetDailyChange() { return m_DailyChange; }
	CString &GetReason() { return m_Reason; }
	CString &GetNote() { return m_Note; }

	virtual void Copy(CFSRec &FSRec);
	const CFSRec &operator = (CFSRec &FSRec) { Copy(FSRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader);
	virtual BOOL DeleteRec(COraLoader &OraLoader);
};
#endif