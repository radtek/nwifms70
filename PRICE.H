#ifndef __PRICE_H__
#define __PRICE_H__

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

/*#ifndef __BLMOBJ_H__
#include "blmobj.h"
#endif */

typedef struct
{
	BOOL m_bLock;
	BOOL m_bOverLimit;
	int  m_nOverUnderEqual;
} PriceStatus_t;

class CPriceData : public CDBRec
{   
DECLARE_DYNAMIC(CPriceData);
   
protected:
	BOOL m_Lock;
	CString m_BlmTick;
	CString m_BlmKey;

protected:
	BOOL GetVal(CString &PriceStr, int DataRange, double &Val);
	BOOL GetVal(LPCTSTR Price, int DataRange, double &Val);
	void SetData();

public:
	CPriceData();
	CPriceData(CPriceData &Price);           
	virtual ~CPriceData() {}
      
	LPCTSTR GetPrice() { return CDBRec::GetAt(2); }
	BOOL GetPriceVal(int DataRange, double &Price) { return GetVal(GetPrice(), DataRange, Price); }
	void SetPrice(LPCTSTR Price) { SetAt(2, (Price && strlen(Price) > 0) ? Price : ""); }
	void SetPrice(LPCTSTR Price, int DataRange);
    
	LPCTSTR GetPrevPrice() { return CDBRec::GetAt(1); }
	BOOL GetPrevPriceVal(int DataRange, double &PrevPrice) { return GetVal(GetPrevPrice(), DataRange, PrevPrice); }
	void SetPrevPrice(LPCTSTR Price) { SetAt(1, (Price && strlen(Price) > 0) ? Price : ""); }
	
	LPCTSTR GetAsset() { return CDBRec::GetAt(0); }
	void SetAsset(LPCTSTR Asset) { SetAt(0, (Asset && strlen(Asset) > 0) ? Asset : ""); }
	
	BOOL GetLock() { return m_Lock; }
	CString &GetBlmTick() { return m_BlmTick; }
	CString &GetBlmKey() { return m_BlmKey; }

	void SetLock(BOOL Lock = TRUE) { m_Lock = Lock; }
	void SetBlmTick(LPCTSTR BlmTick) { m_BlmTick = BlmTick; }
	void SetBlmKey(LPCTSTR BlmKey) { m_BlmKey = BlmKey; }
};



class CPriceDataArray : public CDBRecArray
{
DECLARE_DYNAMIC(CPriceDataArray);	

protected:
	CString m_Date;
	CString m_QDate;
	CString m_PrevDate;
	int	m_DataRange;
	BOOL m_bModified;
	BOOL m_bOptionValue;
	BOOL m_bTwo;
	double m_WarnLimit;
	CStdioFile m_ErrorFile;
	CStdioFile m_InFile;
	COraLoader m_OraLoader;

protected:
	BOOL ParsePriceData(LPCTSTR Data, CString &Asset, CString &Price, int PriceValue);
	void WriteError(LPCTSTR Text);
	void ProcessPriceData(CString &Asset, CString &Price, BOOL bSwap = FALSE);
	void ParseAssetData(LPCTSTR Data, CString &Asset, CString &ID);
	void ChangeChar(CString &Asset);
	CString OptValueToPrice(CString TransNum, CString OptValue);

public:
	CPriceDataArray() { SetWarnLimit(0); SetModify(FALSE); SetTwo(FALSE); }
	CPriceDataArray(COraLoader &OraLoader, LPCTSTR Date, double WarnLimit, int DataRange) 
					{ Setup(OraLoader, Date, WarnLimit, DataRange); }
	CPriceDataArray(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR FName, int DataRange)
					{ Setup(OraLoader, Date, FName, DataRange); }
	~CPriceDataArray() { RemoveAll(); }
	
	void SetWarnLimit(double WarnLimit) { m_WarnLimit = WarnLimit; }
	void SetModify(BOOL Modified) { m_bModified = Modified; }
	void SetModify();
	double GetWarnLimit() { return m_WarnLimit; }
	BOOL GetModify() { return m_bModified; }
	
