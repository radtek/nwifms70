#include "stdafx.h"
#include "NWIFMS70.h"
#include "BlmFXData.h"
#include <math.h>
#include "qdata.h"

IMPLEMENT_DYNAMIC(CBlmFxData, CKeyData)



IMPLEMENT_DYNAMIC(CBlmFxDataArray, CObArray)
int	CBlmFxDataArray::Add(CBlmFxData *pSecData)
{
	int i, j = -1;
	CBlmFxData *pData;

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

void CBlmFxDataArray::SetAt(int Index, CBlmFxData *pBlmSec)
{
	if(FreePointer(Index))
		CObArray::SetAt(Index, pBlmSec);
}

void CBlmFxDataArray::InsertAt(int Index, CBlmFxData *pSecData)
{
	if(Index >= 0 && Index < GetSize())
		CObArray::InsertAt(Index, pSecData);
}

CBlmFxData *CBlmFxDataArray::GetAt(int Index)
{
	if(Index < 0 || Index >= GetSize())
		return NULL;

	return (CBlmFxData*) CObArray::GetAt(Index);
}

void CBlmFxDataArray::RemoveAt(int Index)
{
	if(FreePointer(Index))
		CObArray::RemoveAt(Index);
}

void CBlmFxDataArray::RemoveAll()
{
	for(int i = 0; i < GetSize(); i++)
		FreePointer(i);
    
    CObArray::RemoveAll();
}

void CBlmFxDataArray::Copy(CBlmFxDataArray &BlmFxDataArray)
{
    RemoveAll();

	for(int i = 0; i < BlmFxDataArray.GetSize(); i++)
        Add(*BlmFxDataArray.GetAt(i));
}

BOOL CBlmFxDataArray::IsModified()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i) && GetAt(i)->IsModified())
			return TRUE;
	
	return FALSE;
}

void CBlmFxDataArray::ResetData()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i))
			GetAt(i)->IsModified() = FALSE;
}

int CBlmFxDataArray::Find(const CString Key)
{
	int Start = 0, End, Idx;
	CBlmFxData *pData;
	
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

double CBlmFxDataArray::GetFxrate(const CString Currency)
{
	int i;

	i = Find(Currency);
	if(i >= 0)
		return GetAt(i)->GetFxrate();

	return 0;
}

BOOL CBlmFxDataArray::FreePointer(int Index)
{
	CBlmFxData *pData;

	pData = GetAt(Index);
	if(pData)
	{
		delete pData;
		return TRUE;
	}

	return FALSE;
}




CBlmFxrate::CBlmFxrate()
{
	GetOraLoader().Today(m_Date);
}

void CBlmFxrate::ProcessRefData(CString &security, Element element)
{
    int i;
	CString Field;
	CBlmFxData *pFxData;
	CQData QData;
	CDBRec DBRec;

	i = GetFxDataArray().Find(security);

	if(i < 0)
		return;

	Field = "PX_LAST";

	pFxData = GetFxDataArray().GetAt(i);
	if(pFxData)
    {
	    if(element.hasElement(Field))
		{
			pFxData->GetFxrate() = element.getElementAsFloat64(Field);

			if(pFxData->IsRev() && pFxData->GetFxrate() > 0)
				pFxData->GetFxrate() = 1/pFxData->GetFxrate();
			
			if(pFxData->GetFxrate() > 0)
			{
				DBRec.Add(GetDate());
				DBRec.Add(pFxData->GetCurrency());
				DBRec.Add(QData.WriteNumber(pFxData->GetFxrate(), FALSE, 9));
			}
		}

		if(DBRec.GetSize() > 0)
			GetOraLoader().UpdateRecord(DBRec, TRUE);
	}
}

BOOL CBlmFxrate::PrepareRefDataRequest(Request &request)
{
	int i;
	BOOL bRequested = FALSE;
	CBlmFxData *pFxData;

	for (i = 0; i < GetFxDataArray().GetSize(); i++)
	{
		pFxData = GetFxDataArray().GetAt(i);
		if(pFxData)
		{
			request.append("securities", GetFxDataArray().GetAt(i)->GetKey());
			bRequested = TRUE;
		}
	}

	if(bRequested)
		request.append("fields", "PX_LAST");

	return bRequested;
}

int CBlmFxrate::LoadCurrencys()
{
	CBlmFxData Data;

    Data.GetFxrate() = 0;

	GetFxDataArray().RemoveAll();
	GetOraLoader().Open("SELECT A.CURRENCY, B.REV FROM SEMAM.NW_EXRATES A, SEMAM.NW_CURRENCY B "
						"WHERE B.CURRENCY = A.CURRENCY "
						"AND A.INDATE = (SELECT MAX(INDATE) FROM SEMAM.NW_EXRATES C) "
						"ORDER BY 1 ");
	
	while(!GetOraLoader().IsEOF())
	{
		Data.GetKey() = GetOraLoader().GetDBString(0) + " CURNCY";
		if(GetFxDataArray().Find(Data.GetKey()) < 0)
		{
			Data.GetCurrency() = GetOraLoader().GetDBString(0);
			Data.IsRev() = GetOraLoader().GetDBString(1) == "Y" ? TRUE : FALSE;

			GetFxDataArray().Add(Data);
		}

		GetOraLoader().MoveNext();
	}

	return GetFxDataArray().GetSize(); 
}

BOOL CBlmFxrate::LoadLiveExrates(const CString RefDataServiceType)
{
	if(GetOraLoader().Open("SELECT INDATE, CURRENCY, EXRATE FROM SEMAM.NW_EXRATES ", ODYNASET_DEFAULT))
		return BlmRefDataRequest(RefDataServiceType, 1);
	else
		return FALSE;
}

