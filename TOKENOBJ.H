#ifndef __TOKENOBJ_H__
#define __TOKENOBJ_H__
                          
class CTokenObj : public CStringArray
{
DECLARE_DYNAMIC(CTokenObj)

private:
	CString		m_Token;

public:
	CTokenObj();
	CTokenObj(LPCTSTR TokenName);
	CTokenObj(CString& TokenName);
	CTokenObj(CTokenObj& TokenObj);
	virtual ~CTokenObj();

	void SetTokenName(LPCTSTR TokenName);
	void SetTokenName(CString& Token);
	void AddTokenInfo(LPCTSTR TokenInfo);
	void AddTokenInfo(CString& TokenInfo);
	void InsertTokenInfo(int Index, LPCTSTR TokenInfo);
	void InsertTokenInfo(int Index, CString& TokenInfo);
	void SetTokenInfoAt(int Index, LPCTSTR TokenInfo);
	void SetTokenInfoAt(int Index, CString& TokenInfo);

	CString& GetToken();
	BOOL GetTokenInfo(CString& String, int Index = 0);
	int  GetTokenInfoCount();
};

#endif // __TOKENOBJ_H__
