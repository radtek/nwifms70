#ifndef __BLMPRICEDATA_H__
#define __BLMPRICEDATA_H__

#ifndef __BLMASSETDATA_H__
#include "blmassetdata.h"
#endif

#ifndef __BLMFXDATA_H__
#include "BLMFXData.H"
#endif

#define BLM_LOAD_CURRENCY 2
#define BLM_LOAD_SECURITIES 3
#define BLM_LOAD_OPTION 4
#define BLM_LOAD_IRS 5

// #define USE_BEMU_API

class CBlmMktField : public CKeyData 
{
DECLARE_DYNAMIC(CBlmMktField)
protected:
	CString m_BlmField;
	CString m_BlmField2;
    CString m_BlmSubField;

public:
	CBlmMktField() { }
	CBlmMktField(CBlmMktField &Field) { Copy(Field); }
	~CBlmMktField() {}

	void Copy(CBlmMktField &Field);
	CString &GetBlmField() { return m_BlmField; }
	CString &GetBlmField2() { return m_BlmField2; }
	CString &GetBlmSubField() { return m_BlmSubField; } 
};



class CBlmMktFieldArray : public CObArray
{
DECLARE_DYNAMIC(CBlmMktFieldArray)
public:
	CBlmMktFieldArray() {}
	CBlmMktFieldArray(CBlmMktFieldArray &BlmMktFieldArray) { Copy(BlmMktFieldArray); }
	~CBlmMktFieldArray() { RemoveAll(); }

	int Add(CBlmMktField *pField);
	int Add(CBlmMktField &Field) { return Add(new CBlmMktField(Field)); }
	void SetAt(int Index, CBlmMktField *pField);
	void SetAt(int Index, CBlmMktField &Field) { SetAt(Index, new CBlmMktField(Field)); }
	void InsertAt(int Index, CBlmMktField *pField);
	void InsertAt(int Index, CBlmMktField &Field) { InsertAt(Index, new CBlmMktField(Field)); }
	void RemoveAt(int Index);
	void RemoveAll();

	int Find(const CString Key);
	void Copy(CBlmMktFieldArray &BlmMktFieldArray);

	CBlmMktField *GetAt(int Index);

	BOOL IsModified();
	BOOL FreePointer(int Index);
};


class CBlmPrice : public CBlmRefDataObj
{
protected:
	CString m_Date;
    int m_nLoadType;
    CString m_AssetClass;
	CBlmAssetDataArray m_AssetDataArray;
    CBlmFxDataArray m_FxDataArray;
	CBlmMktFieldArray m_MktFields;

public:
	CBlmPrice() { Setup(); }
	~CBlmPrice() { }

	void Setup();
	CBlmMktFieldArray &GetMktFields() { return m_MktFields; }
	CString &GetDate() { return m_Date; }
	CBlmAssetDataArray &GetAssetDataArray() { return m_AssetDataArray; }
    BOOL LoadLivePrices(int nLoadType, const CString RefDataServiceType = "ReferenceDataRequest");
	CBlmFxDataArray &GetFxDataArray() { return m_FxDataArray; }
	virtual void ProcessRefData(CString &security, Element element);
    virtual BOOL PrepareRefDataRequest(Request &request);
	void UpdatePrices();
	int &GetLoadType() { return m_nLoadType; }

protected:
	void UpdateFxrates();
	int LoadAssets();
	int LoadMktFields();
    int LoadFxData();
	void SetupLoadAssetSql(CString &Sql);
	void SetupPriceSql(CString &Sql);
	BOOL LoadPricingCurrency(const CString RefDataServiceType = "ReferenceDataRequest");
};

#endif