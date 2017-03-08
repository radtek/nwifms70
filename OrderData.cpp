#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "orderdata.h"
#include "orafield.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(COrderData, CRowCtrl)
void COrderData::Setup(COraLoader &OraLoader, CDBSheet *pSS, const CString Trader)
{
	CRowCtrl::Setup(pSS);
	GetOraLoader() = OraLoader;
	GetTrader() = Trader;
}

BOOL COrderData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

int COrderData::LoadDBData()
{
	CQData QData;
	CString Sql, MySql, Buf;

	if(!IsReady())
		return -1;

	Sql = "SELECT ORDER_DATE, ORDER_ID, TRANS_TYPE, DIR, ASSET_CODE, STRIKE, RATE, "
		  "MATURITY, CURRENCY, FXRATE, NOMINAL, PRICE, COUNTERPARTY, REF_TICKET, "
		  "TRADER_INI, NOTE, AA_METHOD, BEST_EXECUTION, DEAL_TYPE, IPO, MARKET_ORDER, "
		  "SAME_DAY, PARTIAL, DISCRETIONARY, EXPIRED, CANCELLED "
		  "FROM SEMAM.V_TRADE_ORDER A "
		  "WHERE EXPIRED IS NULL "
		  "AND CANCELLED IS NULL "
		  "AND TICKET_NUM IS NULL ";

	MySql = Sql + "AND ORDER_DATE = TRUNC(SYSDATE) "
				  "AND SAME_DAY = 'Y' ";
	if(!GetTrader().IsEmpty())
	{
		Buf = QData.GetQueryText(GetTrader());
		MySql += "AND TRADER_INI = " + Buf + " ";
	}		  

	MySql += "UNION ";
	MySql += Sql + "AND SAME_DAY IS NULL ";

	if(!GetTrader().IsEmpty())
		MySql += "AND TRADER_INI = " + Buf + " ";
	
	MySql += " ORDER BY 2, 1 ";
	
	if(!GetOraLoader().Open(MySql))
		return FALSE;

	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL COrderData::AddRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetOrderRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		AddSheetRow();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL COrderData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetOrderRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		UpdateSheetRow(FALSE);
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}