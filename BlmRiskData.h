#ifndef __BLMRISKDATA_H__
#define __BLMRISKDATA_H__

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

class CBlmRiskData : public CKeyData
{
DECLARE_DYNAMIC(CBlmRiskData)

protected:
	CString m_Ticker;
    CString m_Currency;
	CString m_ExdDate;
	CString m_PayDate;
	double m_Dividend;

public:
	CBlmRiskData() {}
	CBlmRiskData(CBlmRiskData &Data) { GetKey() = Data.GetKey(); GetCurrency() = Data.GetCurrency(); GetExdDate() = Data.GetExdDate(); GetPayDate() = Data.GetPayDate(); GetDividend() = Data.GetDividend(); }
	~CBlmRiskData() {}

	CString &GetTicker() { return m_Ticker; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetExdDate() { return m_ExdDate; }
	CString &GetPayDate() { return m_PayDate; }
	double &GetDividend() { return m_Dividend; }
};



class CBlmRiskDataArray : public CObArray
{
DECLARE_DYNAMIC(CBlmRiskDataArray)

public:
	CBlmRiskDataArray() { }
	CBlmRiskDataArray(CBlmRiskDataArray &BlmRiskDataArray) { Copy(BlmRiskDataArray); }
	~CBlmRiskDataArray() { RemoveAll(); }

	int Add(CBlmRiskData *pData);
	int Add(CBlmRiskData &Data) { return Add(new CBlmRiskData(Data)); }
	void SetAt(int Index, CBlmRiskData *pData);
	void SetAt(int Index, CBlmRiskData &Data) { SetAt(Index, new CBlmRiskData(Data)); }
	void InsertAt(int Index, CBlmRiskData *pData);
	void InsertAt(int Index, CBlmRiskData &Data) { InsertAt(Index, new CBlmRiskData(Data)); }
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CBlmRiskDataArray &BlmRiskDatadArray);

	CBlmRiskData *GetAt(int Index);

	BOOL IsModified();
	void ResetData();
	int Find(const CString Key);

protected:
	BOOL FreePointer(int Index);
};



class CBlmRisk : public CBlmRefDataObj
{
protected:
 	COraLoader m_OraLoader;
	CStringArray m_FieldArray;
    CBlmRiskDataArray m_RiskDataArray;

public:
	CBlmRisk();
	~CBlmRisk() { }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	CStringArray &GetFieldArray() { return m_FieldArray; }
	CBlmRiskDataArray &GetRiskDataArray() { return m_RiskDataArray; }

	int LoadAssets();

	BOOL LoadRisk(const CString RefDataServiceType = "ReferenceDataRequest");
	virtual void ProcessRefData(CString &security, Element element);
    virtual BOOL PrepareRefDataRequest(Request &request);
};
#endif