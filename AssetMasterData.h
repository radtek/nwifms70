#ifndef __ASSETMASTERDATA_H__
#define __ASSETMASTERDATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __ASSETMASTERTREC_H__
#include "assetmasterrec.h"
#endif

class CAssetMasterData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CAssetMasterData)
protected:
	CRowCtrl m_AssetClass;
	CAssetMasterRec m_AssetRec;
	CAssetMasterIntRec m_IntRec;
	CAssetMasterClassRec m_ClassRec;

	int AssetExist(LPCTSTR Asset, LPCTSTR AssetDesc = NULL);
	BOOL GetSelIntRec(int Index, CAssetMasterIntRec &IntRec);

public:
	CAssetMasterData() { }
	CAssetMasterData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS) : CMSRowCtrl(OraLoader, pSS, pSSS) { }
	~CAssetMasterData() {}
	
	virtual void LoadSheet();
	void SetHeaders();
	
	virtual BOOL AddSRow();
	virtual BOOL DeleteSRow(int CurrentRow = -1);
	virtual BOOL UpdateSRow();

	BOOL LoadDBData(LPCTSTR TicketNum);
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();

	CAssetMasterRec &GetAssetRec() { return m_AssetRec; }
	CAssetMasterIntRec &GetIntRec() { return m_IntRec; }
	CAssetMasterClassRec &GetClassRec() { return m_ClassRec; }

	BOOL GenerateFixedRates(CString &Asset, CString &AssetClass, CString &RateType, CString &Freq, 
						CString &Maturity, CString &IntToDate, CString &Rate);

	CRowCtrl &GetAssetClass() { return m_AssetClass; }
};
#endif // ASSETMASTERDATA_H