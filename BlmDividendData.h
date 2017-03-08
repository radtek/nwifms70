#ifndef __BLMDIVIDENDDATA_H__
#define __BLMDIVIDENDDATA_H__

#ifndef __BLMASSETDATA_H__
#include "BlmAssetData.h"
#endif

#ifdef __KEYDATA_H__
#include "KeyData.H"
#endif

#ifndef __BLMREFDATAOBJ_H__
#include "BlmRefDataObj.h"
#endif

#ifndef __ORALOADER_H__
#include "ORALOADER.H"
#endif

// #define USE_BEMU_API

class CBlmDividendData : public CKeyData
{
DECLARE_DYNAMIC(CBlmDividendData)

protected:
    CString m_Currency;
	CString m_ExdDate;
	CString m_PayDate;
	double m_Dividend;

public:
	CBlmDividendData() {}
	CBlmDividendData(CBlmDividendData &Data) { GetKey() = Data.GetKey(); GetCurrency() = Data.GetCurrency(); GetExdDate() = Data.GetExdDate(); GetPayDate() = Data.GetPayDate(); GetDividend() = Data.GetDividend(); }
	~CBlmDividendData() {}

	CString &GetCurrency() { return m_Currency; }
	CString &GetExdDate() { return m_ExdDate; }
	CString &GetPayDate() { return m_PayDate; }
	double &GetDividend() { return m_Dividend; }
};



class CBlmDividendDataArray : public CObArray
{
DECLARE_DYNAMIC(CBlmDividendDataArray)

public:
	CBlmDividendDataArray() { }
	CBlmDividendDataArray(CBlmDividendDataArray &BlmDividendDataArray) { Copy(BlmDividendDataArray); }
	~CBlmDividendDataArray() { RemoveAll(); }

	int Add(CBlmDividendData *pData);
	int Add(CBlmDividendData &Data) { return Add(new CBlmDividendData(Data)); }
	void SetAt(int Index, CBlmDividendData *pData);
	void SetAt(int Index, CBlmDividendData &Data) { SetAt(Index, new CBlmDividendData(Data)); }
	void InsertAt(int Index, CBlmDividendData *pData);
	void InsertAt(int Index, CBlmDividendData &Data) { InsertAt(Index, new CBlmDividendData(Data)); }
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CBlmDividendDataArray &BlmDividendDatadArray);

	CBlmDividendData *GetAt(int Index);

	BOOL IsModified();
	void ResetData();
	int Find(const CString Key);

protected:
	BOOL FreePointer(int Index);
};



class CBlmDividend : public CBlmRefDataObj
{
protected:
	CBlmAssetDataArray m_AssetDataArray;
    CBlmDividendDataArray m_DividendDataArray;

public:
	CBlmDividend();
	~CBlmDividend() { }

	CBlmAssetDataArray &GetAssetDataArray() { return m_AssetDataArray; }
	CBlmDividendDataArray &GetDividendDataArray() { return m_DividendDataArray; }

	int LoadAssets();

	BOOL LoadDividend(const CString RefDataServiceType = "ReferenceDataRequest");
	virtual void ProcessRefData(CString &security, Element element);
    virtual BOOL PrepareRefDataRequest(Request &request);

protected:
	void BlmDateToOraDate(CString BlmDate, CString &OraDate);
	void UpdateDividend();
};
#endif