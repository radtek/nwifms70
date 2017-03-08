#ifndef __CLAIMREC_H__
#define __CLAIMREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif


class CClaimRec : public CRecObj
{
DECLARE_DYNAMIC(CClaimRec)
protected:
	CString m_CP;
	CString m_ClaimCode;
	CString m_Name;
	CString m_Phone;
	CString m_Fax;
	CString m_EMail;
	
public:
	CClaimRec() {}
	CClaimRec(CClaimRec &CClaimRec) { Copy(CClaimRec); }
	~CClaimRec() { }

	CString &GetCP() { return m_CP; }
	CString &GetClaimCode() { return m_ClaimCode; }
	CString &GetName() { return m_Name; }
	CString &GetPhone() { return m_Phone; }
	CString &GetFax() { return m_Fax; }
	CString &GetEMail() { return m_EMail; }

	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetClaimCode(LPCTSTR ClaimCode) { m_ClaimCode = ClaimCode; }
	void SetName(LPCTSTR Name) { m_Name = Name; }
	void SetPhone(LPCTSTR Phone) { m_Phone = Phone; }
	void SetFax(LPCTSTR Fax) { m_Fax = Fax; }
	void SetEMail(LPCTSTR EMail) { m_EMail = EMail; }

	virtual void Copy(CClaimRec &CClaimRec);
	const CClaimRec &operator = (CClaimRec &ClaimRec) { Copy(ClaimRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = TRUE);
};
#endif
