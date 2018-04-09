#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "entrydata.h"
#include "qdata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CEntryData, CMSRowCtrl)
void CEntryData::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR DefTrader, 
					   LPCTSTR User, int KeyIndex)
{
	CMSRowCtrl::Setup(OraLoader, pSS, pSSS, KeyIndex);
	GetRawTicket().Setup(User, EMPTYSTRING);
	GetRawTicket().SetDefTrader(DefTrader);
	SetBlotter(FALSE);
	GetFXEntry() = FALSE;
}

int CEntryData::AllocCheckSum(CString Portfolio, double CurrentAmount)
{
	CQData QData;
	CString sNom;
	double dNom;

	GetRawTicket().SetNomAmount(GetRawTicket().GetNomAmount()); // Update
	
	sNom = GetRawTicket().GetNomAmount();

	if(sNom.IsEmpty())
		return -1000;

	dNom = atof(QData.RemoveComma(sNom));

	if(fabs(GetAllocAmount() - dNom) <= 0.00001)
		return 0;

	if(dNom - GetAllocAmount() > 0)
		return -1;

	return 1;
}

/* double CEntryData::ComputeAllocAmount(CString Portfolio, double CurrentAmount, 
						double CurrentBrokerFee, double CurrentSoftDollar, 
						double CurrentOtherFee, double CurrentVAR)
{
	CRawInvRec InvRec;
	CQData QData;
	double AllNom, BrFee, SoftDollar, OtherFee, VAR;

	UpdateData(&GetRawTicket(), &GetRawInv());

	m_AllocAmount = 0;
	m_BrokerFee = 0;
	m_SoftDollar = 0;
	m_OtherFee = 0;
	m_VAR = 0;

	for (int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i++)
	{				
		GetSelRawInv(i, InvRec);
		AllNom = atof(QData.RemoveComma(InvRec.GetNomAmount()));
		BrFee = atof(QData.RemoveComma(InvRec.GetBrokerFee()));
		SoftDollar = atof(QData.RemoveComma(InvRec.GetSoftDollar()));
		OtherFee = atof(QData.RemoveComma(InvRec.GetOtherFee()));
		VAR = atof(QData.RemoveComma(InvRec.GetVAR()));

		if(Portfolio == InvRec.GetPortfolio())
		{
			AllNom = 0;
			BrFee = 0;
			SoftDollar = 0;
			OtherFee = 0;
			VAR = 0;
		}

		m_AllocAmount += AllNom;
		m_BrokerFee += BrFee;
		m_SoftDollar += SoftDollar;
		m_OtherFee += OtherFee;
		m_VAR += VAR;
	}

	if(!Portfolio.IsEmpty())
	{
		m_AllocAmount += CurrentAmount; // Add passed nom_amount
		m_BrokerFee += CurrentBrokerFee;
		m_SoftDollar += CurrentSoftDollar;
		m_OtherFee += CurrentOtherFee;
		m_VAR += CurrentVAR;
	}

	return m_AllocAmount;
} */

double CEntryData::ComputeAllocAmount(CString Portfolio, double CurrentAmount)
{
	CRawInvRec InvRec;
	CQData QData;
	double AllNom;

	UpdateData(&GetRawTicket(), &GetRawInv());

	m_AllocAmount = 0;

	for (int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i++)
	{				
		GetSelRawInv(i, InvRec);
		AllNom = atof(QData.RemoveComma(InvRec.GetNomAmount()));
		if(Portfolio == InvRec.GetPortfolio())
		{
			AllNom = 0;
		}

		m_AllocAmount += AllNom;
	}

	if(!Portfolio.IsEmpty())
	{
		m_AllocAmount += CurrentAmount; // Add passed nom_amount
	}

	return m_AllocAmount;
}

BOOL CEntryData::FindPortfolio(LPCTSTR Portfolio)
{
	UpdateData(&GetRawTicket(), &GetRawInv());

	CRawInvRec InvRec;	
	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
	{
		if(GetSelRawInv(i, InvRec) && InvRec.GetPortfolio() == Portfolio)
			return TRUE;
	}
	
	return FALSE;
}

BOOL CEntryData::GetSelRawInv(int Index, CRawInvRec &RecInvRec)
{
	if(Index < 1 || Index > GetSRowCtrl().GetSS()->GetSheetRows())
		return FALSE;

	CString Text;
	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetCols(); i++)
	{
		Text = GetSRowCtrl().GetSS()->GetSheetText(i, Index);
		switch(i)
		{
			case 1:
				RecInvRec.SetPortfolio(Text);
				break;
			case 2:
				RecInvRec.SetNomAmount(Text);
				break;
			case 3:
				RecInvRec.SetCustodian(Text);
				break;
			case 4:
				RecInvRec.SetAccount(Text);
				break;
			case 5:
				RecInvRec.SetPrice(Text);
				break;
			case 6:
				RecInvRec.SetLink(Text);
				break;
/*			case 5:
				RecInvRec.SetAssignCT(Text);
				break;
			case 4:
				RecInvRec.SetDownPymnt(Text);
				break;
			case 5:
				RecInvRec.SetBrokerFee(Text);
				break;
			case 6:
				RecInvRec.SetSoftDollar(Text);
				break;
			case 7:
				RecInvRec.SetOtherFee(Text);
				break;
			case 8:
				RecInvRec.SetVAR(Text);
				break;
			case 9:
				RecInvRec.SetLink(Text);
				break;
			case 10:
				RecInvRec.SetAssignCP(Text);
				break;
			case 11:
				RecInvRec.SetAssignCT(Text);
				break; */
			default:
				break;
		}
	}
	
	return TRUE;
}

