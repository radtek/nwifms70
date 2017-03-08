#include "stdafx.h"
#include "nwifms70.h"
#include "resource.h"
#include "tokenobj.h"
#include "rletter.h"
#include "qdata.h"
#include "sqlstmt.h"


IMPLEMENT_DYNAMIC(CLetter, CObArray)
void CLetter::AddToken(CTokenObj& Token)
{
	int Index;
	
	Index = FindToken(Token);
	
	if (Index > -1)
		SetTokenAt(Index, Token);
	else
		CObArray::Add(new CTokenObj(Token));
}

int CLetter::FindToken(LPCTSTR Token)
{
	int i;
	
	for (i = 0; i < GetTokenCount(); i ++)
		if (GetToken(i)->GetToken().Compare(Token) == 0)
			return i;
  
	return -1;
}

int CLetter::FindToken(CTokenObj& Token)
{
	int i;
	
	for (i = 0; i < GetTokenCount(); i ++)
		if (GetToken(i)->GetToken() == Token.GetToken())
			return i;
    
	return -1;
}

void CLetter::InsertToken(int Index, CTokenObj& Token)
{
	int      	i;
        
	i = FindToken(Token);
	if (i > -1)
		SetTokenAt(i, Token);
	else	
		CObArray::InsertAt(Index, new CTokenObj(Token));
}

void CLetter::SetTokenAt(int Index, CTokenObj& Token)
{
	CTokenObj* OldToken;	

    OldToken = GetToken(Index);
    if (OldToken != NULL)
    {
		CObArray::SetAt(Index, new CTokenObj(Token));		
		delete OldToken;
	}
}

CTokenObj* CLetter::GetToken(int Index)
{
	CTokenObj* Token = NULL;
	
	if (Index > -1 && Index < GetTokenCount())
		Token = (CTokenObj*) CObArray::GetAt(Index);
  
	return Token;
}

int CLetter::GetTokenCount()
{
	return CObArray::GetSize();
}

void CLetter::RemoveTokenAt(int Index)
{
	CTokenObj* Token;
  
	Token = GetToken(Index);
	if (Token)
	{
 		CObArray::RemoveAt(Index);
		delete Token; 		
	}
	
	CObArray::FreeExtra();   
}

void CLetter::RemoveAllToken()
{
	int  i = CObArray::GetSize();

    while(i)
    {
    	i--;
    	RemoveTokenAt(i);
    }
}

void CLetter::ClearAllTokenInfo()
{
	int 	   i;
	CTokenObj* Token;
    
    for (i = 0; i < GetTokenCount(); i ++)
    {
    	Token = GetToken(i);
    	if (Token)
         	Token->RemoveAll();
    }
}




IMPLEMENT_DYNAMIC(CCustodyLetter, CLetter)
int CCustodyLetter::FindLetter(LPCTSTR Custodian)
{
	char Buffer[200];

	if(!m_InF.ReadString(Buffer, sizeof(Buffer)))
		return 0;

	return strstr(Buffer, Custodian) ? 1 : -1;
}

void CCustodyLetter::SkipLetter()
{
	while(FindLetter("ENDOFLETTER") == -1);
}

void CCustodyLetter::GenerateOutput(LPCTSTR Custodian)
{
	char		Buffer[200], Buffer1[80];
	int			Index;
	int			i;
	int			Limit;
	char		*p, *q;
	CTokenObj*	Token;
	CString		sBuf;
	char*		TokenChars = "[]";

	while(FindLetter(Custodian) == -1)
		SkipLetter();

	do
	{
		while(m_InF.ReadString(Buffer, sizeof(Buffer)) && !strstr(Buffer, "ENDOFLETTER"))
		{
    		p = strtok_s(Buffer, TokenChars, &q);
    		while (p)
    		{
      			Index = FindToken(p);
      			if(Index < 0)
      				m_OutF.Write(p, strlen(p));
      			else
      			{
        			Token = GetToken(Index);
        			Limit = Token->GetTokenInfoCount();
        			for(i = 0; i < Limit; i ++)
        			{
        				if(Limit > 1)
        				{
        					sprintf_s(Buffer1, "%2d.", i+1);
        					m_OutF.Write(Buffer1, strlen(Buffer1));
						}
    			
        				Token->GetTokenInfo(sBuf, i);
        				m_OutF.Write(sBuf, sBuf.GetLength());
        				if (Limit > 1)
						m_OutF.Write("\n", 1);						        			
		   			}
				}  
      		
				p = strtok_s(NULL, TokenChars, &q);
    		}
		}
        
		m_OutF.Write("\f", 1);
	} while(FindLetter(Custodian) == 1);
}  

void CCustodyLetter::FillRunDate()
{
	CTokenObj Token("RUN DATE");	
	Token.AddTokenInfo(m_RunDate);
	
	AddToken(Token);
}

void CCustodyLetter::FillProcessDate()
{
	CTokenObj Token("PROCESS DATE");
	Token.AddTokenInfo(m_Date);
	
	AddToken(Token);
}

int CCustodyLetter::GetMaxFieldLength(CStringArray& FieldArray)
{
	int MaxLength = 0;
	int i;

	for(i = 0; i < FieldArray.GetSize(); i++)
		if((int) strlen(FieldArray.GetAt(i)) > MaxLength)
			MaxLength = strlen(FieldArray.GetAt(i));
	
	return MaxLength;
}

void CCustodyLetter::ProcessAsset(CTokenObj& AssetToken, CStringArray& CountryArray, 
								  CStringArray& AssetArray, CStringArray& CurrArray, 
								  CStringArray& NomArray, CStringArray& OtherArray)
{
	char AssetDesc[200];
	CString Format;
	int MaxCountryLen, MaxAssetLen, MaxNomLen;
	int i;

	MaxCountryLen = GetMaxFieldLength(CountryArray);
	MaxAssetLen = GetMaxFieldLength(AssetArray);
	MaxNomLen = GetMaxFieldLength(NomArray);
	
	Format.Format("%%-%d.%ds\t%%-%d.%ds\t%%s %%%d.%ds\t%%s", MaxCountryLen, MaxCountryLen, 
					MaxAssetLen, MaxAssetLen, MaxNomLen, MaxNomLen);
	for(i = 0; i < CountryArray.GetSize(); i++)
	{
		sprintf_s(AssetDesc, (LPCTSTR) Format, (LPCTSTR) CountryArray.GetAt(i), (LPCTSTR) AssetArray.GetAt(i), 
				(LPCTSTR) CurrArray.GetAt(i), (LPCTSTR) NomArray.GetAt(i), (LPCTSTR) OtherArray.GetAt(i));
		AssetToken.AddTokenInfo(AssetDesc);
		AddToken(AssetToken);
	}
	
	CountryArray.RemoveAll();
	AssetArray.RemoveAll();
	CurrArray.RemoveAll();
	NomArray.RemoveAll();
	OtherArray.RemoveAll();
}

int CCustodyLetter::Generate(COraLoader &OraLoader)
{
	CTokenObj PortToken("PORTFOLIO"), CustToken("CUSTODIAN"), AssetToken("ASSET...");	
	CString	Buffer, Port, OriPort, Custodian, OriCustodian;
	CStringArray CountryArray, AssetArray, CurrArray, NomArray, OtherArray;
	LPCTSTR QDate;
	CQData QData;
	CSqlStmt Sql;

	OriPort.Empty();
	m_InF.Open((LPCTSTR) m_Template, CFile::modeRead);
	m_OutF.Open("Fundcust.doc", CFile::modeCreate | CFile::modeWrite);

    FillRunDate();
    FillProcessDate();
 
	QDate = QData.GetQueryDate(m_Date); 
	OraLoader.GetSql().Format("SELECT A.PORTFOLIO, A.CUSTODIAN, ASS_COUNTRY, ASS_DESC, CURRENCY, "
						"SUM(NOM_AMOUNT) \"AMT\", '' \"T_TYPE\", TRANS_NUM "
						"FROM SEMAM.NW_CUSTREP A, SEMAM.NW_ASSETS B "
						"WHERE B.ASS_CODE = A.ASSET "
						"GROUP BY A.PORTFOLIO, A.CUSTODIAN, ASS_COUNTRY, ASS_DESC, CURRENCY, TRANS_NUM  "
						"HAVING (ROUND(SUM(NOM_AMOUNT), 0) != 0) "
						"UNION ALL "
						"SELECT A.PORTFOLIO, A.CUSTODIAN, ASS_COUNTRY, ASS_DESC, CURRENCY, SUM(REPO_AM), 'REPO', TRANS_NUM "
						"FROM SEMAM.NW_CUSTREP A, SEMAM.NW_ASSETES B "
						"WHERE B.ASS_CODE = A.ASSET "
						"GROUP BY A.PORTFOLIO, A.CUSTODIAN, ASS_COUNTRY, ASS_DESC, CURRENCY, TRANS_NUM "
						"HAVING (ROUND(SUM(REPO_AM), 0) != 0) "
						"UNION ALL "
						"SELECT A.PORTFOLIO, A.COUNTERPARTY, ASS_COUNTRY, ASS_DESC, NVL(X.NEW_CURRENCY, X.CURRENCY), "
						"SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT/DECODE(ASS_TYPE, 'BOND TYP(%%) CAL', NVL(X.CONVERSION, 1), 1)), "
						"'FORWADS', A.TRANS_NUM "
						"FROM SEMAM.ALL_TICKET_INV A, SEMAM.NW_ASSETS B, SEMAM.NW_COUNTRIES X "
						"WHERE C.ASS_CODE = A.ASSET_CODE "
						"AND X.CURRENCY = A.CURRENCY "
						"AND A.TRANS_TYPE IN ('SECURITIES') AND A.DIR IN ('P', 'S') "
						"AND C.ASS_CONV_TYPE = 'F' AND (A.ACTUAL_CLOSING IS NULL OR A.ACTUAL_CLOSING > %s) "
						"AND A.TRADE_DATE <= %s "
						"GROUP BY A.PORTFOLIO, A.COUNTERPARTY, ASS_COUNTRY, ASS_DESC, NVL(X.NEW_CURRENCY, X.CURRENCY), A.TRANS_NUM "
						"HAVING (SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT) != 0) "
						"UNION ALL "
						"SELECT A.PORTFOLIO, A.COUNTERPARTY, ASS_COUNTRY, ASS_DESC, NVL(X.NEW_CURRENCY, X.CURRENCY), "
						"SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT/DECODE(ASS_TYPE, 'BOND TYP(%%) CAL', NVL(X.CONVERSION, 1), 1)), "
						"'FORWADS', A.TRANS_NUM "
						"FROM SEMAM.ALL_TICKET_INV A, SEMAM.NW_ASSETS B, SEMAM.NW_COUNTRIES X "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND X.CURRENCY = A.CURRENCY "
						"AND A.TRANS_TYPE IN ('SECURITIES') AND A.DIR IN ('P', 'S') "
						"AND C.ASS_INDUSTRY = 'CURRENCY FWDS' "
						"AND (A.ACTUAL_CLOSING IS NULL OR A.ACTUAL_CLOSING > %s) "
						"AND A.TRADE_DATE <= %s "
						"GROUP BY A.PORTFOLIO, A.COUNTERPARTY, ASS_COUNTRY, ASS_DESC, NVL(X.NEW_CURRENCY, X.CURRENCY), A.TRANS_NUM "
						"HAVING (SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT) != 0) "
						"ORDER BY 1, 2, 3, 4 ", QDate, QDate, QDate, QDate);
	if(!OraLoader.Open())
		return -1;
	
	while(!OraLoader.IsEOF())
	{
		Port = OraLoader.GetDBString(0); // Portfolio
		Custodian = OraLoader.GetDBString(1); // Custodian

		if(Port != OriPort || Custodian != OriCustodian)
		{
			m_InF.SeekToBegin();
			if(!OriPort.IsEmpty())
			{
				ProcessAsset(AssetToken, CountryArray, AssetArray, CurrArray, NomArray, OtherArray);
				GenerateOutput(OriCustodian);
			}

			if(Port != OriPort)
			{
				PortToken.RemoveAll();
				OriPort = Port;
				PortToken.AddTokenInfo(Port);
				AddToken(PortToken);				
			}

			AssetToken.RemoveAll();
			OriCustodian = Custodian;

			CustToken.RemoveAll();						 
			CustToken.AddTokenInfo(Custodian);
			AddToken(CustToken);
		}

		// AssetDesc
		CountryArray.Add(OraLoader.GetDBString(2)); // Country				
		AssetArray.Add(OraLoader.GetDBString(3)); // Asset		
		
		Buffer = OraLoader.GetDBString(4); // Currency
		if(Buffer.GetLength() > 3)
			Buffer.Format("%3.3s", ((LPCTSTR) Buffer) + 3);
		CurrArray.Add(Buffer);
		
		NomArray.Add(OraLoader.GetDBString(5)); // NomAmount					
		OtherArray.Add(OraLoader.GetDBString(6)); // TransType

		OraLoader.MoveNext();
	}

	if(OraLoader.GetRecordCount() > 0)
	{
		m_InF.SeekToBegin();
		ProcessAsset(AssetToken, CountryArray, AssetArray, CurrArray, NomArray, OtherArray);
		GenerateOutput(OriCustodian);
	}

	m_InF.Close();
	m_OutF.Close();    
	return OraLoader.GetRecordCount();
}
