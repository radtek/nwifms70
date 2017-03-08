#ifndef __RLETTER_H__
#define __RLETTER_H__

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __TOKENOBJ_H__
#include "tokenobj.h"
#endif

class CLetter : public CObArray
{
DECLARE_DYNAMIC(CLetter)

public:
	CLetter() {}
	~CLetter() { RemoveAllToken(); }
       
	void AddToken(CTokenObj& Token);
	int	FindToken(LPCTSTR Token);
	int	FindToken(CTokenObj& Token);
	CTokenObj* GetToken(int Index);
	int	GetTokenCount();
	void InsertToken(int Index, CTokenObj& Token);
	void RemoveTokenAt(int Index);
	void ClearAllTokenInfo();
	void RemoveAllToken();
	void SetTokenAt(int Index, CTokenObj& Token);

	virtual BOOL GetInput() { return TRUE; }
	virtual int	FillTokenArray() { return 1; }
	virtual void GenerateOutput() {}
};

class CCustodyLetter : public CLetter
{  

DECLARE_DYNAMIC(CCustodyLetter)
public:
	CStdioFile m_InF;
	CFile m_OutF;
	CString m_Date;
	CString	m_RunDate;
	CString	m_Template;

protected:
	int GetMaxFieldLength(CStringArray& FieldArray);
	void ProcessAsset(CTokenObj& AssetToken, CStringArray& CountryArray, 
								  CStringArray& AssetArray, CStringArray& CurrArray, 
								  CStringArray& NomArray, CStringArray& OtherArray);

public:
	CCustodyLetter() {}
    ~CCustodyLetter() {}

	void FillRunDate();
	void FillProcessDate();
	void SkipLetter();
	int FindLetter(LPCTSTR Custodian);
	int Generate(COraLoader &OraLoader);
	void GenerateOutput(LPCTSTR Custodian);
};
#endif // __RLETTER_H__