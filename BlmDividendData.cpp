#include "stdafx.h"
#include "NWIFMS70.h"
#include "BlmDividendData.h"
#include <math.h>
#include "qdata.h"

IMPLEMENT_DYNAMIC(CBlmDividendData, CKeyData)



IMPLEMENT_DYNAMIC(CBlmDividendDataArray, CObArray)
int	CBlmDividendDataArray::Add(CBlmDividendData *pSecData)
{
	int i, j = -1;
	CBlmDividendData *pData;

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

void CBlmDividendDataArray::SetAt(int Index, CBlmDividendData *pData)
{
	if(FreePointer(Index))
		CObArray::SetAt(Index, pData);
}

void CBlmDividendDataArray::InsertAt(int Index, CBlmDividendData *pData)
{
	if(Index >= 0 && Index < GetSize())
		CObArray::InsertAt(Index, pData);
}

CBlmDividendData *CBlmDividendDataArray::GetAt(int Index)
{
	if(Index < 0 || Index >= GetSize())
		return NULL;

	return (CBlmDividendData*) CObArray::GetAt(Index);
}

void CBlmDividendDataArray::RemoveAt(int Index)
{
	if(FreePointer(Index))
		CObArray::RemoveAt(Index);
}

void CBlmDividendDataArray::RemoveAll()
{
	for(int i = 0; i < GetSize(); i++)
		FreePointer(i);
    
    CObArray::RemoveAll();
}

void CBlmDividendDataArray::Copy(CBlmDividendDataArray &BlmDividendDataArray)
{
    RemoveAll();

	for(int i = 0; i < BlmDividendDataArray.GetSize(); i++)
        Add(*BlmDividendDataArray.GetAt(i));
}

BOOL CBlmDividendDataArray::IsModified()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i) && GetAt(i)->IsModified())
			return TRUE;
	
	return FALSE;
}

void CBlmDividendDataArray::ResetData()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i))
			GetAt(i)->IsModified() = FALSE;
}

