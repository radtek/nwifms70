#ifndef __ASSETRATEDATA_H__
#define __ASSETRATEDATA_H__

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class CAssetRateData
{
protected:
	BOOL m_bNew;
	int	m_DataRange;
	CString m_Asset;
	CString m_FromDate;
	CString m_ToDate;
	CString m_Rate;
	CString m_Amort;
	CString m_CapRate;
	CString m_SetDate;
	CStdioFile m_ErrorFile;
	CStdioFile m_InFile;
	COraLoader m_OraLoader;

protected:
	void ParseAssetRateData(const CString Data);
	void WriteError(LPCTSTR Text);
	BOOL SaveAssetRateData();
	void ToDBRec(CDBRec &Rec);

public:
	CAssetRateData();
	CAssetRateData(LPCTSTR FName, int DataRange); 
	~CAssetRateData() {}
	
	BOOL Setup(LPCTSTR FName, int DataRange);

	COraLoader &GetOraLoader() { return m_OraLoader; }

	CStdioFile &GetErrorFile() { return m_ErrorFile; }
	CStdioFile &GetInFile() { return m_InFile; }
	void DisplayErrorFile();
	
	BOOL IsNew() { return m_bNew; }
	BOOL IsValid();

	void SetDataRange(int DataRange = 1) { m_DataRange = DataRange; }
	int GetDataRange() { return m_DataRange; }

	void ProcessData();
};
#endif