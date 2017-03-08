#include "stdafx.h"
#include "NWIFMS70.h"
#include "BlmRiskData.h"
#include <math.h>
#include "qdata.h"

IMPLEMENT_DYNAMIC(CBlmRiskData, CKeyData)



IMPLEMENT_DYNAMIC(CBlmRiskDataArray, CObArray)
int	CBlmRiskDataArray::Add(CBlmRiskData *pSecData)
{
	int i, j = -1;
	CBlmRiskData *pData;

	if(!pSecData)
		return -1;

	for(i = 0; i < GetSize(); i++)
	{
		pData = GetAt(i);
		if(pData)
		{
			if(pData->GetKey() == pSecData->GetKey())
				return i;
			else
				if(pData->GetKey() < pSecData->GetKey())
					j = i;
				else
					if(pData->GetKey() > pSecData->GetKey())
					{
						if(j < 0)
							InsertAt(0, pSecData);
						else
							InsertAt(j, pSecData);
						return j;
					}
		}
	}
	
	return CObArray::Add(pSecData);
}

void CBlmRiskDataArray::SetAt(int Index, CBlmRiskData *pData)
{
	if(FreePointer(Index))
		CObArray::SetAt(Index, pData);
}

void CBlmRiskDataArray::InsertAt(int Index, CBlmRiskData *pData)
{
	if(Index >= 0 && Index < GetSize())
		CObArray::InsertAt(Index, pData);
}

CBlmRiskData *CBlmRiskDataArray::GetAt(int Index)
{
	if(Index < 0 || Index >= GetSize())
		return NULL;

	return (CBlmRiskData*) CObArray::GetAt(Index);
}

void CBlmRiskDataArray::RemoveAt(int Index)
{
	if(FreePointer(Index))
		CObArray::RemoveAt(Index);
}

void CBlmRiskDataArray::RemoveAll()
{
	for(int i = 0; i < GetSize(); i++)
		FreePointer(i);
    
    CObArray::RemoveAll();
}

void CBlmRiskDataArray::Copy(CBlmRiskDataArray &BlmRiskDataArray)
{
    RemoveAll();

	for(int i = 0; i < BlmRiskDataArray.GetSize(); i++)
        Add(*BlmRiskDataArray.GetAt(i));
}

BOOL CBlmRiskDataArray::IsModified()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i) && GetAt(i)->IsModified())
			return TRUE;
	
	return FALSE;
}

void CBlmRiskDataArray::ResetData()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i))
			GetAt(i)->IsModified() = FALSE;
}

int CBlmRiskDataArray::Find(const CString Key)
{
	int Start = 0, End, Idx;
	CBlmRiskData *pData;
	
	if(GetSize() <= 0) // EmptyArray
		return -1;
	
	End = GetSize() - 1;

	for(;;)
	{
		switch(End - Start)
		{
			case 0:
				pData = GetAt(Start);
				if(pData && pData->GetKey() == Key)
					return Start;
				else
					return -1;
				break;
			case 1:
				pData = GetAt(Start);
				if(pData && pData->GetKey() == Key)
					return Start;
				else
				{
					pData = GetAt(End);
					if(pData && pData->GetKey() == Key)
						return End;
					else
						return -1;
				}
				break;
			default:
				Idx = (Start + End)/2;
				pData = GetAt(Idx);
				if(pData)
				{
					if(pData->GetKey() == Key)
						return Idx;
					else
						if(pData->GetKey() < Key)
							Start = Idx;
						else
							End = Idx;
				}
				break;
		}
	}

	return -1;
}

BOOL CBlmRiskDataArray::FreePointer(int Index)
{
	CBlmRiskData *pData;

	pData = GetAt(Index);
	if(pData)
	{
		delete pData;
		return TRUE;
	}

	return FALSE;
}




CBlmRisk::CBlmRisk()
{
	GetOraLoader().SetDB(&theDB);
}

