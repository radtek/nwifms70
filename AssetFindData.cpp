#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "qdata.h"
#include "sqlstmt.h"
#include "assetfinddata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CAssetFindData, CRowCtrl)
int CAssetFindData::LoadDBData(BOOL bInitial)
{
	CString NewSql, Sql, Data;
	CStringArray Rec, QRec;
	BOOL rc = TRUE;

	if(!IsReady())
		return -1;

	NewSql = "SELECT A.ASSET_CODE, A.ASSET_DESC, A.ASSET_DESC2, A.COUNTRY, A.CURRENCY, A.TRANS_TYPE, "
				"A.STRIKE, A.MATURITY_DATE, A.CATEGORY, A.ASSET_CLASS, A.BUCKET_DESC, A.INDUSTRY, "
				"A.COMMON, A.CUSIP, A.ISIN, A.SEDOL, A.PID, A.TICK, A.PAR_VALUE, A.TICKET, "
				"A.UNDERLINE_ASSET, A.UNDERLINE_ID, A.DIR, A.NOM_AMOUNT, A.FUTURE, A.EUROPE_BOND, "
				"A.OPT_ID, A.OPT_TICK, A.DELIVERY_DATE, A.OPT_SET_CODE, A.ASSIGN_CP, A.TRADER_INI, "
				"A.OPT_LISTED, A.IDX "
				"FROM SEMAM.ALL_ASSETS_V A "
				"WHERE A.ASSET_CODE = 'NEW ASSET' ";

	Sql = "SELECT A.ASSET_CODE, A.ASSET_DESC, A.ASSET_DESC2, A.COUNTRY, A.CURRENCY, A.TRANS_TYPE, "
			"A.STRIKE, A.MATURITY_DATE, A.CATEGORY, A.ASSET_CLASS, A.BUCKET_DESC, A.INDUSTRY, "
			"A.COMMON, A.CUSIP, A.ISIN, A.SEDOL, A.PID, A.TICK, A.PAR_VALUE, A.TICKET, "
			"A.UNDERLINE_ASSET, A.UNDERLINE_ID, A.DIR, A.NOM_AMOUNT, A.FUTURE, A.EUROPE_BOND, "
			"A.OPT_ID, A.OPT_TICK, A.DELIVERY_DATE, A.OPT_SET_CODE, A.ASSIGN_CP, A.TRADER_INI, "
			"A.OPT_LISTED, A.IDX "
			"FROM SEMAM.ALL_ASSETS_V A ";

	Data = m_Rec.GetAsset(); // AssetCode

	if(Data.IsEmpty())
		Data = NEWASSET;

	GetOraLoader().Close();
	if(bInitial && Data == NEWASSET)
		GetOraLoader().Open(NewSql);
	else
	{
		if(m_bWithNew)
			Sql = NewSql + " UNION " + Sql;
		
		SetRecArray(Rec);

		GetOraLoader().Open(Sql);
		GetOraLoader().RecToQueryRec(QRec, Rec, NULL, PERCENT);
		if(GetTicketBooking())
		{
			Sql += " WHERE NVL(MATURITY_DATE, SYSDATE) > TRUNC(SYSDATE) - 14 ";
			GetOraLoader().SetSqlSearchInfo(Sql, QRec, NULL, TRUE);
		}
		else
		{
			Sql += " WHERE ";
			GetOraLoader().SetSqlSearchInfo(Sql, QRec, NULL, FALSE);
		}

		Sql += " ORDER BY 1, 34 ";

		GetOraLoader().Open(Sql);
	}

	if(GetOraLoader().IsOpen())
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);

	return -1;
}

void CAssetFindData::SetRecArray(CStringArray &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetRec().GetAsset());
	Rec.Add(GetRec().GetDesc());
	Rec.Add(GetRec().GetDesc2());
	Rec.Add(GetRec().GetCountry());
	Rec.Add(GetRec().GetCurrency());
	Rec.Add(GetTransType());
	Rec.Add(GetStrike());
	Rec.Add(GetRec().GetMaturity());
	Rec.Add(GetRec().GetCategory());
	Rec.Add(GetRec().GetClass());
	Rec.Add(GetRec().GetBucket());
	Rec.Add(GetRec().GetIndustry());
	Rec.Add(GetRec().GetCommon());
	Rec.Add(GetRec().GetCusip());
	Rec.Add(GetRec().GetIsin());
	Rec.Add(GetRec().GetSedol());
	Rec.Add(GetRec().GetPID());
	Rec.Add(GetRec().GetSource());
	Rec.Add(GetRec().GetPar());
	Rec.Add(GetTicket());
	Rec.Add(EMPTYSTRING); /* UnderlineAsset */
	Rec.Add(EMPTYSTRING); /* Underline ID */
	Rec.Add(GetDir());
	Rec.Add(GetNominal());
	Rec.Add(EMPTYSTRING); /* Idx */
}

BOOL CAssetFindData::IsRowOK()
{
	if(m_Rec.GetAsset().IsEmpty())
	{
		MessageBox(NULL, "Must Select Asset Code", NULL, MB_OK);
		return FALSE;
	}

	if(m_Rec.GetDesc().IsEmpty())
	{
		MessageBox(NULL, "Must Select Asset Desc", NULL, MB_OK);
		return FALSE;
	}

	if(m_Rec.GetAsset() == "O/N INTRST" || m_Rec.GetAsset() == "ERRORENTRY" || m_Rec.GetAsset().Find("FEES", 0) >= 0)
		return TRUE;

	if(m_Rec.GetClass().IsEmpty())
	{
		MessageBox(NULL, "Must Select Asset Class", NULL, MB_OK);
		return FALSE;
	}

	if(m_Rec.GetCountry().IsEmpty())
	{
		MessageBox(NULL, "Must Select a Country", NULL, MB_OK);
		return FALSE;
	}

	if(m_Rec.GetCurrency().IsEmpty())
	{
		MessageBox(NULL, "Must Select a Currency", NULL, MB_OK);
		return FALSE;
	}

	if(m_Rec.GetCategory().IsEmpty())
	{
		MessageBox(NULL, "Must Select a Category", NULL, MB_OK);
		return FALSE;
	}

	if(m_Rec.GetBucket().IsEmpty())
	{
		MessageBox(NULL, "Must Select a Bucket", NULL, MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CAssetFindData::LoadHoldings(LPCTSTR Asset, CDBSheet &HoldingSS)
{
	CQData QData;

	GetOraLoader().GetSql().Format("SELECT CUSTODIAN, PORTFOLIO, TRANS_TYPE, TICKET_NUM, RATE, "
									"MATURITY, SUM(NOM_AMOUNT) \"NOM_AMOUNT\", ASSET_ID "
									"FROM SEMAM.NW_POSITION_V WHERE ASSET_CODE = %s "
									"GROUP BY CUSTODIAN, PORTFOLIO, TRANS_TYPE, TICKET_NUM, "
									"RATE, MATURITY, ASSET_ID "
									"HAVING SUM(NOM_AMOUNT) != 0 ORDER BY 1, 2, 3 ", 
									QData.GetQueryText(Asset));
	if(GetOraLoader().Open())
	{
		GetOraLoader().GetFieldArray().GetAt(3)->SetWithComma(FALSE);
		GetOraLoader().LoadDBSheet(HoldingSS, TRUE);
	}
}

int CAssetFindData::LoadCustodian(LPCTSTR Asset)
{
	CQData QData;
	CString qAsset, TransType;

	qAsset = QData.GetQueryText(Asset);
	TransType = QData.GetQueryText(GetTransType());
	
	GetOraLoader().Open("SELECT CUSTODIAN FROM SEMAM.NW_ASSET_CUSTODIAN_V WHERE ASSET_CODE = " + qAsset + 
						" AND TRANS_TYPE = " + TransType);
	GetOraLoader().LoadTextString(m_Custodian);

	return 1;
}