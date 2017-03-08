#ifndef __REPOCOUPONDATA_H__
#define __REPOCOUPONDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __REPOCOUPONREC_H__
#include "repocouponrec.h"
#endif

class CRepoCouponData : public CRowCtrl
{
DECLARE_DYNAMIC(CRepoCouponData)
protected:
	COraLoader m_OraLoader;
	CRepoCouponRec m_Rec;

public:
	CRepoCouponData() { }
	CRepoCouponData(COraLoader &OraLoader, CDBSheet *pSS) 
		: CRowCtrl(pSS) { SetOraLoader(OraLoader); }
	~CRepoCouponData() {}
	
	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }
	void Setup(COraLoader &OraLoader, CDBSheet *pSS) 
			{ CRowCtrl::Setup(pSS), SetOraLoader(OraLoader); }
	
	CRepoCouponRec &GetRec() { return m_Rec; }

	int LoadDBData(BOOL bInitial = FALSE);
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};
#endif // RepoCouponDATA_H