void CBlmRisk::ProcessRefData(CString &security, Element element)
{
    int i, Count;
	CString Field, ExdDate;
	CBlmRiskData *pData;
	CQData QData;
	CDBRec DBRec;

	i = GetRiskDataArray().Find(security);

	if(i < 0)
		return;

	pData = GetRiskDataArray().GetAt(i);
	if(pData)
    {
	    for(i = 0; i < GetFieldArray().GetSize(); i ++)
		{
			Field = GetFieldArray().GetAt(i);
			if(element.hasElement(Field))
			{
				switch(i)
				{
					case 0: // Currency
						pData->GetCurrency() = element.getElementAsString(Field);
						break;
					case 1: // Exd_Date
						ExdDate = element.getElementAsString(Field);
						pData->GetExdDate() = ExdDate;
						break;
					case 2:
						pData->GetPayDate() = element.getElementAsString(Field);
						break;
					case 3:
						pData->GetDividend() = element.getElementAsFloat64(Field);
						break;
					default:
						break;
				}
			}

			if(pData->GetDividend() > 0)
			{
				DBRec.Add(pData->GetKey());
				DBRec.Add(pData->GetCurrency());
				DBRec.Add(pData->GetExdDate());
				DBRec.Add(pData->GetPayDate());
				DBRec.Add(QData.WriteNumber(pData->GetDividend(), FALSE, 9));
			}
		}

		if(DBRec.GetSize() > 0)
		{
			Count = GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_DIVIDEND_SCHEDULE WHERE ASSET_CODE = '" + pData->GetKey() + "' AND EXD_DATE = '" + ExdDate + "' ");
			if(Count > 0)
			{
				GetOraLoader().Open("SELECT ASSET_CODE, CURRENCY, EXD_DATE, PAY_DATE, DIVIDEND FROM SEMAM.NW_DIVIDEND_SCHEDULE WHERE ASSET_CODE = '" + pData->GetKey() + "' AND EXD_DATE = '" + ExdDate + "' ", ODYNASET_DEFAULT);
				GetOraLoader().UpdateRecord(DBRec);
			}
			else
			{
				GetOraLoader().Open("SELECT ASSET_CODE, CURRENCY, EXD_DATE, PAY_DATE, DIVIDEND FROM SEMAM.NW_DIVIDEND_SCHEDULE", ODYNASET_DEFAULT);
				GetOraLoader().UpdateRecord(DBRec, TRUE);
			}
		}
	}
}

BOOL CBlmRisk::PrepareRefDataRequest(Request &request)
{
	int i;
	BOOL bRequested = FALSE;
	CBlmRiskData *pData;

	GetFieldArray().RemoveAll();
	GetFieldArray().Add("DV010");
	GetFieldArray().Add("DV018");
	GetFieldArray().Add("DV019");
	GetFieldArray().Add("DV034");

	for (i = 0; i < GetRiskDataArray().GetSize(); i++)
	{
		pData = GetRiskDataArray().GetAt(i);
		if(pData)
		{
			request.append("securities", GetRiskDataArray().GetAt(i)->GetTicker());
			bRequested = TRUE;
		}
	}

	if(bRequested)
	{
		for(i = 0; i < GetFieldArray().GetSize(); i++)
			request.append("fields", GetFieldArray().GetAt(i));
	}

	return bRequested;
}

int CBlmRisk::LoadAssets()
{
	CBlmRiskData Data;

    Data.GetDividend() = 0;

	GetRiskDataArray().RemoveAll();
	GetOraLoader().Open("SELECT DISTINCT A.ASSET_CODE, NVL(ASS_PID, NVL(ASS_MSTC_CODE, NVL(ASS_ISIN_CODE, ASS_SEDOL_NUM))) || ' EQUITY' "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B "
						"WHERE B.ASSET_CODE = A.ASSET_CODE "
						"AND B.ASS_CODE = A.ASSET_CODE "
						"AND B.ASS_INDUSTRY IN ('ADR EQUITIES', 'COUNTRY FUND', 'DOM EQTY-NEW', 'DOM EQUITIES', 'EQUITY FUNDS', 'EXCH TRADE FUND', 'INDEX RELATED') "
						"AND A.TRANS_TYPE IN ('SECURITIES') "
						"AND A.TRADE_DATE <= TRUNC(SYSDATE) "
						"GROUP BY A.PORTFOLIO, A.ASSET_CODE "
						"HAVING ABS(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)) > 0 "
						"ORDER BY 1 ");
	
	while(!GetOraLoader().IsEOF())
	{
		Data.GetKey() = GetOraLoader().GetDBString(0);
		Data.GetTicker() = GetOraLoader().GetDBString(1);
		if(GetRiskDataArray().Find(Data.GetKey()) < 0 && !Data.GetTicker().IsEmpty())
			GetRiskDataArray().Add(Data);

		GetOraLoader().MoveNext();
	}

	return GetRiskDataArray().GetSize(); 
}

BOOL CBlmRisk::LoadRisk(const CString RefDataServiceType)
{
	LoadAssets();
	return BlmRefDataRequest(RefDataServiceType, 1);
}

