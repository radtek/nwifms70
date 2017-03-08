#ifndef __BLMASSETDATA_H__
#define __BLMASSETDATA_H__

#ifndef __BLMFXDATA_H__
#include "BLMFXData.H"
#endif

#define BLM_LOAD_CURRENCY 2
#define BLM_LOAD_SECURITIES 3
#define BLM_LOAD_OPTION 4
#define BLM_LOAD_IRS 5

class CBlmAssetData : public CObject
{
DECLARE_DYNAMIC(CBlmAssetData)
protected:
	CString m_Key;
	CString m_AssetID;
	CString m_Currency;
	CString m_ExdDate;
	CString m_PayDate;
    double m_Price;
	double m_WtFactor;

public:
	CBlmAssetData() { }
	CBlmAssetData(CBlmAssetData &Data) { Copy(Data); }
	~CBlmAssetData() {}

	void Copy(CBlmAssetData &Data);
	CString &GetKey() { return m_Key; }
	CString &GetAssetID() { return m_AssetID; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetExdDate() { return m_ExdDate; }
	CString &GetPayDate() { return m_PayDate; }
	double &GetPrice() { return m_Price; } 
	double &GetWtFactor() { return m_WtFactor; } 
};

class CBlmAssetDataArray : public CObArray
{
DECLARE_DYNAMIC(CBlmAssetDataArray)
public:
	CBlmAssetDataArray() {}
	CBlmAssetDataArray(CBlmAssetDataArray &BlmAssetDataArray) { Copy(BlmAssetDataArray); }
	~CBlmAssetDataArray() { RemoveAll(); }

	int Add(CBlmAssetData *pData);
	int Add(CBlmAssetData &Data) { return Add(new CBlmAssetData(Data)); }
	void SetAt(int Index, CBlmAssetData *pData);
	void SetAt(int Index, CBlmAssetData &Data) { SetAt(Index, new CBlmAssetData(Data)); }
	void InsertAt(int Index, CBlmAssetData *pData);
	void InsertAt(int Index, CBlmAssetData &Data) { InsertAt(Index, new CBlmAssetData(Data)); }
	void RemoveAt(int Index);
	void RemoveAll();

	void Copy(CBlmAssetDataArray &CBlmAssetDataArray);

	CBlmAssetData *GetAt(int Index);

	BOOL FreePointer(int Index);
};

#endif