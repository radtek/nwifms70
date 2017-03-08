#ifndef __AUTOALLOCDATA_H__
#define __AUTOALLOCDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __RAWTICKETREC_H__
#include "rawticketrec.h"
#endif

class CAutoAllocRec : public CRecObj
{
DECLARE_DYNAMIC(CAutoAllocRec)
protected:
	CString m_AAMethod;
	CString m_FromDate;
	CString m_ToDate;
	CString m_Portfolio;
	CString m_AllocRate;

public:
	CAutoAllocRec() {}
	CAutoAllocRec(CAutoAllocRec &AutoAllocRec) { Copy(AutoAllocRec); }
	CAutoAllocRec(LPCTSTR ID) : CRecObj(ID) { }
	~CAutoAllocRec() { }

	void SetAAMethod(LPCTSTR AAMethod) { m_AAMethod = AAMethod; }
	void SetFromDate(LPCTSTR FromDate) { m_FromDate = FromDate; }
	void SetToDate(LPCTSTR ToDate) { m_ToDate = ToDate; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetAllocRate(LPCTSTR AllocRate) { m_AllocRate = AllocRate; }

	CString &GetAAMethod() { return m_AAMethod; }
	CString &GetFromDate() { return m_FromDate; }
	CString &GetToDate() { return m_ToDate; }
	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetAllocRate() { return m_AllocRate; }

	virtual void Copy(CAutoAllocRec &AutoAllocRec);
	const CAutoAllocRec &operator = (CAutoAllocRec &AutoAllocRec) { Copy(AutoAllocRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, LPCTSTR ID);
	virtual BOOL DeleteRec(COraLoader &OraLoader);
};


class CAutoAllocData : public CRowCtrl
{
DECLARE_DYNAMIC(CAutoAllocData)
protected:
	COraLoader m_OraLoader;
	CAutoAllocRec m_Rec;

public:
	CAutoAllocData() { }
	CAutoAllocData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	~CAutoAllocData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { CRowCtrl::Setup(pSS); SetOraLoader(OraLoader); }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	CAutoAllocRec &GetRec() { return m_Rec; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif 