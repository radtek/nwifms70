#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "ticketapprovaldata.h"
#include "orafield.h"
#include "qdata.h"
#include "TicketRec.h"


IMPLEMENT_DYNAMIC(CTicketApprovalData, CMSRowCtrl)
CTicketApprovalData::CTicketApprovalData(CTicketApprovalData &TicketData) 
{ 
	Setup(TicketData.GetOraLoader(), TicketData.GetSS(), TicketData.GetSRowCtrl().GetSS(), 
		TicketData.GetBlotter(), TicketData.IsTrader(), TicketData.GetSign(), TicketData.m_KeyIndex); 
}

void CTicketApprovalData::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, 
								BOOL bBlotter, BOOL bTrader, BOOL bSign, int KeyIndex)
{
	CMSRowCtrl::Setup(OraLoader, pSS, pSSS, KeyIndex);
	SetBlotter(bBlotter);
	SetTrader(bTrader);
	SetSign(bSign);
}

int CTicketApprovalData::LoadTickets()
{
	if(!IsReady())
		return -1;

	CString Sql;

	if(GetBlotter())
	{
		if(IsTrader())
			Sql = "SELECT A.SIGN, A.TRADER_INI, A.TICKET_NUM, A.TRADE_DATE, A.EXEC_TIME, "
					"TO_CHAR(A.BOOK_DATE, 'MM/DD/YY HH24:MI:SS') \"DATE_STAMP\", A.VALUE_DATE, A.DIR, A.TRANS_TYPE, "
					"A.NOM_AMOUNT, A.STRIKE, A.OPT_EXPIRATION, A.OTC, A.OPT_TICK, A.OPT_SET_CODE, A.OPT_SET_CODE2, "
					"A.ASSET_PID, A.EURO_OPT, A.OPT_AUTO, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, A.ASSET_DESC, "
					"A.ASSET_CURRENCY, A.ASSET_COUNTRY, A.ASSET_COMMON_CODE, A.ASSET_MSTC_CODE, A.ASSET_ISIN_CODE, "
					"A.ASSET_SEDOL_NUM, A.ASSET_CLASS, A.ASSET_IND_AREA, A.ASSET_CATEGORY, B.BUCKET_DESC, A.ASSET_COUPON, "
					"A.ASSET_MATURITY, A.IPO, A.TRS, A.FUNDED_SWAP, A.BINARY, A.REPO_CP, A.TR_RATE, A.RATE_BASIS, A.MATURITY, "
					"A.PRICE, A.ASSIGN_CP, A.NOTE, A.NOTE2, A.AA_METHOD, A.AA_FREASON, A.SW_BOOKING, A.SW_MATURITY, "
					"A.FLOAT_RATE, A.FLOAT_RATE_BASIS, A.FLOAT_MATURITY, A.WI, A.BOOKER, A.BEST_EXECUTION, A.SHORT_SALE, "
					"A.ETRADE, A.ORDER_ID, A.CP_TRADE_ID, TO_CHAR(TRUNC(EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(PERFISO_WT, 0)) \"ALLOC\" "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_BUCKETS B, SEMAM.NW_TRADERS C, SEMAM.NW_EMCFPERFISO_EQ_ALLOC_V E "
					"WHERE B.BUCKET(+) = A.ASSET_BUCKET "
					"AND C.TRADER_INI = A.TRADER_INI "
					"AND C.USER_NAME = USER "
					"AND E.ASSET_CODE(+) = A.ASSET_CODE "
					"AND TRUNC(A.BOOK_DATE) = TRUNC(SYSDATE) "
					"UNION "
					"SELECT A.SIGN, A.TRADER_INI, A.TICKET_NUM, TRADE_DATE, A.EXEC_TIME, "
					"TO_CHAR(A.BOOK_DATE, 'MM/DD/YY HH24:MI:SS') \"DATE_STAMP\", A.VALUE_DATE, A.DIR, A.TRANS_TYPE, "
					"A.NOM_AMOUNT, A.STRIKE, A.OPT_EXPIRATION, A.OTC, A.OPT_TICK, A.OPT_SET_CODE, A.OPT_SET_CODE2, "
					"A.ASSET_PID, A.EURO_OPT, A.OPT_AUTO, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, A.ASSET_DESC, "
					"A.ASSET_CURRENCY, A.ASSET_COUNTRY, A.ASSET_COMMON_CODE, A.ASSET_MSTC_CODE, A.ASSET_ISIN_CODE, "
					"A.ASSET_SEDOL_NUM, A.ASSET_CLASS, A.ASSET_IND_AREA, A.ASSET_CATEGORY, B.BUCKET_DESC, A.ASSET_COUPON, "
					"A.ASSET_MATURITY, A.IPO, A.TRS, A.FUNDED_SWAP, A.BINARY, A.REPO_CP, A.TR_RATE, A.RATE_BASIS, A.MATURITY, "
					"A.PRICE, A.ASSIGN_CP, A.NOTE, A.NOTE2, A.AA_METHOD, A.AA_FREASON, A.SW_BOOKING, A.SW_MATURITY, "
					"A.FLOAT_RATE, A.FLOAT_RATE_BASIS, A.FLOAT_MATURITY, A.WI, A.BOOKER, A.BEST_EXECUTION, A.SHORT_SALE, "
					"A.ETRADE, A.ORDER_ID, A.CP_TRADE_ID, TO_CHAR(TRUNC(EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(PERFISO_WT, 0)) \"ALLOC\" "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_BUCKETS B, SEMAM.NW_TRADERS C, SEMAM.NW_EMCFPERFISO_EQ_ALLOC_V E, "
					"SEMAM.NW_INV_STRATEGIES F "
					"WHERE B.BUCKET(+) = A.ASSET_BUCKET " 
					"AND E.ASSET_CODE(+) = A.ASSET_CODE "
					"AND F.STRATEGY(+) = A.ASSET_CATEGORY "
					"AND C.TRADER_INI = NVL(F.TRADER_INI, 'HH') "
					"AND C.USER_NAME = USER "
					"AND TRUNC(A.BOOK_DATE) = TRUNC(SYSDATE) "
					"ORDER BY 2, 3";
		else
			Sql = "SELECT A.SIGN, A.TRADER_INI, TO_CHAR(A.TICKET_NUM) \"TICKET_NUM\", A.TRADE_DATE, A.EXEC_TIME, "
					"TO_CHAR(A.BOOK_DATE, 'MM/DD/YY HH24:MI:SS') \"DATE_STAMP\", A.VALUE_DATE, A.DIR, A.TRANS_TYPE, "
					"A.NOM_AMOUNT, A.STRIKE, A.OPT_EXPIRATION, A.OTC, A.OPT_TICK, A.OPT_SET_CODE, A.OPT_SET_CODE2, "
					"A.ASSET_PID, A.EURO_OPT, A.OPT_AUTO, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, A.ASSET_DESC, "
					"A.ASSET_CURRENCY, A.ASSET_COUNTRY, A.ASSET_COMMON_CODE, A.ASSET_MSTC_CODE, A.ASSET_ISIN_CODE, "
					"A.ASSET_SEDOL_NUM, A.ASSET_CLASS, A.ASSET_IND_AREA, A.ASSET_CATEGORY, B.BUCKET_DESC, A.ASSET_COUPON, "
					"A.ASSET_MATURITY, A.IPO, A.TRS, A.FUNDED_SWAP, A.BINARY, A.REPO_CP, A.TR_RATE, A.RATE_BASIS, "
					"A.MATURITY, A.PRICE, A.ASSIGN_CP, A.NOTE, A.NOTE2, A.AA_METHOD, A.AA_FREASON, A.SW_BOOKING, A.SW_MATURITY, "
					"A.FLOAT_RATE, A.FLOAT_RATE_BASIS, A.FLOAT_MATURITY, A.WI, A.BOOKER, A.BEST_EXECUTION, A.SHORT_SALE, "
					"A.ETRADE, A.ORDER_ID, A.CP_TRADE_ID, TO_CHAR(TRUNC(E.EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(E.PERFISO_WT, 0)) \"ALLOC\" "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_BUCKETS B, SEMAM.NW_EMCFPERFISO_EQ_ALLOC_V E "
					"WHERE B.BUCKET(+) = A.ASSET_BUCKET "
					"AND E.ASSET_CODE(+) = A.ASSET_CODE "
					"AND TRUNC(A.BOOK_DATE) = TRUNC(SYSDATE) "
					"UNION "
					"SELECT A.SIGN, A.TRADER_INI, A.TICKET_NUM||'-'||TO_CHAR(A.INV_NUM), A.TRADE_DATE, NULL \"EXEC_TIME\", "
					"TO_CHAR(A.BOOK_DATE, 'MM/DD/YY HH24:MI:SS') \"DATE_STAMP\", A.VALUE_DATE, A.DIR, A.TRANS_TYPE, "
					"A.NOM_AMOUNT, TO_NUMBER(NULL) \"EXERCISE_PRICE\", TO_DATE(NULL) \"OPT_EXPIRATION\", NULL \"LISTED\", "
					"NULL \"OPT_TICK\", NULL \"OPT_SET_CODE\", NULL \"OPT_SET_CODE2\", B.ASS_PID, NULL \"EURO_OPT\", "
					"NULL \"OPT_AUTO\", A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, B.ASS_DESC, B.ASS_CURRENCY, "
					"B.ASS_COUNTRY, B.ASS_COMMON_CODE, B.ASS_MSTC_CODE, B.ASS_ISIN_CODE, B.ASS_SEDOL_NUM, B.ASS_INDUSTRY, "
					"B.ASS_IND_AREA, B.ASS_CATEGORY, C.BUCKET_DESC, NULL, B.ASS_MATURITY_DATE, NULL \"NEW_ISSUE\", "
					"A.DEAL_TYPE, NULL \"FUNDED_SWAP\", NULL \"BINARY\", NULL, TO_CHAR(A.RATE), A.RATE_BASIS, A.MATURITY, "
					"A.PRICE, A.ASSIGN_CP, A.NOTE, A.NOTE2, NULL \"AA_METHOD\", 'UNWIND LEGACY ASSET' \"AA_FREASON\", "
					"TO_DATE(NULL) \"ORIG_SW_BOOKING\", TO_DATE(NULL) \"ORIG_SW_MATURITY\", TO_NUMBER(NULL), NULL, "
					"TO_DATE(NULL), NULL \"WI\", A.BOOKER, 'OTHERS', NULL, NULL, TO_NUMBER(NULL) \"ORDER_ID\", "
					"NULL \"CP_TRADE_ID\", TO_CHAR(TRUNC(EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(PERFISO_WT, 0)) \"ALLOC\" "
					"FROM SEMAM.NW_MATURITY_TRADE A, SEMAM.NW_ASSETS B, SEMAM.NW_BUCKETS C, SEMAM.NW_EMCFPERFISO_EQ_ALLOC_V E "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND C.BUCKET = B.BUCKET "
					"AND E.ASSET_CODE(+) = A.ASSET_CODE "
					"AND A.SIGN IS NULL AND A.PROCESSED IS NULL "
					"ORDER BY 2, 3 ";
	}
	else
		if(GetSign())
			Sql = "SELECT A.TRADER_SIGN, A.TRADER_INI, A.TICKET_NUM, A.TRADE_DATE, A.EXEC_TIME, "
					"TO_CHAR(A.BOOK_DATE, 'MM/DD/YY HH24:MI:SS') \"DATE_STAMP\", A.VALUE_DATE, A.DIR, A.TRANS_TYPE, "
					"A.NOM_AMOUNT, A.STRIKE, A.OPT_EXPIRATION, A.OTC, A.OPT_TICK, A.OPT_SET_CODE, A.OPT_SET_CODE2, "
					"A.ASSET_PID, A.EURO_OPT, A.OPT_AUTO, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, A.ASSET_DESC, "
					"A.ASSET_CURRENCY, A.ASSET_COUNTRY, A.ASSET_COMMON_CODE, A.ASSET_MSTC_CODE, A.ASSET_ISIN_CODE, "
					"A.ASSET_SEDOL_NUM, A.ASSET_CLASS, A.ASSET_IND_AREA, A.ASSET_CATEGORY, B.BUCKET_DESC, A.ASSET_COUPON, "
					"A.ASSET_MATURITY, A.IPO, A.TRS, A.FUNDED_SWAP, A.BINARY, A.REPO_CP, A.TR_RATE, A.RATE_BASIS, A.MATURITY, "
					"A.PRICE, A.ASSIGN_CP, A.NOTE, A.NOTE2, A.AA_METHOD, A.AA_FREASON, A.SW_BOOKING, A.SW_MATURITY, "
					"A.FLOAT_RATE, A.FLOAT_RATE_BASIS, A.FLOAT_MATURITY, A.WI, A.BOOKER, A.BEST_EXECUTION, A.SHORT_SALE, "
					"A.ETRADE, A.ORDER_ID, A.CP_TRADE_ID, TO_CHAR(TRUNC(EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(PERFISO_WT, 0)) \"ALLOC\" "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_BUCKETS B, SEMAM.NW_TRADERS C, SEMAM.NW_EMCFPERFISO_EQ_ALLOC_V E "
					"WHERE B.BUCKET(+) = A.ASSET_BUCKET "
					"AND C.TRADER_INI = A.TRADER_INI "
					"AND C.USER_NAME = USER "
					"AND E.ASSET_CODE(+) = A.ASSET_CODE "
					"AND A.TRADER_SIGN IS NULL "
					"AND TRUNC(A.BOOK_DATE) <= TRUNC(SYSDATE) ORDER BY 2, 3 ";
		else
			Sql = "SELECT A.SIGN, A.TRADER_INI, TO_CHAR(A.TICKET_NUM) \"TICKET_NUM\", A.TRADE_DATE, A.EXEC_TIME, "
					"TO_CHAR(A.BOOK_DATE, 'MM/DD/YY HH24:MI:SS') \"DATE_STAMP\", A.VALUE_DATE, A.DIR, A.TRANS_TYPE, "
					"A.NOM_AMOUNT, A.STRIKE, A.OPT_EXPIRATION, A.OTC, A.OPT_TICK, A.OPT_SET_CODE, A.OPT_SET_CODE2, "
					"A.ASSET_PID, A.EURO_OPT, A.OPT_AUTO, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, A.ASSET_DESC, "
					"A.ASSET_CURRENCY, A.ASSET_COUNTRY, A.ASSET_COMMON_CODE, A.ASSET_MSTC_CODE, A.ASSET_ISIN_CODE, "
					"A.ASSET_SEDOL_NUM, A.ASSET_CLASS, A.ASSET_IND_AREA, A.ASSET_CATEGORY, B.BUCKET_DESC, A.ASSET_COUPON, "
					"A.ASSET_MATURITY, A.IPO, A.TRS, A.FUNDED_SWAP, A.BINARY, A.REPO_CP, A.TR_RATE, A.RATE_BASIS, A.MATURITY, "
					"A.PRICE, A.ASSIGN_CP, A.NOTE, A.NOTE2, A.AA_METHOD, A.AA_FREASON, A.SW_BOOKING, A.SW_MATURITY, "
					"A.FLOAT_RATE, A.FLOAT_RATE_BASIS, A.FLOAT_MATURITY, A.WI, A.BOOKER, A.BEST_EXECUTION, A.SHORT_SALE, "
					"A.ETRADE, A.ORDER_ID, A.CP_TRADE_ID, TO_CHAR(TRUNC(E.EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(E.PERFISO_WT, 0)) \"ALLOC\" "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_BUCKETS B, SEMAM.NW_EMCFPERFISO_EQ_ALLOC_V E "
					"WHERE B.BUCKET(+) = A.ASSET_BUCKET "
					"AND E.ASSET_CODE(+) = A.ASSET_CODE "
					"AND SIGN IS NULL AND (SYSDATE - DATE_STAMP)*24*60 > 2 "
				    "UNION "
					"SELECT A.SIGN, A.TRADER_INI, A.TICKET_NUM||'-'||TO_CHAR(A.INV_NUM), A.TRADE_DATE, NULL \"EXEC_TIME\", "
					"TO_CHAR(A.BOOK_DATE, 'MM/DD/YY HH24:MI:SS') \"DATE_STAMP\", A.VALUE_DATE, A.DIR, A.TRANS_TYPE, "
					"A.NOM_AMOUNT, TO_NUMBER(NULL) \"EXERCISE_PRICE\", TO_DATE(NULL) \"OPT_EXPIRATION\", NULL \"LISTED\", "
					"NULL \"OPT_TICK\", NULL \"OPT_SET_CODE\", NULL \"OPT_SET_CODE2\", ASS_PID, NULL \"EURO_OPT\", "
					"NULL \"OPT_AUTO\", A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, B.ASS_DESC, B.ASS_CURRENCY, "
					"B.ASS_COUNTRY, B.ASS_COMMON_CODE, B.ASS_MSTC_CODE, B.ASS_ISIN_CODE, B.ASS_SEDOL_NUM, B.ASS_INDUSTRY, "
					"B.ASS_IND_AREA, B.ASS_CATEGORY, C.BUCKET_DESC, NULL, B.ASS_MATURITY_DATE, NULL \"NEW_ISSUE\", "
					"A.DEAL_TYPE, NULL \"FUNDED_SWAP\", NULL \"BINARY\", NULL, TO_CHAR(A.RATE), A.RATE_BASIS, A.MATURITY, "
					"A.PRICE, A.ASSIGN_CP, NOTE, NOTE2, NULL, 'UNWIND LEGACY ASSET' \"AA_FREASON\", TO_DATE(NULL) \"ORIG_SW_BOOKING\", "
					"TO_DATE(NULL) \"ORIG_SW_MATURITY\", TO_NUMBER(NULL), NULL, TO_DATE(NULL), NULL \"WI\", A.BOOKER, "
					"'OTHERS' \"BEST_EXECUTION\", NULL \"SHORT_SALE\", NULL, TO_NUMBER(NULL) \"ORDER_ID\", NULL \"CP_TRADE_ID\", "
					"TO_CHAR(TRUNC(E.EMCF_B_WT, 0))||':'||TO_CHAR(TRUNC(E.PERFISO_WT, 0)) \"ALLOC\" "
					"FROM SEMAM.NW_MATURITY_TRADE A, SEMAM.NW_ASSETS B, SEMAM.NW_BUCKETS C, SEMAM.NW_EMCFPERFISO_OTHER_ALLOC_V E "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND C.BUCKET(+) = B.BUCKET "
					"AND E.ASSET_ID(+) = A.ASSET_CODE||'|'||A.TICKET_NUM "
					"AND SIGN IS NULL AND (SYSDATE - A.BOOK_DATE)*24*60 > 1 "
					"ORDER BY 2, 3"; // Approval

	if(Sql.GetLength() > 0 && GetOraLoader().Open(Sql))
	{
		GetOraLoader().GetFieldArray().GetAt(2)->SetWithComma(FALSE);
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
	}

	return -1;
}