	void SetTwo(BOOL bTwo) { m_bTwo = bTwo; }
	BOOL GetTwo() { return m_bTwo; }

	BOOL Setup(LPCTSTR Date, double m_WarnLimit, int DataRange);
	BOOL Setup(LPCTSTR Date, LPCTSTR FName, int DataRange);
	BOOL Setup(COraLoader &OraLoader, LPCTSTR Date, double m_WarnLimit, int DataRange);
	BOOL Setup(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR FName, int DataRange);
	BOOL SetupVisual(UINT SSSqlID, CDBSheet &SS);
	BOOL SetupVisual(CString &Sql, CDBSheet &SS);

	CString &GetDate() { return m_Date; }
	void SetDate(LPCTSTR Date) { m_Date = (Date && strlen(Date) > 0) ? Date : ""; }
	LPCTSTR GetQDate() { return (LPCTSTR) m_QDate; }
	void SetQDate(LPCTSTR QDate) { m_QDate = QDate; }

	void SetOptionValue(BOOL bOptionValue) { m_bOptionValue = bOptionValue; }
	BOOL IsOptionValue() { return m_bOptionValue; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { GetOraLoader() = OraLoader; }

	CStdioFile &GetErrorFile() { return m_ErrorFile; }
	CStdioFile &GetInFile() { return m_InFile; }
	void DisplayErrorFile();
	
	int	FindAt(LPCTSTR Asset) { return Find(Asset, 0); }
	CPriceData *GetAt(int Index) { return (CPriceData*) CDBRecArray::GetAt(Index); }
	CPriceData *GetAt(LPCTSTR Asset, BOOL bSwap = FALSE);
	void RemoveAll();
	void RemoveAt(int Index);
	void SetAt(int Index, CPriceData &Data);

	void SetDataRange(int DataRange = 1) { m_DataRange = DataRange; }
	int GetDataRange() { return m_DataRange; }

	void SetPrevDate(LPCTSTR PrevDate) { m_PrevDate = PrevDate; }
	LPCTSTR GetPrevDate(BOOL bFromDB = FALSE, LPCTSTR Sql = NULL);	

	void UpdateAssetPos(UINT DelSqlID, UINT UpdtSqlID);
	void UpdateAssetPos(LPCTSTR DelSql, LPCTSTR UpdtSql);
	int LoadPriceDataArray(CString &Sql, BOOL bPrice = FALSE);
	int LoadBlmPriceArray(int MenuID);
	BOOL OpenInsertSql(UINT SqlID);
	BOOL OpenInsertSql(LPCTSTR Sql);
	void FillPriceData(CString &Asset, BOOL bPrice = FALSE);
	BOOL DeletePriceData(UINT SqlID);
	BOOL DeletePriceData(LPCTSTR Sql);
	void InsertPriceData();
	void InsertOptionData(UINT SqlID);
	void InsertOptionData(LPCTSTR Sql);
	void InsertRiskFreeRateData(UINT SqlID);
	void InsertRiskFreeRateData(LPCTSTR Sql);
	void InsertSwapData(UINT SqlID);
	void InsertSwapData(LPCTSTR Sql);
	void InsertDV01Data(LPCTSTR Sql);

	void UpdateDV01Data(LPCTSTR Sql, LPCTSTR Sql2);
	void UpdatePriceData(UINT SqlID, BOOL bCost = FALSE);
	void UpdatePriceData(LPCTSTR Sql, BOOL bCost = FALSE);
	void UpdateSwapData(UINT SqlID);
	void UpdateSwapData(LPCTSTR Sql);
	void UpdateOptionData(UINT SqlID);
	void UpdateOptionData(LPCTSTR Sql);
	void UpdateRiskFreeRateData(UINT SqlID);
	void UpdateRiskFreeRateData(LPCTSTR Sql);
	void FillFilePriceData(CDBRec &Rec, CPriceData *pPriceData);
	void UpdatePriceInfo();
	void LoadFileData(BOOL bPrice = FALSE, BOOL bSwap = FALSE);

	BOOL ComputePriceChange(CPriceData &PriceData, double &Result, PriceStatus_t &Status);
};
#endif // __PRICE_H__