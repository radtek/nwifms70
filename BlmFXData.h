#ifndef __BLMFXDATA_H__
#define __BLMFXDATA_H__

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

class CBlmFxData : public CKeyData
{
DECLARE_DYNAMIC(CBlmFxData)

protected:
    CString m_Currency;
    BOOL m_bRev;
    double m_Fxrate;

public:
	CBlmFxData() {}
	CBlmFxData(CBlmFxData &FxData) { GetKey() = FxData.GetKey(); GetCurrency() = FxData.GetCurrency(); IsRev() = FxData.IsRev(); GetFxrate() = FxData.GetFxrate(); }
	~CBlmFxData() {}

	CString &GetCurrency() { return m_Currency; }
	BOOL &IsRev() { return m_bRev; }
	double &GetFxrate() { return m_Fxrate; }
};



class CBlmFxDataArray : public CObArray
{
DECLARE_DYNAMIC(CBlmFxDataArray)

public:
	CBlmFxDataArray() { }
	CBlmFxDataArray(CBlmFxDataArray &BlmFxDataArray) { Copy(BlmFxDataArray); }
	~CBlmFxDataArray() { RemoveAll(); }

	int Add(CBlmFxData *pData);
	int Add(CBlmFxData &FxData) { return Add(new CBlmFxData(FxData)); }
	void SetAt(int Index, CBlmFxData *pData);
	void SetAt(int Index, CBlmFxData &FxData) { SetAt(Index, new CBlmFxData(FxData)); }
	void InsertAt(int Index, CBlmFxData *pData);
	void InsertAt(int Index, CBlmFxData &FxData) { InsertAt(Index, new CBlmFxData(FxData)); }
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CBlmFxDataArray &BlmFxDatadArray);

	CBlmFxData *GetAt(int Index);

	double GetFxrate(const CString Currency); 

	BOOL IsModified();
	void ResetData();
	int Find(const CString Key);

protected:
	BOOL FreePointer(int Index);
};



class CBlmFxrate : public CBlmRefDataObj
{
protected:
	CString m_Date;
    CBlmFxDataArray m_FxDataArray;

public:
	CBlmFxrate();
	~CBlmFxrate() { }

	CString &GetDate() { return m_Date; }
	CBlmFxDataArray &GetFxDataArray() { return m_FxDataArray; }

	int LoadCurrencys();

	BOOL LoadLiveExrates(const CString RefDataServiceType = "ReferenceDataRequest");
	virtual void ProcessRefData(CString &security, Element element);
    virtual BOOL PrepareRefDataRequest(Request &request);
};
#endif