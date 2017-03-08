#include "stdafx.h"
#include "NWIFMS70.h"
#include "BlmPricedata.h"
#include <math.h>
#include "qdata.h"


const CString BLM_CURRENCYOPT = "CURRENCY OPTIONOPT";


IMPLEMENT_DYNAMIC(CBlmMktField, CKeyData)
void CBlmMktField::Copy(CBlmMktField &BlmField)
{
	GetKey() = BlmField.GetKey();
	GetBlmField() = BlmField.GetBlmField();
	GetBlmField2() = BlmField.GetBlmField2();
    GetBlmSubField() = BlmField.GetBlmSubField();
}



IMPLEMENT_DYNAMIC(CBlmMktFieldArray, CObArray)
void CBlmMktFieldArray::Copy(CBlmMktFieldArray &BlmMktFieldArray)
{
    RemoveAll();
	for(int i = 0; i < BlmMktFieldArray.GetSize(); i++)
        Add(*BlmMktFieldArray.GetAt(i));
}

BOOL CBlmMktFieldArray::FreePointer(int Index)
{
	CBlmMktField *pData;

	pData = GetAt(Index);
	if(pData)
	{
		delete pData;
		return TRUE;
	}

	return FALSE;
}

int	CBlmMktFieldArray::Add(CBlmMktField *pField)
{
	int i, j = -1;
	CBlmMktField *pData;

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

void CBlmMktFieldArray::SetAt(int Index, CBlmMktField *pField)
{
	if(FreePointer(Index))
		CObArray::SetAt(Index, pField);
}

void CBlmMktFieldArray::InsertAt(int Index, CBlmMktField *pField)
{
	if(Index >= 0 && Index < GetSize())
		CObArray::InsertAt(Index, pField);
}

CBlmMktField *CBlmMktFieldArray::GetAt(int Index)
{
	if(Index < 0 || Index >= GetSize())
		return NULL;

	return (CBlmMktField*) CObArray::GetAt(Index);
}

void CBlmMktFieldArray::RemoveAt(int Index)
{
	if(FreePointer(Index))
		CObArray::RemoveAt(Index);
}

void CBlmMktFieldArray::RemoveAll()
{
	for(int i = 0; i < GetSize(); i++)
		FreePointer(i);
    
    CObArray::RemoveAll();
}

BOOL CBlmMktFieldArray::IsModified()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i) && GetAt(i)->IsModified())
			return TRUE;
	
	return FALSE;
}

int CBlmMktFieldArray::Find(const CString Key)
{
	int Start = 0, End, Idx;
	CBlmMktField *pData;
	
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





void CBlmPrice::Setup()
{
	GetOraLoader().Today(m_Date);
	LoadMktFields();
	LoadFxData();
}

int CBlmPrice::LoadMktFields()
{
    CBlmMktField Field;
	CString f2;

   	GetOraLoader().Open("SELECT ASSET_CLASS, BLM_FIELD, BLM_FIELD2, BLM_SUB_FIELD "
                        "FROM SEMAM.NW_BLM_FIELD "
						"WHERE BLM_FIELD IS NOT NULL "
                        "UNION "
						"SELECT ASSET_CLASS||'OPT', BLM_OPT_FIELD, BLM_OPT_FIELD2, BLM_OPT_SUB_FIELD "
						"FROM SEMAM.NW_BLM_FIELD "
                        "WHERE BLM_OPT_FIELD IS NOT NULL "
                        "ORDER BY 1 ");
    
	GetMktFields().RemoveAll();
	while(!GetOraLoader().IsEOF())
	{
        Field.GetKey() = GetOraLoader().GetDBString(0);
        Field.GetBlmField() = GetOraLoader().GetDBString(1);

		f2 = GetOraLoader().GetDBString(2);
		if(!f2.IsEmpty() && f2 != Field.GetBlmField())
			Field.GetBlmField2() = f2;
		else
			Field.GetBlmField2().Empty();
		
		if(!GetOraLoader().GetDBString(3).IsEmpty())
			Field.GetBlmSubField() = GetOraLoader().GetDBString(3); 
		else
			Field.GetBlmSubField().Empty();
        
		GetMktFields().Add(Field);
        GetOraLoader().MoveNext();
	}

	return GetMktFields().GetSize();
}

void CBlmPrice::SetupLoadAssetSql(CString &Sql)
{
	Sql.Empty();
	
	switch(GetLoadType())
	{
		case BLM_LOAD_SECURITIES:
			Sql = "SELECT DISTINCT A.ASSET_CLASS, A.BLM_TICK, A.ASSET_ID, A.CURRENCY, B.MARKET, REV, 'N', 1 "
					"FROM SEMAM.NW_DAILY_HOLDINGS A, SEMAM.NW_TODAY_PRICES B "
					"WHERE B.ASSET(+) = A.ASSET_CODE "
					"AND A.ASSET_CLASS NOT IN ('CURRENCY FWDS') "
					"AND A.TRANS_TYPE IN ('CDS', 'SECURITIES') "
					"AND A.BLM_TICK IS NOT NULL "
					"UNION "
					"SELECT DISTINCT DECODE(SIGN(LENGTH(A.BLM_TICK) - 13), 1, A.ASSET_CLASS, 'CURRENCY SPOT') \"ASSET_CLASS\", "
					"A.BLM_TICK, A.ASSET_ID, A.CURRENCY, B.MARKET, REV, 'N', 1 "
					"FROM SEMAM.NW_DAILY_HOLDINGS A, SEMAM.NW_TODAY_PRICES B "
					"WHERE B.ASSET(+) = A.ASSET_CODE "
					"AND A.ASSET_CLASS IN ('CURRENCY FWDS') "
					"AND A.TRANS_TYPE IN ('SECURITIES') "
					"AND A.BLM_TICK IS NOT NULL "
					"UNION "
					"SELECT DISTINCT DECODE(ASSET_CLASS, 'CURRENCY OPTION', 'CURRENCY SPOT', A.ASSET_CLASS), A.BLM_TICK, A.ASSET_CODE, A.CURRENCY, "
					"B.MARKET, REV, NULL, 1 "
					"FROM SEMAM.NW_DAILY_HOLDINGS A, SEMAM.NW_TODAY_PRICES B "
					"WHERE B.ASSET(+) = A.ASSET_CODE "
					"AND A.TRANS_TYPE IN ('CALL', 'PUT')  "
					"AND A.BLM_TICK IS NOT NULL "
					"ORDER BY 1, 2, 3 ";
			break;

		case BLM_LOAD_OPTION:
			Sql = "SELECT DISTINCT A.ASSET_CLASS||'OPT', A.BLM_OPT_TICK, A.ASSET_ID, A.CURRENCY, B.MARKET, NULL, 'Y', 1 "
					"FROM SEMAM.NW_DAILY_HOLDINGS A, SEMAM.NW_TODAY_PRICES B "
					"WHERE B.ASSET(+) = A.ASSET_CODE "
					"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
					"AND A.BLM_OPT_TICK IS NOT NULL "
					"ORDER BY 1, 2, 3 ";
			break;
		case BLM_LOAD_IRS:
			Sql = "SELECT DISTINCT A.ASSET_CLASS, A.BLM_TICK, A.ASSET_ID, A.CURRENCY, B.MARKET, REV, 'Y', DECODE(C.PORTFOLIO, NULL, 1, A.NOM_AMOUNT/C.NOM_AMOUNT) "
					"FROM SEMAM.NW_DAILY_HOLDINGS A, SEMAM.NW_TODAY_PRICES B, SEMAM.NW_DAILY_HOLIDNGS C "
					"WHERE B.ASSET(+) = A.ASSET_CODE "
					"AND C.TICKET_NUM(+) = A.TICKET_NUM "
					"AND C.ASSET_CODE(+) = A.ASSET_CODE "
					"AND C.PORTFOLIO(+) = 'EMCF_B' "
					"AND A.TRANS_TYPE IN ('INT. SWAP') "
					"AND A.BLM_TICK IS NOT NULL "
					"ORDER BY 1, 2, 3 ";
			break;
		default:
			return;
	}
}

void CBlmPrice::SetupPriceSql(CString &Sql)
{
	Sql.Empty();
	
	switch(GetLoadType())
	{
		case BLM_LOAD_SECURITIES:
			Sql = "SELECT PR_DATE, ASSET, MARKET FROM SEMAM.NW_MARKET_PRICES ";
			break;
		case BLM_LOAD_OPTION:
			Sql = "SELECT MM_DATE, TRANS_NUM, MARKET_MARK FROM SEMAM.NW_OPT_PRICES ";
			break;
		case BLM_LOAD_IRS:
			Sql = "SELECT INDATE, TRANS_NUM, VALUE, TICKET_NUM, ASSET_CODE, TR_DIR, TR_TYPE FROM SEMAM.NW_DEAL_NAV ";
			break;
		default:
			return;
	}
}

int CBlmPrice::LoadAssets()
{
	CBlmSec Data;
	CBlmAssetData AssetData;
	OValue Value;
	CString AssetID, Key, OldKey;

	Data.IsModified() = FALSE;
	Data.GetPrice() = 0;
	Data.GetFxrate() = 1;

	SetupLoadAssetSql(GetOraLoader().GetSql());

	if(GetOraLoader().GetSql().IsEmpty())
		return 0;
	
	if(!GetOraLoader().Open(GetOraLoader().GetSql()))
		return 0;

	GetBlmSecArr().RemoveAll();
	GetAssetDataArray().RemoveAll();
	while(!GetOraLoader().IsEOF())
	{
		Data.GetAssetClass() = GetOraLoader().GetDBString(0);
		Data.GetTicker() = GetOraLoader().GetDBString(1);
		Data.GetKey() = Data.GetAssetClass() + Data.GetTicker();
		AssetID = GetOraLoader().GetDBString(2);
		Key = Data.GetAssetClass() + Data.GetTicker() + AssetID;

		if(Key != OldKey)
		{
			AssetData.GetKey() = Key;
			AssetData.GetAssetID() = AssetID;
			GetOraLoader().GetFieldValue(7, &Value);
			AssetData.GetWtFactor() = (double) Value; // WtFactor
			GetAssetDataArray().Add(AssetData);
			OldKey = Key;
		}

		if(GetBlmSecArr().Find(Data.GetKey()) < 0) // Add new ticker only
		{
			Data.GetFxIdx() = -1;
			if(Data.GetAssetClass() == BLM_CURRENCYOPT)
			{
				if(Data.GetBlmCurr().IsEmpty())
					Data.GetBlmCurr() = GetOraLoader().GetDBString(3);
			}
			else
			{
				Data.GetBlmCurr() = GetOraLoader().GetDBString(3);
				Data.GetFxIdx() = GetFxDataArray().Find(Data.GetBlmCurr() + " CURNCY");
			}
		
			GetOraLoader().GetFieldValue(4, &Value);
			if(Value.IsNull())
				Data.GetPrevPrice() = 0;
			else
				Data.GetPrevPrice() = (double) Value;
			Data.IsRev() = GetOraLoader().GetDBString(5) == "Y" ? TRUE : FALSE;
			Data.IsUSDPrice() = GetOraLoader().GetDBString(6) == "Y" ? TRUE : FALSE;

			GetBlmSecArr().Add(Data);
		}

		GetOraLoader().MoveNext();
	}

	return GetBlmSecArr().GetSize();
}

void CBlmPrice::ProcessRefData(CString &security, Element element)
{
    int i;
	CString Field, OldField;
	CBlmSec *pSecData;

	i = GetBlmSecArr().Find(m_AssetClass + security);

	if(i < 0)
		return;

    switch(m_nLoadType)
	{
		case BLM_LOAD_SECURITIES:
		case BLM_LOAD_OPTION:
		case BLM_LOAD_IRS:
			pSecData = GetBlmSecArr().GetAt(i);
			for(int j = 0; j < GetFields().GetSize(); j++)
			{
				Field = GetFields().GetAt(j);
				if(!Field.IsEmpty() && Field != OldField && element.hasElement(Field))
				{
					OldField = Field;
					pSecData->GetPrice() = element.getElementAsFloat64(Field);

					if(pSecData->IsRev() && fabs(pSecData->GetPrice()) > 0)
						pSecData->GetPrice() = 1/pSecData->GetPrice();
					pSecData->IsModified() = TRUE;
					return;
				}
			}
			break;
		case BLM_LOAD_CURRENCY:
			Field = "FX061";
			pSecData = GetBlmSecArr().GetAt(i);
			if(pSecData && element.hasElement(Field))
			{
				pSecData->GetBlmCurr() = element.getElementAsString(Field);
				pSecData->GetFxIdx() = GetFxDataArray().Find(security);
			}
			break;
		default:
			break;
	}
}

BOOL CBlmPrice::PrepareRefDataRequest(Request &request)
{
	int i;
	BOOL bRequested = FALSE;
	CBlmSec *pSecData;

    switch(m_nLoadType)
	{
		case BLM_LOAD_SECURITIES:
		case BLM_LOAD_OPTION:
		case BLM_LOAD_IRS:
			CBlmMktField *pField;
			for(i = 0; i < GetBlmSecArr().GetSize(); i++)
			{
				pSecData = GetBlmSecArr().GetAt(i);
				if(pSecData && !pSecData->GetTicker().IsEmpty() && pSecData->GetAssetClass() == m_AssetClass) 
				{
					if((m_AssetClass != "CURRENCY SPOT" && m_AssetClass != "DM SWAPSOPT" && m_AssetClass != "EM SWAPSOPT")) // cannot do swaption yet
					{
						if(pSecData->GetTicker() != "CT781269 CORP" && pSecData->GetTicker() != "DIGITAL CURNCY") // cannot do digit option
						{
							request.append("securities", pSecData->GetTicker());
							bRequested = TRUE;
						}
					}
				}
			}

			if(bRequested)
			{
				i = GetMktFields().Find(m_AssetClass);
				if(i >= 0)
				{
					pField = GetMktFields().GetAt(i);
					if(pField)
					{
						GetFields().RemoveAll();
						
						GetFields().Add(pField->GetBlmField());
						request.append("fields", pField->GetBlmField());
					}

					if(!pField->GetBlmField2().IsEmpty()) 
					{
						GetFields().Add(pField->GetBlmField2());
						request.append("fields", pField->GetBlmField2()); 
					}
				}

				if(m_AssetClass == BLM_CURRENCYOPT) // CURRENCY OPTION
				{
					Element overrides = request.getElement("overrides");
			
					Element ovrdPercentageSel = overrides.appendElement();
					ovrdPercentageSel.setElement(FIELD_ID, "PIPS_PERCENTAGE_SELECTION");
					ovrdPercentageSel.setElement("value", "PERC"); //accepts either "C" for calls or "P" for puts

					Element ovrdValuation = overrides.appendElement();
					ovrdValuation.setElement(FIELD_ID, "FXOPT_VALUATION_CONVENTION");
					ovrdValuation.setElement("value", "M"); //accepts a positive integer //
				}
			}
			break;
		case BLM_LOAD_CURRENCY:
			for(i = 0; i < GetBlmSecArr().GetSize(); i++)
			{
				pSecData = GetBlmSecArr().GetAt(i);
				// CURRENCY OPTION; 

				if(pSecData)
				{
					if(pSecData->GetAssetClass() == m_AssetClass)
					{
						if(!pSecData->GetTicker().IsEmpty() && pSecData->GetBlmCurr().IsEmpty())
						{
							request.append("securities", pSecData->GetTicker());
							bRequested = TRUE;
						}
					}
					else
						if(pSecData->GetAssetClass() > m_AssetClass)
							break;
				}
			}

			if(bRequested)
				request.append("fields", "FX061");

			break;
		default:
			break;
	}

	return bRequested;
}

int CBlmPrice::LoadFxData()
{
	CBlmFxData Data;
	OValue Value;

	GetFxDataArray().RemoveAll();
	GetOraLoader().Open("SELECT A.CURRENCY, EXRATE FROM SEMAM.NW_EXRATES A "
						"WHERE INDATE = '" + GetDate() + "' "
						"ORDER BY 1 ");
	
	while(!GetOraLoader().IsEOF())
	{
		Data.GetKey() = GetOraLoader().GetDBString(0) + " CURNCY";
		if(GetFxDataArray().Find(Data.GetKey()) < 0)
		{
			Data.GetCurrency() = GetOraLoader().GetDBString(0);
			GetOraLoader().GetFieldValue(1, &Value);
			if(!Value.IsNull())
				Data.GetFxrate() = (double) Value;
			else
				Data.GetFxrate() = 0;

			GetFxDataArray().Add(Data);
		}

		GetOraLoader().MoveNext();
	}

	return GetFxDataArray().GetSize(); 
}

BOOL CBlmPrice::LoadPricingCurrency(const CString RefDataServiceType)
{
	m_nLoadType = BLM_LOAD_CURRENCY;
	m_AssetClass = BLM_CURRENCYOPT;

	return BlmRefDataRequest(RefDataServiceType, 500);
}

void CBlmPrice::UpdateFxrates()
{
	int i, j;
	CBlmSec *pData;
	CBlmFxData *pFxData;

	for(i = 0; i < GetBlmSecArr().GetSize(); i++)
	{
		pData = GetBlmSecArr().GetAt(i);
		if(pData)
		{
			switch(GetLoadType())
			{
				case BLM_LOAD_SECURITIES:
					if(pData->GetAssetClass() == "CURRENCY SPOT")
					{
						j = GetFxDataArray().Find(pData->GetTicker());
						if(j >= 0)
						{
							pFxData = GetFxDataArray().GetAt(j);
							if(pFxData && pFxData->GetFxrate() > 0)
							{
								pData->GetFxrate() = pFxData->GetFxrate();
								pData->GetPrice() = 1/pData->GetFxrate();
							}
						}
					}
					break;
				case BLM_LOAD_OPTION:
				case BLM_LOAD_IRS:
					if(pData->GetBlmCurr() != USD)
					{
						if(pData->GetFxIdx() >= 0)
						{
							pData->GetFxrate() = GetFxDataArray().GetAt(pData->GetFxIdx())->GetFxrate();
							if(pFxData && pData->GetFxrate() > 0)
								pData->GetFxrate() = pFxData->GetFxrate();
						}
						else
						{
							j = GetFxDataArray().Find(pData->GetBlmCurr() + " CURNCY");
							if(j >= 0)
							{
								pFxData = GetFxDataArray().GetAt(j);
								if(pFxData && pFxData->GetFxrate() > 0)
								{
									pData->GetFxrate() = pFxData->GetFxrate();
									pData->GetPrice() = 1/pData->GetFxrate();
								}
						}
							
						}
					}
					break;
				default:
					break;
			}
		}
	}
}

void CBlmPrice::UpdatePrices()
{
	int i, j, nEnd;
	BOOL bOk;
	CString Price, AssetID;
	CBlmSec *pData;
	CBlmAssetData *pAssetData;
	CDBRec DBRec;
	COraLoader OraLoader;
	CQData QData;

	OraLoader.SetDB(&theDB);
	SetupPriceSql(GetOraLoader().GetSql());

	GetOraLoader().Open(GetOraLoader().GetSql(), ODYNASET_DEFAULT);
	GetOraLoader().BeginTransaction();

	for(i = 0; i < GetBlmSecArr().GetSize(); i++)
	{
		pData = GetBlmSecArr().GetAt(i);
		if(pData && pData->IsModified()) // Got prices
		{
			bOk = TRUE;
			if(GetLoadType() == BLM_LOAD_OPTION || GetLoadType() == BLM_LOAD_IRS)
			{
				if(pData->IsUSDPrice() && pData->GetFxrate() > 0)
					pData->GetPrice() /= pData->GetFxrate();
				else
					bOk = FALSE;
			}

			if(bOk && (pData->GetPrice() >= 0 || GetLoadType() == BLM_LOAD_IRS)) // A valid entry
				bOk = TRUE;
			else
				bOk = FALSE;

			for(j = 0; j < GetAssetDataArray().GetSize() && bOk; j++)
			{
				pAssetData = GetAssetDataArray().GetAt(j);
				if(pAssetData)
				{
					if(pAssetData->GetKey() == pData->GetKey())
						pAssetData->GetPrice() = pData->GetPrice()*pAssetData->GetWtFactor();
					else
						if(pAssetData->GetKey() > pData->GetKey())
							break;
				}
			}
		}
	}

	// Setup data space
	DBRec.RemoveAll();
	DBRec.Add(GetDate());
	if(BLM_LOAD_IRS)
		nEnd = 7;
	else
		nEnd = 3;

	for(i = 1; i < nEnd; i++)
		DBRec.Add("A" - 1 + i);
	
	for(i = 0; i < GetAssetDataArray().GetSize(); i++)
	{
		pAssetData = GetAssetDataArray().GetAt(i);
		if(pAssetData)
		{
			AssetID = pAssetData->GetAssetID(); // AssetID
			if(GetLoadType() == BLM_LOAD_OPTION || GetLoadType() == BLM_LOAD_IRS) // Get TransNum and Remove the pre-fix
			{
				j = AssetID.Find("@", 0);
				AssetID = AssetID.Right(AssetID.GetLength() - j - 1);
			}

			DBRec.SetAt(1, AssetID);
			DBRec.SetAt(2, QData.WriteNumber(pAssetData->GetPrice(), FALSE, 12)); // Price
			if(GetLoadType() == BLM_LOAD_IRS)
			{
				OraLoader.Open("SELECT TICKET_NUM, ASSET_CODE, TRANS_DIRECTION, TRANS_TYPE FROM SEMAM.NW_TR_TICKETS WHERE TRANS_NUM = " + AssetID);
				while(!OraLoader.IsEOF())
				{
					DBRec.SetAt(3, OraLoader.GetDBString(0));
					DBRec.SetAt(4, OraLoader.GetDBString(1));
					DBRec.SetAt(5, OraLoader.GetDBString(2));
					DBRec.SetAt(6, OraLoader.GetDBString(3));
				}
			}

			GetOraLoader().UpdateRecord(DBRec, TRUE);
		}
	}
	GetOraLoader().Commit();
}

BOOL CBlmPrice::LoadLivePrices(int nLoadType, const CString RefDataServiceType)
{
    CBlmSec *pData;

	GetLoadType() = nLoadType;
	LoadAssets();

    m_AssetClass.Empty();

    for(int i = 0; i < GetBlmSecArr().GetSize(); i ++)
	{
        pData = GetBlmSecArr().GetAt(i);
        if(pData && pData->GetAssetClass() != m_AssetClass)
		{
            m_AssetClass = pData->GetAssetClass();
            BlmRefDataRequest(RefDataServiceType, 100 + i);
		}
	}

	if(GetLoadType() == BLM_LOAD_OPTION)
	{
		LoadPricingCurrency(RefDataServiceType);
		GetLoadType() = BLM_LOAD_OPTION;
	}

	UpdateFxrates();

	UpdatePrices();
    return TRUE;
}