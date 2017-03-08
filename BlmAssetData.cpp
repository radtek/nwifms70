#include "stdafx.h"
#include "NWIFMS70.h"
#include "BlmAssetData.h"
#include <math.h>
#include "qdata.h"


IMPLEMENT_DYNAMIC(CBlmAssetData, CKeyData)
void CBlmAssetData::Copy(CBlmAssetData &Data)
{
	GetKey() = Data.GetKey();
	GetAssetID() = Data.GetAssetID();
	GetCurrency() = Data.GetCurrency();
	GetExdDate() = Data.GetExdDate();
	GetPayDate() = Data.GetPayDate();
	GetPrice() = Data.GetPrice();
    GetWtFactor() = Data.GetWtFactor();
}



IMPLEMENT_DYNAMIC(CBlmAssetDataArray, CObArray)
void CBlmAssetDataArray::Copy(CBlmAssetDataArray &BlmAssetDataArray)
{
    RemoveAll();
	for(int i = 0; i < BlmAssetDataArray.GetSize(); i++)
        Add(*BlmAssetDataArray.GetAt(i));
}

BOOL CBlmAssetDataArray::FreePointer(int Index)
{
	CBlmAssetData *pData;

	pData = GetAt(Index);
	if(pData)
	{
		delete pData;
		return TRUE;
	}

	return FALSE;
}

int	CBlmAssetDataArray::Add(CBlmAssetData *pField)
{
	int i, j = -1;
	CBlmAssetData *pData;

	if(!pField)
		return -1;

	for(i = 0; i < GetSize(); i++)
	{
		pData = GetAt(i);
		if(pData)
		{
			if(pData->GetKey() == pField->GetKey())
				return i;
			else
				if(pData->GetKey() < pField->GetKey())
					j = i;
				else
					if(pData->GetKey() > pField->GetKey())
					{
						if(j < 0)
							InsertAt(0, pField);
						else
							InsertAt(j, pField);
						return j;
					}
		}
	}
	
	return CObArray::Add(pField);
}

void CBlmAssetDataArray::SetAt(int Index, CBlmAssetData *pData)
{
	if(FreePointer(Index))
		CObArray::SetAt(Index, pData);
}

void CBlmAssetDataArray::InsertAt(int Index, CBlmAssetData *pData)
{
	if(Index >= 0 && Index < GetSize())
		CObArray::InsertAt(Index, pData);
}

CBlmAssetData *CBlmAssetDataArray::GetAt(int Index)
{
	if(Index < 0 || Index >= GetSize())
		return NULL;

	return (CBlmAssetData*) CObArray::GetAt(Index);
}

void CBlmAssetDataArray::RemoveAt(int Index)
{
	if(FreePointer(Index))
		CObArray::RemoveAt(Index);
}

void CBlmAssetDataArray::RemoveAll()
{
	for(int i = 0; i < GetSize(); i++)
		FreePointer(i);
    
    CObArray::RemoveAll();
}

