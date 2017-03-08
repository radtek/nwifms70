#ifndef __INVESTORDATA_H__
#define __INVESTORDATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __INVESTORREC_H__
#include "investorrec.h"
#endif

class CInvestorData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CInvestorData)
protected:
	CInvestorRec m_InvestorRec;
	CInvestorAssetRec m_InvestorAssetRec;

public:
	CInvestorData() {}
	CInvestorData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS) 
		: CMSRowCtrl(OraLoader, pSS, pSSS) { SetKeyIndex(0); }
	~CInvestorData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS)
				{ CMSRowCtrl::Setup(OraLoader, pSS, pSSS); SetKeyIndex(0); }
	virtual void LoadSheet();
	void SetHeaders();

	virtual BOOL AddSRow(BOOL bDirectly = TRUE);
	virtual BOOL DeleteSRow(int CurrentRow = -1);
	virtual BOOL UpdateSRow(BOOL bDirectly = TRUE);

	BOOL LoadDBData();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();

	CInvestorRec &GetInvestorRec() { return m_InvestorRec; }
	CInvestorAssetRec &GetInvestorAssetRec() { return m_InvestorAssetRec; }
};

#endif // __INVESTORDATA_H__