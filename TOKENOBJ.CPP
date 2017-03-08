#include "stdafx.h"
#include "tokenobj.h"

IMPLEMENT_DYNAMIC(CTokenObj, CStringArray)

CTokenObj::CTokenObj()
          :CStringArray() 
{
	m_Token = "";
}
   
CTokenObj::CTokenObj(CTokenObj& TokenObj) 
          :CStringArray()
{  
	SetTokenName(TokenObj.GetToken());
   
	int i;
	CString String;

	for(i = 0; i < TokenObj.GetTokenInfoCount(); i++)
	{     
		TokenObj.GetTokenInfo(String, i);
		AddTokenInfo(String);
	}  
}
           
CTokenObj::CTokenObj(LPCTSTR TokenName) 
          :CStringArray()
{  
	SetTokenName(TokenName);
}

CTokenObj::CTokenObj(CString& TokenName) 
          :CStringArray()
{  
	SetTokenName(TokenName);
}
           
CTokenObj::~CTokenObj() 
{
}   

void CTokenObj::SetTokenName(LPCTSTR Token)
{
	if(Token && strlen(Token) > 0)
		m_Token = Token;
}

void CTokenObj::SetTokenName(CString& Token)
{
	if(!Token.IsEmpty())
		m_Token = Token;
}

void CTokenObj::AddTokenInfo(LPCTSTR TokenInfo)
{
	if(TokenInfo && strlen(TokenInfo) > 0)
		CStringArray::Add(TokenInfo);
}

void CTokenObj::AddTokenInfo(CString& TokenInfo)
{
	if(!TokenInfo.IsEmpty())
		CStringArray::Add((LPCTSTR) TokenInfo);
}

void CTokenObj::SetTokenInfoAt(int Index, LPCTSTR TokenInfo)
{
	if(TokenInfo && Index >= 0 && Index < GetTokenInfoCount())
		CStringArray::SetAt(Index, TokenInfo);
}

void CTokenObj::SetTokenInfoAt(int Index, CString& TokenInfo)
{
	if(!TokenInfo.IsEmpty() && Index >= 0 && Index < GetTokenInfoCount())
		CStringArray::SetAt(Index, (LPCTSTR) TokenInfo);
}

void CTokenObj::InsertTokenInfo(int Index, LPCTSTR TokenInfo)
{
	if(TokenInfo && strlen(TokenInfo) > 0 && Index >= 0)
		CStringArray::InsertAt(Index, TokenInfo);
}

void CTokenObj::InsertTokenInfo(int Index, CString& TokenInfo)
{
	if(!TokenInfo.IsEmpty() && Index >= 0)
		CStringArray::InsertAt(Index, (LPCTSTR) TokenInfo);
}

CString& CTokenObj::GetToken()
{
	return m_Token;
}

BOOL CTokenObj::GetTokenInfo(CString& String, int Index)
{
	if(Index >= 0 && Index < GetTokenInfoCount())
	{
		String = CStringArray::GetAt(Index);
		return TRUE;
	}
   
	return FALSE;  
}

int CTokenObj::GetTokenInfoCount()
{
	return CStringArray::GetSize();
}
