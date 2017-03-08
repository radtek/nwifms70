#ifndef __CLAIMDATA_H__
#define __CLAIMDATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

#ifndef __CLAIMREC_H__
#include "claimrec.h"
#endif 

class CClaimData : public CRowCtrl
{
DECLARE_DYNAMIC(CClaimData)
protected:
	CClaimRec m_Rec;
	COraLoader m_OraLoader;

public:
	CClaimData() { }
	CClaimData(COraLoader &OraLoader, CDBSheet *pSS) : CRowCtrl(pSS) { SetOraLoader(OraLoader); }
	CClaimData(CClaimData &Data) : CRowCtrl(Data) { SetOraLoader(Data.GetOraLoader()); }
	~CClaimData() { }

	CClaimRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif