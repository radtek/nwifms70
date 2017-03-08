#ifndef __ASSETDATA_H__
#define __ASSETDATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __ASSETREC_H__
#include "assetrec.h"
#endif

class CAssetData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CAssetData)
protected:
	CAssetRec m_AssetRec;
	CAssetIntRec m_IntRec;
	CString m_InvType;

	int AssetExist(LPCTSTR Asset, LPCTSTR AssetDesc = NULL);
	BOOL GetSelIntRec(int Index, CAssetIntRec &IntRec);
	int YearAdj(int yy);

public:
	CAssetData() {}
	CAssetData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS) 
		: CMSRowCtrl(OraLoader, pSS, pSSS) { }
	~CAssetData() {}
	
	virtual void LoadSheet();
	void SetHeaders();
	
	void LoadHoldings(LPCTSTR Asset, CDBSheet &HoldingSS);

	virtual BOOL AddSRow();
	virtual BOOL DeleteSRow(int CurrentRow = -1);
	virtual BOOL UpdateSRow();

	BOOL LoadDBData(LPCTSTR TicketNum);
	BOOL ImportImagineAsset();

	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();

	BOOL GenerateFixedRates(CString &Asset, CString &AssetClass, CString &RateType, CString &Freq, 
						CString &Maturity, CString &IntToDate, CString &Rate);

	CAssetRec &GetAssetRec() { return m_AssetRec; }
	CAssetIntRec &GetIntRec() { return m_IntRec; }
	CString &GetInvType() { return m_InvType; }
};
#endif // ASSETDATA_H