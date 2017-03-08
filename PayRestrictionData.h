#ifndef __PAYRESTRICTIONDATA_H__
#define __PAYRESTRICTIONDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __PAYMENTREC_H__
#include "paymentrec.h"
#endif

class CPayRestrictionRec : public CRecObj
{
DECLARE_DYNAMIC(CPayRestrictionRec)
protected:
	CString m_Portfolio;
	CString m_RefID;
	
public:	
	CPayRestrictionRec(CPayRestrictionRec &PayRestrictionRec) { Copy(PayRestrictionRec); }
	CPayRestrictionRec(const char *ID = NULL) { SetID(ID); }

	void SetPortfolio(const char *Portfolio) { m_Portfolio = Portfolio; }
	CString &GetPortfolio() { return m_Portfolio; }

	void SetRefID(const char *RefID) { m_RefID = RefID; }
	CString &GetRefID() { return m_RefID; }

	virtual void Copy(CPayRestrictionRec &PayRestrictionRec);
	const CPayRestrictionRec &operator = (CPayRestrictionRec &PayRestrictionRec) 
			{ Copy(PayRestrictionRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = TRUE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = TRUE);
};

class CPayRestrictionData : public CRowCtrl
{
DECLARE_DYNAMIC(CPayRestrictionData)
protected:
	CPayRestrictionRec m_Rec;
	COraLoader m_OraLoader;

public:
	CPayRestrictionData() { }
	CPayRestrictionData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	CPayRestrictionData(CPayRestrictionData &PayRestrictionData) : CRowCtrl(PayRestrictionData) 
				{ SetOraLoader(PayRestrictionData.GetOraLoader()); }
	~CPayRestrictionData() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { SetOraLoader(OraLoader), SetSS(pSS); }
	CPayRestrictionRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	BOOL LoadRefData(CDBSheet &SS);
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif