#ifndef __CPREC_H__
#define __CPREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif


class CCTRec : public CRecObj
{
DECLARE_DYNAMIC(CCTRec)
protected:
	CString m_CP;
	CString m_CTCode;
	CString m_Name;
	CString m_CT;
	CString m_Address;
	CString m_Address2;
	CString m_Phone;
	CString m_Fax;
	CString m_Cedel;
	CString m_EuroClear;
	CString m_MSTC;
	CString m_FedID;
	CString m_Limit;
	CString m_Isin;
	CString m_Bic;
	CString m_DTC;
	CString m_Group;
	CString m_SecID;
	CString m_FSAID;
	CString m_CTIntl;
	CString m_CTFx;
	CString m_Active;

	CString m_User;
	
public:
	CCTRec() {}
	CCTRec(CCTRec &CTRec) { Copy(CTRec); }
	CCTRec(LPCTSTR User, LPCTSTR ID) { Setup(User, ID); }

	void Setup(LPCTSTR User, LPCTSTR ID = NULL)	{ SetUser(User); SetID(ID); }

	CString &GetCP() { return m_CP; }
	CString &GetCTCode() { return m_CTCode; }
	CString &GetName() { return m_Name; }
	CString &GetCT() { return m_CT; }
	CString &GetAddress() { return m_Address; }
	CString &GetAddress2() { return m_Address2; }
	CString &GetPhone() { return m_Phone; }
	CString &GetFax() { return m_Fax; }
	CString &GetCedel() { return m_Cedel; }
	CString &GetEuroClear() { return m_EuroClear; }
	CString &GetMSTC() { return m_MSTC; }
	CString &GetFedID() { return m_FedID; }
	CString &GetLimit() { return m_Limit; }
	CString &GetIsin() { return m_Isin; }
	CString &GetBic() { return m_Bic; }
	CString &GetDTC() { return m_DTC; }
	CString &GetGroup() { return m_Group; }
	CString &GetSecID() { return m_SecID; }
	CString &GetFSAID() { return m_FSAID; }
	CString &GetCTIntl() { return m_CTIntl; }
	CString &GetCTFx() { return m_CTFx; }
	CString &GetActive() { return m_Active; }

	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetCTCode(LPCTSTR CTCode) { m_CTCode = CTCode; }
	void SetName(LPCTSTR Name) { m_Name = Name; }
	void SetCT(LPCTSTR CT) { m_CT = CT; }
	void SetAddress(LPCTSTR Address) { m_Address = Address; }
	void SetAddress2(LPCTSTR Address2) { m_Address2 = Address2; }
	void SetPhone(LPCTSTR Phone) { m_Phone = Phone; }
	void SetFax(LPCTSTR Fax) { m_Fax = Fax; }
	void SetCedel(LPCTSTR Cedel) { m_Cedel = Cedel; }
	void SetEuroClear(LPCTSTR EuroClear) { m_EuroClear = EuroClear; }
	void SetMSTC(LPCTSTR MSTC) { m_MSTC = MSTC; }
	void SetFedID(LPCTSTR FedID) { m_FedID = FedID; }
	void SetLimit(LPCTSTR Limit) { m_Limit = Limit; }
	void SetIsin(LPCTSTR Isin) { m_Isin = Isin; }
	void SetBic(LPCTSTR Bic) { m_Bic = Bic; }
	void SetDTC(LPCTSTR DTC) { m_DTC = DTC; }
	void SetGroup(LPCTSTR Group) { m_Group = Group; }
	void SetSecID(LPCTSTR SecID) { m_SecID = SecID; }
	void SetFSAID(LPCTSTR FSAID) { m_FSAID = FSAID; }
	void SetCTIntl(LPCTSTR CTIntl) { m_CTIntl = CTIntl; }
	void SetCTFx(LPCTSTR CTFx) { m_CTFx = CTFx; }
	void SetActive(LPCTSTR Active) { m_Active = Active; }

	void SetUser(LPCTSTR User) { m_User = User; }
	CString &GetUser() { return m_User; }
	
	virtual void Copy(CCTRec &CTRec);
	const CCTRec &operator = (CCTRec &CTRec) { Copy(CTRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};

class CCPRec : public CRecObj
{
DECLARE_DYNAMIC(CCPRec)
protected:
	CString m_CPCode;
	CString m_CP;
	CString m_ImgCP;
	CString m_Limit;
	CString m_SetLimit;
	CString	m_Pru;
	CString m_Active;

	CString m_User;

public:
	CCPRec() { }
	CCPRec(LPCTSTR User, LPCTSTR ID) { Setup(User, ID); }
	CCPRec(CCPRec &CPRec) { Copy(CPRec); }
	~CCPRec() {}

	void Setup(LPCTSTR User, LPCTSTR ID = NULL) { SetUser(User); SetID(ID); }
	CString &GetCPCode() { return m_CPCode; }
    CString &GetCP() { return m_CP; }
	CString &GetImgCP() { return m_ImgCP; }
	CString &GetLimit() { return m_Limit; }
	CString &GetSetLimit() { return m_SetLimit; }
	CString &GetPru() { return m_Pru; }
	CString &GetActive() { return m_Active; }

	void SetCPCode(LPCTSTR CPCode) { m_CPCode = CPCode; }
	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetImgCP(LPCTSTR ImgCP) { m_ImgCP = ImgCP; }
	void SetLimit(LPCTSTR Limit) { m_Limit = Limit; }
	void SetSetLimit(LPCTSTR SetLimit) { m_SetLimit = SetLimit; }
	void SetPru(LPCTSTR Pru) { m_Pru = Pru; }
	void SetActive(LPCTSTR Active) { m_Active = Active; }

	void SetUser(LPCTSTR User) { m_User = User; }
	CString &GetUser() { return m_User; }

	virtual void Copy(CCPRec &CPRec);
	const CCPRec &operator = (CCPRec &CPRec) { Copy(CPRec); }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};
#endif
