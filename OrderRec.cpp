#include "stdafx.h"
#include "resource.h"
#include "orderrec.h"
#include "nwivar.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(COrderRec, CRecObj)
void COrderRec::Copy(COrderRec &OrderRec)
{
	CRecObj::Copy(OrderRec);

	GetOrderID() = OrderRec.GetOrderID();
	GetOrderDate() = OrderRec.GetOrderDate();
	GetTransType() = OrderRec.GetTransType();
	GetDir() = OrderRec.GetDir();
	GetAsset() = OrderRec.GetAsset();
	GetStrike() = OrderRec.GetStrike();
	GetRate() = OrderRec.GetRate();
	GetMaturity() = OrderRec.GetMaturity();
	GetCurrency() = OrderRec.GetCurrency();
	GetFxrate() = OrderRec.GetFxrate();
	GetNomAmount() = OrderRec.GetNomAmount();
	GetPrice() = OrderRec.GetPrice();
	GetCP() = OrderRec.GetCP();
	GetRefTicket() = OrderRec.GetRefTicket();
	GetTrader() = OrderRec.GetTrader();
	GetAAMethod() = OrderRec.GetAAMethod();
	GetBestExecution() = OrderRec.GetBestExecution();
//	GetReason() = OrderRec.GetReason();
	GetNote() = OrderRec.GetNote();
	GetTRS() = OrderRec.GetTRS();
	GetIPO() = OrderRec.GetIPO();
	GetMarketOrder() = OrderRec.GetMarketOrder();
	GetSameDay() = OrderRec.GetSameDay();
	GetPartialFill() = OrderRec.GetPartialFill();
	GetDiscretionary() = OrderRec.GetDiscretionary();
	GetExpired() = OrderRec.GetExpired();
	GetCancelled() = OrderRec.GetCancelled();
}

void COrderRec::ToDBRec(CDBRec &Rec)
{
	CQData QData;

	Rec.RemoveAll();

	Rec.Add(QData.RemoveComma(GetOrderID()));
	Rec.Add(GetOrderDate());
	Rec.Add(GetTransType());
	Rec.Add(GetDir());
	Rec.Add(GetAsset());
	if(GetTransType() == CALL || GetTransType() == PUT)
		Rec.Add(GetStrike());
	else
		Rec.Add(EMPTYSTRING);
	if(GetTransType() == REPO || GetTransType() == INTSWAP)
		Rec.Add(GetRate());
	else
		Rec.Add(EMPTYSTRING);

	if(GetTransType() == CALL || GetTransType() == PUT|| GetTransType() == REPO || GetTransType() == INTSWAP)
		Rec.Add(GetMaturity());
	else
		Rec.Add(EMPTYSTRING);

	Rec.Add(GetCurrency());
	Rec.Add(GetFxrate());
	Rec.Add(GetNomAmount());
	if(GetTransType() == FOREX)
		Rec.Add(EMPTYSTRING);
	else
		Rec.Add(GetPrice());
	Rec.Add(GetCP());
	Rec.Add(GetRefTicket());
	Rec.Add(GetTrader());
	Rec.Add(GetAAMethod());
	Rec.Add(GetBestExecution());
//	Rec.Add(GetReason());
	Rec.Add(GetNote());
	Rec.Add(GetTRS());
	Rec.Add(GetIPO());
	Rec.Add(GetMarketOrder());
	Rec.Add(GetSameDay());
	Rec.Add(GetPartialFill());
	Rec.Add(GetDiscretionary());
	Rec.Add(GetExpired());
	Rec.Add(GetCancelled());
}

BOOL COrderRec::AddRec(COraLoader &OraLoader)
{
	if(GetOrderID().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT ORDER_ID, ORDER_DATE, TRANS_TYPE, DIR, ASSET_CODE, STRIKE, RATE, MATURITY, "
						"CURRENCY, FXRATE, NOMINAL, PRICE, COUNTERPARTY, REF_TICKET, TRADER_INI, AA_METHOD, "
						"BEST_EXECUTION, NOTE, DEAL_TYPE, IPO, MARKET_ORDER, SAME_DAY, PARTIAL, DISCRETIONARY, "
						"EXPIRED, CANCELLED FROM SEMAM.NW_TRADE_ORDER ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;

	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL COrderRec::UpdateRec(COraLoader &OraLoader)
{
	if(GetOrderID().IsEmpty())
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("SELECT ORDER_ID, ORDER_DATE, TRANS_TYPE, DIR, ASSET_CODE, STRIKE, RATE, MATURITY, "
							"CURRENCY, FXRATE, NOMINAL, PRICE, COUNTERPARTY, REF_TICKET, TRADER_INI, AA_METHOD, "
							"BEST_EXECUTION, NOTE, DEAL_TYPE, IPO, MARKET_ORDER, SAME_DAY, PARTIAL, DISCRETIONARY, "
							"EXPIRED, CANCELLED FROM SEMAM.NW_TRADE_ORDER WHERE ORDER_ID = %s", QData.GetQueryNumber(GetOrderID()));

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL COrderRec::DeleteRec(COraLoader &OraLoader)
{
	CQData QData;

	if(GetOrderID().IsEmpty())
		return FALSE;
	
	OraLoader.GetSql().Format("DELETE SEMAM.NW_TRADE_ORDER WHERE ORDER_ID = %s ", QData.GetQueryNumber(GetOrderID()));

	return OraLoader.ExecuteSql();
}

