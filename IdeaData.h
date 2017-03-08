#ifndef __IDEADATA_H__
#define __IDEADATA_H__

#ifndef __ROWCTRL_H__
#include "ROWCTRL.h"
#endif

#ifndef __IDEAREC_H__
#include "idearec.h"
#endif

class CIdeaData : public CRowCtrl
{
DECLARE_DYNAMIC(CIdeaData)
protected:
	CIdeaRec m_IdeaRec;
	COraLoader m_OraLoader;

public:
	CIdeaData() { }
	CIdeaData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	CIdeaData(CIdeaData &IdeaData);
	~CIdeaData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS);

	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }
	COraLoader &GetOraLoader() { return m_OraLoader; }

	CIdeaRec &GetIdeaRec() { return m_IdeaRec; }

	int LoadIdeaRecords(int nUserType, BOOL bNew = FALSE);
	int FindRecords();

	BOOL RiskApprove(const CString ID, const CString Note);

	BOOL ApproveIdea(const CString ID, const CString Decision, 
					const CString Capital, const CString Note);
	
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif