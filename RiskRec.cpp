#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "riskrec.h"
#include "qdata.h"
#include "capbond.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CRiskRec, CRecObj)

void CRiskRec::Copy(CRiskRec &RiskRec)
{
	GetPortfolio() = RiskRec.GetPortfolio();
	GetRiskType() = RiskRec.GetRiskType();
	GetAssetClass() = RiskRec.GetAssetClass();
	GetWLimit() = RiskRec.GetWLimit();
	GetLimit() = RiskRec.GetLimit();
	GetAmount() = RiskRec.GetAmount();
}




IMPLEMENT_DYNAMIC(CRiskRecArray, CObArray)

void CRiskRecArray::Copy(CRiskRecArray &RiskRecArray)
{
	RemoveAll();

	SetAsset(RiskRecArray.GetAsset());

	for(int i = 0; i < RiskRecArray.GetSize(); i++)
		Add(RiskRecArray.GetAt(i));
}

void CRiskRecArray::SetAsset(const CString Asset)
{
	 m_Asset.Empty(); 
	 
	 if(Asset != NEWASSET)
		 m_Asset = Asset;
	 else
		 m_Asset.Empty();
}

void CRiskRecArray::SetAt(int Index, CRiskRec *Rec)
{
	CRiskRec *pRec = GetAt(Index);
	
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, Rec);
	}
}

void CRiskRecArray::SetAt(int Index, CRiskRec &Rec)
{
	CRiskRec *pRec = GetAt(Index);

	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, new CRiskRec(Rec));
	}
}

void CRiskRecArray::RemoveAt(int Index)
{
	CRiskRec *pRec = GetAt(Index);

	if(pRec)
	{
		CObArray::RemoveAt(Index);
		delete pRec;		
	}
}

void CRiskRecArray::RemoveAll()
{
	CRiskRec *pRec;
	
	for(int i = 0; i < GetSize(); i++)
	{
		pRec = GetAt(i);
		if(pRec)
			delete pRec;
    }
    
    CObArray::RemoveAll();
}

CRiskRec *CRiskRecArray::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;
	return (CRiskRec*) CObArray::GetAt(Index);
}





void CRiskControl::Setup(COraLoader &OraLoader)
{
	OValue Value;
	CQData QData;

	GetOraLoader() = OraLoader;
	
	if(!GetOraLoader().Open("SELECT INDATE, NAV FROM SEMAM.NWB_BNAM_NAV A "
							"WHERE PORTFOLIO = 'BNAMTR' "
							"AND INDATE = (SELECT MAX(INDATE) FROM SEMAM.NWB_BNAM_NAV B "
											"WHERE B.PORTFOLIO = A.PORTFOLIO "
											"AND B.INDATE <= TRUNC(SYSDATE)) "))
		return;

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		GetPrevBDate() = GetOraLoader().GetDBString(0);
		GetQPrevBDate() = QData.GetQueryDate(GetPrevBDate());
		GetOraLoader().GetFieldValue(1, &Value);
		GetNAV() = (double) Value;
		GetOraLoader().MoveNext();
	}
}

void CRiskControl::AddIDPhrase(CString &Sql, const CString ID, const CString sID, BOOL bByTicket)
{
	if(ID.IsEmpty())
		return;
	
	if(bByTicket)
		Sql += " AND (B.TICKET_NUM IS NULL OR B.TICKET_NUM != " + sID + ")";
	else
		Sql += " AND (B.ORDER_ID IS NULL OR B.ORDER_ID != " + sID + ")";
}

BOOL CRiskControl::LoadRisk(const CString Asset, const CString ID, BOOL bByTicket)
{
	CString Sql, sAsset, sID;
	CQData QData;
	CRiskRec RiskRec;
	OValue Value;

	if(Asset.IsEmpty() || Asset == NEWASSET)
		return FALSE;

	m_RiskArray.GetAsset() = Asset;
	sAsset = QData.GetQueryText(Asset);
	if(!ID.IsEmpty())
		sID = QData.GetQueryNumber(ID);

	Sql = "SELECT 'BNAMTR', 'CURRENCY', A.ASS_CURRENCY, D.W_LIMIT, D.LIMIT, SUM(NVL(AMOUNT, 0)) "
			"FROM SEMAM.NW_ASSETS A, SEMAM.NWB_BNAM_Q_RISK B, SEMAM.NWB_BNAM_RISK_TYPE D "
			"WHERE B.RISK_TYPE(+) = 'CURRENCY' "
			"AND B.ASSET_CLASS(+) = A.ASS_CURRENCY "
			"AND D.RISK_TYPE = 'CURRENCY' "
			"AND A.ASS_INDUSTRY IN ('CURRENCY FWDS') "
			"AND A.ASS_CODE = " + sAsset; 
	AddIDPhrase(Sql, ID, sID, bByTicket);
	Sql += " AND A.ASS_CURRENCY NOT IN ('EUR', 'JPY', 'USD') "
			"GROUP BY A.ASS_CURRENCY, D.W_LIMIT, D.LIMIT "
			"UNION "
			"SELECT 'BNAMTR', 'CURRENCY', A.ASS_CURRENCY2, D.W_LIMIT, D.LIMIT, SUM(NVL(AMOUNT, 0)) "
			"FROM SEMAM.NW_ASSETS A, SEMAM.NWB_BNAM_Q_RISK B, SEMAM.NWB_BNAM_RISK_TYPE D "
			"WHERE B.RISK_TYPE = 'CURRENCY' "
			"AND B.ASSET_CLASS = A.ASS_CURRENCY2 "
			"AND D.RISK_TYPE = 'CURRENCY' "
			"AND A.ASS_INDUSTRY IN ('CURRENCY FWDS') "
			"AND A.ASS_CODE = " + sAsset; 
	AddIDPhrase(Sql, ID, sID, bByTicket);
	Sql += " AND A.ASS_CURRENCY2 NOT IN ('EUR', 'JPY', 'USD') "
			"GROUP BY A.ASS_CURRENCY2, D.W_LIMIT, D.LIMIT "
			"UNION "
			"SELECT 'BNAMTR', 'CORP BOND', ASS_ISSUER, D.W_LIMIT, D.LIMIT, SUM(NVL(AMOUNT, 0)) "
			"FROM SEMAM.NW_ASSETS A, SEMAM.NWB_BNAM_Q_RISK B, SEMAM.NW_ISSUERS C, SEMAM.NWB_BNAM_RISK_TYPE D "
			"WHERE B.RISK_TYPE(+) = 'CORP BOND' "
			"AND B.ASSET_CLASS(+) = A.ASS_ISSUER "
			"AND C.ISSUER = A.ASS_ISSUER "
			"AND C.SOV_FLAG IS NULL "
			"AND D.RISK_TYPE = 'CORP BOND' "
			"AND A.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
			"AND A.ASS_CODE = " + sAsset;
	AddIDPhrase(Sql, ID, sID, bByTicket);
	Sql += " GROUP BY ASS_ISSUER, D.W_LIMIT, D.LIMIT "
			"UNION "
			"SELECT 'BNAMTR', 'SOVEREIGN', ASS_ISSUER, D.W_LIMIT, D.LIMIT, SUM(NVL(AMOUNT, 0)) "
			"FROM SEMAM.NW_ASSETS A, SEMAM.NWB_BNAM_Q_RISK B, SEMAM.NW_ISSUERS C, SEMAM.NWB_BNAM_RISK_TYPE D "
			"WHERE B.RISK_TYPE(+) = 'SOVEREIGN' "
			"AND B.ASSET_CLASS(+) = A.ASS_ISSUER "
			"AND C.ISSUER = A.ASS_ISSUER "
			"AND C.SOV_FLAG IS NOT NULL "
			"AND D.RISK_TYPE = 'SOVEREIGN' "
			"AND A.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
			"AND A.ASS_CODE = " + sAsset;
	AddIDPhrase(Sql, ID, sID, bByTicket);
	Sql += " GROUP BY ASS_ISSUER, D.W_LIMIT, D.LIMIT "
			"UNION "
			"SELECT 'BNAMTR', 'COUNTRY', ASS_COUNTRY, D.W_LIMIT, D.LIMIT, SUM(NVL(AMOUNT, 0)) " 
			"FROM SEMAM.NW_ASSETS A, SEMAM.NWB_BNAM_Q_RISK B, SEMAM.NWB_BNAM_RISK_TYPE D "
			"WHERE B.RISK_TYPE(+) = 'COUNTRY' "
			"AND B.ASSET_CLASS(+) = A.ASS_COUNTRY "
			"AND D.RISK_TYPE = 'COUNTRY' "
			"AND A.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
			"AND A.ASS_CODE = " + sAsset; 
	AddIDPhrase(Sql, ID, sID, bByTicket);
	Sql += " GROUP BY ASS_COUNTRY, D.W_LIMIT, D.LIMIT "
			"UNION "
			"SELECT 'BNAMTR', 'CURRENCY', ASS_CURRENCY, D.W_LIMIT, D.LIMIT, SUM(NVL(AMOUNT, 0)) "
			"FROM SEMAM.NW_ASSETS A, SEMAM.NWB_BNAM_Q_RISK B, SEMAM.NWB_BNAM_RISK_TYPE D "
			"WHERE B.RISK_TYPE(+) = 'CURRENCY' "
			"AND B.ASSET_CLASS(+) = A.ASS_CURRENCY "
			"AND D.RISK_TYPE = 'CURRENCY' "
			"AND A.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
			"AND A.ASS_CODE = " + sAsset;
	AddIDPhrase(Sql, ID, sID, bByTicket);
	Sql += " AND A.ASS_CURRENCY NOT IN ('EUR', 'JPY', 'USD') ";
	Sql += "GROUP BY ASS_CURRENCY, D.W_LIMIT, D.LIMIT ";

	if(!GetOraLoader().Open(Sql))
		return FALSE;

	m_RiskArray.RemoveAll();
	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		RiskRec.GetPortfolio() = GetOraLoader().GetDBString(0);
		RiskRec.GetRiskType() = GetOraLoader().GetDBString(1);
		RiskRec.GetAssetClass() = GetOraLoader().GetDBString(2);
		GetOraLoader().GetFieldValue(3, &Value);
		RiskRec.GetWLimit() = (double) Value;
		GetOraLoader().GetFieldValue(4, &Value);
		RiskRec.GetLimit() = (double) Value;
		GetOraLoader().GetFieldValue(5, &Value);
		RiskRec.GetAmount() = (double) Value;

		m_RiskArray.Add(RiskRec);

		GetOraLoader().MoveNext();
	}

	return TRUE;
}

BOOL CRiskControl::AddRisk(const CString sID, double Amount, BOOL bByTicket, BOOL bFirstLeg)
{
	CString Sql, sAmount;

	sAmount.Format("%.2lf", Amount);

	if(bByTicket)
	{
		Sql = "INSERT INTO SEMAM.NWB_BNAM_Q_RISK(INDATE, PORTFOLIO, RISK_TYPE, ASSET_CLASS, TICKET_NUM, ORDER_ID, AMOUNT) ";
		
		if(bFirstLeg)
			Sql += "SELECT TRADE_DATE, 'BNAMTR', DECODE(SOV_FLAG, 'Y', 'SOVEREIGN', 'CORP BOND'), ASS_ISSUER, A.TICKET_NUM, "
					"A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS_V B, SEMAM.NW_ISSUERS C "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
					"AND C.ISSUER = B.ASS_ISSUER "
					"AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.TICKET_NUM = " + sID + 
					" UNION "
					"SELECT TRADE_DATE, 'BNAMTR', 'COUNTRY', ASS_COUNTRY, A.TICKET_NUM, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS_V B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
					" AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.TICKET_NUM = " + sID + 
					" UNION "
					"SELECT TRADE_DATE, 'BNAMTR', 'CURRENCY', A.CURRENCY, A.TICKET_NUM, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS_V B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
					" AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.TICKET_NUM = " + sID + 
					" AND A.CURRENCY NOT IN ('EUR', 'JPY', 'USD' ) "
					"UNION "
					"SELECT TRADE_DATE, 'BNAMTR', 'CURRENCY', A.CURRENCY, A.TICKET_NUM, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS_V B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY IN ('CURRENCY FWDS') "
					" AND A.TICKET_NUM = " + sID + 
					" AND A.CURRENCY NOT IN ('EUR', 'JPY', 'USD' ) ";
		else
			Sql += "SELECT TRADE_DATE, 'BNAMTR', 'CURRENCY', DECODE(ASS_CURRENCY, A.CURRENCY, ASS_CURRENCY2, ASS_CURRENCY), "
					"A.TICKET_NUM, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY IN ('CURRENCY FWDS') "
					" AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.TICKET_NUM = " + sID + 
					" AND DECODE(ASS_CURRENCY, A.CURRENCY, ASS_CURRENCY2, ASS_CURRENCY) NOT IN ('EUR', 'JPY', 'USD' ) ";
	}
	else
	{
		Sql = "INSERT INTO SEMAM.NWB_BNAM_Q_RISK(INDATE, PORTFOLIO, RISK_TYPE, ASSET_CLASS, ORDER_ID, AMOUNT) ";

		if(bFirstLeg)
			Sql += "SELECT ORDER_DATE, 'BNAMTR', DECODE(SOV_FLAG, 'Y', 'SOVEREIGN', 'CORP BOND'), ASS_ISSUER, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_TRADE_ORDER A, SEMAM.NW_ASSETS_V B, SEMAM.NW_ISSUERS C "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
					"AND C.ISSUER = B.ASS_ISSUER "
					"AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.AA_METHOD = 'BNAMTR' "
					"AND A.ORDER_ID = " + sID + 
					" UNION "
					"SELECT ORDER_DATE, 'BNAMTR', 'COUNTRY', ASS_COUNTRY, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_TRADE_ORDER A, SEMAM.NW_ASSETS_V B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS', 'CD/CP', 'MONEY MKT PLMT') "
					"AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.AA_METHOD = 'BNAMTR' "
					"AND A.ORDER_ID = " + sID + 
					" UNION "
					"SELECT ORDER_DATE, 'BNAMTR', 'CURRENCY', A.CURRENCY, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_TRADE_ORDER A, SEMAM.NW_ASSETS_V B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS') "
					" AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.AA_METHOD = 'BNAMTR' "
					"AND A.ORDER_ID = " + sID + 
					" AND A.CURRENCY NOT IN ('EUR', 'JPY', 'USD' ) "
					"UNION "
					"SELECT ORDER_DATE, 'BNAMTR', 'CURRENCY', A.CURRENCY, A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_TRADE_ORDER A, SEMAM.NW_ASSETS_V B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY IN ('CURRENCY FWDS') "
					"AND A.AA_METHOD = 'BNAMTR' "
					"AND A.ORDER_ID = " + sID + 
					" AND A.CURRENCY NOT IN ('EUR', 'JPY', 'USD' ) ";
		else
			Sql += "SELECT ORDER_DATE, 'BNAMTR', 'CURRENCY', DECODE(ASS_CURRENCY, A.CURRENCY, ASS_CURRENCY2, ASS_CURRENCY), "
					"A.ORDER_ID, " + sAmount + 
					" FROM SEMAM.NW_TRADE_ORDER A, SEMAM.NW_ASSETS_V B "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND B.ASS_INDUSTRY IN ('CURRENCY FWDS') "
					"AND A.TRANS_TYPE IN ('CALL', 'CDS', 'INT. SWAP', 'PUT', 'SECURITIES') "
					"AND A.AA_METHOD = 'BNAMTR' "
					"AND A.ORDER_ID = " + sID + 
					" AND DECODE(ASS_CURRENCY, A.CURRENCY, ASS_CURRENCY2, ASS_CURRENCY) NOT IN ('EUR', 'JPY', 'USD' ) ";
	}

	return GetOraLoader().ExecuteSql(Sql);
}

BOOL CRiskControl::UpdateRisk(const CString ID, BOOL bByTicket)
{
	CQData QData;
	CString Portfolio, TransType, ValueDate, Currency;
	double Amount, Amount2 = 0, Nominal, Price, Fxrate;
	CString sID;
	OValue Value;

	if(ID.IsEmpty())
		return FALSE;
	
	sID = QData.GetQueryNumber(ID); 

	if(bByTicket)
		GetOraLoader().GetSql() = "SELECT PORTFOLIO, TRANS_TYPE, VALUE_DATE, CURRENCY, DECODE(A.DIR, 'P', 1, 'S', -1)*B.NOM_AMOUNT, "
								"A.PRICE, A.FXRATE "
								"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_RAW_TICKET_DETAIL B "
								"WHERE B.TICKET_NUM = A.TICKET_NUM "
								"AND B.PORTFOLIO = 'BNAMTR' "
								"AND A.TICKET_NUM = " + sID;
	else
		GetOraLoader().GetSql() = "SELECT A.AA_METHOD, A.TRANS_TYPE, A.ORDER_DATE, A.CURRENCY, DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOMINAL, "
									"A.PRICE, A.FXRATE "
									"FROM SEMAM.NW_TRADE_ORDER A "
									"WHERE A.AA_METHOD = 'BNAMTR' "
									"AND A.ORDER_ID = " + sID;
	if(!GetOraLoader().Open())
		return FALSE;

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		Portfolio = GetOraLoader().GetDBString(0);
		TransType = GetOraLoader().GetDBString(1);
		ValueDate = GetOraLoader().GetDBString(2);
		Currency = GetOraLoader().GetDBString(3);
		GetOraLoader().GetFieldValue(4, &Value);
		Nominal = (double) Value;
		GetOraLoader().GetFieldValue(5, &Value);
		Price = (double) Value;
		GetOraLoader().GetFieldValue(6, &Value);
		if(Value.IsNull())
			Fxrate = 0;
		else
			Fxrate = (double) Value;
		GetOraLoader().MoveNext();
	}
								
	Amount = ComputeAmount(Amount2, TransType, ValueDate, Currency, Nominal, Price, Fxrate);

	if(bByTicket)
		GetOraLoader().GetSql() = "DELETE FROM SEMAM.NWB_BNAM_Q_RISK A WHERE TICKET_NUM = " + sID +
									" OR ORDER_ID = (SELECT ORDER_ID FROM SEMAM.NW_RAW_TICKETS B "
													"WHERE B.TICKET_NUM = A.TICKET_NUM "
													"AND  A.TICKET_NUM = " + sID + ") ";
		
	else
		GetOraLoader().GetSql() = "DELETE FROM SEMAM.NWB_BNAM_Q_RISK WHERE ORDER_ID = " + sID;

	if(!GetOraLoader().ExecuteSql())
		return FALSE;

	AddRisk(sID, Amount, bByTicket);
	if(Amount2 != 0)
		AddRisk(sID, Amount2, bByTicket, FALSE);

	return TRUE;
}

BOOL CRiskControl::DeleteRisk(const CString ID, BOOL bByTicket)
{
	CString sID;
	CQData QData;

	if(ID.IsEmpty())
		return TRUE;

	sID = QData.GetQueryNumber(ID);
	if(bByTicket)
		return GetOraLoader().ExecuteSql("DELETE FROM SEMAM.NWB_BNAM_Q_RISK WHERE TICKET_NUM = " + sID);
	else
		return GetOraLoader().ExecuteSql("DELETE FROM SEMAM.NWB_BNAM_Q_RISK WHERE ORDER_D = " + sID);
}

int CRiskControl::VerifyRisk(CString &Text, const CString Fund, const CString Currency, double Nominal, double Amount)
{
	double dRisk, UnitPrice, LimitAmt;
	CString LimitNom, WLimitNom, Nav, Amt;
	CRiskRec *pRec;
	CQData QData;
	int i, nRet = 0;

	if(Fund != "BNAMTR")
		return nRet;

	UnitPrice = Amount/Nominal;

	for(i = 0; i < m_RiskArray.GetSize(); i ++)
	{
		pRec = m_RiskArray.GetAt(i);
		if(pRec)
		{
			if(GetRiskType().IsEmpty()) // None Currency Fwds
				dRisk = fabs(pRec->GetAmount() + Amount)/m_NAV*100;
			else
			{
				if(pRec->GetAssetClass() == Currency)
					dRisk = fabs(pRec->GetAmount() + Amount)/m_NAV*100;
				else
					dRisk = 0;
			}

			if(dRisk > pRec->GetLimit())
			{
				LimitAmt = pRec->GetWLimit()*m_NAV/100 - fabs(pRec->GetAmount());
				WLimitNom = QData.WriteNumber(LimitAmt/UnitPrice, TRUE, 0);
				LimitAmt = pRec->GetLimit()*m_NAV/100 - fabs(pRec->GetAmount());
				LimitNom = QData.WriteNumber(LimitAmt/UnitPrice, TRUE, 0);
				Amt = QData.WriteNumber(pRec->GetAmount() + Amount, TRUE, 2);
				Nav = QData.WriteNumber(m_NAV, TRUE, 2);
				Text.Format("Amount: %s, NAV: %s, %.2lf%% Exceeds " + pRec->GetRiskType() + " (" + pRec->GetAssetClass() + ") Risk Limit!!! "
							"Warning Nominal: %s.  Maximum Nominal: %s.", (LPCTSTR) Amt, (LPCTSTR) Nav, dRisk, (LPCTSTR) WLimitNom, (LPCSTR) LimitNom);
				nRet = 3;
				return nRet;
			}
			else
				if(dRisk > pRec->GetWLimit())
				{					
					LimitAmt = pRec->GetWLimit()*m_NAV/100 - fabs(pRec->GetAmount());
					WLimitNom = QData.WriteNumber(LimitAmt/UnitPrice, TRUE, 0);
					LimitAmt = pRec->GetLimit()*m_NAV/100 - fabs(pRec->GetAmount());
					LimitNom = QData.WriteNumber(LimitAmt/UnitPrice, TRUE, 0);
					Amt = QData.WriteNumber(pRec->GetAmount() + Amount, TRUE, 2);
					Nav = QData.WriteNumber(m_NAV, TRUE, 2);
					Text.Format("Amount: %s, NAV: %s, %.2lf%% is above " + pRec->GetRiskType() + " (" + pRec->GetAssetClass() + ") Risk Warning Limit. "
								"Warning Nominal: %s.  Maximum Nominal: %s.", (LPCTSTR) Amt, (LPCTSTR) Nav, dRisk, (LPCTSTR) WLimitNom, (LPCTSTR) LimitNom);
					nRet = 1;
				}
		}
	}

	return nRet;
}

int CRiskControl::VerifyRisk(CString &Text, const CString Fund, const CString TransType, const CString ValueDate, const CString Currency, double Nominal, double Price, double Fxrate)
{
	double Amount, Amount2;
	int nRet = 0;

	if(Fund != "BNAMTR" || TransType == FOREX || TransType == REPO || TransType == LEVERAGE)
		return nRet;

	Amount = ComputeAmount(Amount2, TransType, ValueDate, Currency, Nominal, Price, Fxrate);

	nRet = VerifyRisk(Text, Fund, GetCurrency(), Nominal, Amount);

	if(Amount2 != 0)
		return VerifyRisk(Text, Fund, GetCurrency2(), Nominal, Amount2);

	return nRet;
}

int CRiskControl::VerifyRisk(CString &Text, const CString Fund, const CString TransType, const CString ValueDate, const CString Currency, 
								const CString Nominal, const CString Price, const CString Fxrate)
{
	double dNominal, dPrice, dFxrate;
	CQData QData;

	if(Fund != "BNAMTR" || TransType == FOREX || TransType == REPO || TransType == LEVERAGE)
		return 0;

	dNominal = atof(QData.RemoveComma(Nominal));

	if(Price.IsEmpty())
		dPrice = 1.0;
	else
		dPrice = atof(QData.RemoveComma(Price));

	if(Fxrate.IsEmpty())
		dFxrate = 0;
	else
		dFxrate = atof(QData.RemoveComma(Fxrate));

	return VerifyRisk(Text, Fund, TransType, ValueDate, Currency, dNominal, dPrice, dFxrate);
}

double CRiskControl::GetFxrate(const CString Currency, double Fxrate)
{
	double Exrate = 0;
	CQData QData;
	CString sCurrency;
	OValue Value;

	if(Fxrate > 0)
		Exrate = Fxrate;
	else
		if(Currency == USD)
			Exrate = 1;
		
	if(Exrate <= 0)
	{
		sCurrency = QData.GetQueryText(Currency);
		GetOraLoader().Open("SELECT EXRATE FROM SEMAM.NW_EXRATES "
							"WHERE CURRENCY = " + sCurrency + 
							" AND INDATE = " + GetQPrevBDate());

		while(!GetOraLoader().IsEOF())
		{
			GetOraLoader().GetFieldValue(0, &Value);
			Exrate = (double) Value;
			GetOraLoader().MoveNext();
		}
	}

	if(Exrate <= 0)
		Exrate = 1;

	return Exrate;
}

void CRiskControl::SetupCurrency(CString &Currency, double &Exrate, const CString SrcCurr, double Fxrate)
{
	if(SrcCurr.IsEmpty())
		return;

	Currency = SrcCurr;
	Exrate = GetFxrate(SrcCurr, Fxrate);
}