int CEntryData::LoadTickets()
{
	CQData QData;
	CString sUser, sUserIni, Sql;

	sUser = QData.GetQueryText(GetRawTicket().GetUser());
	sUserIni = QData.GetQueryText(GetRawTicket().GetTrader());

	GetSRowCtrl().GetSS()->ClearSheet();
	if(GetFXEntry())
	{
		Sql = "SELECT A.TICKET_NUM, A.TRADER_INI, TRADE_DATE, EXEC_TIME, TRANS_TYPE, DIR, VALUE_DATE, NOM_AMOUNT, "
				"STRIKE, OPT_EXPIRATION, OTC, OPT_TICK, OPT_SET_CODE, OPT_SET_CODE2, ASSET_PID, EURO_OPT, "
				"OPT_AUTO, COUNTERPARTY, CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, IPO, TRS, "
				"REPO_CP, TR_RATE, RATE_BASIS, MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, FLOAT_RATE, "
				"FLOAT_RATE_BASIS, FLOAT_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
				"MARGIN_AMOUNT, FUNDED_SWAP, BINARY, AA_METHOD, AA_FREASON, BEST_EXECUTION, SHORT_SALE, "
				"CSPB_SHORT, ETRADE, PRICE, DOWN_PYMNT, BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, "
				"UNWIND_TICKET, CANCEL_TICKET, CORRECT_TICKET, ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, "
				"ASSET_ISIN_CODE, ASSET_SEDOL_NUM, ASSET_CLASS, ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, "
				"SOURCE, ASSET_COUPON, ASSET_MATURITY, NULL \"CP_KEY\", A.ORDER_ID ";
		if(GetBlock()) // Block Trade not allocation
			Sql += "FROM SEMAM.NW_RAW_TICKETS A "
					"WHERE SIGN IS NULL "
					"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKET_DETAIL C "
					"WHERE C.TICKET_NUM = C.TICKET_NUM) ";
		else
			Sql += "FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_TRADERS B "
					"WHERE B.TRADER_INI = A.TRADER_INI "
					"AND SIGN IS NULL "
					"AND 0 < (SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKET_DETAIL C WHERE C.TICKET_NUM = C.TICKET_NUM) ";

		Sql +=	"AND (A.ASSET_CLASS IN ('CURRENCY FWDS', 'CURRENCY OPTION') OR TRANS_TYPE = 'FOREX') ";
				"UNION "
				"SELECT TO_NUMBER(NULL) \"TICKET_NUM\", A.TRADER_INI, A.TRADE_DATE, NULL \"EXEC_TIME\", "
				"TRANS_TYPE, DIR, VALUE_DATE, NOM_AMOUNT, STRIKE, MATURITY, OTC, OPT_TICK, OPT_SET_CODE, "
				"OPT_SET_CODE2, ASSET_PID, EURO_OPT, NULL \"OPT_AUTO\", COUNTERPARTY, CURRENCY, "
				"DECODE(CURRENCY, 'USD', 1, TO_NUMBER(NULL)) \"FXRATE\", ASSET_CODE, ASSET, A.ASSET_CURRENCY, "
				"IPO, NULL \"TRS\", NULL \"REPO_CP\", RATE, NULL \"RATE_BASIS\", "
				"DECODE(TRANS_TYPE, 'CALL', TO_DATE(NULL), 'PUT', TO_DATE(NULL), MATURITY), \"MATURITY\", "
				"NULL \"NOTE\", NULL \"NOTE2\", TO_DATE(NULL) \"SW_BOOKING\", TO_DATE(NULL) \"SW_MATURITY\", "
				"TO_NUMBER(NULL) \"FLOAT_RATE\", NULL \"FLOAT_RATE_BASIS\", TO_DATE(NULL) \"FLOAT_MATURITY\", "
				"NULL \"WI\", TO_NUMBER(NULL) \"SWAP_TICKET\", DELIVERY_DATE, TO_NUMBER(NULL) \"MARGIN\", "
				"NULL \"MARGIN_CURRENCY\", TO_NUMBER(NULL) \"MARGIN_AMOUNT\", NULL \"FUNDED_SWAP\", BINARY, "
				"AA_METHOD, NULL \"AA_FREASON\", NULL \"BEST_EXECUTION\", NULL \"SHORT_SALE\", NULL \"CSPB_SHORT\", "
				"ETRADE, PRICE, DECODE(A.DEAL_TYPE, 'TRS', 0, 100) \"DOWN_PYMNT\", TO_NUMBER(NULL) \"BROKER_FEE\", "
				"TO_NUMBER(NULL) \"OTHER_FEE\", TO_NUMBER(NULL) \"SOFTDOLLAR\", NULL \"ASSIGN_CP\", "
				"TO_NUMBER(NULL) \"DV01\", TO_NUMBER(NULL) \"VAR\", TO_NUMBER(REF_TICKET) \"UNWIND_TICKET\", "
				"TO_NUMBER(NULL) \"CANCEL_TICKET\", TO_NUMBER(NULL) \"CORRECT_TICKET\", ASS_COUNTRY, ASS_COMMON_CODE, "
				"ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, ASSET_CLASS, ASS_IND_AREA, CATEGORY, BUCKET, SOURCE, "
				"NULL \"ASSET_COUPON\", ASSET_MATURITY, A.CP_KEY, A.ORDER_ID "
				"FROM SEMAM.V_PRE_RAW_FX_TICKETS A, SEMAM.NW_ASSETS B "
				"WHERE AND B.ASS_CODE(+) = A.ASSET_CODE "
				"AND A.PROCESSED IS NULL "
				"ORDER BY 1 ";
	}
	else
	{
		if(GetBlotter())
			Sql = "SELECT TICKET_NUM, A.TRADER_INI, TRADE_DATE, EXEC_TIME, TRANS_TYPE, DIR, VALUE_DATE, NOM_AMOUNT, "
					"STRIKE, OPT_EXPIRATION, OTC, OPT_TICK, OPT_SET_CODE, OPT_SET_CODE2, ASSET_PID, EURO_OPT, "
					"OPT_AUTO, COUNTERPARTY, CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, IPO, TRS, "
					"REPO_CP, TR_RATE, RATE_BASIS, MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, FLOAT_RATE, "
					"FLOAT_RATE_BASIS, FLOAT_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
					"MARGIN_AMOUNT, FUNDED_SWAP, BINARY, AA_METHOD, AA_FREASON, BEST_EXECUTION, SHORT_SALE, CSPB_SHORT, "
					"ETRADE, PRICE, DOWN_PYMNT, BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, UNWIND_TICKET, "
					"CANCEL_TICKET, CORRECT_TICKET, ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, "
					"ASSET_SEDOL_NUM, ASSET_CLASS, ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, SOURCE, ASSET_COUPON, "
					"ASSET_MATURITY, REV, ORDER_ID "
					"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_TRADERS B, SEMAM.NW_ASSETS C, SEMAM.NW_CURRENCY D "
					"WHERER B.TRADER_INI = A.TRADER_INI "
					"AND C.ASS_CODE(+) = A.ASSET_CODE "
					"AND D.CURRENCY(+) = C.ASS_CURRENCY2 "
					"AND A.BOOKER = " + sUser + 
					" AND (TRADE_DATE = TRUNC(SYSDATE) OR SIGN IS NULL OR PROCESSED IS NULL) "
					"ORDER BY TICKET_NUM ";
		else
		{
			Sql = "SELECT A.TICKET_NUM, A.TRADER_INI, TRADE_DATE, EXEC_TIME, TRANS_TYPE, DIR, VALUE_DATE, NOM_AMOUNT, "
					"STRIKE, OPT_EXPIRATION, OTC, OPT_TICK, OPT_SET_CODE, OPT_SET_CODE2, ASSET_PID, EURO_OPT, OPT_AUTO, "
					"COUNTERPARTY, A.CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, IPO, TRS, REPO_CP, TR_RATE, "
					"RATE_BASIS, MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, FLOAT_RATE, FLOAT_RATE_BASIS, "
					"FLOAT_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, "
					"BINARY, AA_METHOD, AA_FREASON, BEST_EXECUTION, SHORT_SALE, CSPB_SHORT, ETRADE, PRICE, DOWN_PYMNT, "
					"BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, UNWIND_TICKET, CANCEL_TICKET, CORRECT_TICKET, "
					"ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, ASSET_CLASS, "
					"ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, A.SOURCE, ASSET_COUPON, ASSET_MATURITY, REV, ORDER_ID ";		
			if(GetBlock())
				Sql += 	"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS C, SEMAM.NW_CURRENCY D "
						"WHERE C.ASS_CODE(+) = A.ASSET_CODE "
						"AND D.CURRENCY(+) = C.ASS_CURRENCY2 "
						"AND SIGN IS NULL "
						"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKET_DETAIL B WHERE B.TICKET_NUM = A.TICKET_NUM) ";
			else
				Sql += "FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_TRADERS B, SEMAM.NW_ASSETS C, SEMAM.NW_CURRENCY D "
						"WHERE B.TRADER_INI = A.TRADER_INI "
						"AND C.ASS_CODE(+) = A.ASSET_CODE "
						"AND D.CURRENCY(+) = C.ASS_CURRENCY2 "
						"AND A.BOOKER = " + sUser + 
						" AND SIGN IS NULL "
						"AND 0 < (SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKET_DETAIL C WHERE C.TICKET_NUM = C.TICKET_NUM) ";
			Sql += "UNION "
					"SELECT A.TICKET_NUM, A.TRADER_INI, A.ORDER_DATE, NULL, A.TRANS_TYPE, A.TRANS_DIR, "
					"DECODE(B.ASS_INDUSTRY, 'CURRENCY FWDS', B.ASS_MATURITY_DATE, TO_DATE(NULL)) \"VALUE_DATE\", "
					"A.NOMINAL, A.STRIKE, A.OPT_EXP, NULL \"OTC\", NVL(E.OPT_TICK, C.OPT_TICK), C.OPT_SET_CODE, "
					"C.OPT_SET_CODE2, DECODE(A.TRANS_TYPE, 'CALL', C.OPT_ID, 'PUT', C.OPT_ID, B.ASS_PID), "
					"C.EURO_OPT, C.OPT_AUTO, A.COUNTERPARTY, A.CURRENCY, A.FXRATE, A.ASSET_CODE, B.ASS_DESC, "
					"B.ASS_CURRENCY, A.IPO, NULL \"TRS\", NULL \"REPO_CP\", TO_CHAR(RATE), "
					"DECODE(A.TRANS_TYPE, 'LEVERAGE', 'A/360', 'REPO', 'A/360', B.ASS_RATE_BASIS), MATURITY, "
					"NULL \"NOTE\", NULL \"NOTE2\", TO_DATE(NULL) \"SW_BOOKING\", TO_DATE(NULL) \"SW_MATURITY\", "
					"TO_NUMBER(NULL) \"FLOAT_RATE\", D.ASS_RATE_BASIS, A.MATURITY, NULL \"WI\", "
					"TO_NUMBER(NULL) \"SWAP_TICKET\", C.DELIVERY_DATE, TO_NUMBER(NULL) \"MARGIN\", A.CURRENCY, "
					"TO_NUMBER(NULL) \"MARGIN_AMOUNT\", NULL \"FUNDED_SWAP\", NULL \"BINARY\", A.AA_METHOD, "
					"NULL \"AA_FREASON\", A.BEST_EXECUTION, NULL \"SHORT_SALE\", NULL \"CSPB_SHORT\", C.ETRADE, "
					"A.PRICE, DECODE(A.DEAL_TYPE, 'TRS', 0, 100) \"DOWN_PYMNT\", TO_NUMBER(NULL) \"BROKER_FEE\", "
					"TO_NUMBER(NULL) \"OTHER_FEE\", TO_NUMBER(NULL) \"SOFTDOLLAR\", NULL \"ASSIGN_CP\", "
					"TO_NUMBER(NULL) \"DV01\", TO_NUMBER(NULL) \"VAR\", TO_NUMBER(A.REF_TICKET) \"UNWIND_TICKET\", "
					"TO_NUMBER(NULL) \"CANCEL_TICKET\", TO_NUMBER(NULL) \"CORRECT_TICKET\", B.ASS_COUNTRY, "
					"B.ASS_COMMON_CODE, B.ASS_MSTC_CODE, B.ASS_ISIN_CODE, B.ASS_SEDOL_NUM, B.ASS_INDUSTRY, "
					"B.ASS_IND_AREA, B.ASS_CATEGORY, B.BUCKET, B.SOURCE, NULL, B.ASS_MATURITY_DATE, REV, A.ORDER_ID "
					"FROM SEMAM.V_TRADE_ORDER A, SEMAM.NW_ASSETS B, SEMAM.NW_TR_TICKETS C, "
					"SEMAM.NW_ASSETS D, SEMAM.V_OPTION_TICK E, SEMAM.NW_CURRENCY F "
					"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
					"AND C.TICKET_NUM(+) = A.REF_TICKET "
					"AND D.ASS_CODE(+) = A.ASSET_CODE2 "
					"AND E.OPT_ASSET_CODE(+) = A.ASSET_CODE||'|'||SUBSTR(A.TRANS_TYPE, 1, 1)||'|'||TO_CHAR(A.STRIKE)||'|'||TO_CHAR(A.OPT_EXP) "
					"AND F.CURRENCY(+) = B.ASS_CURRENCY2 "
					"AND A.TICKET_NUM IS NULL "
					"AND A.ORDER_DATE = TRUNC(SYSDATE) "
					"AND A.SAME_DAY = 'Y' "
					"AND A.EXPIRED IS NULL "
					"AND A.CANCELLED IS NULL ";
			if(!GetBlock())
				Sql += "AND A.BOOKER = " + sUser + " ";
			Sql += "UNION "
					"SELECT A.TICKET_NUM, A.TRADER_INI, A.ORDER_DATE, NULL, A.TRANS_TYPE, A.TRANS_DIR, "
					"DECODE(B.ASS_INDUSTRY, 'CURRENCY FWDS', B.ASS_MATURITY_DATE, TO_DATE(NULL)) \"VALUE_DATE\", "
					"A.NOMINAL, A.STRIKE, A.OPT_EXP, NULL \"OTC\", NVL(E.OPT_TICK, C.OPT_TICK), C.OPT_SET_CODE, "
					"C.OPT_SET_CODE2, DECODE(A.TRANS_TYPE, 'CALL', C.OPT_ID, 'PUT', C.OPT_ID, B.ASS_PID), "
					"C.EURO_OPT, C.OPT_AUTO, A.COUNTERPARTY, A.CURRENCY, A.FXRATE, A.ASSET_CODE, B.ASS_DESC, "
					"B.ASS_CURRENCY, A.IPO, NULL \"TRS\", NULL \"REPO_CP\", TO_CHAR(RATE), "
					"DECODE(A.TRANS_TYPE, 'LEVERAGE', 'A/360', 'REPO', 'A/360', B.ASS_RATE_BASIS), MATURITY, "
					"NULL \"NOTE\", NULL \"NOTE2\", TO_DATE(NULL) \"SW_BOOKING\", TO_DATE(NULL) \"SW_MATURITY\", "
					"TO_NUMBER(NULL) \"FLOAT_RATE\", D.ASS_RATE_BASIS, A.MATURITY, NULL \"WI\", "
					"TO_NUMBER(NULL) \"SWAP_TICKET\", C.DELIVERY_DATE, TO_NUMBER(NULL) \"MARGIN\", A.CURRENCY, "
					"TO_NUMBER(NULL) \"MARGIN_AMOUNT\", NULL \"FUNDED_SWAP\", NULL \"BINARY\", A.AA_METHOD, "
					"NULL \"AA_FREASON\", A.BEST_EXECUTION, NULL \"SHORT_SALE\", NULL \"CSPB_SHORT\", C.ETRADE, "
					"A.PRICE, DECODE(A.DEAL_TYPE, 'TRS', 0, 100) \"DOWN_PYMNT\", TO_NUMBER(NULL) \"BROKER_FEE\", "
					"TO_NUMBER(NULL) \"OTHER_FEE\", TO_NUMBER(NULL) \"SOFTDOLLAR\", NULL \"ASSIGN_CP\", "
					"TO_NUMBER(NULL) \"DV01\", TO_NUMBER(NULL) \"VAR\", TO_NUMBER(A.REF_TICKET) \"UNWIND_TICKET\", "
					"TO_NUMBER(NULL) \"CANCEL_TICKET\", TO_NUMBER(NULL) \"CORRECT_TICKET\", B.ASS_COUNTRY, "
					"B.ASS_COMMON_CODE, B.ASS_MSTC_CODE, B.ASS_ISIN_CODE, B.ASS_SEDOL_NUM, B.ASS_INDUSTRY, "
					"B.ASS_IND_AREA, B.ASS_CATEGORY, B.BUCKET, B.SOURCE, NULL, B.ASS_MATURITY_DATE, REV, A.ORDER_ID "
					"FROM SEMAM.V_TRADE_ORDER A, SEMAM.NW_ASSETS B, SEMAM.NW_TR_TICKETS C, SEMAM.NW_ASSETS D, "
					"SEMAM.V_OPTION_TICK E, SEMAM.NW_CURRENCY F "
					"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
					"AND C.TICKET_NUM(+) = A.REF_TICKET "
					"AND D.ASS_CODE(+) = A.ASSET_CODE2 "
					"AND E.OPT_ASSET_CODE(+) = A.ASSET_CODE||'|'||SUBSTR(A.TRANS_TYPE, 1, 1)||'|'||TO_CHAR(A.STRIKE)||'|'||TO_CHAR(A.OPT_EXP) "
					"AND F.CURRENCY(+) = B.ASS_CURRENCY2 "
					"AND A.TICKET_NUM IS NULL "
					"AND A.ORDER_DATE <= TRUNC(SYSDATE) "
					" AND A.SAME_DAY IS NULL "
					"AND A.EXPIRED IS NULL "
					"AND A.CANCELLED IS NULL ";
			if(!GetBlock())
				Sql += "AND A.BOOKER = " + sUser + " ";
			Sql += "ORDER BY 1 ";
		}
	}

	if(GetOraLoader().Open(Sql)) // Set Integer Field for Ticket
	{
		GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
		GetOraLoader().GetFieldArray().GetAt(GetOraLoader().GetFieldArray().GetSize() - 1)->SetWithComma(FALSE);
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
	}

	return -1;
}

int CEntryData::LoadAllocList()
{
	CQData QData;
	
	if(GetBlotter())
		GetOraLoader().GetSql().Format("SELECT A.TICKET_NUM, ROWIDTOCHAR(A.ROWID) \"ID\", A.PORTFOLIO, "
										"A.NOM_AMOUNT, A.CUSTODIAN, A.ACCOUNT, A.PRICE, A.OPT_BACK "
										"FROM SEMAM.NW_RAW_TICKET_DETAIL A, SEMAM.NW_RAW_TICKETS B, SEMAM.NW_TRADERS C "
										"WHERE B.TICKET_NUM = A.TICKET_NUM "
										"AND B.BOOKER = %s "
										"AND C.TRADER_INI = B.TRADER_INI "
										"AND TRADE_DATE = TRUNC(SYSDATE) OR SIGN IS NULL OR PROCESSED IS NULL) "
										"ORDER BY 1, 3 ", QData.GetQueryText(GetRawTicket().GetUser()));
	else
		GetOraLoader().GetSql().Format("SELECT A.TICKET_NUM, ROWIDTOCHAR(A.ROWID) \"ID\", PORTFOLIO, "
										"A.NOM_AMOUNT, A.CUSTODIAN, A.ACCOUNT, A.PRICE, A.OPT_BACK "
										"FROM SEMAM.NW_RAW_TICKET_DETAIL A, SEMAM.NW_RAW_TICKETS B, SEMAM.NW_TRADERS C "
										"WHERE B.TICKET_NUM = A.TICKET_NUM "
										"AND B.BOOKER = %s "
										"AND B.SIGN IS NULL "
										"AND C.TRADER_INI = B.TRADER_INI "
										"ORDER BY 1, 3 ", QData.GetQueryText(GetRawTicket().GetUser()));

	if(GetOraLoader().Open())
	{
		// Set Integer Field for Ticket
		GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
		GetOraLoader().GetFieldArray().GetAt(7)->SetWithComma(FALSE);
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove Key
		GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove		
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
		return GetOraLoader().LoadKeyDBList(GetAllocList());
	}

	return -1;
}

int CEntryData::LoadVARs(CStringArray &Rec, CString Portfolio, CString Category, double VAR)
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

void CEntryData::AddAllocListItem()
{
	int i, j;
	CKeyDBRecArray KeyRecArray;
	CDBRec Rec;

	KeyRecArray.SetKey(GetKey());
	for(i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
	{
		Rec.RemoveAll();
		for(j = 1; j <= GetSRowCtrl().GetSS()->GetSheetCols(); j ++)
			Rec.Add(GetSRowCtrl().GetSS()->GetSheetText(j, i));
		KeyRecArray.Add(Rec);
	}

	GetAllocList().Add(KeyRecArray);
}

void CEntryData::DeleteAllocListItem()
{
	int i;
	i = GetAllocList().Find(GetKey());
	if(i >= 0)
		GetAllocList().RemoveAt(i);
}

BOOL CEntryData::DeleteDataRow()
{
	GetRawInv().SetTicket(GetKey());
	if(GetRawInv().DeleteRec(GetOraLoader()) && GetRawTicket().DeleteRec(GetOraLoader()))	
	{
		DeleteSheetRow(GetCurrentRow());
		DeleteAllocListItem();		
		SetCurrentRow(-1);
		Refresh();
		return TRUE;
	}

	return FALSE;
}

BOOL CEntryData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetRawTicket(), &GetRawInv());

	if(strlen(GetKey()) == 0)
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