int CBlmDividendDataArray::Find(const CString Key)
{
	int Start = 0, End, Idx;
	CBlmDividendData *pData;
	
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

BOOL CBlmDividendDataArray::FreePointer(int Index)
{
	CBlmDividendData *pData;

	pData = GetAt(Index);
	if(pData)
	{
		delete pData;
		return TRUE;
	}

	return FALSE;
}




CBlmDividend::CBlmDividend()
{
	GetFields().RemoveAll();
	GetFields().Add("DV010");
	GetFields().Add("DV018");
	GetFields().Add("DV019");
	GetFields().Add("DV034");
}

void CBlmDividend::BlmDateToOraDate(CString BlmDate, CString &OraDate)
// YYYY-MM-DD to MM/DD/YYYY
{
	OraDate = BlmDate.Left(7);
	OraDate = OraDate.Right(2) + "/";
	OraDate += BlmDate.Right(2) + "/";
	OraDate += BlmDate.Left(4);
}

void CBlmDividend::ProcessRefData(CString &security, Element element)
{
    int i;
	CString Field, sDate, Date;
	CBlmDividendData *pData;

	i = GetDividendDataArray().Find(security);

	if(i < 0)
		return;

	pData = GetDividendDataArray().GetAt(i);
	if(pData)
    {
	    for(i = 0; i < GetFields().GetSize(); i ++)
		{
			Field = GetFields().GetAt(i);
			if(element.hasElement(Field))
			{
				switch(i)
				{
					case 0: // Currency
						pData->GetCurrency() = element.getElementAsString(Field);
						break;
					case 1: // Exd_Date
						Date = element.getElementAsString(Field);
						BlmDateToOraDate(Date, sDate);
						pData->GetExdDate() = sDate;
						break;
					case 2:
						Date = element.getElementAsString(Field);
						BlmDateToOraDate(Date, sDate);
						pData->GetPayDate() = sDate;
						break;
					case 3:
						pData->GetDividend() = element.getElementAsFloat64(Field);
						break;
					default:
						break;
				}
			}
		}
	}
}

BOOL CBlmDividend::PrepareRefDataRequest(Request &request)
{
	int i;
	BOOL bRequested = FALSE;
	CBlmDividendData *pData;

	for (i = 0; i < GetDividendDataArray().GetSize(); i++)
	{
		pData = GetDividendDataArray().GetAt(i);
		if(pData)
		{
			request.append("securities", GetDividendDataArray().GetAt(i)->GetKey());
			bRequested = TRUE;
		}
	}

	if(bRequested)
	{
		for(i = 0; i < GetFields().GetSize(); i++)
			request.append("fields", GetFields().GetAt(i));
	}

	return bRequested;
}

int CBlmDividend::LoadAssets()
{
	CBlmDividendData Data;
	CBlmAssetData AssetData;
	CString AssetID, Key, OldKey;

    Data.GetDividend() = 0;

	GetDividendDataArray().RemoveAll();
	GetAssetDataArray().RemoveAll();
	GetOraLoader().Open("SELECT DISTINCT NVL(ASS_PID, NVL(ASS_MSTC_CODE, NVL(ASS_ISIN_CODE, ASS_SEDOL_NUM))) || ' EQUITY', A.ASSET_CODE, DECODE(SIGN(ABS(SUM(DECODE(A.TRANS_DIRECTION, 'P', 1, 'S', -1)*A.NOM_AMOUNT))), 1, 1, 1) "
						"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND NVL(ASS_PID, NVL(ASS_MSTC_CODE, NVL(ASS_ISIN_CODE, ASS_SEDOL_NUM))) IS NOT NULL "
						"AND B.ASS_INDUSTRY IN ('ADR EQUITIES', 'COUNTRY FUND', 'DOM EQTY-NEW', 'DOM EQUITIES', 'EQUITY FUNDS', 'EXCH TRADE FUND', 'INDEX RELATED') "
						"AND A.TRANS_TYPE IN ('SECURITIES') "
						"AND A.TRADE_DATE <= TRUNC(SYSDATE) "
						"GROUP BY A.PORTFOLIO, A.ASSET_CODE, NVL(ASS_PID, NVL(ASS_MSTC_CODE, NVL(ASS_ISIN_CODE, ASS_SEDOL_NUM))) "
						"HAVING ABS(SUM(DECODE(A.TRANS_DIRECTION, 'P', 1, 'S', -1)*A.NOM_AMOUNT)) > 0 "
						"ORDER BY 1, 2 ");
	
	while(!GetOraLoader().IsEOF())
	{
		Key = GetOraLoader().GetDBString(0);

		Data.GetKey() = Key;
		if(Key != OldKey)
		{
			AssetData.GetKey() = Key;
			AssetData.GetAssetID() = GetOraLoader().GetDBString(1);
			GetAssetDataArray().Add(AssetData);
			OldKey = Key;
		}

		if(GetDividendDataArray().Find(Data.GetKey()) < 0)
			GetDividendDataArray().Add(Data);

		GetOraLoader().MoveNext();
	}

	return GetDividendDataArray().GetSize(); 
}

void CBlmDividend::UpdateDividend()
{
	int i, j, Count;
	CString Asset, ExdDate, T;
	CBlmDividendData *pData;
	CBlmAssetData *pAssetData;
	CDBRec DBRec;
	CQData QData;

	// Setup data space
	for(i = 0; i < GetDividendDataArray().GetSize(); i++)
	{
		pData = GetDividendDataArray().GetAt(i);
		if(pData && pData->IsModified()) // Got prices
		{
			for(j = 0; j < GetAssetDataArray().GetSize(); j++)
			{
				pAssetData = GetAssetDataArray().GetAt(j);
				if(pAssetData)
				{
					if(pAssetData->GetKey() == pData->GetKey())
					{
						pAssetData->GetCurrency() = pData->GetCurrency();
						pAssetData->GetExdDate() = pData->GetExdDate();
						pAssetData->GetPayDate() = pData->GetPayDate();
						pAssetData->GetPrice() = pData->GetDividend();
					}
					else
						if(pAssetData->GetKey() > pData->GetKey())
							break;
				}
			}
		}
	}

	Count = 5;
	for(i = 0; i < Count; i ++)
		DBRec.Add("A" - 1 + 1);

	for(i = 0; i < GetAssetDataArray().GetSize(); i++)
	{
		pAssetData = GetAssetDataArray().GetAt(i);
		if(pAssetData)
		{
			Asset = pAssetData->GetAssetID();
			ExdDate = pAssetData->GetExdDate();
			DBRec.SetAt(0, Asset); // AssetCode;
			DBRec.SetAt(1, pAssetData->GetCurrency()); // Currency;
			DBRec.SetAt(2, ExdDate); // ExdDate
			DBRec.SetAt(3, pAssetData->GetPayDate()); // PayDate
			DBRec.SetAt(4, QData.WriteNumber(pAssetData->GetPrice(), FALSE, 12)); // Dividend

			Count = GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_DIVIDEND_SCHEDULE WHERE ASSET_CODE = '" + Asset + "' AND EXD_DATE = '" + ExdDate + "' ");
			if(Count > 0)
			{
				GetOraLoader().Open("SELECT ASSET_CODE, CURRENCY, EXD_DATE, PAY_DATE, DIVIDEND FROM SEMAM.NW_DIVIDEND_SCHEDULE "
									"WHERE ASSET_CODE = '" + Asset + "' AND EXD_DATE = '" + ExdDate + "' ", ODYNASET_DEFAULT);
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

BOOL CBlmDividend::LoadDividend(const CString RefDataServiceType)
{
	LoadAssets();

	if(BlmRefDataRequest(RefDataServiceType, 1))
	{
		UpdateDividend();
		return TRUE;
	}

	return FALSE;
}

