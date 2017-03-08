#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "blockdata.h"
#include "qdata.h"
#include "nwivar.h"

IMPLEMENT_DYNAMIC(CBlockData, CRowCtrl)

void CBlockData::Setup(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR DefTrader, LPCTSTR User)
{
	m_OraLoader = OraLoader;
	CRowCtrl::SetSS(pSS);
	GetRawTicket().Setup(User, EMPTYSTRING);
	GetRawTicket().SetDefTrader(DefTrader);
	SetBlotter(FALSE);
}

int CBlockData::LoadTickets()
{
	CQData QData;	

	if(GetBlotter())
		GetOraLoader().GetSql().Format("SELECT TICKET_NUM, A.TRADER_INI, TRADE_DATE, TRANS_TYPE, DIR, VALUE_DATE, "
				"NOM_AMOUNT, STRIKE, OPT_EXPIRATION, OTC, OPT_TICK, ASSET_PID, EURO_OPT, OPT_AUTO, OPT_SET_CODE, "
				"OPT_SET_CODE2, COUNTERPARTY, CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, IPO, TRS, "
				"REPO_CP, TR_RATE, RATE_BASIS, MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, FLOAT_RATE, "
				"FLOAT_RATE_BASIS, FLOAT_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
				"MARGIN_AMOUNT, FUNDED_SWAP, BINARY, BEST_EXECUTION, SHORT_SALE, PRICE, DOWN_PYMNT, BROKER_FEE, "
				"OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, UNWIND_TICKET, CANCEL_TICKET, CORRECT_TICKET, "
				"ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, ASSET_CLASS, "
				"ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, SOURCE, ASSET_COUPON, ASSET_MATURITY "
				"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_TRADERS B "
				"WHERE B.TRADER_INI = A.TRADER_INI "
				"AND A.BOOKER = %s "
				"AND (TRADE_DATE = TRUNC(SYSDATE) OR SIGN IS NULL OR PROCESSED IS NULL) "
					"ORDER BY TICKET_NUM ", QData.GetQueryText(GetRawTicket().GetUser()));
	else
		GetOraLoader().GetSql().Format("SELECT TICKET_NUM, A.TRADER_INI, TRADE_DATE, TRANS_TYPE, DIR, VALUE_DATE, "
				"NOM_AMOUNT, STRIKE, OPT_EXPIRATION, OTC, OPT_TICK, ASSET_PID, EURO_OPT, OPT_AUTO, OPT_SET_CODE, "
				"OPT_SET_CODE2, COUNTERPARTY, CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, IPO, TRS, "
				"REPO_CP, TR_RATE, RATE_BASIS, MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, FLOAT_RATE, "
				"FLOAT_RATE_BASIS, FLOAT_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
				"MARGIN_AMOUNT, FUNDED_SWAP, BINARY, BEST_EXECUTION, SHORT_SALE, PRICE, DOWN_PYMNT, BROKER_FEE, "
				"OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, UNWIND_TICKET, CANCEL_TICKET, CORRECT_TICKET, "
				"ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, ASSET_CLASS, "
				"ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, SOURCE, ASSET_COUPON, ASSET_MATURITY "
				"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_TRADERS B "
				"WHERE B.TRADER_INI = A.TRADER_INI "
				"AND A.BOOKER = %s AND SIGN IS NULL "
				"ORDER BY TICKET_NUM ", QData.GetQueryText(GetRawTicket().GetUser()));

	if(GetOraLoader().Open()) // Set Integer Field for Ticket
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);

	return -1;
}

int CBlockData::LoadVARs(CStringArray &Rec, CString Portfolio, CString Category, double VAR)
{
	CString Port;
	CQData QData;

	Port = QData.GetQueryText(Portfolio);
	GetOraLoader().GetSql().Format("SELECT SUM(DECODE(SIGN(INDATE - TRUNC(SYSDATE)), -1, VAR, 0)), "
						"SUM(VAR) + %lf, NVL(VAR_LIMIT, 0), "
						"DECODE(SIGN(ABS(SUM(VAR) + %lf) - NVL(VAR_LIMIT, 0)), 1, 'OVER', 'OK') "
						"FROM SEMAM.ESTIMATED_VAR A "
						"LEFT OUTER JOIN SEMAM.NW_VAR_LIMIT B ON (A.PORTFOLIO = B.PORTFOLIO "
												"AND A.CATEGORY = B.CATEGORY) "
						"WHERE A.INDATE <= TRUNC(SYSDATE) "
						"AND A.PORTFOLIO = %s "
						"AND A.CATEGORY = %s "
						"GROUP BY NVL(VAR_LIMIT, 0) ", VAR, VAR, (LPCTSTR) Port,
						QData.GetQueryText(Category));

	if(!GetOraLoader().Open())
		return 0;

	Rec.RemoveAll();
	Rec.Add(GetOraLoader().GetDBString(0));
	Rec.Add(GetOraLoader().GetDBString(1));
	Rec.Add(GetOraLoader().GetDBString(2));
	Rec.Add(GetOraLoader().GetDBString(3));
	
	return Rec.GetSize();
}

BOOL CBlockData::DeleteDataRow()
{
	if(GetRawTicket().DeleteRawRecOnly(GetOraLoader()))
	{
		DeleteSheetRow(GetCurrentRow());
		SetCurrentRow(-1);
		Refresh();
		return TRUE;
	}

	return FALSE;
}

BOOL CBlockData::DeleteRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	UpdateData();

	if(GetRawTicket().GetTicket().IsEmpty())
		return FALSE;

	GetOraLoader().BeginTransaction();
	if(DeleteDataRow())
	{
		GetOraLoader().Commit();
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CBlockData::UpdateDataRow()
{
	BOOL bEmpty;

	bEmpty = GetRawTicket().GetTicket().IsEmpty();

	UpdateData();
	if(bEmpty)
	{
		GetRawTicket().SetTicket(GetRawTicket().GetID());

		if(!GetRawTicket().AddRec(GetOraLoader()))
			return FALSE;
		LoadTickets();
	}
	else
	{
		if(!GetRawTicket().UpdateRec(GetOraLoader()))
			return FALSE;

		UpdateSheetRow(FALSE);
	}

	return TRUE;
}

BOOL CBlockData::UpdateRow(BOOL bFromSheet)
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().BeginTransaction();
	if(UpdateDataRow())
	{
		GetOraLoader().Commit();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CBlockData::GetMargin(LPCTSTR Asset)
{
	GetOraLoader().GetSql().Format("SELECT NVL(MARGIN_CURRENCY, CURRENCY), "
								"MAX(NVL(MARGIN_AMOUNT, 0)), MAX(NVL(MARGIN, 0)) "
									"FROM SEMAM.NW_TR_TICKETS A "
									"WHERE ASSET_CODE = '%s' "
									"AND A.TRANS_TYPE = 'SECURITIES' "
									"GROUP BY NVL(MARGIN_CURRENCY, CURRENCY) ", Asset);
	if(!GetOraLoader().Open(GetOraLoader().GetSql()))
		return FALSE;

	GetRawTicket().SetMarginCurrency(GetOraLoader().GetDBString(0));
	GetRawTicket().SetMarginAmount(GetOraLoader().GetDBString(1));
	GetRawTicket().SetMargin(GetOraLoader().GetDBString(2));

	return TRUE;
}