BOOL CEntryData::UpdateDataRow()
{
	BOOL bInDatabase;

	bInDatabase = strlen(GetKey());

	UpdateData(&GetRawTicket(), &GetRawInv());
	if(bInDatabase)
	{
		if(!GetRawTicket().UpdateRec(GetOraLoader()))
			return FALSE;
	}
	else
	{
		if(!GetRawTicket().AddRec(GetOraLoader()))
			return FALSE;
	}

	GetRawInv().SetTicket(GetKey());
	if(bInDatabase && !GetRawInv().DeleteRec(GetOraLoader()))
		return FALSE;

	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
	{
		GetSRowCtrl().SetCurrentRow(i);
		GetRawInv().GetCustodian() = GetCustodian();
		if(!GetRawInv().AddRec(GetOraLoader()))
			return FALSE;
	}

	if(bInDatabase)
		UpdateSheetRow(FALSE);
	else // If new, reload details
		LoadTickets();

	LoadAllocList();

	return TRUE;
}

BOOL CEntryData::UpdateRow()
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

int CEntryData::GetUnSignTicketCount()
{
	CString Sql;

	if(!IsReady())
		return 0;

	Sql = "SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_TRADERS B "
			"WHERE B.TRADER_INI = A.TRADER_INI "
			"AND B.USER_NAME = USER "
			"AND SIGN IS NOT NULL AND TRADER_SIGN IS NULL "
			"AND TRADE_DATE < TRUNC(SYSDATE) ";
	return GetOraLoader().GetCount(Sql);
}