int CTicketApprovalData::LoadAllocList()
{
	if(!IsReady())
		return -1;
	
	CString Sql;

	if(GetBlotter())
	{
		if(IsTrader())
			Sql = "SELECT B.TICKET_NUM, ROWIDTOCHAR(B.ROWID) \"ID\", B.PORTFOLIO, B.NOM_AMOUNT, A.PRICE, "
					"A.DOWN_PYMNT, B.OPT_BACK "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_RAW_TICKET_DETAIL B, SEMAM.NW_TRADERS C "
					"WHERE B.TICKET_NUM = A.TICKET_NUM "
					"AND C.TRADER_INI = A.TRADER_INI "
					"AND C.USER_NAME = USER "
					"AND A.TRADE_DATE = TRUNC(SYSDATE) "
					"ORDER BY A.TICKET_NUM, PORTFOLIO ";
		else
			Sql = "SELECT TO_CHAR(B.TICKET_NUM) \"TICKET_NUM\", ROWIDTOCHAR(B.ROWID) \"ID\", B.PORTFOLIO, "
					"B.NOM_AMOUNT, A.PRICE, A.DOWN_PYMNT, B.OPT_BACK "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_RAW_TICKET_DETAIL B "
					"WHERE B.TICKET_NUM = A.TICKET_NUM "
					"AND TRADE_DATE = TRUNC(SYSDATE) "
					"UNION "
					"SELECT A.TICKET_NUM||'-'||TO_CHAR(A.INV_NUM), ROWIDTOCHAR(A.ROWID) \"ID\", A.PORTFOLIO, A.NOM_AMOUNT, "
					"A.PRICE, 100, TO_NUMBER(NULL) \"OPTION_BACKING\" "
					"FROM SEMAM.NW_MATURITY_TRADE A "
					"WHERE SIGN IS NULL "
					"ORDER BY 1, 3 ";
	}
	else
		if(GetSign())
			Sql = "SELECT B.TICKET_NUM, ROWIDTOCHAR(B.ROWID) \"ID\", PORTFOLIO, B.NOM_AMOUNT, A.PRICE, "
					"A.DOWN_PYMNT, B.OPT_BACK "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_RAW_TICKET_DETAIL B, SEMAM.NW_TRADERS C "
					"WHERE B.TICKET_NUM = A.TICKET_NUM "
					"AND C.TRADER_INI = A.TRADER_INI "
					"AND C.USER_NAME = USER "
					"AND A.SIGN IS NOT NULL "
					"AND TRADER_SIGN IS NULL "
					"AND A.TRADE_DATE <= TRUNC(SYSDATE) ORDER BY  1, 2 ";
		else
			Sql = "SELECT TO_CHAR(B.TICKET_NUM) \"TICKET_NUM\", ROWIDTOCHAR(B.ROWID) \"ID\", PORTFOLIO, B.NOM_AMOUNT, "
					"A.PRICE, A.DOWN_PYMNT, OPT_BACK "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_RAW_TICKET_DETAIL B "
					"WHERE B.TICKET_NUM = A.TICKET_NUM "
					"UNION "
					"SELECT A.TICKET_NUM||'-'||TO_CHAR(A.INV_NUM), ROWIDTOCHAR(A.ROWID) \"ID\", A.PORTFOLIO, A.NOM_AMOUNT, "
					"A.PRICE, 100, TO_NUMBER(NULL) \"OPTION_BACKING\" "
					"FROM SEMAM.NW_MATURITY_TRADE A "
					"WHERE A.SIGN IS NULL ORDER BY  1, 2 ";

	if(Sql.GetLength() > 0 && GetOraLoader().Open(Sql))
	{
		GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove Key
		GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove ID
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
		return GetOraLoader().LoadKeyDBList(m_AllocList);
	}

	return -1;
}

int CTicketApprovalData::LoadVARs(CStringArray &Rec, CString Portfolio, CString Category, double VAR)
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

int CTicketApprovalData::LoadPositionList(CString Asset, CDBSheet &DBSheet)
{
	LPCTSTR p;
	CQData QData;

	p = QData.GetQueryText(Asset);

	GetOraLoader().GetSql().Format("SELECT PORTFOLIO, TRANS_TYPE, STRIKE, DECODE(TRANS_TYPE, 'INT. SWAP', RATE) \"RATE\", "
									"MATURITY, TRUNC(SUM(NOM_AMOUNT), 2) \"NOMINAL\", SUM(AMOUNT) \"AMOUNT\" "
									"FROM SEMAM.HOLDING_V A, SEMAM.NW_ASSETS B "
									"WHERE B.ASS_CODE = A.ASSET_CODE "
									"AND TRANS_TYPE IN ('CALL', 'PUT', 'CDS', 'INT. SWAP', 'SECURITIES') "
									"AND A.ASSET_CODE = %s "
									"GROUP BY PORTFOLIO, ASS_DESC, TRANS_TYPE, STRIKE, RATE, MATURITY "
									"ORDER BY 1, 2, 3 ", p);
	
	if(GetOraLoader().Open())
	{
		GetOraLoader().GetFieldArray().GetAt(5)->SetPrecision(2);
		GetOraLoader().GetFieldArray().GetAt(6)->SetPrecision(2);
		GetOraLoader().LoadDBSheet(DBSheet, TRUE);
	}

	return DBSheet.GetSheetRows();
}

BOOL CTicketApprovalData::ApprovalTicket(const CString SNote)
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().BeginTransaction();

	if(GetTicket().SignTicket(GetOraLoader(), GetSign()))
	{
		if(!GetSign())
			StraightThoughTrade();
		
		if(!SNote.IsEmpty())
		{
			CString Data;
			CDBRec Rec;

			Rec.Add(GetTicket().GetTicket());
			Rec.Add(SNote);
			GetOraLoader().Open("SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKET_OPINION "
								"WHERE TICKET_NUM = " + GetTicket().GetTicket());
			GetOraLoader().LoadTextString(Data);

			if(Data.IsEmpty() || atof(Data) == 0)
			{
				GetOraLoader().Open("SELECT TICKET_NUM, NOTE FROM SEMAM.NW_RAW_TICKET_OPINION ", ODYNASET_DEFAULT);
				GetOraLoader().UpdateRecord(Rec, TRUE);
			}
			else
			{
				GetOraLoader().Open("SELECT TICKET_NUM, NOTE FROM SEMAM.NW_RAW_TICKET_OPINION "
									"WHERE TICKET_NUM = " + GetTicket().GetTicket(), ODYNASET_DEFAULT);
				GetOraLoader().UpdateRecord(Rec);
			}
		}
		
		GetOraLoader().Commit();
		GetSS()->SetSheetText(1, GetSS()->GetSheetCurRow(), GetTicket().GetSign());
		GetSS()->SheetAutoFit();

		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

void CTicketApprovalData::GetCTCode(const CString Counterparty, CString &CTCode)
{
	COraLoader OraLoader;
	CQData QData;

	OraLoader.SetDB(&theDB);

	CTCode.Empty();
	if(GetTicket().GetTransType() == FOREX || GetTicket().GetAssetClass() == "CURRENCY FWDS" || GetTicket().GetAssetClass() == "CURRENCY OPTION")
	{
		OraLoader.GetSql().Format("SELECT CT_CODE FROM SEMAM.NW_COUNTER_CONTACTS WHERE CT_FX = 'Y' AND CP_CODE = %s ", QData.GetQueryText(Counterparty));
		OraLoader.Open();
		CTCode = OraLoader.GetDBString(0);
	}
	else
	{
		if(!GetTicket().GetAsset().IsEmpty())
		{
			OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASSETS WHERE ASS_CONV_TYPE = 'F' AND ASS_CODE = %s ", QData.GetQueryText(GetTicket().GetAsset()));
			if(OraLoader.GetCount() > 0)
				CTCode = "1";
		}
	}
}

BOOL CTicketApprovalData::StraightThoughTrade()
{
	if(GetTicket().GetCPTradeID().IsEmpty() || GetTicket().GetCP().IsEmpty() || GetTicket().GetNote2() == "DEPF" || GetTicket().GetAsset() == NEWASSET)
		return FALSE;

	BOOL bWithTransNum, bRet;
	CTicketRec TicketRec;
	CInvRec InvRec;
	COraLoader OraLoader, OraLoader2;
	CQData QData;
	CString CTCode, TicketNum, TransNum, InvNum, DownPay, AssignCT;

	OraLoader.SetDB(&theDB);
	OraLoader2.SetDB(&theDB);

	TicketNum = GetTicket().GetTicket();
	GetCTCode(GetTicket().GetCP(), CTCode);
	GetCTCode(GetTicket().GetAssignCP(), AssignCT);

	if(CTCode.IsEmpty())
		return FALSE;

	OraLoader.GetSql() = "SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS WHERE TICKET_NUM = " + TicketNum;
	if(OraLoader.GetCount() > 0)
	{
		OraLoader.GetSql() = "SELECT A.PORTFOLIO, C.TRANS_NUM, C.INV_NUM, NVL(A.CUSTODIAN, DEF_CUSTODIAN) \"CUSTODIAN\", "
							"NVL(A.ACCOUNT, DEF_ACCOUNT) \"ACCOUNT\", A.NOM_AMOUNT, TRUNC(A.BROKER_FEE*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"BR_FEES\", "
							"TRUNC(A.OTHER_FEE*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"OTHER_FEES\", A.MARGIN, A.MARGIN_CURRENCY, "
							"TRUNC(A.MARGIN_AMOUNT*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"MARGIN_AMOUNT\", "
							"TRUNC(A.VAR*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"VAR\", A.CSPB_SHORT, A.PRICE, A.DOWN_PYMNT "
							"FROM SEMAM.RAW_TICKET_V A, SEMAM.NW_PORTFOLIOS B, SEMAM.TICKET_INV_V C "
							"WHERE B.PORTFOLIO = A.PORTFOLIO "
							"AND C.PORTFOLIO = A.PORTFOLIO "
							"AND C.TICKET_NUM = A.TICKET_NUM "
							"AND A.TICKET_NUM = " + TicketNum;
		bWithTransNum = 1;
	}
	else
	{
		OraLoader.GetSql() = "SELECT A.PORTFOLIO, 0 \"TRANS_NUM\", 0 \"INV_NUM\", NVL(A.CUSTODIAN, DEF_CUSTODIAN) \"CUSTODIAN\", "
								"NVL(A.ACCOUNT, DEF_ACCOUNT) \"ACCOUNT\", A.NOM_AMOUNT, TRUNC(A.BROKER_FEE*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"BR_FEES\", "
								"TRUNC(A.OTHER_FEE*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"OTHER_FEES\", A.MARGIN, A.MARGIN_CURRENCY, "
								"TRUNC(A.MARGIN_AMOUNT*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"MARGIN_AMOUNT\", "
								"TRUNC(A.VAR*A.NOM_AMOUNT/A.TOTAL_NOM_AMOUNT, 2) \"VAR\", A.CSPB_SHORT, A.PRICE, A.DOWN_PYMNT "
								"FROM SEMAM.RAW_TICKET_V A, SEMAM.NW_PORTFOLIOS B "
								"WHERE B.PORTFOLIO = A.PORTFOLIO "
								"AND A.TICKET_NUM = " + TicketNum;
		bWithTransNum = 0;
	}

	if(!OraLoader.Open(OraLoader.GetSql()))
		return FALSE;

	OraLoader.MoveFirst();
	while(!OraLoader.IsEOF())
	{
		TicketRec.SetPortfolio(OraLoader.GetDBString(0)); // Fund
		TransNum = QData.RemoveComma(OraLoader.GetDBString(1)); // TransNum
		TicketRec.SetTransNum(TransNum); // TransNum
		InvNum = QData.RemoveComma(OraLoader.GetDBString(2)); // InvNum
		TicketRec.SetCustodian(OraLoader.GetDBString(3)); // Custodian
		TicketRec.SetAccount(OraLoader.GetDBString(4)); // Account
		TicketRec.SetAmount(OraLoader.GetDBString(5)); // NomAmount;
		TicketRec.SetBrokerFee(OraLoader.GetDBString(6)); // Broker Fee
		TicketRec.SetOtherFee(OraLoader.GetDBString(7)); // Other Fee
		TicketRec.SetMargin(OraLoader.GetDBString(8)); // Margin
		TicketRec.SetMarginCurrency(OraLoader.GetDBString(9)); // Margin Currency
		TicketRec.SetMarginAmount(OraLoader.GetDBString(10)); // Margin Amount
		TicketRec.SetVAR(OraLoader.GetDBString(11)); // VAR
		TicketRec.SetCSPBShort(OraLoader.GetDBString(12)); // CSPB_Short
		TicketRec.SetPrice(OraLoader.GetDBString(13)); // Price
		DownPay = OraLoader.GetDBString(14);

		TicketRec.SetTrader(GetTicket().GetTrader()); // Trader
		TicketRec.SetTicket(GetTicket().GetTicket()); // Ticket
		TicketRec.SetTradeDate(GetTicket().GetTradeDate()); // Trade Date
		TicketRec.SetExecTime(GetTicket().GetExecTime()); // Exec Time
		TicketRec.SetValueDate(GetTicket().GetValueDate()); // Value Date
		TicketRec.SetDir(GetTicket().GetDir()); // Dir

		TicketRec.SetTransType(GetTicket().GetTransType()); // TransType
		TicketRec.SetTRS(GetTicket().GetTRS());  // TRS

		if(GetTicket().GetTRS() == TRS && DownPay == "100")
			TicketRec.SetTransType(GetTicket().GetRepoType()); // TransType

		if(TicketRec.GetTransType() == CALL || TicketRec.GetTransType() == PUT)
		{
			TicketRec.SetStrike(GetTicket().GetStrike());
			TicketRec.SetOptExp(GetTicket().GetOptExp());
			TicketRec.SetListed(GetTicket().GetListed());
			TicketRec.SetOptTicker(GetTicket().GetOptTicker());
			TicketRec.SetOptSet(GetTicket().GetOptSetCode());
			TicketRec.SetOptSet2(GetTicket().GetOptSetCode2());
			TicketRec.SetOptID(GetTicket().GetOptID());
			if(GetTicket().GetAssetClass() == "CURRENCY OPTION")
				TicketRec.SetEuroOpt("Y");
			else
				TicketRec.SetEuroOpt(GetTicket().GetEuroOpt());
			
			TicketRec.SetOptAuto(GetTicket().GetOptAuto());
		}
		else
		{
			TicketRec.SetStrike(EMPTYSTRING);
			TicketRec.SetOptExp(EMPTYSTRING);
			TicketRec.SetListed(EMPTYSTRING);
			TicketRec.SetOptTicker(EMPTYSTRING);
			TicketRec.SetOptSet(EMPTYSTRING);
			TicketRec.SetOptSet2(EMPTYSTRING);
			TicketRec.SetOptID(EMPTYSTRING);
			TicketRec.SetEuroOpt(EMPTYSTRING);
			TicketRec.SetOptAuto(EMPTYSTRING);
		}

		TicketRec.SetCurrency(GetTicket().GetCurrency());
		TicketRec.SetFxRate(GetTicket().GetFxRate());
		TicketRec.SetCP(GetTicket().GetCP());
		TicketRec.SetCT(CTCode);
		TicketRec.SetAsset(QData.RemoveString(GetTicket().GetAsset(), "'"));
		TicketRec.SetIPO(GetTicket().GetIPO());
		TicketRec.SetFunded(GetTicket().GetFunded());
		TicketRec.SetBinary(GetTicket().GetBinary());
		if(TicketRec.GetTransType() == REPO || TicketRec.GetTransType() == INTSWAP || TicketRec.GetTransType() == LEVERAGE)
		{
			TicketRec.SetRate(GetTicket().GetRate());
			TicketRec.SetRateBasis(GetTicket().GetRateBasis());
			TicketRec.SetMaturity(GetTicket().GetMaturity());
		}
		else
		{
			TicketRec.SetRate(EMPTYSTRING);
			TicketRec.SetRateBasis(EMPTYSTRING);
			TicketRec.SetMaturity(EMPTYSTRING);
		}

		TicketRec.SetNote(GetTicket().GetNote());
		TicketRec.SetNote2(GetTicket().GetNote2());
		TicketRec.SetSWBooking(GetTicket().GetSWBooking());
		TicketRec.SetSWMaturity(GetTicket().GetSWMaturity());
		TicketRec.SetWI(GetTicket().GetWI());
		TicketRec.SetBestExecution(GetTicket().GetBestExecution());
		TicketRec.SetShortSale(GetTicket().GetShortSale());
		TicketRec.SetETrade(GetTicket().GetETrade());
		TicketRec.SetCPID(GetTicket().GetCPTradeID());
		if(GetTicket().GetTransType() == CDS || GetTicket().GetTransType() == REPO || GetTicket().GetTransType() == LEVERAGE || GetTicket().GetTransType() == INTSWAP)
			TicketRec.SetPFU("U");
		else
			TicketRec.SetPFU("P");

		TicketRec.SetConfirm(GetTicket().GetCPTradeID().IsEmpty() ? EMPTYSTRING : "Y");
		TicketRec.SetAssignCP(GetTicket().GetAssignCP());
		TicketRec.SetAssignCT(AssignCT);
		TicketRec.SetBooker(EMPTYSTRING);
		TicketRec.SetBookDate(EMPTYSTRING);

		OraLoader2.BeginTransaction();
		if(bWithTransNum) // Correct
		{
			InvRec.SetID(InvNum);
			InvRec.SetTransNum(TransNum);
			bRet = TicketRec.UpdateRec(OraLoader2);
		}
		else
		{
			OraLoader2.GenerateUniqueID("TRANS_NUM", TransNum);
			if(TransNum.IsEmpty())
				return FALSE;
			
			TicketRec.SetTransNum(TransNum);
			InvRec.SetTransNum(TransNum);
			bRet = TicketRec.AddRec(OraLoader2);
		}

		if(bRet)
			OraLoader2.Commit();
		else
			OraLoader2.Rollback();

		OraLoader2.ExecuteSql("UPDATE SEMAM.NW_RAW_TICKETS SET PROCESSED = 'Y' WHERE TICKET_NUM = " + TicketNum);
		OraLoader.MoveNext();
	}

	return TRUE;
}
