#ifndef __REPODATA_H__
#define __REPODATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __ASSETVAL_H__
#include "assetval.h"
#endif

#ifndef __FLOATREPOREC_H__
#include "floatreporec.h"
#endif


class CFloatRepoData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CFloatRepoData);
// Construction
protected:
	CFloatRepoRec m_Rec;
	CFloatRepoInvRec m_InvRec;
	CAssetLev m_Val;
	CString m_CP;
	double m_Interest;
	double m_TotalInterest;

public:
	CFloatRepoData() { m_CP.Empty(); }
	CFloatRepoData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS)
				{ Setup(OraLoader, pSS, pSSS); }
	~CFloatRepoData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex = -1);

	CString &GetCP() { return m_CP; }
	void SetCP(LPCTSTR CP) { m_CP = CP; }

	BOOL IsReady();

	void LoadCP(COptComboBox &CP);
	void LoadDate(COptComboBox &Date);
	
	double GetInterest() { return m_Interest; }
	double GetTotalInterest() { return m_TotalInterest; }
	CAssetLev &GetVal() { return m_Val; }

    double ComputeInterest(LPCTSTR IntFrom, LPCTSTR IntTo, double Rate);
	double ComputeInterest(LPCTSTR IntFrom, LPCTSTR IntTo, LPCTSTR Rate);
	double ComputeTotalInterest();
	
	CFloatRepoRec &GetRec() { return m_Rec; }
	CFloatRepoInvRec &GetInvRec() { return m_InvRec; }

	virtual void LoadSheet();
	virtual int LoadTickets(int nOrder = 0);
	virtual BOOL AddSRow();
	virtual BOOL DeleteSRow(int CurrentRow = -1);
	virtual BOOL UpdateSRow();
	virtual void UpdateData(CFloatRepoRec *pRec, CFloatRepoInvRec *pInvRec);
};
#endif // __FLOATREPODATA_H__