BOOL CEntryData::GetMargin(LPCTSTR Asset)
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

const CString CEntryData::GetAutoAllocationSql(const CString AAMethod, const CString NomAmount, 
						const CString BrokerFee, const CString SoftDollar, 
						const CString OtherFee, const CString VAR, const CString Ticket)
{
	CQData QData;
	CString Sql, Text;

	Text.Format("SELECT PORTFOLIO, %s*ALLOC/100 \"NOMINAL\", ", QData.RemoveComma((LPCTSTR) NomAmount));
	Sql = Text;
	
	if(Ticket.IsEmpty())
	{
		Text.Format("TO_NUMBER(NULL) \"OPT_BACK\" FROM SEMAM.NW_AA_ALLOC A "
				   "WHERE FROM_DATE <= TRUNC(SYSDATE) "
				   "AND NVL(TO_DATE, TRUNC(SYSDATE) + 1) > TRUNC(SYSDATE) "
					"AND AA_METHOD = '%s' ", QData.RemoveComma((LPCTSTR) AAMethod));
	}
	else
		Text.Format("B.TRANS_NUM \"OPT_BACK\" FROM SEMAM.NW_AA_ALLOC A, SEMAM.NW_POSITION_V B "
					"WHERE A.PORTFOLIO = B.PORTFOLIO(+) "
					"AND B.OPT(+) = 'Y' "
					"AND B.TICKET_NUM(+) = '%s' "
					"AND FROM_DATE <= TRUNC(SYSDATE) "
				    "AND NVL(TO_DATE, TRUNC(SYSDATE) + 1) > TRUNC(SYSDATE) "
					"AND AA_METHOD = '%s' ", (LPCTSTR) Ticket, QData.RemoveComma((LPCTSTR) AAMethod));
					
	Sql += Text;

	return Sql;
}

const CString CEntryData::GetAutoAllocationSql(const CString AAMethod, 
								const CString NomAmount, const CString Ticket)
{
	CQData QData;
	CString Sql, Text;

	Text = QData.RemoveComma((LPCTSTR) AAMethod);
	Sql.Format("SELECT PORTFOLIO, %s*ALLOC/100 FROM SEMAM.NW_AA_ALLOC A "
				"WHERE FROM_DATE <= TRUNC(SYSDATE) "
				"AND NVL(TO_DATE, TRUNC(SYSDATE) + 1) > TRUNC(SYSDATE) "
				"AND AA_METHOD = '%s' ", QData.RemoveComma((LPCTSTR) NomAmount), (LPCTSTR) Text);

	return Sql;
}