double CRiskControl::ComputeAmount(double &Amount2, const CString TransType, const CString ValueDate, const CString Currency, double Nominal, double Price, double Fxrate)
{
	double Amount = 0, AssetFactor, Amort, PlusAmt, Rate, CapRate;
	OValue Value;
	CString AssetClass, sValueDate, sAsset, AssetCurr, AssetCurr2, Future, RateBasis, FromDate, ToDate, Dir, PrePaid;
	CQData QData;
	CCapBond AssetVal;

	Amount2 = 0;

	GetRiskType().Empty();
	if(TransType == CALL || TransType == CDS || TransType == PUT || TransType == SECURITIES)
	{
		GetValueDate() = ValueDate;
		SetCurrency(Currency, Fxrate);
		sValueDate = QData.GetQueryDate(GetValueDate());
		sAsset = QData.GetQueryText(m_RiskArray.GetAsset());

		if(GetOraLoader().Open("SELECT ASS_INDUSTRY, ASS_CONV_TYPE, ASS_CURRENCY, ASS_CURRENCY2, ASS_RATE_BASIS, ASSET_FACTOR, "
			"ASS_PPAID_INT, NVL(START_DATE, ASS_FROM), ASS_TO, NVL(RATE, 0), NVL(AMORT_FACT, 1), NVL(CAPIT_RATE, 0), NVL(PLUS_AMOUNT, 0) "
								"FROM SEMAM.NW_ASSETS_V A, SEMAM.NW_ASS_PERIODS B "
								"WHERE B.ASS_CODE(+) = A.ASS_CODE "
								"AND B.ASS_FROM(+) <= " + sValueDate + 
								" AND B.ASS_TO(+) > " + sValueDate + 
								" AND A.ASS_CODE = " + sAsset))
		{
			GetOraLoader().MoveFirst();
			while(!GetOraLoader().IsEOF())
			{
				AssetClass = GetOraLoader().GetDBString(0);
				Future = GetOraLoader().GetDBString(1);
				AssetCurr = GetOraLoader().GetDBString(2);
				AssetCurr2 = GetOraLoader().GetDBString(3);
				RateBasis = GetOraLoader().GetDBString(4);
				GetOraLoader().GetFieldValue(5, &Value);
				AssetFactor = (double) Value;
				PrePaid = GetOraLoader().GetDBString(6);
				FromDate = GetOraLoader().GetDBString(7);
				ToDate = GetOraLoader().GetDBString(8);
				GetOraLoader().GetFieldValue(9, &Value);
				Rate = (double) Value;
				GetOraLoader().GetFieldValue(10, &Value);
				Amort = (double) Value;
				GetOraLoader().GetFieldValue(11, &Value);
				CapRate = (double) Value;
				GetOraLoader().GetFieldValue(12, &Value);
				PlusAmt = (double) Value;
				GetOraLoader().MoveNext();
			}
		}

		Dir = (Nominal > 0) ? P : S;
		AssetVal.Setup(GetOraLoader(), TransType, fabs(Nominal), Price, GetExrate(), AssetFactor, Amort, PlusAmt, Dir);
		AssetVal.SetupIntInfo(FromDate, ValueDate, ValueDate, ToDate, RateBasis, Rate, 0, N, PrePaid);
		AssetVal.SetupCapInfo(FromDate, ValueDate, ValueDate, RateBasis, CapRate);
		if(Future == "F")
			Amount = 0;
		else
			if(TransType == CDS)
			{
				Amount = -1*Nominal*(Price - 100)*AssetFactor*Amort/GetExrate();
				Amount += (Dir == S ? -1 : 1)*AssetVal.GetPrePaidInt()/GetExrate();
			}
			else
				if(AssetClass == "CURRENCY FWDS")
					Amount = -1*Nominal*Price*AssetFactor*Amort/GetExrate();
				else
				{
					Amount = (AssetVal.GetPrePaidInt() + AssetVal.GetPrePaidAccretion() + AssetVal.GetValue())/GetExrate();
					Amount *= (Dir == S ? -1 : 1);
//					Amount = Nominal*Price*AssetFactor*Amort/GetExrate();
				}

		if(AssetClass == "CURRENCY FWDS")
		{
			if(GetCurrency() == "EUR" || GetCurrency() == "JPY" || GetCurrency() == USD)
				Amount = 0;

			GetRiskType() = "CURRENCY";
			if(AssetCurr == Currency)
				SetCurrency2(AssetCurr2);
			else
				SetCurrency2(AssetCurr);

			Amount2 = Nominal/GetExrate2();

			if(GetCurrency2() == "EUR" || GetCurrency2() == "JPY" || GetCurrency2() == USD)
				Amount2 = 0;
		}
	}

	return Amount;
}