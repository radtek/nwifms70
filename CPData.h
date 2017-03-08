#ifndef __CPDATA_H__
#define __CPDATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __CPREC_H__
#include "cprec.h"
#endif

class CCPData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CCPData)
protected:
	CCPRec m_CPRec;
	CCTRec m_CTRec;

public:
	CCPData() {}
	CCPData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User) 
		: CMSRowCtrl(OraLoader, pSS, pSSS) { m_CPRec.SetUser(User); m_CTRec.SetUser(User); SetKeyIndex(0); }
	~CCPData() {}
	
	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User)
				{ CMSRowCtrl::Setup(OraLoader, pSS, pSSS); m_CPRec.SetUser(User); m_CTRec.SetUser(User); SetKeyIndex(0); }
	virtual void LoadSheet();
	void SetHeaders();
	
	virtual BOOL AddSRow(BOOL bDirectly = TRUE);
	virtual BOOL DeleteSRow(int CurrentRow = -1);
	virtual BOOL UpdateSRow(BOOL bDirectly = TRUE);

	BOOL LoadDBData();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();

	CCPRec &GetCPRec() { return m_CPRec; }
	CCTRec &GetCTRec() { return m_CTRec; }
};
#endif // CPDATA_H